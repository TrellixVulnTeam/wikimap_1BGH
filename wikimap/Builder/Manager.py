import os
import sys
from pprint import pformat
from Utils import make_links, format_duration
from ..Utils import Colors, color_text, make_table, get_logger, get_number_width, thin_line_separator
from Job import Job, Properties
from Explorer import build_explorer

class BuildManager(object):
    def __init__(self, build):
        self._logger = get_logger(__name__)
        self._build = build
        self._changed_files = set()
        self._previous_config = build_explorer.get_base_config()
        self._previous_build_dir = build_explorer.get_base_build_dir()
        self._new_config = build.get_full_config()
        self._new_build_dir = build_explorer.make_new_build_dir()

    def run(self):
        self._logger.important('STARTING BUILD IN {}'.format(self._new_build_dir))
        self._logger.important('CUSTOM BUILD CONFIG:\n{}'.format(pformat(self._build.get_custom_config())))
        self._logger.info('FULL BUILD CONFIG:\n{}'.format(pformat(self._build.get_full_config())))
        self._logger.important(thin_line_separator)
        try:
            for job in self._build:
                if self._should_run(job):
                    self._run_job(job)
                else:
                    self._skip_job(job)
        except KeyboardInterrupt:
            raise
        except Exception, e:
            self._logger.exception(str(e))
            raise
        except:
            self._logger.exception("Unexpected error: {}".format(sys.exc_info()[0]))
            raise
        finally:
            self._print_summary()
            self._print_logs_and_warnings()
            build_explorer.save_config(self._new_config)

    def _run_job(self, job):
        self._log_job_action('STARTING', job.number, job.name)
        self._changed_files.update(job.outputs())
        job.run()

    def _skip_job(self, job):
        self._log_job_action('SKIPPING', job.number, job.name)
        job.skip()
        make_links(zip(job.outputs(base=self._previous_build_dir), job.outputs()))

    def _should_run(self, job):
        return self._is_forced(job)\
            or self._inputs_changed(job)\
            or self._config_changed(job)\
            or not self._outputs_computed(job)

    def _is_forced(self, job):
        return Properties.Forced in job.properties

    def _outputs_computed(self, job):
        return self._previous_build_dir and all(os.path.exists(o) for o in job.outputs(base=self._previous_build_dir))

    def _inputs_changed(self, job):
        return any(input_ in self._changed_files for input_ in job.inputs())

    def _config_changed(self, job):
        previous_build_config = self._previous_config.get(job.name, {})
        current_build_config = job.config
        return previous_build_config != current_build_config

    def _print_summary(self):
        outcome_2_color = {
            Job.SUCCESS: Colors.GREEN,
            Job.FAILURE: Colors.RED,
            Job.SKIPPED: Colors.BLUE,
            Job.ABORTED: Colors.YELLOW,
            Job.WARNING: Colors.YELLOW,
            Job.NOT_RUN: Colors.RED
        }

        def make_summary_row(job):
            return (str(job.number),
                job.name,
                color_text('[{}]'.format(job.outcome), outcome_2_color[job.outcome]),
                format_duration(job.duration))

        rows = [make_summary_row(job) for job in self._build]
        table = make_table(('#', 'JOB NAME', 'OUTCOME', 'DURATION'), rows, ('r', 'l', 'c', 'c'))
        self._logger.info('\n\n'+table+'\n')

    def _print_logs_and_warnings(self):
        for job in self._build:
            if len(job.logs) > 0:
                self._logger.info(job.name+' LOGS:\n'+'\n'.join(job.logs)+'\n')
            if len(job.warnings) > 0:
                self._logger.info(job.name+' WARNINGS:\n'+'\n'.join(job.warnings)+'\n')

    def _log_job_action(self, job_action, job_number, job_name):
        format_str = '{{}} JOB [{{:{}}}/{{}}]: {{}}'.format(get_number_width(len(self._build)))
        self._logger.important(format_str.format(job_action, (job_number + 1), len(self._build), job_name))

import numpy as np
import logging

# http://stackoverflow.com/a/30646659/2121473
def fromiter(iterator, dtype, *shape):
    """Generalises `numpy.fromiter()` to multi-dimesional arrays.

    Instead of the number of elements, the parameter `shape` has to be given,
    which contains the shape of the output array. The first dimension may be
    `-1`, in which case it is inferred from the iterator.
    """
    res_shape = shape[1:]
    if not res_shape:  # Fallback to the "normal" fromiter in the 1-D case
        return np.fromiter(iterator, dtype, shape[0])

    # This wrapping of the iterator is necessary because when used with the
    # field trick, np.fromiter does not enforce consistency of the shapes
    # returned with the '_' field and silently cuts additional elements.
    def shape_checker(iterator, res_shape):
        for value in iterator:
            value = np.array(value)
            if value.shape != res_shape:
                raise ValueError("shape of returned object %s does not match given shape %s" % (value.shape, res_shape))
            yield value,

    return np.fromiter(shape_checker(iterator, res_shape), [("_", dtype, res_shape)], shape[0])["_"]

class NormalizedLinksArray(object):
    def __init__(self, path, shuffle=False, log=True):
        self._path = path
        self._array = None
        self._shuffle = shuffle
        self._log = log

    def populate(self, iterator):
        if self._log:
            logging.info("Populating the normalized links array")
        self._array = fromiter(iterator, np.int32, -1, 2)
        np.save(self._path, self._array)

    def __iter__(self):
        logger = logging.getLogger(__name__)

        if self._array is None:
            if self._log:
                logger.info("Loading the normalized links array into memory.")
            self._array = np.load(self._path)

            if self._log:
                logger.info("Loaded array shape: {}".format(self._array.shape))

        if self._shuffle:
            if self._log:
                logger.info("Shuffling the normalized links array.")
            np.random.shuffle(self._array)

        return iter(self._array)

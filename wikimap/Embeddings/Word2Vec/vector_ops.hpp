#pragma once


namespace w2v {

namespace vec {


template<class Target, class Source>
inline void add_to(Target&& target, const Source& source) {
    for (int i = 0; i < source.size(); ++i) {
        target[i] += source[i];
    }
}

template<class Target>
inline void fill_with_zeros(Target&& target) {
    for (int i = 0; i < target.size(); ++i) {
        target[i] = 0;
    }
}

template<class Source1, class Source2>
inline double dot_product(const Source1& s1, const Source2& s2) {
    double res = 0.;
    for (int i = 0; i < s1.size(); ++i) {
        res += s1[i] * s2[i];
    }
    return res;
}

template<class Target>
inline void normalize(Target&& target) {
    double sum = 0.;
    for (int i = 0; i < target.size(); ++i) {
        sum += target[i];
    }
    for (int i = 0; i < target.size(); ++i) {
        target[i] /= sum;
    }
}


} // namespace vec


} // namespace w2v
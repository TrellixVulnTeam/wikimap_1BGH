#pragma once

#include <cmath>

#include <parallel/algorithm>


namespace emb {

namespace vec {


template<class Target, class Source>
inline void add_to(Target&& target, const Source& source) {
    for (size_t i = 0; i < source.size(); ++i) {
        target[i] += source[i];
    }
}

template<class Target>
inline void fill_with_zeros(Target&& target) {
    for (size_t i = 0; i < target.size(); ++i) {
        target[i] = 0;
    }
}

template<class Source1, class Source2>
inline float dot_product(const Source1& s1, const Source2& s2) {
    float res = 0.;
    for (size_t i = 0; i < s1.size(); ++i) {
        res += s1[i] * s2[i];
    }
    return res;
}

template<class Target>
inline void normalize(Target&& target) {
    double sum = 0.;
    for (size_t i = 0; i < target.size(); ++i) {
        sum += target[i] * target[i];
    }
    double root = sqrt(sum);
    for (size_t i = 0; i < target.size(); ++i) {
        target[i] /= root;
    }
}

template<class Target>
inline void shuffle(Target&& target) {
    __gnu_parallel::random_shuffle(target.begin(), target.end());
}


} // namespace vec


} // namespace emb
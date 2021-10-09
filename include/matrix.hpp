#ifndef MATRIX_H_
#define MATRIX_H_

#include <array>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>

namespace matrix {
// A Row is a row of doubles
    template<size_t C>
    using Row = std::array<double, C>;

// A Matrix is an array of rows
    template<size_t R, size_t C>
    using Matrix = std::array<Row<C>, R>;

// The identity matrix
    template<size_t R>
    inline const matrix::Matrix<R, R> I() {
        matrix::Matrix<R, R> result {};
        for (int i = 0; i < R; ++i) {
            result[i][i] = 1;
        }
        return result;
    }

// 3D vector
    using Vector3 = matrix::Row<3>;
}

template<size_t C>
std::ostream & operator<<(std::ostream& out, const matrix::Row<C>& v) {
    for (const auto elem : v) {
        out << elem << "\t";
    }
    return out;
}

template<size_t C>
const matrix::Row<C> operator *(const double k, const matrix::Row<C>& v) {
    matrix::Row<C> result {};
    for (int i = 0; i < C; ++i) {
        result[i] = k * v[i];
    }
    return result;
}

template<size_t C>
const matrix::Row<C> operator+(const matrix::Row<C>& a, const matrix::Row<C>& b) {
    matrix::Row<C> result {};
    std::transform(a.begin(), a.end(), b.begin(), result.begin(), std::plus<double>());
    return result;
}

template<size_t C>
const matrix::Row<C> operator-(const matrix::Row<C>& a, const matrix::Row<C>& b) {
    matrix::Row<C> result {};
    std::transform(a.begin(), a.end(), b.begin(), result.begin(), std::minus<double>());
    return result;
}

template<size_t C>
double operator*(const matrix::Row<C> & a, const matrix::Row<3>& b) {
    return std::inner_product(a.begin(), a.end(), b.begin(), 0);
}

template<size_t C>
const matrix::Row<C> operator-(const matrix::Row<C>& a) {
    matrix::Row<C> result{};
    std::transform(a.begin(), a.end(), result.begin(), std::negate<double>());
    return result;
}

template<size_t R, size_t C>
const matrix::Matrix<R, C> operator*(const double n, const matrix::Matrix<R, C> m) {
    matrix::Matrix<R, C> result {};
    std::transform(m.begin(), m.end(), result.begin(), [n](const matrix::Row<C> col) {return n*col;});
    return result;
}

template<size_t R, size_t C>
const matrix::Matrix<R, C> operator+(const matrix::Matrix<R, C>& a, const matrix::Matrix<R, C>& b) {
    matrix::Matrix<R, C> result {};
    std::transform(a.begin(), a.end(), b.begin(), result.begin(),
                   [](const matrix::Row<C> ca, const matrix::Row<C> cb) {return ca + cb;});
    return result;
}

template<size_t R1, size_t C, size_t C2>
const matrix::Matrix<R1, C2> operator*(const matrix::Matrix<R1, C>& a, const matrix::Matrix<C, C2>& b) {
    matrix::Matrix<R1, C2> result {};
    for (int i = 0; i < R1; ++i) {
        for (int j = 0; j < C2; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < C; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

template<size_t R, size_t C>
const matrix::Row<R> column(const matrix::Matrix<R, C>& a, const size_t j) {
    matrix::Row<R> result {};
    for (int i = 0; i < R; ++i) {
        result[i] = a[i][j];
    }
    return result;
}

template<size_t R, size_t C>
const matrix::Matrix<R, C> transpose(const matrix::Matrix<R, C>& a) {
    matrix::Matrix<R, C> result {};
    for (int i = 0; i < C; ++i) {
        result[i] = column(a, i);
    }
    return result;
}

template<size_t R, size_t C>
const matrix::Row<C> operator*(const matrix::Row<R>& r, const matrix::Matrix<R, C> m) {
    matrix::Row<C> result {};
    for (int i = 0; i < R; ++i) {
        result = result + r[i] * m[i];
    }
    return result;
}

#endif // MATRIX_H_

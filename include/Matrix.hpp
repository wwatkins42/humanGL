#pragma once

#include <iostream>
#include <cmath>
#include <array>

#include "Exception.hpp"

/* H being the height (or rows), W being the width (or cols) */
template<typename T, size_t H, size_t W>
class Mat2d {

public:
    Mat2d( void ) : w(W), h(H), size(W * H) {
        for (size_t i = 0; i < size; ++i)
            data[i] = 0;
    }
    Mat2d( std::array<T, W * H> const & value ) : w(W), h(H), size(W * H) {
        for (size_t i = 0; i < size; ++i)
            data[i] = value[i];
    }
    Mat2d( Mat2d const & rhs ) : w(W), h(H), size(W * H) {
        *this = rhs;
    }
    Mat2d & operator=( Mat2d const & rhs ) {
        w = rhs.w;
        h = rhs.h;
        size = rhs.size;
        data = rhs.getData();
        return (*this);
    }
    ~Mat2d( void ) {
    }
    // access the data as 2 dimensions
    T & operator()( size_t j, size_t i ) {
        if (j < 0 || j > h - 1 || i < 0 || i > w - 1)
            throw Exception::MatrixAccessError(j, i, h, w);
        return (data[j * w + i]);
    }
    // access the data as it is in the array
    T & operator[]( size_t i ) {
        if (i < 0 || i > size - 1)
            throw Exception::MatrixAccessError(i, size);
        return (data[i]);
    }

    /* scalar operator overload (we could take type other than T) */
    Mat2d   operator+( T scalar ) {
        auto res = *this;
        return (res += scalar);
    }
    Mat2d   operator-( T scalar ) {
        auto res = *this;
        return (res -= scalar);
    }
    Mat2d   operator*( T scalar ) {
        auto res = *this;
        return (res *= scalar);
    }
    Mat2d   operator/( T scalar ) {
        auto res = *this;
        return (res /= scalar);
    }
    /* matrix operator overload */
    Mat2d   operator+( Mat2d const & rhs ) {
        auto res = *this;
        return (res += rhs);
    }
    Mat2d   operator-( Mat2d const & rhs ) {
        auto res = *this;
        return (res -= rhs);
    }
    Mat2d   operator*( Mat2d const & rhs ) {
        if (rhs.h != w)
            throw Exception::MatrixOperationError(h, w, rhs.h, rhs.w);
        Mat2d<T, H, W>  tmp;
        for (size_t i = 0; i < h; ++i) {
            for (size_t j = 0; j < rhs.w; ++j) {
                tmp(i,j) = 0;
                for (size_t k = 0; k < w; ++k)
                    tmp(i,j) += (*this)(i,k) * rhs(k,j);
            }
        }
        return (tmp);
    }
    Mat2d   operator/( Mat2d const & rhs ) {
        auto res = *this;
        return (res /= rhs);
    }
    /* scalar operator overload (we could take type other than T) */
    Mat2d   &operator+=( T scalar ) {
        for (size_t i = 0; i < size; ++i)
            data[i] += scalar;
        return (*this);
    }
    Mat2d   &operator-=( T scalar ) {
        for (size_t i = 0; i < size; ++i)
            data[i] -= scalar;
        return (*this);
    }
    Mat2d   &operator*=( T scalar ) {
        for (size_t i = 0; i < size; ++i)
            data[i] *= scalar;
        return (*this);
    }
    Mat2d   &operator/=( T scalar ) {
        for (size_t i = 0; i < size; ++i)
            data[i] /= scalar;
        return (*this);
    }
    /* matrix operator overload */
    Mat2d   &operator+=( Mat2d const & rhs ) {
        if (rhs.size != size)
            throw Exception::MatrixOperationError(size, rhs.size);
        for (size_t i = 0; i < size; ++i)
            data[i] += rhs[i];
        return (*this);
    }
    Mat2d   &operator-=( Mat2d const & rhs ) {
        if (rhs.size != size)
            throw Exception::MatrixOperationError(size, rhs.size);
        for (size_t i = 0; i < size; ++i)
            data[i] -= rhs[i];
        return (*this);
    }
    Mat2d   &operator*=( Mat2d const & rhs ) {
        if (rhs.h != w)
            throw Exception::MatrixOperationError(h, w, rhs.h, rhs.w);
        // well we would have to reshape the matrix
        // Mat2d<T, H, W>  tmp;
        // for (size_t i = 0; i < m; ++i) {
        //     for (size_t j = 0; j < q; ++j) {
        //         tmp(i,j) = 0;
        //         for (size_t k = 0; k < n; ++k)
        //             tmp(i,j) = c(i,j) + (a(i,k) * b(k,j));
        //     }
        // }
        // *this = tmp;
        return (*this);
    }
    Mat2d   &operator/=( Mat2d const & rhs ) {
        if (rhs.h != w)
            throw Exception::MatrixOperationError(h, w, rhs.h, rhs.w);

        return (*this);
    }

    void   compareDims( Mat2d const & rhs ) {
        if (rhs.w != w or rhs.h != h)
            throw Exception::MatrixError("");
    }

    const size_t    w;
    const size_t    h;
    const size_t    size;

    std::array<T, W * H>    &getData( void ) const { return (data); };

    friend std::ostream &operator<<( std::ostream & stream, Mat2d & mat ) {
        stream << "[[";
        for (size_t j = 0; j < mat.h; ++j) {
            for (size_t i = 0; i < mat.w; ++i)
                stream << mat[j * mat.w + i] << (i != mat.w - 1 ? ", " : "");
            stream << (j != mat.h - 1 ? "],\n [" : "]]");
        }
        return (stream);
    }

private:
    std::array<T, W * H> data; // maybe we'll change that for when we implement reshape

};

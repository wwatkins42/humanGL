#pragma once

#include <iostream>
#include <cmath>
#include <array>

#include "Exception.hpp"

#define RAD2DEG (180.0 / M_PI)
#define DEG2RAD (M_PI / 180.0)

/* H being the height (or rows), W being the width (or cols) */
template<typename T, size_t H, size_t W>
class Mat2d {

public:
    Mat2d( void ) : w(W), h(H), size(W * H) {
        for (size_t i = 0; i < size; ++i)
            data[i] = 0;
    }
    Mat2d( const std::array<T, W * H>& value ) : w(W), h(H), size(W * H) {
        for (size_t i = 0; i < size; ++i)
            data[i] = value[i];
    }
    Mat2d( const Mat2d& rhs ) : w(W), h(H), size(W * H) {
        *this = rhs;
    }
    Mat2d& operator=( const Mat2d& rhs ) {
        w = rhs.w;
        h = rhs.h;
        size = rhs.size;
        data = rhs.getData();
        return (*this);
    }
    ~Mat2d( void ) {
    }

    Mat2d   &identity( void ) {
        if (h != w)
            throw Exception::MatrixTypeError(h, w);
        for (size_t i = 0; i < size; ++i)
            data[i] = (!(i % (w + 1)) ? 1 : 0);
        return (*this);
    }
    Mat2d<T, W, H>   transpose( void ) {
        Mat2d<T, W, H> res;
        for (int i = 0; i < h; ++i)
            for (int j = 0; j < w; ++j)
                res(j,i) = (*this)(i,j);
        return (res);
    }

    /*  MUTABLE ACCESSORS
    */
    T& operator()( size_t j, size_t i ) {
        if (j < 0 || j > h - 1 || i < 0 || i > w - 1)
            throw Exception::MatrixAccessError(j, i, h, w);
        return (data[j * w + i]);
    }
    T& operator()( size_t i ) {
        if (i < 0 || i > size - 1)
            throw Exception::MatrixAccessError(i, size);
        return (data[i]);
    }
    /*  IMMUTABLE ACCESSOR
    */
    const T& operator[]( size_t i ) const {
        if (i < 0 || i > size - 1)
            throw Exception::MatrixAccessError(i, size);
        return (data[i]);
    }
    /*  SCALAR OPERATORS
    */
    Mat2d   operator+( T scalar ) const {
        auto res = *this;
        return (res += scalar);
    }
    Mat2d   operator-( T scalar ) const {
        auto res = *this;
        return (res -= scalar);
    }
    Mat2d   operator*( T scalar ) const {
        auto res = *this;
        return (res *= scalar);
    }
    Mat2d   operator/( T scalar ) const {
        auto res = *this;
        return (res /= scalar);
    }
    /*  OTHER MATRIX INSTANCE OPERATORS
    */
    Mat2d   operator+( const Mat2d& rhs ) const {
        auto res = *this;
        return (res += rhs);
    }
    Mat2d   operator-( const Mat2d& rhs ) const {
        auto res = *this;
        return (res -= rhs);
    }
    template<typename _T, size_t _H, size_t _W>
    Mat2d<_T, H,_W>   operator*( const Mat2d<_T,_H,_W>& rhs ) const {
        if (rhs.h != w)
            throw Exception::MatrixOperationError(h, w, rhs.h, rhs.w);
        Mat2d<_T, H,_W>  res;
        for (size_t i = 0; i < h; ++i)
            for (size_t j = 0; j < rhs.w; ++j) {
                res(i,j) = 0;
                for (size_t k = 0; k < w; ++k)
                    res(i,j) += (*this)[i * w + k] * rhs[k * rhs.w + j];
                    // res(i,j) += (*this)(i,k) * rhs[k * rhs.w + j]; // not const
            }
        return (res);
    }
    /*  OTHER MATRIX INSTANCE OPERATORS
    */
    Mat2d&  operator+=( T scalar ) {
        for (size_t i = 0; i < size; ++i)
            data[i] += scalar;
        return (*this);
    }
    Mat2d&  operator-=( T scalar ) {
        for (size_t i = 0; i < size; ++i)
            data[i] -= scalar;
        return (*this);
    }
    Mat2d&  operator*=( T scalar ) {
        for (size_t i = 0; i < size; ++i)
            data[i] *= scalar;
        return (*this);
    }
    Mat2d&  operator/=( T scalar ) {
        for (size_t i = 0; i < size; ++i)
            data[i] /= scalar;
        return (*this);
    }
    /*  OTHER MATRIX INSTANCE OPERATORS
    */
    Mat2d&  operator+=( const Mat2d& rhs ) {
        if (rhs.size != size)
            throw Exception::MatrixOperationError(size, rhs.size);
        for (size_t i = 0; i < size; ++i)
            data[i] += rhs[i];
        return (*this);
    }
    Mat2d&  operator-=( const Mat2d& rhs ) {
        if (rhs.size != size)
            throw Exception::MatrixOperationError(size, rhs.size);
        for (size_t i = 0; i < size; ++i)
            data[i] -= rhs[i];
        return (*this);
    }
    /* element wise multiply */
    Mat2d<T,H,W>    multiply( const Mat2d<T,H,W>& m ) const {
        Mat2d<T,H,W>    res;
        for (size_t i = 0; i < H * W; ++i)
            res(i) = (*this)[i] * m[i];
        return (res);
    }
    /* element wise divide */
    Mat2d<T,H,W>    divide( const Mat2d<T,H,W>& m ) const {
        Mat2d<T,H,W>    res;
        for (size_t i = 0; i < H * W; ++i)
            res(i) = (*this)[i] / m[i];
        return (res);
    }

    const T*                    getRawData( void ) const { return (data.data()); };
    const std::array<T, W * H>& getData( void ) const { return (data); };
    std::array<T, W * H>        getDataCpy( void ) const { return (data); };

    friend std::ostream&    operator<<( std::ostream& stream, const Mat2d& mat ) {
        stream << "[[";
        for (size_t j = 0; j < mat.h; ++j) {
            for (size_t i = 0; i < mat.w; ++i)
                stream << mat[j * mat.w + i] << (i != mat.w - 1 ? ", " : "");
            stream << (j != mat.h - 1 ? "],\n [" : "]]");
        }
        return (stream);
    }
    // allows implicit cast for vector types to vector types (of different types)
    template<size_t _H>
    operator Mat2d<T,_H,1>() const {
        Mat2d<T,_H,1>    res;
        for (size_t i = 0; i < (_H<H?_H:H); ++i)
            res(i) = this->data[i];
        return (res);
    }

    size_t  w;
    size_t  h;
    size_t  size;

private:
    std::array<T, W * H> data; // maybe we'll change that for if we implement reshape

};

/* Vectors typedefs */
typedef Mat2d<int, 4, 1> vec4i;
typedef Mat2d<int, 3, 1> vec3i;
typedef Mat2d<int, 2, 1> vec2i;

typedef Mat2d<float, 4, 1> vec4;
typedef Mat2d<float, 3, 1> vec3;
typedef Mat2d<float, 2, 1> vec2;

typedef Mat2d<double, 4, 1> vec4d;
typedef Mat2d<double, 3, 1> vec3d;
typedef Mat2d<double, 2, 1> vec2d;

/* Matrices typedefs */
typedef Mat2d<float, 4, 4> mat4;
typedef Mat2d<float, 3, 3> mat3;
typedef Mat2d<float, 2, 2> mat2;

namespace mtls {
    float   radians( const float deg );
    float   degrees( const float rad );

    vec3    createAxisUnitVec3( size_t index );

    template<size_t H>
    Mat2d<float,H,1>  sign( const Mat2d<float,H,1>& v ) {
        Mat2d<float,H,1>    res;
        for (size_t i = 0; i < H; ++i)
            res(i) = (v[i] < 0.0f ? -1 : v[i] == 0.0f ? 0 : 1);
        return (res);
    }
    /* element wise multiply */
    template<typename T, size_t H, size_t W>
    Mat2d<T,H,W>    multiply( const Mat2d<T,H,W>& a, const Mat2d<T,H,W>& b ) {
        Mat2d<T,H,W>    res;
        for (size_t i = 0; i < H * W; ++i)
            res(i) = a[i] * b[i];
        return (res);
    }
    /* element wise divide */
    template<typename T, size_t H, size_t W>
    Mat2d<T,H,W>    divide( const Mat2d<T,H,W>& a, const Mat2d<T,H,W>& b ) {
        Mat2d<T,H,W>    res;
        for (size_t i = 0; i < H * W; ++i)
            res(i) = a[i] / b[i];
        return (res);
    }

    mat4    &scale( mat4& m, const vec3& s );
    mat4    &translate( mat4& m, const vec3& t );
    mat4    &rotate( mat4& m, const vec3& r, const vec3& offset );
    mat4    &rotate( mat4& m, const vec3& r );
    mat4    &rotate( mat4& m, double theta, const vec3& r );
    mat4    inverse( const mat4& m );

    extern const mat4  mat4identity;
    extern const mat3  mat3identity;
    extern const mat2  mat2identity;
}

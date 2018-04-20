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
        if (h == w)
            this->identity();
        else {
            for (size_t i = 0; i < size; ++i)
                data[i] = 0;
        }
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
    /*  OTHER MATRIX INSTANCE OPERATORS
    */
    Mat2d   operator+( const Mat2d& rhs ) {
        auto res = *this;
        return (res += rhs);
    }
    Mat2d   operator-( const Mat2d& rhs ) {
        auto res = *this;
        return (res -= rhs);
    }
    template<typename _T, size_t _H, size_t _W>
    Mat2d<_T, H,_W>   operator*( const Mat2d<_T,_H,_W>& rhs ) {
        if (rhs.h != w)
            throw Exception::MatrixOperationError(h, w, rhs.h, rhs.w);
        Mat2d<_T, H,_W>  res;
        for (size_t i = 0; i < h; ++i)
            for (size_t j = 0; j < rhs.w; ++j) {
                res(i,j) = 0;
                for (size_t k = 0; k < w; ++k)
                    res(i,j) += (*this)(i,k) * rhs[k * rhs.w + j];
            }
        return (res);
    }
    /*  OTHER MATRIX INSTANCE OPERATORS
    */
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
    /*  OTHER MATRIX INSTANCE OPERATORS
    */
    Mat2d   &operator+=( const Mat2d& rhs ) {
        if (rhs.size != size)
            throw Exception::MatrixOperationError(size, rhs.size);
        for (size_t i = 0; i < size; ++i)
            data[i] += rhs[i];
        return (*this);
    }
    Mat2d   &operator-=( const Mat2d& rhs ) {
        if (rhs.size != size)
            throw Exception::MatrixOperationError(size, rhs.size);
        for (size_t i = 0; i < size; ++i)
            data[i] -= rhs[i];
        return (*this);
    }
    // Mat2d   &operator*=( const Mat2d& rhs ) {
    //     // NOTE: well we would have to reshape the matrix
    //     return (*this);
    // }

    std::array<T, W * H>    getData( void ) const { return (data); };

    friend std::ostream &operator<<( std::ostream& stream, const Mat2d& mat ) {
        stream << "[[";
        for (size_t j = 0; j < mat.h; ++j) {
            for (size_t i = 0; i < mat.w; ++i)
                stream << mat[j * mat.w + i] << (i != mat.w - 1 ? ", " : "");
            stream << (j != mat.h - 1 ? "],\n [" : "]]");
        }
        return (stream);
    }

    size_t  w;
    size_t  h;
    size_t  size;

private:
    std::array<T, W * H> data; // maybe we'll change that for when we implement reshape

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
    float   radians( const float deg ) { return (deg * DEG2RAD); }
    float   degrees( const float rad ) { return (rad * RAD2DEG); }

    mat4    &scale( mat4& m, const vec3& scale ) {
        mat4    tmp;
        tmp(0)  = scale[0];
        tmp(5)  = scale[1];
        tmp(10) = scale[2];
        m = tmp * m;
        return (m);
    }
    mat4    &translate( mat4& m, const vec3& translate ) {
        mat4    tmp;
        tmp(3)  = translate[0];
        tmp(7)  = translate[1];
        tmp(11) = translate[2];
        m = tmp * m;
        return (m);
    }
    mat4    &rotate( mat4& m, double theta, const vec3& r ) {
        const float sin = std::sin(theta);
        const float cos = std::cos(theta);
        mat4    tmp({
            cos+r[0]*r[0]*(1-cos),      r[0]*r[1]*(1-cos)-r[2]*sin, r[0]*r[2]*(1-cos)+r[1]*sin, 0,
            r[1]*r[0]*(1-cos)+r[2]*sin, cos+r[1]*r[1]*(1-cos),      r[1]*r[2]*(1-cos)-r[0]*sin, 0,
            r[2]*r[0]*(1-cos)-r[1]*sin, r[2]*r[1]*(1-cos)+r[0]*sin, cos+r[2]*r[2]*(1-cos),      0,
            0,                          0,                          0,                          1
        });
        m = tmp * m;
        return (m);
    }

}

/*
    _IDENTITY______
    [[ 1  0  0  0 ]
     [ 0  1  0  0 ]
     [ 0  0  1  0 ]
     [ 0  0  0  1 ]]

    _SCALE_________
    [[ Sx 0  0  0 ]
     [ 0  Sy 0  0 ]
     [ 0  0  Sz 0 ]
     [ 0  0  0  1 ]]

    _TRANSLATE_____
    [[ 1  0  0  Tx]
     [ 0  1  0  Ty]
     [ 0  0  1  Tz]
     [ 0  0  0  1 ]]

     when doing matrix multiplication it is advised to do it in the following order:
        scaling -> rotation -> translation
     as matrix multiplications are read from right to left, we do:
        translation * rotation * scaling * V

*/

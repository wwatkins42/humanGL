#include "Matrix.hpp"

float   mtls::radians( const float deg ) {
    return (deg * DEG2RAD);
}

float   mtls::degrees( const float rad ) {
    return (rad * RAD2DEG);
}

mat4    &mtls::scale( mat4& m, const vec3& s ) {
    mat4    tmp({
        s[0], 0, 0, 0,
        0, s[1], 0, 0,
        0, 0, s[2], 0,
        0, 0, 0, 1
    });
    m = tmp * m;
    return (m);
}

mat4    &mtls::translate( mat4& m, const vec3& t ) {
    mat4    tmp({
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        t[0], t[1], t[2], 1
    });
    m = tmp * m;
    return (m);
}

mat4    &mtls::rotate( mat4& m, double theta, const vec3& r ) { // use quaternions in the future
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

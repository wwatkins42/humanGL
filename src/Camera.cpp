#include "Camera.hpp"

Camera::Camera( float fov, float aspect, short projectionType ) : aspect(aspect), fov(fov) {
    this->projectionMatrix = Camera::createPerspectiveProjectionMatrix(fov, aspect);
    this->viewMatrix.identity();
    this->viewMatrix = mtls::translate(this->viewMatrix, vec3({ 0, 0, -3 }));
}

Camera::Camera( const Camera& rhs ) : aspect(rhs.getAspect()) {
    *this = rhs;
}

Camera& Camera::operator=( const Camera& rhs ) {
    aspect = rhs.getAspect();
    fov = rhs.getFov();
    return (*this);
}

Camera::~Camera( void ) {
}

void    Camera::setFov( float fov ) {
    this->fov = fov;
    float yScale = 1.0 / std::tan(mtls::radians(this->fov / 2.0));
    float xScale = yScale / this->aspect;
    this->projectionMatrix(0) = xScale;
    this->projectionMatrix(6) = yScale;
}

void    Camera::setAspect( float aspect ) {
    this->aspect = aspect;
    float yScale = 1.0 / std::tan(mtls::radians(this->fov / 2.0));
    float xScale = yScale / this->aspect;
    this->projectionMatrix(0) = xScale;
    this->projectionMatrix(6) = yScale;
}

mat4    Camera::createPerspectiveProjectionMatrix( float fov, float aspect, float near, float far ) {
    float yScale = 1 / std::tan(mtls::radians(fov * 0.5));
    float xScale = yScale / aspect;
    float nearfar = far - near;
    // float nearfar = near - far;
    mat4    projectionMatrix({
        xScale, 0, 0, 0,
        0, yScale, 0, 0,
        0, 0, -(far+near)/nearfar, -1,
        0, 0, -2*far*near/nearfar, 0
    });
    return (projectionMatrix);
}

// mat4    Camera::createOrthographicProjectionMatrix( double fov, double aspect, double near, double far ) {
//     mat4    projectionMatrix({{
//         1/r, 0, 0, 0,
//         0, 1/t, 0, 0,
//         0, 0, -2/(far-near), -((far+near)/(far-near)),
//         0, 0, -1, 0
//     }});
//     return (projectionMatrix);
// }

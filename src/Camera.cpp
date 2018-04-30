#include "Camera.hpp"

Camera::Camera( float fov, float aspect ) : aspect(aspect), fov(fov) {
    this->projectionMatrix = Camera::createPerspectiveProjectionMatrix(fov, aspect);
    this->position = vec3({0, 0, 10});
    this->target = vec3({0, 0, 0});
    this->viewMatrix = mtls::lookAt(this->position, this->target);
}

Camera::Camera( const Camera& rhs ) {
    *this = rhs;
}

Camera& Camera::operator=( const Camera& rhs ) {
    this->projectionMatrix = rhs.getProjectionMatrix();
    this->viewMatrix = rhs.getViewMatrix();
    this->aspect = rhs.getAspect();
    this->fov = rhs.getFov();
    this->position = rhs.getPosition();
    this->target = rhs.getTarget();
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
    float nearfar = near - far;
    mat4    projectionMatrix({
        xScale, 0, 0, 0,
        0, yScale, 0, 0,
        0, 0, (far+near)/nearfar, -1,
        0, 0, 2*far*near/nearfar, 0
    });
    return (projectionMatrix);
}

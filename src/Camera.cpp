#include "Camera.hpp"

Camera::Camera( float fov, float aspect ) : aspect(aspect), fov(fov) {
    this->projectionMatrix = Camera::createPerspectiveProjectionMatrix(fov, aspect);
    // this->position = vec3({0, 0, 10});
    this->position = vec3({0, 0, 6});
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

void    Camera::handleKeys( const std::array<tKey, N_KEY>& keys, const vec3& lockPos ) {
    vec4    translate({
        (float)(keys[GLFW_KEY_A].value - keys[GLFW_KEY_D].value),
        (float)(keys[GLFW_KEY_LEFT_SHIFT].value - keys[GLFW_KEY_SPACE].value),
        (float)(keys[GLFW_KEY_W].value - keys[GLFW_KEY_S].value),
        1.0f
    });
    /* translation is in the same coordinate system as view (moves in same direction) */
    translate = this->viewMatrix * mtls::normalize(translate);
    /* change the target if we are in orbit or free mode */
    if (keys[GLFW_KEY_C].value)
        this->target = this->interpolate(this->target, lockPos, keys[GLFW_KEY_C].stamp, 2000);
    else
        this->target = this->target + static_cast<vec3>(this->viewMatrix * vec4({0,0,-1,0}));
    this->position = this->position - static_cast<vec3>(translate) * 0.5f;
    this->viewMatrix = mtls::lookAt(this->position, this->target);
}

vec3    Camera::interpolate( const vec3& v0, const vec3& v1, tTimePoint last, size_t duration ) {
    float t = (1 - ((float)duration - this->getElapsedMilliseconds(last).count()) / (float)duration);
    t = std::min(t, 1.0f);
    return (mtls::lerp(v0, v1, t));
}

tMilliseconds   Camera::getElapsedMilliseconds( tTimePoint last ) {
    return (std::chrono::steady_clock::now() - last);
}

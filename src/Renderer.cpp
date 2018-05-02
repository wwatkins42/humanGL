#include "Renderer.hpp"

Renderer::Renderer( Env* env ) :
    env(env),
    camera(70, (float)env->getWindow().width / (float)env->getWindow().height),
    shader("./shader/vertex.glsl", "./shader/fragment.glsl") {
        this->env->getCharacter()->setShader(&this->shader);
}

Renderer::~Renderer( void ) {
}

inline float rayEllipsoidIntersect( const vec3& rayOrigin, const vec3& rayDir, const vec3& ellipsoidOrigin, const vec3& ellipsoidRadius ) {
    vec3 length = (rayOrigin - ellipsoidOrigin).divide(ellipsoidRadius);
    vec3    dir = rayDir.divide(ellipsoidRadius);

    float a = mtls::dot(dir, dir);
    float b = 2 * mtls::dot(length, dir);
    float c = mtls::dot(length, length) - 1;
    float disc = b * b - 4 * a * c;

    if (disc < 0)
        return (-1);
    disc = std::sqrt(disc);

    float x1 = (-b + disc) / (2 * a);
    float x2 = (-b - disc) / (2 * a);
    return (x1 < x2 ? x1 : x2);
}

// void    Renderer::raycast( void ) {
//     float       dist = 10000000.0f;
//     std::string objectId = "none";

//     /* put mouse position in clip-space (-1, 1) */
//     vec2    mouse = mousePosToClipSpace( this->env->getController()->getMousePosition(), this->env->getWindow().width, this->env->getWindow().height );
//     /* matrix to transform from clip-space to world-space */
//     mat4    toWorld = mtls::inverse(this->camera.getProjectionMatrix() * this->camera.getViewMatrix());
//     vec4    rayWorld = toWorld * vec4({mouse[0], mouse[1], -1, 1});
//     vec3    rayDir = mtls::normalize(vec4({rayWorld[0], rayWorld[1], -1, 0}));
//
//     vec3    from = this->camera.getPosition();
//
//     std::unordered_map<std::string, Bone*>   obj = this->env->getCharacter()->getBones();
//     for (auto it = obj.begin(); it != obj.end(); it++) {
//         float t = rayEllipsoidIntersect(from, rayDir, obj[it->first]->getModel()->getWorldPosition(), obj[it->first]->getModel()->getScale());
//         if (t > 0 and t < dist) {
//             dist = t;
//             objectId = it->first;
//         }
//     }
//     std::cout << "Intersect: " << objectId << std::endl;
// }

void    Renderer::raycast( void ) {
    float       dist = 10000000.0f;
    std::string objectId = "none";

    /* put mouse position in clip-space (-1, 1) */
    vec2    mouse = mousePosToClipSpace( this->env->getController()->getMousePosition(), this->env->getWindow().width, this->env->getWindow().height );
    /* matrix to transform from clip-space to world-space */
    // mat4    toWorld = mtls::inverse(this->camera.getProjectionMatrix() * this->camera.getViewMatrix()); // same as view.inverse * projection.inverse
    // vec4    rayWorld = toWorld * vec4({mouse[0], mouse[1], 0, 1});
    // vec3    rayDir = mtls::normalize(vec4({rayWorld[0], rayWorld[1], -1, 0}));

    /* https://stackoverflow.com/questions/2093096/implementing-ray-picking */
    // vec4    view = mtls::normalize(this->camera.getFront() - this->camera.getPosition());
    // vec4    nearPoint = this->camera.getPosition() + view + vec4({ mouse[0], mouse[1], 0, 1 });
    // vec3    rayDir = (vec3)nearPoint - this->camera.getPosition();
    // rayDir = mtls::normalize(rayDir);


    // mat4    toWorld = mtls::inverse(this->camera.getProjectionMatrix() * this->camera.getViewMatrix()); // same as view.inverse * projection.inverse
    // vec4    nearPoint = toWorld * vec4({ mouse[0], mouse[1], 0, 1 });
    // vec3    rayDir = mtls::normalize(nearPoint - this->camera.getPosition());

    vec4    m = vec4({ mouse[0], mouse[1], 0, 1 });
    vec4    ndcNear = vec4({ m[0], m[1],-1, 1 });
    vec4    ndcFar  = vec4({ m[0], m[1], 1, 1 });
    ndcNear = mtls::inverse(this->camera.getProjectionMatrix().transpose()) * ndcNear;
    ndcFar  = mtls::inverse(this->camera.getProjectionMatrix().transpose()) * ndcFar;

    vec3    near = static_cast<vec3>(ndcNear / ndcNear[3]);
    vec3    far  = static_cast<vec3>( ndcFar /  ndcFar[3]);
    near = static_cast<vec3>(mtls::inverse(this->camera.getViewMatrix().transpose()) * vec4({ near[0], near[1], near[2], 1 }) );
    far  = static_cast<vec3>(mtls::inverse(this->camera.getViewMatrix().transpose()) * vec4({ far[0], far[1], far[2], 1 }) );

    vec3    rayDir = mtls::normalize(far - near);

    std::cout << rayDir << std::endl;

    std::unordered_map<std::string, Bone*>   obj = this->env->getCharacter()->getBones();
    for (auto it = obj.begin(); it != obj.end(); it++) {
        float t = rayEllipsoidIntersect(this->camera.getPosition(), rayDir, obj[it->first]->getModel()->getWorldPosition(), obj[it->first]->getModel()->getScale());
        if (t > 0 and t < dist) {
            dist = t;
            objectId = it->first;
        }
    }
    /* reset the selected value of all models to false */
    for (auto it = obj.begin(); it != obj.end(); it++)
        (*this->env->getCharacter())[it->first]->getModel()->setSelected(false);
    /* set the selected one */
    if (objectId != "none") {
        (*this->env->getCharacter())[objectId]->getModel()->setSelected(true);
        std::cout << "Intersect: " << objectId << std::endl;
    }
}

void	Renderer::loop( void ) {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->env->getWindow().ptr)) {
        glfwPollEvents();
        glClearColor(0.09f, 0.08f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->shader.use();
        this->env->getController()->update();

        // this->raycast();

        this->camera.handleKeys( this->env->getController()->getKeys(), this->env->getCharacter()->getParentBone()->getModel()->getWorldPosition() );
        this->env->getAnimator()->handleKeys( this->env->getController()->getKeys() );
        this->env->getAnimator()->update();

        this->raycast();

        this->updateShaderUniforms();
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
}

void    Renderer::updateShaderUniforms( void ) {
    this->shader.setMat4UniformValue("view", this->camera.getViewMatrix());
    this->shader.setMat4UniformValue("projection", this->camera.getProjectionMatrix());
}

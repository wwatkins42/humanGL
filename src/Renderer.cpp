#include "Renderer.hpp"

Renderer::Renderer( Env* env ) :
    env(env),
    camera(60, (float)env->getWindow().width / (float)env->getWindow().height),
    shader("./shader/vertex.glsl", "./shader/fragment.glsl") {
        this->env->getCharacter()->setShader(&this->shader);
}

Renderer::~Renderer( void ) {
}

void	Renderer::loop( void ) {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->env->getWindow().ptr)) {
        glfwPollEvents();
        glClearColor(0.09f, 0.08f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->shader.use();
        this->env->getController()->update();
        this->camera.handleKeys( this->env->getController()->getKeys(), this->env->getCharacter()->getParentBone()->getModel()->getWorldPosition() );
        this->raycastObjectSelect();
        this->env->getAnimator()->handleKeys( this->env->getController()->getKeys() );
        this->env->getAnimator()->update();
        this->updateShaderUniforms();
        this->env->getCharacter()->switchBonesModel( this->env->getController()->getKeyValue(GLFW_KEY_M) );
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
}

void    Renderer::updateShaderUniforms( void ) {
    this->shader.setMat4UniformValue("view", this->camera.getViewMatrix());
    this->shader.setMat4UniformValue("projection", this->camera.getProjectionMatrix());
}

inline float rayEllipsoidIntersect( const vec3& rayOrigin, const vec3& rayDir, const vec3& ellipsoidOrigin, const vec3& ellipsoidRadius ) {
    vec3 length = (rayOrigin - ellipsoidOrigin).divide(ellipsoidRadius * 0.5);
    vec3    dir = rayDir.divide(ellipsoidRadius * 0.5);

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

void    Renderer::raycastObjectSelect( void ) {
    float       dist = 100000.0f;
    std::string objectId = "none";
    std::unordered_map<std::string, Bone*> obj = this->env->getCharacter()->getBones();

    if (this->env->getController()->getMouseButtonValue(GLFW_MOUSE_BUTTON_1)) {
        /* put mouse position in clip-space (-1, 1) */
        vec2    m = mousePosToClipSpace( this->env->getController()->getMousePosition(), this->env->getWindow().width, this->env->getWindow().height );
        /* matrix to transform from clip-space to world-space */
        mat4    toWorld = mtls::inverse(this->camera.getProjectionMatrix().transpose() * this->camera.getViewMatrix().transpose());
        /* create a ray from the camera in world-space */
        vec4   nearWorld = toWorld * vec4({ m[0], m[1], 1, 1 });
        vec4    farWorld = toWorld * vec4({ m[0], m[1],-1, 1 });
        vec3    near = static_cast<vec3>(nearWorld / nearWorld[3]);
        vec3     far = static_cast<vec3>(farWorld / farWorld[3]);
        vec3    rayDir = mtls::normalize(near - far);
        /* check ray-ellipsoid intersection for all bones */
        for (auto it = obj.begin(); it != obj.end(); it++) {
            float t = rayEllipsoidIntersect(this->camera.getPosition(), rayDir, obj[it->first]->getModel()->getWorldPosition(), obj[it->first]->getModel()->getScale() + obj[it->first]->getModel()->getScaling());
            if (t > 0 and t < dist) {
                dist = t;
                objectId = it->first;
            }
        }
        /* reset the selected value of all models to false */
        for (auto it = obj.begin(); it != obj.end(); it++)
            (*this->env->getCharacter())[it->first]->getModel()->setSelected(false);
        /* set the selected one */
        if (objectId != "none")
            (*this->env->getCharacter())[objectId]->getModel()->setSelected(true);
    }
    /* key handling for scaling of the selected bone */
    for (auto it = obj.begin(); it != obj.end(); it++) {
        if ((*this->env->getCharacter())[it->first]->getModel()->getSelected())
            this->env->getCharacter()->scaleSelection(this->env->getController()->getKeys(), it->first);
    }
}

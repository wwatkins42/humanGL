#include "Renderer.hpp"

Renderer::Renderer( Env* environment ) :
    env(environment),
    camera(90, (float)env->getWindow().width / (float)env->getWindow().height, projection::perspective),
    shader("./shader/vertex.glsl", "./shader/fragment.glsl"),
    controller(environment) {
}

Renderer::Renderer( const Renderer& rhs ) :
    camera(90, (float)env->getWindow().width / (float)env->getWindow().height, projection::perspective),
    shader("./shader/vertex.glsl", "./shader/fragment.glsl"),
    controller(rhs.getEnv()) {
    *this = rhs;
}

Renderer& Renderer::operator=( const Renderer& rhs ) {
    (void)rhs;
    return (*this);
}

Renderer::~Renderer( void ) {
}

void	Renderer::loop( void ) {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->env->getWindow().ptr)) {
        glfwPollEvents();
        glClearColor(0.09f, 0.08f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->controller.keyHandler();
        this->controller.mouseHandler();
        this->shader.use();
        this->updateShaderUniforms();
        this->env->getAnimator()->update();
        this->env->getCharacter()->render(&this->shader);
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
}

void    Renderer::updateShaderUniforms( void ) {
    this->shader.setMat4UniformValue("view", this->camera.getViewMatrix());
    this->shader.setMat4UniformValue("projection", this->camera.getProjectionMatrix());
}

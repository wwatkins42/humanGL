#include "Renderer.hpp"

Renderer::Renderer( Env* env ) :
    env(env),
    camera(70, (float)env->getWindow().width / (float)env->getWindow().height),
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
        /* update controller keys */
        this->env->getController()->update();
        /* handle the keys for Animator and Camera */
        this->camera.handleKeys( this->env->getController()->getKeys(), this->env->getCharacter()->getParentBone()->getModel()->getPosition() );
        this->env->getAnimator()->handleKeys( this->env->getController()->getKeys() );

        this->shader.use();
        this->updateShaderUniforms();
        this->env->getAnimator()->update();
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
}

void    Renderer::updateShaderUniforms( void ) {
    this->shader.setMat4UniformValue("view", this->camera.getViewMatrix());
    this->shader.setMat4UniformValue("projection", this->camera.getProjectionMatrix());
}

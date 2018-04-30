#include "Renderer.hpp"

Renderer::Renderer( Env* env ) :
    env(env),
    camera(90, (float)env->getWindow().width / (float)env->getWindow().height),
    shader("./shader/vertex.glsl", "./shader/fragment.glsl") {
        this->env->getCharacter()->setShader(&this->shader);
}

Renderer::Renderer( const Renderer& rhs ) :
    camera(90, (float)env->getWindow().width / (float)env->getWindow().height),
    shader("./shader/vertex.glsl", "./shader/fragment.glsl") {
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
        this->env->getController()->update();

        // float radius = 10.0f;
        // float camX = std::sin(glfwGetTime()*3) * radius;
        // float camZ = std::cos(glfwGetTime()*3) * radius;
        // std::cout << vec3({camX, 10.0, camZ}) << std::endl;
        // mat4 view = mtls::lookAt(vec3({camX, 2, camZ}), vec3({0.0, 0.0, 0.0}));
        // this->camera.getViewMatrix() = view;

        // TMP: animation switch
        if (this->env->getController()->getKeyValue(GLFW_KEY_1))
            this->env->getAnimator()->selectAnim(0);
        else if (this->env->getController()->getKeyValue(GLFW_KEY_2))
            this->env->getAnimator()->selectAnim(1);
        else if (this->env->getController()->getKeyValue(GLFW_KEY_3))
            this->env->getAnimator()->selectAnim(2);
        else if (this->env->getController()->getKeyValue(GLFW_KEY_4))
            this->env->getAnimator()->selectAnim(3);

        this->shader.use();
        this->updateShaderUniforms();
        this->env->getAnimator()->update();
        // this->env->getCharacter()->render(&this->shader);
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
}

void    Renderer::updateShaderUniforms( void ) {
    this->shader.setMat4UniformValue("view", this->camera.getViewMatrix());
    this->shader.setMat4UniformValue("projection", this->camera.getProjectionMatrix());
}

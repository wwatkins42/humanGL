#include "Renderer.hpp"

Renderer::Renderer( Env* environment ) :
env(environment),
camera(90, (float)env->getWindow().width / (float)env->getWindow().height, projection::perspective),
shader("./shader/vertex.glsl", "./shader/fragment.glsl") {
}

Renderer::Renderer( const Renderer& rhs ) :
camera(60, (float)env->getWindow().width / (float)env->getWindow().height, projection::perspective),
shader("./shader/vertex.glsl", "./shader/fragment.glsl") {
    *this = rhs;
}

Renderer& Renderer::operator=( const Renderer& rhs ) {
    (void)rhs;
    return (*this);
}

Renderer::~Renderer( void ) {
}

void    Renderer::keyHandler( void ) {
	if (glfwGetKey(this->env->getWindow().ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->env->getWindow().ptr, GL_TRUE);
}

void	Renderer::loop( void ) {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->env->getWindow().ptr)) {
        glfwPollEvents();
        glClearColor(0.09f, 0.08f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->keyHandler();
        this->shader.use();
        this->updateShaderUniforms();
        this->env->getCharacter()->update(); // updates all the body parts model matrices
        this->env->getCharacter()->render(&this->shader); // render and update the model uniform in the shader at the same time
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
}

void    Renderer::updateShaderUniforms( void ) {
    float timeValue = glfwGetTime();

    /* playing with transformations */
    // mat4    trans;
    // trans.identity();
    // trans = mtls::translate(trans, vec3({ 0, 0, 1 }));
    // trans = mtls::rotate(trans, timeValue, vec3({ 0, 0, 1 }));
    // trans = mtls::rotate(trans, timeValue, vec3({ 1, 0, 0 }));
    // trans = mtls::rotate(trans, timeValue, vec3({ 0, 1, 0 }));
    // trans = mtls::scale(trans, vec3({ 0.5, 0.5, 0.5 }));
    // this->shader.setMat4UniformValue("model", trans);

    this->shader.setVecUniformValue(
        "customColor",
        0.5f,
        (std::sin(timeValue) / 2.0f) + 0.5f,
        (std::cos(timeValue)) + 0.5f,
        1.0f
    );
    // this->shader.setMat4UniformValue("model", trans); // This changes depending on the model
    this->shader.setMat4UniformValue("view", this->camera.getViewMatrix());
    this->shader.setMat4UniformValue("projection", this->camera.getProjectionMatrix());
}

// void	key_handle(t_env *env)
// {
// 	int	i;
//
// 	if (glfwGetKey(env->win.ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 		glfwSetWindowShouldClose(env->win.ptr, GL_TRUE);
// 	i = -1;
// 	while (++i < MAX_KEYS)
// 		env->key[i].code = glfwGetKey(env->win.ptr, i) == GLFW_PRESS ? 1 : 0;
// 	key_toggle(&env->key[MW], &env->mod.wireframe, GL_FILL, GL_LINE);
// 	key_toggle(&env->key[MF], &env->mod.focus, 0, 1);
// 	key_toggle(&env->key[MS], &env->mod.shading, 0, 1);
// 	key_toggle(&env->key[MC], &env->mod.color, 0, 1);
// 	key_toggle(&env->key[MG], &env->mod.greyscale, 0, 1);
// 	key_toggle(&env->key[MM], &env->mod.mapping, 0, 1);
// 	key_toggle(&env->key[MT], &env->mod.texture, 0, 1);
// 	key_action(env);
// }

// void	key_action(t_env *env)
// {
// 	if (env->key[MW].code)
// 		glPolygonMode(GL_FRONT_AND_BACK, env->mod.wireframe);
// 	if (env->key[ZP].code || env->key[ZM].code)
// 		camera_zoom(env);
// 	if (env->mod.focus)
// 		camera_center(env);
// 	model_move_demo(env);
// 	model_move_inertia(env, 0.9);
// 	camera_move_inertia(env, 0.93, FREE);
// 	camera_look_at_target(env);
// }

// void	key_toggle(t_key *key, short *var, int v0, int v1)
// {
// 	key->cooldown > 1 ? key->cooldown -= 1 : 0;
// 	if (key->code && key->cooldown <= 1)
// 	{
// 		if (*var == v1)
// 			*var = v0;
// 		else if (*var == v0)
// 			*var = v1;
// 		key->cooldown = COOLDOWN;
// 	}
// }

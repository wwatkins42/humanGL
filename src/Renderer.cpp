#include "Renderer.hpp"

Renderer::Renderer( Env * environment ) : env(environment) {
}

Renderer::Renderer( Renderer const & rhs ) {
    *this = rhs;
}

Renderer & Renderer::operator=( Renderer const & rhs ) {
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
    // load_obj(&env, env.model.filename);
    // load_bmp(&env, "chaton.bmp");
    // build_shader_program(&env);
    // create_buffers(&env, GL_DYNAMIC_DRAW);
    // glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->env->getWindow().ptr)) {
        glfwPollEvents();
        glClearColor(0.09f, 0.08f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->keyHandler();
        // this->env->getCharacter().updateFrame();
        this->env->getCharacter().render();
        std::exit(1);

        // env.sim.model = mat4_mul(env.model.translation, env.model.rotation);
        // glUseProgram(env.shader.program);
        // compute_mvp_matrix(&env);
        // update_shader_uniforms(&env);
        // glBindTexture(GL_TEXTURE_2D, env.buffer.texture);
        // glBindVertexArray(env.buffer.vao);
        // glDrawElements(GL_TRIANGLES, env.model.num_indices, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0);
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
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

#include "Renderer.hpp"

Renderer::Renderer( Env * environment ) : env(environment) {
    try {
        GLuint   vertexShader = Renderer::createShader("./shader/vertex.glsl", GL_VERTEX_SHADER);
        GLuint fragmentShader = Renderer::createShader("./shader/fragment.glsl", GL_FRAGMENT_SHADER);
        this->shaderProgram = Renderer::createShaderProgram( {{ vertexShader, fragmentShader }} );
    } catch (std::exception const & err) {
        std::cout << err.what() << std::endl;
    }
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
    // create_buffers(&env, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->env->getWindow().ptr)) {
        glfwPollEvents();
        glClearColor(0.09f, 0.08f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->keyHandler();
        // this->env->getCharacter().updateFrame();
        this->env->getCharacter().render();

        // env.sim.model = mat4_mul(env.model.translation, env.model.rotation);
        glUseProgram(this->shaderProgram); // maybe a shader program will be associated with a model to render
        // compute_mvp_matrix(&env);
        // update_shader_uniforms(&env);
        // glBindTexture(GL_TEXTURE_2D, env.buffer.texture);
        // glBindVertexArray(env.buffer.vao);
        // glDrawElements(GL_TRIANGLES, env.model.num_indices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
}

/* STATIC METHODS
   -------------- */
/*  we load the content of a file in a string (we need that because the shader compilation is done at
    runtime and glCompileShader expects a <const GLchar *> value)
*/
const std::string   Renderer::getShaderSource( std::string const & filename ) {
    std::ifstream   ifs(filename);
    std::string     content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return (content);
}

/*  we create the shader from a file in format glsl. The shaderType defines what type of shader it is
    and it returns the id to the created shader (the shader object is allocated by OpenGL in the back)
*/
GLuint  Renderer::createShader( std::string const & filename, GLenum shaderType ) {
	GLint success;
    const GLchar *shaderSource = Renderer::getShaderSource(filename).c_str();
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    Renderer::isCompilationSuccess(shader, success, shaderType);
	return (shader);
}

/*  here we create the shader program that will be used to render our objects. It takes a list of shaders
    that will instruct the GPU how to manage the vertices, etc... and we delete the compiled shaders at the
    end because we no longer need them. We return the id of the created shader program.
*/
GLuint  Renderer::createShaderProgram( std::forward_list<GLuint> const & shaders ) {
	GLint success;
	GLuint shaderProgram = glCreateProgram();
    for (std::forward_list<GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it)
        glAttachShader(shaderProgram, *it);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    Renderer::isCompilationSuccess(shaderProgram, success, -1);
    for (std::forward_list<GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it)
        glDeleteShader(*it);
	return (shaderProgram);
}

/*  check if the shader or shader program compilation returned an error, if so throw an exception
    with the message specified by glGetShaderInfoLog or glGetProgramInfoLog.
*/
void    Renderer::isCompilationSuccess( GLint handle, GLint success, int shaderType ) {
    if (!success) {
        char infoLog[512];
        if (shaderType != -1)
            glGetShaderInfoLog(handle, 512, nullptr, infoLog);
        else
            glGetProgramInfoLog(handle, 512, nullptr, infoLog);
        throw Exception::ShaderError(shaderType, infoLog);
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

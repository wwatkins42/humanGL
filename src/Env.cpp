#include "Env.hpp"

Env::Env( void ) {
    try {
        this->initGlfwEnvironment("4.0");
        this->initGlfwWindow(1280, 960);
    } catch (std::exception const & err) {
        std::cout << err.what() << std::endl;
    }
}

Env::Env( Env const & rhs ) {
    *this = rhs;
}

Env & Env::operator=( Env const & rhs ) {
    (void)rhs;
    return (*this);
}

Env::~Env( void ) {
    // glDeleteVertexArrays(1, &env->buffer.vao);
    // glDeleteBuffers(1, &env->buffer.vbo);
    // glDeleteBuffers(1, &env->buffer.ebo);
    glfwDestroyWindow(this->window.ptr);
    glfwTerminate();
}

void	Env::initGlfwEnvironment( std::string const & glVersion ) {
	if (!glfwInit())
		throw Exception::InitError("glfw initialization failed");
    if (!std::regex_match(glVersion, static_cast<std::regex>("^[0-9]{1}.[0-9]{1}$")))
        throw Exception::InitError("invalid openGL version specified");
    size_t  p = glVersion.find('.');
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, std::stoi(glVersion.substr(0,p)));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, std::stoi(glVersion.substr(p+1)));
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void	Env::initGlfwWindow( size_t width, size_t height ) {
	this->window.ptr = glfwCreateWindow(width, height, "humanGL", NULL, NULL);
	glfwMakeContextCurrent(this->window.ptr);
	glfwGetFramebufferSize(this->window.ptr, &this->window.width, &this->window.height);
	glViewport(0, 0, this->window.width, this->window.height);
	glfwSetInputMode(this->window.ptr, GLFW_STICKY_KEYS, 1);
}

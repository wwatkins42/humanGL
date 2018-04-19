#include "Env.hpp"

Env::Env( void ) : character() {
    try {
        this->initGlfwEnvironment("4.0");
        this->initGlfwWindow(1280, 960);
        // glad : load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw Exception::InitError("glad initialization failed");
        this->character = new Character();
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
	// glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);//GL_FALSE);
}

void	Env::initGlfwWindow( size_t width, size_t height ) {
	if (!(this->window.ptr = glfwCreateWindow(width, height, "humanGL", NULL, NULL)))
        throw Exception::InitError("glfwCreateWindow failed");
	glfwMakeContextCurrent(this->window.ptr);
	glfwSetFramebufferSizeCallback(this->window.ptr, this->framebufferSizeCallback);
	glfwSetInputMode(this->window.ptr, GLFW_STICKY_KEYS, 1);
}

void    Env::framebufferSizeCallback( GLFWwindow* window, int width, int height ) {
    glViewport(0, 0, width, height);
}

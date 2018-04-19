#include "Shader.hpp"

Shader::Shader( std::string const & vertexShader, std::string const & fragmentShader ) {
    GLuint vertShader = Shader::create(vertexShader, GL_VERTEX_SHADER);
    GLuint fragShader = Shader::create(fragmentShader, GL_FRAGMENT_SHADER);
    this->id = Shader::createProgram({{ vertShader, fragShader }});
}

Shader::Shader( Shader const & rhs ) {
    *this = rhs;
}

Shader & Shader::operator=( Shader const & rhs ) {
    (void)rhs;
    return (*this);
}

Shader::~Shader( void ) {
}

void    Shader::use( void ) const {
    glUseProgram(this->id);
}

/*  we load the content of a file in a string (we need that because the shader compilation is done at
    runtime and glCompileShader expects a <const GLchar *> value)
*/
const std::string   Shader::getFromFile( std::string const & filename ) {
    std::ifstream   ifs(filename);
    std::string     content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return (content);
}

/*  we create the shader from a file in format glsl. The shaderType defines what type of shader it is
    and it returns the id to the created shader (the shader object is allocated by OpenGL in the back)
*/
GLuint  Shader::create( std::string const & filename, GLenum shaderType ) {
	GLint success;
    const GLchar *shaderSource = Shader::getFromFile(filename).c_str();
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    Shader::isCompilationSuccess(shader, success, shaderType);
	return (shader);
}

/*  here we create the shader program that will be used to render our objects. It takes a list of shaders
    that will instruct the GPU how to manage the vertices, etc... and we delete the compiled shaders at the
    end because we no longer need them. We return the id of the created shader program.
*/
GLuint  Shader::createProgram( std::forward_list<GLuint> const & shaders ) {
	GLint success;
	GLuint shaderProgram = glCreateProgram();
    for (std::forward_list<GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it)
        glAttachShader(shaderProgram, *it);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    Shader::isCompilationSuccess(shaderProgram, success, -1);
    for (std::forward_list<GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it)
        glDeleteShader(*it);
	return (shaderProgram);
}

/*  check if the shader or shader program compilation returned an error, if so throw an exception
    with the message specified by glGetShaderInfoLog or glGetProgramInfoLog.
*/
void    Shader::isCompilationSuccess( GLint handle, GLint success, int shaderType ) {
    if (!success) {
        char infoLog[512];
        if (shaderType != -1)
            glGetShaderInfoLog(handle, 512, nullptr, infoLog);
        else
            glGetProgramInfoLog(handle, 512, nullptr, infoLog);
        throw Exception::ShaderError(shaderType, infoLog);
    }
}

/* not really good looking but it does the job */
template<>
void    Shader::setUniformValue<int>( std::string const & name, int x ) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), x);
}
template<>
void    Shader::setUniformValue<unsigned int>( std::string const & name, unsigned int x ) const {
    glUniform1ui(glGetUniformLocation(this->id, name.c_str()), x);
}
template<>
void    Shader::setUniformValue<float>( std::string const & name, float x ) const {
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), x);
}
template<>
void    Shader::setUniformValue<int>( std::string const & name, int x, int y ) const {
    glUniform2i(glGetUniformLocation(this->id, name.c_str()), x, y);
}
template<>
void    Shader::setUniformValue<unsigned int>( std::string const & name, unsigned int x, unsigned int y ) const {
    glUniform2ui(glGetUniformLocation(this->id, name.c_str()), x, y);
}
template<>
void    Shader::setUniformValue<float>( std::string const & name, float x, float y ) const {
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), x, y);
}
template<>
void    Shader::setUniformValue<int>( std::string const & name, int x, int y, int z ) const {
    glUniform3i(glGetUniformLocation(this->id, name.c_str()), x, y, z);
}
template<>
void    Shader::setUniformValue<unsigned int>( std::string const & name, unsigned int x, unsigned int y, unsigned int z ) const {
    glUniform3ui(glGetUniformLocation(this->id, name.c_str()), x, y, z);
}
template<>
void    Shader::setUniformValue<float>( std::string const & name, float x, float y, float z ) const {
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), x, y, z);
}
template<>
void    Shader::setUniformValue<int>( std::string const & name, int x, int y, int z, int w ) const {
    glUniform4i(glGetUniformLocation(this->id, name.c_str()), x, y, z, w);
}
template<>
void    Shader::setUniformValue<unsigned int>( std::string const & name, unsigned int x, unsigned int y, unsigned int z, unsigned int w ) const {
    glUniform4ui(glGetUniformLocation(this->id, name.c_str()), x, y, z, w);
}
template<>
void    Shader::setUniformValue<float>( std::string const & name, float x, float y, float z, float w ) const {
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), x, y, z, w);
}

#include "Shader.hpp"

Shader::Shader( const std::string& vertexShader, const std::string& fragmentShader ) {
    GLuint vertShader = Shader::create(vertexShader, GL_VERTEX_SHADER);
    GLuint fragShader = Shader::create(fragmentShader, GL_FRAGMENT_SHADER);
    this->id = Shader::createProgram({{ vertShader, fragShader }});
}

Shader::Shader( const Shader& rhs ) {
    *this = rhs;
}

Shader& Shader::operator=( const Shader& rhs ) {
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
const std::string   Shader::getFromFile( const std::string& filename ) {
    std::ifstream   ifs(filename);
    std::string     content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return (content);
}

/*  we create the shader from a file in format glsl. The shaderType defines what type of shader it is
    and it returns the id to the created shader (the shader object is allocated by OpenGL in the back)
*/
GLuint  Shader::create( const std::string& filename, GLenum shaderType ) {
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
GLuint  Shader::createProgram( const std::forward_list<GLuint>& shaders ) {
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

/*  find the uniform location in the shader and store it in an unordered_map.
    next time we want to use it we just have to get the location from the map
*/
unsigned int    Shader::getUniformLocation( const std::string& name ) {
    if (this->uniformLocations.find(name) != this->uniformLocations.end())
        return (this->uniformLocations[name]);
    unsigned int newLoc = glGetUniformLocation(this->id, name.c_str());
    this->uniformLocations[name] = newLoc;
    return (newLoc);
}

/* not really good looking but it does the job */
template<>
void    Shader::setVecUniformValue<int>( const std::string& name, int x ) {
    glUniform1i(getUniformLocation(name), x);
}
template<>
void    Shader::setVecUniformValue<unsigned int>( const std::string& name, unsigned int x ) {
    glUniform1ui(getUniformLocation(name), x);
}
template<>
void    Shader::setVecUniformValue<float>( const std::string& name, float x ) {
    glUniform1f(getUniformLocation(name), x);
}
template<>
void    Shader::setVecUniformValue<int>( const std::string& name, int x, int y ) {
    glUniform2i(getUniformLocation(name), x, y);
}
template<>
void    Shader::setVecUniformValue<unsigned int>( const std::string& name, unsigned int x, unsigned int y ) {
    glUniform2ui(getUniformLocation(name), x, y);
}
template<>
void    Shader::setVecUniformValue<float>( const std::string& name, float x, float y ) {
    glUniform2f(getUniformLocation(name), x, y);
}
template<>
void    Shader::setVecUniformValue<int>( const std::string& name, int x, int y, int z ) {
    glUniform3i(getUniformLocation(name), x, y, z);
}
template<>
void    Shader::setVecUniformValue<unsigned int>( const std::string& name, unsigned int x, unsigned int y, unsigned int z ) {
    glUniform3ui(getUniformLocation(name), x, y, z);
}
template<>
void    Shader::setVecUniformValue<float>( const std::string& name, float x, float y, float z ) {
    glUniform3f(getUniformLocation(name), x, y, z);
}
template<>
void    Shader::setVecUniformValue<int>( const std::string& name, int x, int y, int z, int w ) {
    glUniform4i(getUniformLocation(name), x, y, z, w);
}
template<>
void    Shader::setVecUniformValue<unsigned int>( const std::string& name, unsigned int x, unsigned int y, unsigned int z, unsigned int w ) {
    glUniform4ui(getUniformLocation(name), x, y, z, w);
}
template<>
void    Shader::setVecUniformValue<float>( const std::string& name, float x, float y, float z, float w ) {
    glUniform4f(getUniformLocation(name), x, y, z, w);
}

void    Shader::setMat2UniformValue( const std::string& name, const mat2& m ) {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_TRUE, m.getRawData());
}
void    Shader::setMat3UniformValue( const std::string& name, const mat3& m ) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_TRUE, m.getRawData());
}
void    Shader::setMat4UniformValue( const std::string& name, const mat4& m ) {
    // glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, m.getRawData());
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, m.getRawData());
}

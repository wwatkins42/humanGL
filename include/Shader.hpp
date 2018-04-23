#pragma once

#include <glad/glad.h>

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <forward_list>
#include <unordered_map>

#include "Exception.hpp"
#include "Matrix.hpp"

class Shader {

public:
    Shader( const std::string& vertexShader, const std::string& fragmentShader );
    Shader( const Shader& rhs );
    Shader& operator=( const Shader& rhs );
    ~Shader( void );

    std::string         getFromFile( const std::string& filename );
    // GLuint              create( const std::string& filename, GLenum shaderType );
    GLuint              create( const char* shaderSource, GLenum shaderType );
    GLuint              createProgram( const std::forward_list<GLuint>& shaders );
    void                isCompilationSuccess( GLint handle, GLint success, int shaderType );

    void                        use( void ) const;

    unsigned int                getUniformLocation( const std::string& name );

    template<typename T>
    void    setVecUniformValue( const std::string& name, T x );
    template<typename T>
    void    setVecUniformValue( const std::string& name, T x, T y );
    template<typename T>
    void    setVecUniformValue( const std::string& name, T x, T y, T z );
    template<typename T>
    void    setVecUniformValue( const std::string& name, T x, T y, T z, T w );
    /* save the uniform location in a hashMap for efficiency */

    void    setMat2UniformValue( const std::string& name, const mat2& m );
    void    setMat3UniformValue( const std::string& name, const mat3& m );
    void    setMat4UniformValue( const std::string& name, const mat4& m );

    GLuint  id;

private:
    std::unordered_map<std::string, unsigned int>   uniformLocations;

};

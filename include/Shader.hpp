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
    ~Shader( void );

    std::string         getFromFile( const std::string& filename );
    GLuint              create( const char* shaderSource, GLenum shaderType );
    GLuint              createProgram( const std::forward_list<GLuint>& shaders );
    void                isCompilationSuccess( GLint handle, GLint success, int shaderType );

    void                use( void ) const;

    unsigned int        getUniformLocation( const std::string& name );

    void                setMat2UniformValue( const std::string& name, const mat2& m );
    void                setMat3UniformValue( const std::string& name, const mat3& m );
    void                setMat4UniformValue( const std::string& name, const mat4& m );
    void                setVec2UniformValue( const std::string& name, const vec2& v );
    void                setVec3UniformValue( const std::string& name, const vec3& v );
    void                setVec4UniformValue( const std::string& name, const vec4& v );

    GLuint  id;

private:
    std::unordered_map<std::string, unsigned int>   uniformLocations;

};

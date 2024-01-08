#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    enum Status {
        SUCCESS,
        FAILED_LOADING_SHADER,
        FAILED_COMPILING_SHADER,
        FAILED_COMPILING_VERTEX,
        FAILED_COMPILING_FRAGMENT,
        FAILED_COMPILING_GEOMETRY,
        FAILED_COMPILING_TESSCONTROL,
        FAILED_COMPILING_TESSEVAL,
        FAILED_LINKING_SHADER,
        FAILED_COMPILING_PROGRAM
    };
    unsigned int ID;
    Shader ( ) { }
    Shader ( const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessControlPath = nullptr, const char* tessEvalPath = nullptr ) 
    { Make ( vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath ); }
    ~Shader ( ) { glDeleteProgram (ID); }

    int Make
    ( const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessControlPath = nullptr, const char* tessEvalPath = nullptr ) {
        std::string vertexCode, fragmentCode, geometryCode, tessControlCode, tessEvalCode;
        std::ifstream vShaderFile, fShaderFile, gShaderFile, tcShaderFile, teShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        tcShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        teShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if ( geometryPath != nullptr) {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
            if ( tessControlPath != nullptr ) {
                tcShaderFile.open(tessControlPath);
                std::stringstream tcShaderStream;
                tcShaderStream << tcShaderFile.rdbuf();
                tcShaderFile.close();
                tessControlCode = tcShaderStream.str();
            }
            if ( tessEvalPath != nullptr ) {
                teShaderFile.open(tessEvalPath);
                std::stringstream teShaderStream;
                teShaderStream << teShaderFile.rdbuf();
                teShaderFile.close();
                tessEvalCode = teShaderStream.str();
            }
        } catch ( std::ifstream::failure& e ) { return FAILED_LOADING_SHADER; }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        if ( checkCompileErrors(vertex, "VERTEX") != SUCCESS ) 
        { return FAILED_COMPILING_VERTEX; }
        
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        if ( checkCompileErrors ( fragment, "FRAGMENT" ) != SUCCESS ) 
        { return FAILED_COMPILING_FRAGMENT; }

        // if geometry shader is given, compile geometry shader
        unsigned int geometry = -1;
        if ( geometryPath != nullptr ) {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            if ( checkCompileErrors ( geometry, "GEOMETRY" ) != SUCCESS ) 
            { return FAILED_COMPILING_GEOMETRY; }
        }
        // if tessellation control shader is given, compile tessellation shader
        unsigned int tessControl = -1;
        if ( tessControlPath != nullptr ) {
            const char * tcShaderCode = tessControlCode.c_str();
            tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
            glShaderSource(tessControl, 1, &tcShaderCode, NULL);
            glCompileShader(tessControl);
            if ( checkCompileErrors(tessControl, "TESS_CONTROL") != SUCCESS ) 
            { return FAILED_COMPILING_TESSCONTROL; }

        }
        // if tessellation evaluation shader is given, compile tessellation shader
        unsigned int tessEval = -1;
        if ( tessEvalPath != nullptr ) {
            const char * teShaderCode = tessEvalCode.c_str();
            tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
            glShaderSource(tessEval, 1, &teShaderCode, NULL);
            glCompileShader(tessEval);
            if ( checkCompileErrors(tessEval, "TESS_EVALUATION") != SUCCESS ) 
            { return FAILED_COMPILING_TESSEVAL; }
        }

        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if ( geometryPath != nullptr ) { glAttachShader(ID, geometry); }
        if ( tessControlPath != nullptr ) { glAttachShader ( ID, tessControl ); }
        if ( tessEvalPath != nullptr ) { glAttachShader ( ID, tessEval ); }

        glLinkProgram(ID);
        if ( checkCompileErrors(ID, "PROGRAM") != SUCCESS ) { return FAILED_LINKING_SHADER; }
        
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if ( geometryPath != nullptr ) { glDeleteShader ( geometry ); }
        if ( geometryPath != nullptr ) { glDeleteShader ( tessControl ); }
        if ( geometryPath != nullptr ) { glDeleteShader ( tessEval ); }

        return SUCCESS;
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void Use() { glUseProgram(ID); }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) { Use(); glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) { Use(); glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) { Use(); glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) { Use(); glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
    void setVec2(const std::string &name, float x, float y) { Use(); glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) { Use(); glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
    void setVec3(const std::string &name, float x, float y, float z) { Use(); glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) { Use(); glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
    void setVec4(const std::string &name, float x, float y, float z, float w) { Use(); glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) { Use(); glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) { Use(); glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) { Use(); glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    int checkCompileErrors 
    ( GLuint shader, std::string type ) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- "
                    << std::endl;
                return FAILED_COMPILING_PROGRAM;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- "
                    << std::endl;
                return FAILED_COMPILING_SHADER; 
            }
        }
        return SUCCESS;
    }
};

#endif
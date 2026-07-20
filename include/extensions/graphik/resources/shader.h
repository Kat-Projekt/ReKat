#ifndef SHADER_H
#define SHADER_H

#include "../graphik_resource.hpp"
#include "../graphik_debugger.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader : public Resource {
private:

	unsigned int _id;
	// utility function for checking shader compilation/linking errors.
	// ----------------------------------------------------------------
	int checkCompileErrors ( GLuint shader, std::string type );

public:

	Shader ( );
	Shader (
		const char* vertexPath,
		const char* fragmentPath,
		const char* geometryPath = nullptr,
		const char* tessControlPath = nullptr,
		const char* tessEvalPath = nullptr
	);
	
	int Make ( );
	int Make (
		const char* vertexPath,
		const char* fragmentPath,
		const bool source = false,
		const char* geometryPath = nullptr,
		const char* tessControlPath = nullptr,
		const char* tessEvalPath = nullptr
	);
	// creates the shader pass paths
	// -----------------------------
	int Make_From_Path (
		const char* vertexPath,
		const char* fragmentPath,
		const char* geometryPath = nullptr,
		const char* tessControlPath = nullptr,
		const char* tessEvalPath = nullptr
	);
	// creates the shader pass source	
	// ------------------------------
    	int Make_From_Source (
		const char* vertexCode,
		const char* fragmentCode,
		const char* geometryCode = nullptr,
		const char* tessControlCode = nullptr,
		const char* tessEvalCode = nullptr
	);

	// activate the shader
	// -------------------
	void Use ( );

	// utility uniform functions
	// -------------------------
	void setBool ( const std::string &name, bool value );
	// ------------------------------------------------------------------------
	void setInt ( const std::string &name, int value );
	// ------------------------------------------------------------------------
	void setFloat ( const std::string &name, float value );
	// ------------------------------------------------------------------------
	void setVec2 ( const std::string &name, const glm::vec2 &value );
	void setVec2 ( const std::string &name, float x, float y );
	// ------------------------------------------------------------------------
	void setVec3 ( const std::string &name, const glm::vec3 &value );
	void setVec3 ( const std::string &name, float x, float y, float z );
	// ------------------------------------------------------------------------
	void setVec4 ( const std::string &name, const glm::vec4 &value );
	void setVec4 ( const std::string &name, float x, float y, float z, float w );
	// ------------------------------------------------------------------------
	void setMat2 ( const std::string &name, const glm::mat2 &mat );
	// ------------------------------------------------------------------------
	void setMat3 ( const std::string &name, const glm::mat3 &mat );
	// ------------------------------------------------------------------------
	void setMat4 ( const std::string &name, const glm::mat4 &mat );

	// deletes the shader
	// ------------------
	void End ( );
};

#endif
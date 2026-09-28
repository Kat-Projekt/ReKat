#include "extensions/graphik/resources/shader.h"

Shader::Shader
( ) { }
Shader::Shader
(
	const char* vertexPath,
	const char* fragmentPath,
	const char* geometryPath,
	const char* tessControlPath,
	const char* tessEvalPath
) 
{
	Make ( vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath );
}

void Shader::Use
( )
{
	if ( !glIsProgram(_id) )
	{
		DEBUG ( 1, "not a opengl program in this context" );
		return;
	}
	
	glUseProgram(_id); GL_CHECK_ERROR;
}

void Shader::End
( )
{ glDeleteProgram (_id); }
// utility uniform functions
// -------------------------
void Shader::setBool ( const std::string &name, bool value ) { Use ( ); glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value); GL_CHECK_ERROR; }
// ------------------------------------------------------------------------
void Shader::setInt ( const std::string &name, int value ) { Use ( ); glUniform1i(glGetUniformLocation(_id, name.c_str()), value); GL_CHECK_ERROR; }
// ------------------------------------------------------------------------
void Shader::setFloat ( const std::string &name, float value ) { Use ( ); glUniform1f(glGetUniformLocation(_id, name.c_str()), value); GL_CHECK_ERROR; }
// ------------------------------------------------------------------------
void Shader::setVec2 ( const std::string &name, const glm::vec2 &value ) { Use ( ); glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]); GL_CHECK_ERROR; }
void Shader::setVec2 ( const std::string &name, float x, float y ) { Use ( ); glUniform2f(glGetUniformLocation(_id, name.c_str()), x, y); GL_CHECK_ERROR; }
// ------------------------------------------------------------------------
void Shader::setVec3 ( const std::string &name, const glm::vec3 &value ) { Use ( ); glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]); GL_CHECK_ERROR; }
void Shader::setVec3 ( const std::string &name, float x, float y, float z ) { Use ( ); glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z); GL_CHECK_ERROR; }
// ------------------------------------------------------------------------
void Shader::setVec4 ( const std::string &name, const glm::vec4 &value ) { Use ( ); glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]); GL_CHECK_ERROR; }
void Shader::setVec4 ( const std::string &name, float x, float y, float z, float w ) { Use ( ); glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w); GL_CHECK_ERROR; }
// ------------------------------------------------------------------------
void Shader::setMat2 ( const std::string &name, const glm::mat2 &mat ) { Use ( ); glUniformMatrix2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]); GL_CHECK_ERROR; }
// ------------------------------------------------------------------------
void Shader::setMat3 ( const std::string &name, const glm::mat3 &mat ) { Use ( ); glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]); GL_CHECK_ERROR; }
// ------------------------------------------------------------------------
void Shader::setMat4 ( const std::string &name, const glm::mat4 &mat ) { Use ( ); glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]); GL_CHECK_ERROR; }

int Shader::Make
( )
{ return 1; }

int Shader::Make (
	const char* vertexPath,
	const char* fragmentPath,
	const bool source,
	const char* geometryPath,
	const char* tessControlPath,
	const char* tessEvalPath
) {
	if ( source )
	{ return Make_From_Source ( vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath ); }
	else
	{ return Make_From_Path ( vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath ); }
}

int Shader::Make_From_Path (
	const char* vertexPath,
	const char* fragmentPath,
	const char* geometryPath,
	const char* tessControlPath,
	const char* tessEvalPath
) {
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
	}
	catch ( std::ifstream::failure& e )
	{
		DEBUG ( 2, "Shader loading error ", vertexPath, " code: ", e.code() );
		return FAILED_LOADING_SHADER;
	}

	const char * vShaderCode = vertexCode.c_str();
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
	unsigned int geometry = 0;
	if ( geometryPath != nullptr ) {
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		if ( checkCompileErrors ( geometry, "GEOMETRY" ) != SUCCESS ) 
		{ return FAILED_COMPILING_GEOMETRY; }
	}
	// if tessellation control shader is given, compile tessellation shader
	unsigned int tessControl = 0;
	if ( tessControlPath != nullptr ) {
		const char * tcShaderCode = tessControlCode.c_str();
		tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tessControl, 1, &tcShaderCode, NULL);
		glCompileShader(tessControl);
		if ( checkCompileErrors(tessControl, "TESS_CONTROL") != SUCCESS ) 
		{ return FAILED_COMPILING_TESSCONTROL; }

	}
	// if tessellation evaluation shader is given, compile tessellation shader
	unsigned int tessEval = 0;
	if ( tessEvalPath != nullptr ) {
		const char * teShaderCode = tessEvalCode.c_str();
		tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tessEval, 1, &teShaderCode, NULL);
		glCompileShader(tessEval);
		if ( checkCompileErrors(tessEval, "TESS_EVALUATION") != SUCCESS ) 
		{ return FAILED_COMPILING_TESSEVAL; }
	}

	// shader Program
	_id = glCreateProgram();
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	if ( geometryPath != nullptr ) { glAttachShader(_id, geometry); }
	if ( tessControlPath != nullptr ) { glAttachShader ( _id, tessControl ); }
	if ( tessEvalPath != nullptr ) { glAttachShader ( _id, tessEval ); }

	glLinkProgram(_id);
	if ( checkCompileErrors(_id, "PROGRAM") != SUCCESS ) { return FAILED_LINKING_SHADER; }
	
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader ( vertex );
	glDeleteShader ( fragment );
	if ( geometryPath != nullptr ) { glDeleteShader ( geometry ); }
	if ( tessControlPath != nullptr ) { glDeleteShader ( tessControl ); }
	if ( tessEvalPath != nullptr ) { glDeleteShader ( tessEval ); }

	return SUCCESS;
}

int Shader::Make_From_Source (
	const char* vertexCode,
	const char* fragmentCode,
	const char* geometryCode,
	const char* tessControlCode,
	const char* tessEvalCode
) {
	unsigned int vertex = 0, fragment = 0, geometry = 0, tessControl = 0, tessEval = 0;
	// compile
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	if ( checkCompileErrors(vertex, "VERTEX") != SUCCESS ) 
	{ return FAILED_COMPILING_VERTEX; }
	
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	if ( checkCompileErrors ( fragment, "FRAGMENT" ) != SUCCESS ) 
	{ return FAILED_COMPILING_FRAGMENT; }

	if ( geometryCode != nullptr ) {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometryCode, NULL);
		glCompileShader(geometry);
		if ( checkCompileErrors ( geometry, "GEOMETRY" ) != SUCCESS ) 
		{ return FAILED_COMPILING_GEOMETRY; }
	}
	
	if ( tessControlCode != nullptr ) {
		tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tessControl, 1, &tessControlCode, NULL);
		glCompileShader(tessControl);
		if ( checkCompileErrors(tessControl, "TESS_CONTROL") != SUCCESS ) 
		{ return FAILED_COMPILING_TESSCONTROL; }

	}

	if ( tessEvalCode != nullptr ) {
		tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tessEval, 1, &tessEvalCode, NULL);
		glCompileShader(tessEval);
		if ( checkCompileErrors(tessEval, "TESS_EVALUATION") != SUCCESS ) 
		{ return FAILED_COMPILING_TESSEVAL; }
	}
	
	// create and link shader Program
	_id = glCreateProgram();
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	if ( geometryCode != nullptr ) { glAttachShader(_id, geometry); }
	if ( tessControlCode != nullptr ) { glAttachShader ( _id, tessControl ); }
	if ( tessEvalCode != nullptr ) { glAttachShader ( _id, tessEval ); }

	glLinkProgram(_id);
	if ( int error = checkCompileErrors(_id, "PROGRAM") != SUCCESS ) { return error; }

	// delete the shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if ( geometryCode != nullptr ) { glDeleteShader ( geometry ); }
	if ( tessControlCode != nullptr ) { glDeleteShader ( tessControl ); }
	if ( tessEvalCode != nullptr ) { glDeleteShader ( tessEval ); }

	return 0;
}

int Shader::checkCompileErrors
( GLuint shader, std::string type )
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			DEBUG ( 3, "Compile time ", infoLog );
			return FAILED_COMPILING_PROGRAM;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			DEBUG ( 3, "linking time ", infoLog );
			return FAILED_LINKING_SHADER;
		}
	}
	return SUCCESS;
}

#pragma once

#include <string>
#include <GL/glew.h>


// This class handles the compilation, 
// linking, and usage of a GLSL shader program.
// Reference: http://www.opengl.org/wiki/Shader_Compilation
namespace AE {

//-------------------------------------------------------------------
class GLSLProgram
{
	public:
//-------------------------------------------------------------------
    GLSLProgram();
    ~GLSLProgram();
   
	void compileShadersFromSource(const char* vertexSource,
								 const char* fragmentSource);
	//-----------------------------------
	void compileShaders(const std::string& vertexShaderFilePath, 
					    const std::string& fragmentShaderFilePath);
	//-----------------------------------
    void linkShaders();
    void addAttribute(const std::string& attributeName);
    GLint getUniformLocation(const std::string& uniformName);
    
	void use();
    void unuse();

	void dispose();
	private:
//-------------------------------------------------------------------
    void compileShader(const char* source, 
					   const std::string& name, 
					   GLuint id);
	//-----------------------------------
	int m_numAttributes = 0;
    GLuint m_programID = 0;
    GLuint m_vertexShaderID = 0;
    GLuint m_fragmentShaderID = 0;
};
}

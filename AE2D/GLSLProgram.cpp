#include "GLSLProgram.h"
#include "AEErrors.h"
#include "IOManager.h"

#include <vector>
#include <fstream>

//-------------------------------------------------------------------
AE::GLSLProgram::GLSLProgram() 
{ 

}

//-------------------------------------------------------------------
AE::GLSLProgram::~GLSLProgram() 
{

}

//-------------------------------------------------------------------
void AE::GLSLProgram::compileShadersFromSource(const char* vertexSource,
											   const char* fragmentSource)
//-----------------------------------
{
	// Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_programID = glCreateProgram();

    //Create the vertex shader object, and store its ID
    m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (m_vertexShaderID == 0) {
        fatalError("Vertex shader failed to be created!");
    }

    //Create the fragment shader object, and store its ID
    m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (m_fragmentShaderID == 0) {
        fatalError("Fragment shader failed to be created!");
    }

    //Compile each shader
    compileShader(vertexSource, "Vertex File", m_vertexShaderID);
    compileShader(fragmentSource, "Fragment File", m_fragmentShaderID);
}

//-------------------------------------------------------------------
//Compiles the shaders
void AE::GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, 
				const std::string& fragmentShaderFilePath) 
//-----------------------------------
{
	std::string vertSource;
	std::string fragSource;

	IOManager::readFileToBuffer(vertexShaderFilePath, vertSource);
	IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);


    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_programID = glCreateProgram();

    //Create the vertex shader object, and store its ID
    m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (m_vertexShaderID == 0) {
        fatalError("Vertex shader failed to be created!");
    }

    //Create the fragment shader object, and store its ID
    m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (m_fragmentShaderID == 0) {
        fatalError("Fragment shader failed to be created!");
    }

    //Compile each shader
    compileShader(vertSource.c_str(), vertexShaderFilePath, m_vertexShaderID);
    compileShader(fragSource.c_str(), fragmentShaderFilePath, m_fragmentShaderID);
}

//-------------------------------------------------------------------
void AE::GLSLProgram::linkShaders() 
{
    //Attach our shaders to our program
    glAttachShader(m_programID, m_vertexShaderID);
    glAttachShader(m_programID, m_fragmentShaderID);

    //Link our program
    glLinkProgram(m_programID);

    //Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

        //We don't need the program anymore.
        glDeleteProgram(m_programID);
        //Don't leak shaders either.
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);

        //print the error log and quit
        std::printf("%s\n", &(errorLog[0]));
        fatalError("Shaders failed to link!");
    }

    //Always detach shaders after a successful link.
    glDetachShader(m_programID, m_vertexShaderID);
    glDetachShader(m_programID, m_fragmentShaderID);
    glDeleteShader(m_vertexShaderID);
    glDeleteShader(m_fragmentShaderID);
}

//-------------------------------------------------------------------
//Adds an attribute to shader. Called between compiling and linking.
void AE::GLSLProgram::addAttribute(const std::string& attributeName) 
{
    glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
}

//-------------------------------------------------------------------
GLint AE::GLSLProgram::getUniformLocation(const std::string& uniformName) 
{
    GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
    if (location == (GLint)GL_INVALID_INDEX) {
        fatalError("Uniform " + uniformName + " not found in shader!");
    }
    return location;
}

//-------------------------------------------------------------------
//enable the shader, and all its attributes
void AE::GLSLProgram::use() 
{
    glUseProgram(m_programID);
    //enable all the attributes we added with addAttribute
    for (int i = 0; i < m_numAttributes; i++) {
        glEnableVertexAttribArray(i);
    }
}

//-------------------------------------------------------------------
void AE::GLSLProgram::unuse() //disable the shader
{
    glUseProgram(0);
    for (int i = 0; i < m_numAttributes; i++) {
        glDisableVertexAttribArray(i);
    }
}

//-------------------------------------------------------------------
void AE::GLSLProgram::dispose()
{
	if (m_programID) {
		glDeleteProgram(m_programID);
	}
}

//-------------------------------------------------------------------
//Compiles a single shader file
void AE::GLSLProgram::compileShader(const char* source, const std::string& name, GLuint id) 
{
    //tell opengl that we want to use fileContents as the contents of the shader file
    glShaderSource(id, 1, &source, nullptr);

    //compile the shader
    glCompileShader(id);

    //check for errors
    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

        //Provide the infolog in whatever manor you deem best.
        //Exit with failure.
        glDeleteShader(id); //Don't leak the shader.

        //Print error log and quit
        std::printf("%s\n", &(errorLog[0]));
        fatalError("Shader " + name + " failed to compile");
    }
}




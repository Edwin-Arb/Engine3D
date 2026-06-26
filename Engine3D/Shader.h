#pragma once
#include <glad/gl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Shader class for working with OpenGL shaders.
 */
class Shader
{
public:
    /**
     * @brief Constructs a shader program by compiling vertex and fragment shaders from files.
     * Constructor generates the shader on the fly.
     * 
     * @param InVertexPath Path to the vertex shader.
     * @param InFragmentPath Path to the fragment shader.
     */
    explicit Shader(const GLchar* InVertexPath, const GLchar* InFragmentPath);

    /**
     * @brief Destructor for the Shader class.
     * Cleans up OpenGL shader resources when the object is destroyed.
     */
    ~Shader();
    
    /**
     * @brief Deleted copy constructor.
     * Prevents copying of shader objects to avoid resource management issues.
     */
    Shader(const Shader&) = delete;
    
    /**
     * @brief Deleted assignment operator.
     * Prevents assignment of shader objects to avoid resource management issues.
     */
    Shader& operator=(const Shader&) = delete;
    
    /**
     * @brief Activates the shader.
     */
    void Use() const;

    /**
     * @brief Sets a boolean value for a shader parameter.
     * 
     * @param InName Name of the parameter.
     * @param InValue Value of the parameter.
     */
    void SetBool(const std::string& InName, GLboolean InValue) const;

    /**
     * @brief Sets an integer value for a shader parameter.
     * 
     * @param InName Name of the parameter.
     * @param InValue Value of the parameter.
     */
    void SetInt(const std::string& InName, GLint InValue) const;

    /**
     * @brief Sets a float value for a shader parameter.
     * 
     * @param InName Name of the parameter.
     * @param InValue Value of the parameter.
     */
    void SetFloat(const std::string& InName, GLfloat InValue) const;

private:
    /**
     * @brief Checks for compilation errors in the shader.
     * 
     * @param InShader Shader ID.
     * @param InType Type of shader (VERTEX_SHADER or FRAGMENT_SHADER).
     */
    void CheckCompileErrors(GLuint InShader, const std::string& InType) const;

private:
    GLuint ID = 0;
};

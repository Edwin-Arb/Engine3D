// GLAD must be included BEFORE GLFW - it provides the OpenGL headers.
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

void APIENTRY OpenGLDebugCallback(
    GLenum Source,
    GLenum Type,
    GLuint Id,
    GLenum Severity,
    GLsizei Length,
    const GLchar* Message,
    const void* UserParam)
{
    std::cerr << "\n========== OpenGL ==========\n" << "ID: " << Id << '\n' << "Severity: ";

    switch (Source)
    {
    case GL_DEBUG_SOURCE_API:
        {
            std::cout << "API";
            break;
        }
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        {
            std::cout << "Shader Compiler";
            break;
        }
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        {
            std::cout << "Window System";
            break;
        }
    }

    switch (Severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        {
            std::cerr << "HIGH";
            break;
        }
    case GL_DEBUG_SEVERITY_MEDIUM:
        {
            std::cerr << "MEDIUM";
            break;
        }
    case GL_DEBUG_SEVERITY_LOW:
        {
            std::cerr << "LOW";
            break;
        }
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        {
            std::cerr << "NOTIFICATION";
            break;
        }
    }

    std::cerr << "\nMessage: " << Message << "\n============================\n";
}

const char* VertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* FragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

static void FramebufferSizeCallback(GLFWwindow* InWindow, GLint InWidth, GLint InHeight)
{
    glViewport(0, 0, InWidth, InHeight);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* Window = glfwCreateWindow(1000, 1000, "Engine3D", nullptr, nullptr);
    if (!Window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

    // Load OpenGL function pointers via GLAD
    GLint Version = gladLoadGL(glfwGetProcAddress);
    if (Version == 0)
    {
        std::cerr << "Failed to initialize OpenGL context (GLAD)\n";
        glfwTerminate();
        return -1;
    }

    GLint Flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &Flags);

    if (Flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(OpenGLDebugCallback, nullptr);

        glDebugMessageControl(
            GL_DEBUG_SOURCE_API,
            GL_DEBUG_TYPE_OTHER,
            GL_DEBUG_SEVERITY_NOTIFICATION,
            0,
            nullptr,
            GL_FALSE);
    }

    std::cout << "OpenGL " << GLAD_VERSION_MAJOR(Version) << "." << GLAD_VERSION_MINOR(Version) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";

    // 1. Lesson 2 - Creating a vertex shader 
    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, nullptr);
    glCompileShader(VertexShader);

    GLint Success;
    GLchar InfoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(VertexShader, 512, nullptr, InfoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << "\n";
    }

    // 2. Lesson 2 - Creating a fragment shader (Color)
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, nullptr);
    glCompileShader(FragmentShader);

    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(FragmentShader, 512, nullptr, InfoLog);
        std::cerr << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << InfoLog << "\n";
    }

    // 3. Lesson 2 a shader program
    GLuint ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(ShaderProgram, 512, nullptr, InfoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << "\n";
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    // 4. Lesson 2 - Creating a shader
    float Vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<const void*>(0)); // TODO: check with a nullptr

    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // Main loop
    while (!glfwWindowShouldClose(Window))
    {
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(Window, GLFW_TRUE);
        }

        glClearColor(0.10f, 0.15f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 5. Lesson 2 - Draw triangles
        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0, 3);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ShaderProgram);

    glfwTerminate();
    return 0;
}

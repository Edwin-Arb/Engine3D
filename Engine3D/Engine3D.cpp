// GLAD must be included BEFORE GLFW - it provides the OpenGL headers.
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

constexpr GLint WIDTH_SCREEN  = 1000;
constexpr GLint HEIGHT_SCREEN = 1000;

// Called by the driver (when a debug context is active) whenever OpenGL has
// something to report. Turns the raw report into readable console output.
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

    switch (Severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "HIGH";         break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "MEDIUM";       break;
    case GL_DEBUG_SEVERITY_LOW:          std::cerr << "LOW";          break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "NOTIFICATION"; break;
    }

    std::cerr << "\nMessage: " << Message << "\n============================\n";
}

// --- Shader sources (GLSL) ---------------------------------------------------
// Vertex shader shared by both triangles: passes the position straight through.
const char* VertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}";

// Fragment shader for the orange triangle.
const char* FragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}";

// Fragment shader for the yellow triangle (exercise 3: a second program).
const char* FragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}";

// Keeps the OpenGL viewport in sync with the window size when it is resized.
static void FramebufferSizeCallback(GLFWwindow* InWindow, GLint InWidth, GLint InHeight)
{
    glViewport(0, 0, InWidth, InHeight);
}

int main()
{
    // --- GLFW, window and OpenGL context -------------------------------------
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

    GLFWwindow* Window = glfwCreateWindow(WIDTH_SCREEN, HEIGHT_SCREEN, "Engine3D", nullptr, nullptr);
    if (!Window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

    // Load OpenGL function pointers via GLAD.
    GLint Version = gladLoadGL(glfwGetProcAddress);
    if (!Version)
    {
        std::cerr << "Failed to initialize OpenGL context (GLAD)\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL " << GLAD_VERSION_MAJOR(Version) << "." << GLAD_VERSION_MINOR(Version) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";

    // --- Optional debug output (only if the context was created in debug mode) ---
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

    GLint Success = 0;
    GLchar InfoLog[512] = {};

    // ============================ Orange triangle ============================
    // --- Shader program ---
    // Vertex shader: create -> source -> compile -> check.
    const GLuint VertexShaderOrange = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderOrange, 1, &VertexShaderSource, nullptr);
    glCompileShader(VertexShaderOrange);

    glGetShaderiv(VertexShaderOrange, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(VertexShaderOrange, 512, nullptr, InfoLog);
        std::cerr << "ERROR::SHADER::VERTEX::ORANGE::COMPILATION_FAILED\n" << InfoLog << "\n";
    }

    // Fragment shader (orange color).
    Success = 0;
    const GLuint FragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderOrange, 1, &FragmentShaderSource, nullptr);
    glCompileShader(FragmentShaderOrange);

    glGetShaderiv(FragmentShaderOrange, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(FragmentShaderOrange, 512, nullptr, InfoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::ORANGE::COMPILATION_FAILED\n" << InfoLog << "\n";
    }

    // Program: create -> attach both shaders -> link -> check.
    Success = 0;
    const GLuint ShaderProgramOrange = glCreateProgram();
    glAttachShader(ShaderProgramOrange, VertexShaderOrange);
    glAttachShader(ShaderProgramOrange, FragmentShaderOrange);
    glLinkProgram(ShaderProgramOrange);

    glGetProgramiv(ShaderProgramOrange, GL_LINK_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(ShaderProgramOrange, 512, nullptr, InfoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::ORANGE::LINKING_FAILED\n" << InfoLog << "\n";
    }

    // The shaders are baked into the program now; the standalone objects can go.
    glDeleteShader(VertexShaderOrange);
    glDeleteShader(FragmentShaderOrange);

    // --- Geometry (right-hand triangle) ---
    GLfloat VerticesOrange[] =
    {
        0.8f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f,
        0.4f,  0.5f, 0.0f,
    };

    GLuint OrangeVBO, OrangeVAO;
    glGenBuffers(1, &OrangeVBO);
    glGenVertexArrays(1, &OrangeVAO);

    glBindVertexArray(OrangeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, OrangeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesOrange), VerticesOrange, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ============================ Yellow triangle ============================
    // --- Shader program ---
    // Vertex shader (same source as the orange one).
    Success = 0;
    const GLuint VertexShaderYellow = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderYellow, 1, &VertexShaderSource, nullptr);
    glCompileShader(VertexShaderYellow);

    glGetShaderiv(VertexShaderYellow, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(VertexShaderYellow, 512, nullptr, InfoLog);
        std::cerr << "ERROR::SHADER::VERTEX::YELLOW::COMPILATION_FAILED\n" << InfoLog << "\n";
    }

    // Fragment shader (yellow color).
    Success = 0;
    const GLuint FragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderYellow, 1, &FragmentShader2Source, nullptr);
    glCompileShader(FragmentShaderYellow);

    glGetShaderiv(FragmentShaderYellow, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(FragmentShaderYellow, 512, nullptr, InfoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::YELLOW::COMPILATION_FAILED\n" << InfoLog << "\n";
    }

    // Program.
    Success = 0;
    const GLuint ShaderProgramYellow = glCreateProgram();
    glAttachShader(ShaderProgramYellow, VertexShaderYellow);
    glAttachShader(ShaderProgramYellow, FragmentShaderYellow);
    glLinkProgram(ShaderProgramYellow);

    glGetProgramiv(ShaderProgramYellow, GL_LINK_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(ShaderProgramYellow, 512, nullptr, InfoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::YELLOW::LINKING_FAILED\n" << InfoLog << "\n";
    }

    glDeleteShader(VertexShaderYellow);
    glDeleteShader(FragmentShaderYellow);

    // --- Geometry (left-hand triangle) ---
    GLfloat VerticesYellow[] =
    {
        -0.8f, -0.5f, 0.0f,
         0.0f, -0.5f, 0.0f,
        -0.4f,  0.5f, 0.0f,
    };

    GLuint YellowVBO, YellowVAO;
    glGenBuffers(1, &YellowVBO);
    glGenVertexArrays(1, &YellowVAO);

    glBindVertexArray(YellowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, YellowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesYellow), VerticesYellow, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --- Render loop ---------------------------------------------------------
    while (!glfwWindowShouldClose(Window))
    {
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(Window, GLFW_TRUE);
        }

        glClearColor(0.10f, 0.15f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Orange triangle: its own program + VAO.
        glUseProgram(ShaderProgramOrange);
        glBindVertexArray(OrangeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Yellow triangle: its own program + VAO.
        glUseProgram(ShaderProgramYellow);
        glBindVertexArray(YellowVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    // --- Cleanup -------------------------------------------------------------
    glDeleteBuffers(1, &OrangeVBO);
    glDeleteVertexArrays(1, &OrangeVAO);
    glDeleteProgram(ShaderProgramOrange);

    glDeleteBuffers(1, &YellowVBO);
    glDeleteVertexArrays(1, &YellowVAO);
    glDeleteProgram(ShaderProgramYellow);

    glfwTerminate();
    return 0;
}

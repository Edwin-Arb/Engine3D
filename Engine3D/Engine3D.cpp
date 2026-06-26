// GLAD must be included BEFORE GLFW - it provides the OpenGL headers.
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

constexpr GLint WIDTH_SCREEN = 1000;
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
    case GL_DEBUG_SEVERITY_HIGH: std::cerr << "HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM: std::cerr << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW: std::cerr << "LOW";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "NOTIFICATION";
        break;
    }

    std::cerr << "\nMessage: " << Message << "\n============================\n";
}

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

    // Shaders now carry the color per vertex, so one program drives both triangles.
    Shader OurShaderProgram("Shaders/3.3.Shader.vs", "Shaders/3.3.Shader.fs");

    // --- Geometry (right-hand triangle) ---
    GLfloat VerticesOrange[] =
    {
        0.8f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.4f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    GLuint OrangeVBO = 0;
    GLuint OrangeVAO = 0;
    glGenBuffers(1, &OrangeVBO);
    glGenVertexArrays(1, &OrangeVAO);

    glBindVertexArray(OrangeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, OrangeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesOrange), VerticesOrange, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --- Geometry (left-hand triangle) ---
    GLfloat VerticesYellow[] =
    {
        // positions        // colors
        -0.8f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.4f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    GLuint YellowVBO = 0;
    GLuint YellowVAO = 0;
    glGenBuffers(1, &YellowVBO);
    glGenVertexArrays(1, &YellowVAO);

    glBindVertexArray(YellowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, YellowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesYellow), VerticesYellow, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

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

        // One shader program for both triangles; the color comes from the vertices.
        OurShaderProgram.Use();

        // Right-hand triangle: bind its VAO and draw.
        glBindVertexArray(OrangeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Left-hand triangle: same shader, its own VAO.
        glBindVertexArray(YellowVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    // --- Cleanup -------------------------------------------------------------
    glDeleteBuffers(1, &OrangeVBO);
    glDeleteVertexArrays(1, &OrangeVAO);

    glDeleteBuffers(1, &YellowVBO);
    glDeleteVertexArrays(1, &YellowVAO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

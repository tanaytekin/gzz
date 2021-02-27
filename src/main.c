

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"

/* temp */
#include "shader.h"


/* TODO : Do a proper setup */
void GLAPIENTRY gzz_opengl_debug_callback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                           const GLchar *message, const void *userParam)
{
    if(type == GL_DEBUG_TYPE_ERROR)
    {
        GZZ_LOG_ERROR("OpenGL error | severity : 0x%x\n%s", severity, message);
    }
    else
    {
        /* TODO : For now */
        return;
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
            case GL_DEBUG_SEVERITY_MEDIUM:
                GZZ_LOG_WARNING("OpenGL warning | severity : 0x%x\n%s", severity, message);
                break;

            case GL_DEBUG_SEVERITY_LOW:
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                GZZ_LOG_INFO("OpenGL info | severity : 0x%x\n%s", severity, message);
                break;
        }
    }

}



int main()
{
    /* Init GLFW */
    if(glfwInit() != GLFW_TRUE)
    {
        GZZ_LOG_FATAL("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow *window = glfwCreateWindow(800, 600, "gzz", NULL, NULL);

    if(window == NULL)
    {
        GZZ_LOG_FATAL("Failed to initialize window.");
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        GZZ_LOG_FATAL("Failed to initialize glad.");
    }


    /* Enable OpenGL debugging */
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gzz_opengl_debug_callback, NULL);




    u32 shader = gzz_shader_create("../shaders/vertex.glsl", "../shaders/fragment.glsl");



    /* Main Loop */
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glClearColor(1.0f, 0.4f, 0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
    }

    gzz_shader_destroy(shader);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

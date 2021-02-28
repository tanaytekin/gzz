#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "log.h"

/* temp */
#include "shader.h"
#include "batch.h"
#include <time.h>


#define SCR_WIDTH 1280
#define SCR_HEIGHT 720
#define W_TITLE "Gazoz"

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

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


typedef struct vertex
{
    vec2 position;
    vec4 color;
}vertex;


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


    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, W_TITLE, NULL, NULL);

    if(window == NULL)
    {
        GZZ_LOG_FATAL("Failed to initialize window.");
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        GZZ_LOG_FATAL("Failed to initialize glad.");
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSwapInterval(0);

    /* Enable OpenGL debugging */
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gzz_opengl_debug_callback, NULL);




    u32 shader = gzz_shader_create("../shaders/vertex.glsl", "../shaders/fragment.glsl");

    mat4 projection;
    glm_ortho(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f, projection);

    gzz_shader_use(shader);
    gzz_shader_set_mat4(shader, "projection", projection);


#define QUAD_COUNT 100000
    gzz_batch *batch = gzz_batch_create(QUAD_COUNT);

    srand(time(NULL));


    f32 delta_time = 0.0f;
    f32 last_time = 0.0f;
    /* Main Loop */
    while(!glfwWindowShouldClose(window))
    {
        f32 current_time = (f32)glfwGetTime();
        delta_time = current_time -last_time;
        last_time = current_time;

        fprintf(stderr, "%f\n", 1.0f/delta_time);


        glfwSwapBuffers(window);
        glClearColor(0.0f, 0.0f, 0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(u32 i=0; i<QUAD_COUNT; i++)
        {
            f32 x = (f32)rand()/RAND_MAX * 16.0f;
            f32 y = (f32)rand()/RAND_MAX * 9.0f;

            f32 r = (f32)rand()/RAND_MAX;
            f32 g = (f32)rand()/RAND_MAX;
            f32 b = (f32)rand()/RAND_MAX;


            gzz_batch_add_rectangle(batch,x, y, 0.1f, 0.1f, r, g, b, 1.0f);
        }
        gzz_batch_draw(batch);


        glfwPollEvents();
    }

    gzz_shader_destroy(shader);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

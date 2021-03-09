

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include "log.h"

/* temp */
#include "shader.h"
#include "texture2d.h"
#include "batch_renderer.h"


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


    GLFWwindow *window = glfwCreateWindow(1280, 720, "gzz", NULL, NULL);

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

    /* Enable OpenGL debugging */
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gzz_opengl_debug_callback, NULL);


    srand(NULL);
    glfwSwapInterval(0);




    gzz_texture2d texture = gzz_texture2d_create("../res/wall.jpg");
    gzz_texture2d texture2 = gzz_texture2d_create("../res/wall2.jpg");

    gzz_texture_region region;
    region.uv[0] = 0.0f;
    region.uv[1] = 0.0f;
    region.uv2[0] = 1.0f;
    region.uv2[1] = 1.0f;
    region.texture = &texture;

    gzz_texture_region region2;
    region2.uv[0] = 0.0f;
    region2.uv[1] = 0.0f;
    region2.uv2[0] = 1.0f;
    region2.uv2[1] = 1.0f;
    region2.texture = &texture2;

    gzz_batch_renderer_performance_info performance_info = {0};

#define MAX_QUADS 20000
    gzz_batch_renderer *batch = gzz_batch_renderer_create(MAX_QUADS, &performance_info);



    f32 last_time = 0.0f;
    f32 delta_time = 0.0f;


    /* Main Loop */
    while(!glfwWindowShouldClose(window))
    {



        glfwSwapBuffers(window);
        glClearColor(1.0f, 0.4f, 0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        gzz_batch_renderer_begin(batch);

#define NUM_QUADS 5000

        for(u32 i=0; i<NUM_QUADS; i++)
        {
            gzz_sprite sprite;
            sprite.size[0] = 0.5f;
            sprite.size[1] = 0.5f;
            sprite.texture_region = &region;
            sprite.position[0] = (f32)rand()/(f32)RAND_MAX * 16.0f;
            sprite.position[1] = (f32)rand()/(f32)RAND_MAX * 9.0f;

            gzz_sprite sprite2;
            sprite2.size[0] = 0.5f;
            sprite2.size[1] = 0.5f;
            sprite2.texture_region = &region2;
            sprite2.position[0] = (f32)rand()/(f32)RAND_MAX * 16.0f;
            sprite2.position[1] = (f32)rand()/(f32)RAND_MAX * 9.0f;
            gzz_batch_renderer_add_sprite(batch, &sprite);
            gzz_batch_renderer_add_sprite(batch, &sprite2);

        }
        gzz_batch_renderer_end(batch);
        gzz_batch_renderer_flush(batch);


        glfwPollEvents();

        f32 current_time = (f32)glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;
        fprintf(stderr, "FPS : %f , Quads : %d , Max Quads : %d , Draw Calls : %d\n",1.0f/delta_time,performance_info.quad_count,MAX_QUADS, performance_info.draw_call_count);
        performance_info.draw_call_count = 0;
        performance_info.quad_count = 0;
    }


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

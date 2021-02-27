#include <glad/glad.h>
#include <GLFW/glfw3.h>


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "gzz", NULL, NULL);

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        return 1;
    }


    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glClearColor(1.0f, 0.4f, 0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}

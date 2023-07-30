#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

//#include "main_window/main_window.h"

int main(void)
{
    GLFWwindow* window;

    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create Window (GLFW)
    window = glfwCreateWindow(640, 480, "OepnGL Demo", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(err) << "" << std::endl;
    }
    else {
        std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << "" << std::endl;
    }

    while (!glfwWindowShouldClose(window)) {
        /* Game loop */
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

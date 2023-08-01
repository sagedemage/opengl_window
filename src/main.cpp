/* Standard Libaries */
#include <iostream>
#include <string>

/* OpenGL Libraries */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* SDL_mixer Library */
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

//#include "main_window/main_window.h"

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

// Shader Source Code (GLSL code)
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

/* SDL_mixer */
const int music_volume = 12;
const int chunksize = 1024;
const char *music_path = "music/square.ogg";

int main(void)
{
    GLFWwindow* window;

    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create Window (GLFW)
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OepnGL Demo", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW which loads all OpenGL functions
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(err) << "" << std::endl;
    }
    else {
        std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << "" << std::endl;
    }

    /* Build and compile the Shader */
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error: Vertex Shader Compilation Failed" << infoLog << std::endl;
    }

    // frament shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error: Fragment Shader Compilation Failed" << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error: Program Shader Compilation Failed" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /*
     * setup vertex data and buffers
     * configure vertex attributes
     */

    float vertices[] = {
        // positions        // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);

    /*
     * Audio
     */

    /* Open Audio using SDL_mixer */

    int open_audio_status = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, chunksize);

    if (open_audio_status == -1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
    }

    Mix_Music *music = Mix_LoadMUS(music_path);

    /* Play Music Theme */
    int music_status = Mix_PlayMusic(music, -1);

    if (music_status == -1) {
        std::cout << "Mix_PlayMusic: " << Mix_GetError() << "" << std::endl;
    }

    Mix_VolumeMusic(music_volume);


    while (!glfwWindowShouldClose(window)) {
        /* Game loop */

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Dealocate all resources once they have outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Dealocate Music and Audio
    Mix_FreeMusic(music);
    Mix_CloseAudio();

    // Terminates the GLFW library
    glfwTerminate();

    return 0;
}

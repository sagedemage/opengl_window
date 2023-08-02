#include "pch/opengl_demo-pch.h"
#include "audio/audio.h"

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

std::string get_shader_code(std::string shader_file) {
    // get a string of the shader source code from a file
    std::ifstream read_file(shader_file);

    std::string shader_code;
    std::string line;

    while (getline (read_file, line)) {
        // add line to the string
        shader_code += line + "\n";
    }

    read_file.close();

    return shader_code;
}

int main(void)
{
    /* SDL_mixer */
    const int music_volume = 12;
    const int channels = 2;
    const int chunksize = 1024;
    const char *music_path = "music/square.ogg";

    /* Shader File Path */
    const char *vertex_shader_path = "shader/shader.vert";
    const char *fragment_shader_path = "shader/shader.frag";

    /* Shader Source Code (GLSL code) */
    std::string vertex_shader_string = get_shader_code(vertex_shader_path);
    std::string fragment_shader_string = get_shader_code(fragment_shader_path);
    const char *vertex_shader_source = vertex_shader_string.c_str();
    const char *fragment_shader_source = fragment_shader_string.c_str();

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

    glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error: Vertex Shader Compilation Failed: " << infoLog << std::endl;
    }

    // frament shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error: Fragment Shader Compilation Failed: " << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error: Program Shader Compilation Failed: " << infoLog << std::endl;
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

    // Open audio using SDL_mixer
    Audio audio(channels, chunksize);

    // Load and play music
    audio.loadMusic(music_path);
    audio.playMusic();

    audio.changeVolume(music_volume);

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
    audio.freeResources();

    // Terminates the GLFW library
    glfwTerminate();

    return 0;
}

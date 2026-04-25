#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.h"
#include "stb_image.hpp"

//---------- Function Declarations -----------//
void processInput(GLFWwindow*);
void frameBufferSizeCallback(GLFWwindow*, int, int);

int main() {

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "My App", nullptr, nullptr
        );
    if (window == nullptr) {
        std::cerr << "GLFW Couldn't initialise window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);

    Shader shader("../res/shaders/basic.shader");

    float positions[] = {
        // positions            // Colors           //texture coordinates
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,      // bottom left
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,      // bottom right
         0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f,      // top right
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f       // top left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    // VertexArrayObject
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VertexBufferObject
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // VertexAttributeArrays
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //ElementBufferObject
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // Texture 1
    unsigned int texture1;
    glGenTextures(1, &texture1);

    // Put texture in slot 0 then bind it to that state.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
    //loading the texture
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../res/textures/container.jpg", &width, &height, &nChannels, 0);

    if (data!=nullptr)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } 
    else
    {
        std::cout << "Failed to load the texture." << std::endl;
    }

    stbi_image_free(data);

    // Texture 2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Put texture in slot 1 then bind it to that state.
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    data = stbi_load("../res/textures/awesomeface.png", &width, &height, &nChannels, 0);
    if (data!=nullptr)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } 
    else
    {
        std::cout << "Failed to load the image." << std::endl;
    }
    stbi_image_free(data);

    shader.Bind();
    shader.setUniform1i("texture1", 0);
    shader.setUniform1i("texture2", 1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    //CLEAN UP ZONE
    glfwTerminate();
}

void processInput(GLFWwindow* window_) {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, GL_TRUE);
    }
}

void frameBufferSizeCallback(GLFWwindow* window_, int width_, int height_) {
    glViewport(0, 0, width_, height_);
    std::cout << "WIDTH [" << width_ << "]  HEIGHT [" << height_ << "]\n";
}
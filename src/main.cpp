#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.h"


//---------- Function Declarations -----------//
void processInput(GLFWwindow*);
void frameBufferSizeCallback(GLFWwindow*, int, int);

int main() {

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    /*
     *We are now enabling the core profile, meaning that we have to specify our
     *own shaders and Vertex Array Object
     *
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    // We need a window we can render unto, so yeah create a window.
    GLFWwindow* window = glfwCreateWindow(800, 600, "My App", nullptr, nullptr
        );
    if (window == nullptr) {
        std::cerr << "GLFW Couldn't initialise window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    /*
     * Register a call back that will get called when the window resize event happens
     *
     * Callbacks is just like leaving your phone number at a busy restaurant, and
     * then they will call you when there is free tables, instead of you keeping
     * an eye after every five minutes.
     *
     * The phone Number --> A certain predefined interface they can reach you through.
     *
     * A callback is predefined, so you have to take in the same parameters as it,
     * then do your code inside it.
     */
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    // This initializes Glad so we can call any OpenGL function now
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // This specifies the size of the rendering window, if we pass other coordinates OPENGL
    //      will only see that portion of the window we actually created.
    glViewport(0, 0, 800, 600);

    Shader shader("res/shaders/basic.shader");

    // now we want to draw a square, let us draw two triangles positioned together.
    float positions[] = {
        -0.5f, -0.5f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f, // bottom right
         0.5f,  0.5f, 0.0f, // top right
         -0.5f, 0.5f, 0.0f, // top left
    };

    unsigned int indices[] = {
        0, 1, 2, //-> Bottom left, bottom right, Top right
        2, 3, 0 // -> Top right, top left, bottom left
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    //CLEAN UP ZONE
    glfwTerminate();
}

// Listens for input events
void processInput(GLFWwindow* window_) {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, GL_TRUE);
    }
}

//The call back that gets called when a window(frame buffer) gets resized,
// so as to set a new viewPort.
void frameBufferSizeCallback(GLFWwindow* window_, int width_, int height_) {
    glViewport(0, 0, width_, height_);
    std::cout << "WIDTH [" << width_ << "]  HEIGHT [" << height_ << "]\n";
}
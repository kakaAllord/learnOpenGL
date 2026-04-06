#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


//---------- Function Declarations -----------//
void processInput(GLFWwindow*);
void frameBufferSizeCallback(GLFWwindow*, int, int);

int main() {

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    //
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    float positions[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f,  -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        //code here

        glDrawArrays(GL_TRIANGLES, 0, 3);

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
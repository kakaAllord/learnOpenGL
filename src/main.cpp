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


    //A shader is just a program that runs in the GPU

    /*
     *The Vertex shader is responsible with handling the position of vertices.
     */
    const char* VertexShadersource =
        "#version 330 core\n"
        "\n"
        "in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(aPos, 1.0f);"
        "}\n";

    /*
     *The Fragment Shader is responsible with handing the color stuff
     */
    const char* FragmentShaderSource =
        "#version 330 core\n"
        "\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0f, 0.0f, 0.0f, 1.0f);"
        "}\n";

    // Compile the shader so we can link it and create a Shader Program later
    unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShadersource, nullptr);
    glCompileShader(VertexShader);

    // Checking for compile status
    int vertex_compiled;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &vertex_compiled);

    // Printing error in case of one in the shader
    if (!vertex_compiled) {
        char message[1024];
        glGetShaderInfoLog(VertexShader, 1024, nullptr, message);
        std::cerr << "Faile to compile Vertex Shader: " << message << std::endl;
    }

    // Compiling the FragmentShader
    unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, nullptr);
    glCompileShader(FragmentShader);

    //Fragment Shader Error Handling
    int fragment_compiled;
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &fragment_compiled);
    if (!fragment_compiled) {
        char message[1024];
        glGetShaderInfoLog(FragmentShader, 1024, nullptr, message);
        std::cerr << "Failed to compile Fragment Shader : " << message << std::endl;
    }

    // He we create a program then link the shaders/
    unsigned int ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);

    glLinkProgram(ShaderProgram);
    glValidateProgram(ShaderProgram);

    // now we want to draw a square, let us draw two triangles positioned together.
    float positions[] = {
        // First triangle
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,

        // Second triangle
         0.5f,  0.5f, 0.0f,
         -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,

    };

    /*
     *The Vertex Array Object handles the Array Attributes of the
     *current bound VBO
     *
     */
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

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        // We then bind our shaderProgram and the Vertex Array before drawing our triangles
        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);

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
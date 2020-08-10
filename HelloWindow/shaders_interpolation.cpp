/*

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//source code for the vertex shader
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos; //the position variable has attribute position 0\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main(){\n"
"   gl_Position = vec4(aPos, 1.0); //see how we directly give a vec3 to vec4's constructor\n"
"   ourColor = aColor;\n"
"}\0";

//source code for the fragment shader
const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main(){\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\0";

//glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    //glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfWindowHint(GLFW_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

    //glfw: window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int success;
    char infoLog[512];

    //vertex shader creation
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //attach the shader source code to the shader object and compile it
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //check if the compilation was successful
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //fragment shader creation
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //attach the shader source code to the shader object and compile it
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //check if the compilation was successful
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //shader program creation (link shaders)
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //attach previously compiled shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //check if the the linking was successful
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //We do not the the shader objects anymore, so we can delete them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //set up vertex data and configure vertex attributes
    float vertices[] = {
        //positions          //colors        
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, //bottom left
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, //bottom right
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  //top
    };

    unsigned VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //bind the vertex array object
    glBindVertexArray(VAO);

    //bind the vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //copy our vertices array in a buffer for OpenGL to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //set the vertex attributes pointers
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //note that this is allowed, the call to the glVertexAttributePointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind 
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //Getting the number of attributes I am allowed to have
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    //render loop
    while (!glfwWindowShouldClose(window)) {
        //input
        processInput(window);

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glViewport(0, 0, 500, 500);
    
        //drawing
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); //seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        //glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}

*/
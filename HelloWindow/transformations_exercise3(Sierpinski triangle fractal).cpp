/*

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <shader/Shader.h>

#include <iostream>

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void drawInsideTriangles(Shader, unsigned int, int, float, float, float);

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

    Shader myShader("vertex_shader_for_transformations_exercise3.txt", "fragment_shader_for_transformations_exercise3.txt");

    unsigned int texture0, texture1;
    glGenTextures(1, &texture0);
    glGenTextures(1, &texture1);

    glBindTexture(GL_TEXTURE_2D, texture0);

    //set texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //set both texture and mipmap filtering options at the same time
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resources/textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        //link the previously loaded image data to the currently bound texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture0" << std::endl;
    }
    //free the image memory
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, texture1);

    //set texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //set both texture and mipmap filtering options at the same time
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load and generate the texture
    data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        //link the previously loaded image data to the currently bound texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture1" << std::endl;
    }
    //free the image memory
    stbi_image_free(data);


    //set up vertex data and configure vertex attributes
    float vertices[] = {
        //positions          //colors           //texture coords
        -1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
         1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //bottom right
         0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f  //top
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    //note that this is allowed, the call to the glVertexAttributePointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //tell opengl for each sampler to wich texture unit it belong to
    myShader.use();
    myShader.setInt("texture0", 0);
    myShader.setInt("texture1", 1);

    //render loop
    while (!glfwWindowShouldClose(window)) {
        //input
        processInput(window);

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glViewport(0, 0, 500, 500);

        //bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);


        //create transformations
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

        //set matrix to the uniform
        myShader.use();
        myShader.setMat4("transform", trans);

        //changing the value to the uniform that defines the color
        float timeValue = (float)glfwGetTime();
        float redValue = (sin(timeValue) / 2.0f) + 0.5f;
        float greenValue = (cos(timeValue) / 2.0f) + 0.5f;
        float blueValue = (sin(timeValue) / 2.0f) + 0.5f;
        myShader.use();
        myShader.setVec4("changingColor", redValue, greenValue, blueValue, 1.0f);

        //drawing
        myShader.use();
        glBindVertexArray(VAO); //seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        drawInsideTriangles(myShader, VAO, 6, 0.5, 0.0, 0.0);

        //glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}

void drawInsideTriangles(Shader myShader, unsigned int VAO, int level, float shrink_val, float dx, float dy) {
    if (level != 0) {
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-shrink_val, -shrink_val, 0.0f));
        trans = glm::translate(trans, glm::vec3(dx, dy, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime() + level, glm::vec3(0.0f, 1.0f, 0.0));
        trans = glm::scale(trans, glm::vec3(shrink_val, shrink_val, 1.0));
        myShader.use();
        myShader.setMat4("transform", trans);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(shrink_val, -shrink_val, 0.0));
        trans = glm::translate(trans, glm::vec3(dx, dy, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime() + level, glm::vec3(0.0f, 1.0f, 0.0f));
        trans = glm::scale(trans, glm::vec3(shrink_val, shrink_val, 1.0));
        myShader.use();
        myShader.setMat4("transform", trans);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.0, shrink_val, 0.0));
        trans = glm::translate(trans, glm::vec3(dx, dy, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime() + level, glm::vec3(0.0f, 1.0f, 0.0f));
        trans = glm::scale(trans, glm::vec3(shrink_val, shrink_val, 1.0));
        myShader.use();
        myShader.setMat4("transform", trans);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        
    }
}

*/
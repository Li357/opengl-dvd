#define STB_IMAGE_IMPLEMENTATION
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <random>
#include <cmath>
#include "main.h"
#include "ShaderProgram.h"

int main() {
    constexpr int SCR_WIDTH = 500;
    constexpr int SCR_HEIGHT = 500;

    constexpr float VELOCITY = 0.5f;

    constexpr float DVD_WIDTH = 0.3f;
    constexpr float DVD_HEIGHT = 0.2f;
    constexpr float DVD_VERTICAL_BOUND { 1 - DVD_HEIGHT / 2 };
    constexpr float DVD_HORIZONTAL_BOUND { 1 - DVD_WIDTH / 2 };

    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_real_distribution<> thetaDist(0, 2 * M_PI);
    double THETA = thetaDist(engine);

    GLFWwindow* window { createWindow("maybemaybemaybe", SCR_HEIGHT, SCR_WIDTH) };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    ShaderProgram program { "vertex.glsl", "fragment.glsl" };

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("dvd.jpg", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to read dvd.jpg!" << std::endl;
        stbi_image_free(data);
        return 1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    float vertices[] = {
        DVD_WIDTH / 2,  DVD_HEIGHT / 2, 0.0f,   1.0f, 1.0f,
        DVD_WIDTH / 2, -DVD_HEIGHT / 2, 0.0f,  1.0f, 0.0f,
        -DVD_WIDTH / 2, -DVD_HEIGHT / 2, 0.0f,  0.0f, 0.0f,
        -DVD_WIDTH / 2,  DVD_HEIGHT / 2, 0.0f,    0.0f, 1.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glfwSetTime(0);
    double deltaX { 0.0 };
    double deltaY { 0.0 };
    double currTheta { THETA };

    program.use();
    std::uniform_real_distribution<> colorDist(0.1, 1);
    int colorUniformLoc { glGetUniformLocation(program.getId(), "dvdColor") };
    glUniform3f(colorUniformLoc, colorDist(engine), colorDist(engine), colorDist(engine));

    int transformUniformLoc { glGetUniformLocation(program.getId(), "delta") };
    glUniform2f(transformUniformLoc, deltaX, deltaY);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();

        double elapsedTime { glfwGetTime() };
        glfwSetTime(0);

        bool hitsTopOrBottom = deltaY >= DVD_VERTICAL_BOUND || deltaY <= -DVD_VERTICAL_BOUND;
        bool hitsRight = deltaX >= DVD_HORIZONTAL_BOUND;
        bool hitsLeft = deltaX <= -DVD_HORIZONTAL_BOUND;
        bool hitsSide = hitsTopOrBottom || hitsRight || hitsLeft;

        if (hitsSide) {
            if (hitsTopOrBottom) {
                currTheta = -currTheta;
                deltaY = (deltaY > 0 ? 1.0f : -1.0f) * DVD_VERTICAL_BOUND;
            } else if (hitsRight) {
                currTheta = -currTheta - M_PI;
                deltaX = DVD_HORIZONTAL_BOUND;
            } else if (hitsLeft) {
                currTheta = M_PI - currTheta;
                deltaX = -DVD_HORIZONTAL_BOUND;
            }
            glUniform3f(colorUniformLoc, colorDist(engine), colorDist(engine), colorDist(engine));
        }

        deltaX += VELOCITY * cos(currTheta) * elapsedTime;
        deltaY += VELOCITY * sin(currTheta) * elapsedTime;

        glUniform2f(transformUniformLoc, deltaX, deltaY);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* createWindow(const char* title, int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw;
    }
    glfwMakeContextCurrent(window);
    return window;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
//
// Created by Andrew Li on 12/24/21.
//

#ifndef LEARN_OPENGL_SHADER_PROGRAM_H
#define LEARN_OPENGL_SHADER_PROGRAM_H

#include <OpenGL/gl3.h>
#include <iostream>
#include <fstream>
#include <string>

class ShaderProgram {
    unsigned int shaderProgram;

public:
    ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
        auto vertexShader { compileShader(vertexShaderPath, GL_VERTEX_SHADER) };
        auto fragmentShader { compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER) };
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cout << "Failed to link shader program!\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    ~ShaderProgram() {
        glDeleteProgram(shaderProgram);
    }

    void use() const {
        glUseProgram(shaderProgram);
    }

    [[nodiscard]] auto getId() const {
        return shaderProgram;
    }

private:
    static unsigned int compileShader(const std::string& path, GLenum type) {
        std::ifstream file(path, std::ios::in | std::ios::binary);
        auto size = std::filesystem::file_size(path);
        std::string content(size, '\0');
        file.read(content.data(), size);

        const char *source { content.c_str() };
        unsigned int shader { glCreateShader(type) };
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "Failed to compile shader at " << path << "!\n" << infoLog << std::endl;
            return 0;
        }
        return shader;
    }
};

#endif //LEARN_OPENGL_SHADER_PROGRAM_H

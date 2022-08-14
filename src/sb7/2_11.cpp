//
// Created by user on 8/14/22.
//
#include "sb7.h"
#include <iostream>
class my_application : public sb7::application {
public:
    static GLuint compile_shaders() {
        static constexpr const char *vertex_shader_source = R"(
        #version 330 core
        void main(void)
        {
            gl_Position = vec4(0.0, 0.0, 0.5, 1.0);
        }
        )";

        static constexpr const char *fragment_shader_source = R"(
        #version 330 core
        out vec4 color;
        void main(void)
        {
            color = vec4(0.0, 0.8, 1.0, 1.0);
        }
        )";

        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;
        int success;
        char infoLog[512];

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }

    void startup() override {
        rendering_program = compile_shaders();
        glGenVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);
    }

    void render(double currentTime) override {

        const GLfloat red[] = {(float)(sin(currentTime) * 0.5f + 0.5),
                               (float)(cos(currentTime) * 0.5f + 0.5),
                               0.0f,
                               1.0f};
        glClearBufferfv(GL_COLOR, 0, red);

        // use the program object we created earlier
        glUseProgram(rendering_program);

        glPointSize(40.0f);

        // draw one point
        glDrawArrays(GL_POINTS, 0, 1);
    }

    void shutdown() override {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);
    }

private:
    GLuint rendering_program{};
    GLuint vertex_array_object{};
};

DECLARE_MAIN(my_application);
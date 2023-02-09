//
// Created by user on 10/9/22.
//
#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *kVertexShaderSource = R"(
    #version 330
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char *kFragmentShaderSource = R"(
    #version 330
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

using namespace std;

int main() {
    // glfw initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // gflw create window
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == nullptr) {
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
        std::cerr << "failed to initialize GLAD\n";
        return -1;
    }

    // build and compile our shader program
    int succ = -1;
    char info_log[512];

    // vertex shader
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &kVertexShaderSource, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << info_log << std::endl;
        return -1;
    }

    // fragment shader
    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &kFragmentShaderSource, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << info_log << std::endl;
        return -1;
    }

    // links shaders
    auto shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vertex_shader);
    glAttachShader(shader_prog, fragment_shader);
    glLinkProgram(shader_prog);

    // check for link error
    glGetProgramiv(shader_prog, GL_LINK_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(shader_prog, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << info_log << std::endl;
        return -1;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // setup vertex data and buffers and configure vertext attributes

    // two triangle's vertex
    float vertices[] = {
        -1.0f, -0.5f, 0.0f, // left
        0.0f, -0.5f, 0.0f,  // right
        -0.5f, 0.5f, 0.0f,  // top

        0.0f, -0.5f, 0.0f, // left
        1.0f, -0.5f, 0.0f, // right
        0.5f, 0.5f, 0.0f,  // top
    };

    GLuint vbo{0};
    GLuint vao{0};
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // bind the vertex array object first, then bind and set vertex buffers
    // and then configure vertex attributes
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    for (; !glfwWindowShouldClose(window);) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw out triangles
        glUseProgram(shader_prog);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_prog);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
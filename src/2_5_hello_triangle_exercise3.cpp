//
// Created by user on 10/9/22.
//

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

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

const char *kFragmentShaderSource2 = R"(
    #version 330
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    }
)";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main() {
    // glfw initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw create window
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
        std::cerr << "failed to initialize GLAD\n";
        return -1;
    }

    // build and compile shader program
    int succ = -1;
    char info_log[512];

    // vertex shader
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &kVertexShaderSource, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << info_log << std::endl;
        return -1;
    }

    // fragment shader
    auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &kFragmentShaderSource, nullptr);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << info_log << std::endl;
        return -1;
    }

    auto frag_shader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader2, 1, &kFragmentShaderSource2, nullptr);
    glCompileShader(frag_shader2);
    glGetShaderiv(frag_shader2, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(frag_shader2, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << info_log << std::endl;
        return -1;
    }

    // links shaders
    auto shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vertex_shader);
    glAttachShader(shader_prog, frag_shader);
    glLinkProgram(shader_prog);
    glGetProgramiv(shader_prog, GL_LINK_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(shader_prog, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << info_log << std::endl;
        return -1;
    }

    auto shader_prog2 = glCreateProgram();
    glAttachShader(shader_prog2, vertex_shader);
    glAttachShader(shader_prog2, frag_shader2);
    glLinkProgram(shader_prog2);
    glGetProgramiv(shader_prog2, GL_LINK_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(shader_prog2, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM2::LINKING_FAILED\n"
                  << info_log << std::endl;
        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);
    glDeleteShader(frag_shader2);

    // setup vertex data and buffers and configure vertex attributes
    float first_triangle[] = {
        -1.0f, -0.5f, 0.0f, // left
        0.0f, -0.5f, 0.0f,  // right
        -0.5f, 0.5f, 0.0f,  // top
    };

    float second_triangle[] = {
        0.0f, -0.5f, 0.0f, // left
        1.0f, -0.5f, 0.0f, // right
        0.5f, 0.5f, 0.0f,  // top
    };

    GLuint vbos[2] = {
        0,
    };
    GLuint vaos[2] = {
        0,
    };

    glGenVertexArrays(2, vaos);
    glGenBuffers(2, vbos);

    // setup first triangle
    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // setup second triangle
    glBindVertexArray(vaos[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle), second_triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //

    for (; !glfwWindowShouldClose(window);) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw first triangle using the data from the first vao
        glUseProgram(shader_prog);
        glBindVertexArray(vaos[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw second triangle using the dat from the second vao
        glUseProgram(shader_prog2);
        glBindVertexArray(vaos[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, vaos);
    glDeleteBuffers(2, vbos);
    glDeleteProgram(shader_prog);

    glfwDestroyWindow(window);
    glfwTerminate();
}
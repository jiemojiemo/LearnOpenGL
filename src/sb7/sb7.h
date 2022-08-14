//
// Created by user on 8/14/22.
//

#ifndef MY_LEARN_OPENGL_SB7_H
#define MY_LEARN_OPENGL_SB7_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cmath>
#include <cstring>
#include <iostream>

namespace sb7 {
class application {
public:
    application() {}
    virtual ~application() {}
    virtual void run(sb7::application *the_app) {
        bool running = true;
        app = the_app;

        if (!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW\n");
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }

        init();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifndef _DEBUG
        if (info.flags.debug)
#endif /* _DEBUG */
        {
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        }
        if (info.flags.robust) {
            glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
        }
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, info.samples);
        glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);

        {
            window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
            if (!window) {
                fprintf(stderr, "Failed to open window\n");
                return;
            }
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "gladLoadGLLoader failed\n";
            return;
        }

        glfwSetWindowSizeCallback(window, glfw_onResize);
        glfwSetKeyCallback(window, glfw_onKey);
        glfwSetMouseButtonCallback(window, glfw_onMouseButton);
        glfwSetCursorPosCallback(window, glfw_onMouseMove);
        glfwSetScrollCallback(window, glfw_onMouseWheel);
        if (!info.flags.cursor) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }

        // info.flags.stereo = (glfwGetWindowParam(GLFW_STEREO) ? 1 : 0);

#ifdef _DEBUG
        fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
        fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
        fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
#endif

        startup();

        do {
            render(glfwGetTime());

            glfwSwapBuffers(window);
            glfwPollEvents();

            running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
            running &= (glfwWindowShouldClose(window) != GL_TRUE);
        } while (running);

        shutdown();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    virtual void init() {
        strcpy(info.title, "OpenGL SuperBible Example");
        info.windowWidth = 800;
        info.windowHeight = 600;
#ifdef __APPLE__
        info.majorVersion = 3;
        info.minorVersion = 3;
#else
        info.majorVersion = 4;
        info.minorVersion = 3;
#endif
        info.samples = 0;
        info.flags.all = 0;
        info.flags.cursor = 1;
#ifdef _DEBUG
        info.flags.debug = 1;
#endif
    }

    virtual void startup() {
    }

    virtual void render(double currentTime) {
    }

    virtual void shutdown() {
    }

    void setWindowTitle(const char *title) {
        glfwSetWindowTitle(window, title);
    }

    virtual void onResize(int w, int h) {
        info.windowWidth = w;
        info.windowHeight = h;
    }

    virtual void onKey(int key, int action) {
    }

    virtual void onMouseButton(int button, int action) {
    }

    virtual void onMouseMove(int x, int y) {
    }

    virtual void onMouseWheel(int pos) {
    }

    virtual void onDebugMessage(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar *message) {
#ifdef _WIN32
        OutputDebugStringA(message);
        OutputDebugStringA("\n");
#endif /* _WIN32 */
    }

    void getMousePosition(int &x, int &y) {
        double dx, dy;
        glfwGetCursorPos(window, &dx, &dy);

        x = static_cast<int>(floor(dx));
        y = static_cast<int>(floor(dy));
    }

public:
    struct APPINFO {
        char title[128];
        int windowWidth;
        int windowHeight;
        int majorVersion;
        int minorVersion;
        int samples;
        union {
            struct
            {
                unsigned int fullscreen : 1;
                unsigned int vsync : 1;
                unsigned int cursor : 1;
                unsigned int stereo : 1;
                unsigned int debug : 1;
                unsigned int robust : 1;
            };
            unsigned int all;
        } flags;
    };

protected:
    APPINFO info;
    static sb7::application *app;
    GLFWwindow *window;

    static void glfw_onResize(GLFWwindow *window, int w, int h) {
        app->onResize(w, h);
    }

    static void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
        app->onKey(key, action);
    }

    static void glfw_onMouseButton(GLFWwindow *window, int button, int action, int mods) {
        app->onMouseButton(button, action);
    }

    static void glfw_onMouseMove(GLFWwindow *window, double x, double y) {
        app->onMouseMove(static_cast<int>(x), static_cast<int>(y));
    }

    static void glfw_onMouseWheel(GLFWwindow *window, double xoffset, double yoffset) {
        app->onMouseWheel(static_cast<int>(yoffset));
    }

    void setVsync(bool enable) {
        info.flags.vsync = enable ? 1 : 0;
        glfwSwapInterval((int)info.flags.vsync);
    }
};

} // namespace sb7

#if defined _WIN32
#define DECLARE_MAIN(a)                           \
    sb7::application *app = 0;                    \
    int CALLBACK WinMain(HINSTANCE hInstance,     \
                         HINSTANCE hPrevInstance, \
                         LPSTR lpCmdLine,         \
                         int nCmdShow) {          \
        a *app = new a;                           \
        app->run(app);                            \
        delete app;                               \
        return 0;                                 \
    }
#elif defined _LINUX || defined __APPLE__
#define DECLARE_MAIN(a)                     \
    int main(int argc, const char **argv) { \
        a *app = new a;                     \
        app->run(app);                      \
        delete app;                         \
        return 0;                           \
    }
#else
#error Undefined platform!
#endif

#endif // MY_LEARN_OPENGL_SB7_H

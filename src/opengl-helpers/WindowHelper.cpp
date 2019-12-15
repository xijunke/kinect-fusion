#include <stdexcept>
#include <GL/glew.h>

#include "WindowHelper.h"

Window::Window(string title, unsigned int width, unsigned int height){

    if (!glfwInit())
        throw runtime_error("Cannot initialize GLFW");

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL) 
        throw runtime_error("Cannot open window");

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw runtime_error("Cannot initialize GLEW");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Set black window background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

Window::~Window(){
    glfwTerminate();
}

bool Window::shouldClose(){
    return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0;
}

void Window::update(){
    glfwSwapBuffers(window);
    glfwPollEvents();
}
#pragma once

#include <string>
#include <GLFW/glfw3.h>

using namespace std;

class Window{
    private:
        GLFWwindow *window;
    public:
        Window(string title, unsigned int width, unsigned int height);
        ~Window();

        bool shouldClose();
        void update();
};
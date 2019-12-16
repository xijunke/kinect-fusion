#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Window{
    private:
        GLFWwindow *window;

        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        float time;

        GLuint shaderProgramId;
        GLuint viewProjMatrixId;
        GLuint vaoId;

        float lastMouseX, lastMouseY, pitch, yaw;
        bool firstMouse;
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        
    public:
        Window(string title, unsigned int width, unsigned int height);
        ~Window();

        bool shouldClose();

        void setup(); // To be called before preparing all the scene
        void update(); // To be called after preparing all the scene
};
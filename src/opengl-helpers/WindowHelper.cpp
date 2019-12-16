#include <stdexcept>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "WindowHelper.h"
#include "ShaderHelper.h"

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

    // Acquire mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);  
    lastMouseX = width / 2;
    lastMouseY = height / 2;
    yaw = pitch = 0;
    firstMouse = true;

    //Save 'this' in glfw to access that in callbacks
    glfwSetWindowUserPointer(window, this);

    // Create OpenGL default VAO. To be done before any other GL call.
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // Set black window background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Initialize camera
    this->cameraPos = glm::vec3(width/2, height/2, 100);
    this->cameraFront = glm::vec3(0, 0, -1);
    this->cameraUp = glm::vec3(0, 1, 0);

    // Load shader
    this->shaderProgramId = ShaderHelper::loadProgram("src/opengl-helpers/vertex.glsl", "src/opengl-helpers/pixel.glsl");
    this->viewProjMatrixId = glGetUniformLocation(shaderProgramId, "ViewProjection");
}

Window::~Window(){
    glfwTerminate();
    glDeleteVertexArrays(1, &vaoId);
    glDeleteProgram(shaderProgramId);
}

bool Window::shouldClose(){
    return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0;
}

void Window::update(){

    float cameraSpeed = 50.0f * (glfwGetTime() - this->time);
    this->time = glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::setup(){
    // Update viewProjection matrix
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 1000.0f);

    glm::mat4 viewProj = proj * glm::lookAt(
        this->cameraPos,    // Camera pos in world coords
        this->cameraPos + this->cameraFront, // Camera looking at in world coords
        this->cameraUp   // Head is up (set to 0,-1,0 to look upside-down)
    );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define which shader will be used
    glUseProgram(this->shaderProgramId);

    // Send our transformation to the currently bound shader
    glUniformMatrix4fv(this->viewProjMatrixId, 1, GL_FALSE, &viewProj[0][0]);
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos){
    //https://learnopengl.com/Getting-started/Camera

    Window *ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (ptr->firstMouse){
        ptr->firstMouse = false;
        return;
    }

    float xoffset = xpos - ptr->lastMouseX;
    float yoffset = ptr->lastMouseY - ypos; // reversed since y-coordinates range from bottom to top

    ptr->lastMouseX = xpos;
    ptr->lastMouseY = ypos;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    ptr->yaw += xoffset;
    ptr->pitch += yoffset;

    if (ptr->pitch > 89.0f)
        ptr->pitch = 89.0f;
    if (ptr->pitch < -89.0f)
        ptr->pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(ptr->pitch)) * cos(glm::radians(ptr->yaw));
    front.y = sin(glm::radians(ptr->pitch));
    front.z = cos(glm::radians(ptr->pitch)) * sin(glm::radians(ptr->yaw));
    ptr->cameraFront = glm::normalize(front);
}
#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "sensor/Sensor.h"
#include "sensor/MockSensor.h"
#include "opengl-helpers/WindowHelper.h"

int testFunction();

int main(int argc, char * argv[]) {
    //testFunction();

    auto sensor = std::make_unique<MockSensor>(MockSensor("data/rgbd_dataset_freiburg1_xyz", 30));

    Window win("KinectFusion", 1024, 1024);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    while(true){
        auto frame = sensor->getNextFrame();

        int SIZE = frame->getWidth() * frame->getHeight() * 3;
        GLfloat points[SIZE];
        GLfloat *p = points;
        for (int u = 0; u < frame->getWidth(); u++){
            for (int v = 0; v < frame->getHeight(); v++){
                (*p++) = u;
                (*p++) = v;
                (*p++) = -frame->d(u, v) * 100;
            }
        }

        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        win.setup();

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

        glPointSize(5.0f);
		glDrawArrays(GL_POINTS, 0, SIZE);

		glDisableVertexAttribArray(0);

        glDeleteBuffers(1, &vertexbuffer);

        win.update();
        if (win.shouldClose()) 
            break;
    }

    glDeleteVertexArrays(1, &VertexArrayID);

    return 0;
}
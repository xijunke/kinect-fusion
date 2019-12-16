#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "sensor/Sensor.h"
#include "sensor/MockSensor.h"
#include "opengl-helpers/WindowHelper.h"

int testFunction();

#include "denoising.cpp"
void denoiseDepthMap(unique_ptr<Frame> &frame, vector<float> &denoised);

#include "depth-to-vertex-and-normals.cpp"
void computeMipMaps(unique_ptr<Frame> &frame, vector<float> const &denoisedDepthMap, vector<vector<Vector3f>> &vertexMipMap, vector<vector<Vector3f>> &normalsMipMap);

const auto MINF = -std::numeric_limits<float>::infinity();

int main(int argc, char * argv[]) {
    //testFunction();

    auto sensor = std::make_unique<MockSensor>(MockSensor("data/rgbd_dataset_freiburg1_xyz", 60));

    Window win("KinectFusion", 1024, 1024);

    GLuint vertexBuffer, colorBuffer;
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &colorBuffer);

    while(true){
        auto frame = sensor->getNextFrame();




        const int SIZE = frame->getWidth() * frame->getHeight();

        vector<float> denoisedDepthMap(SIZE); // D_k
        vector<bool> validityMask(SIZE); // M
        denoiseDepthMap(frame, denoisedDepthMap, validityMask);


        vector<vector<Vector3f>> vertexMipMap(3, vector<Vector3f>(SIZE, Vector3f::Zero())); //V^l_k
        vector<vector<Vector3f>> normalMipMap(3, vector<Vector3f>(SIZE, Vector3f::Zero())); //N^l_k
        computeDepthMipMap(frame, denoisedDepthMap, vertexMipMap, normalMipMap);



        const int MAX_SIZE = frame->getWidth() * frame->getHeight() * 3;
        GLfloat points[MAX_SIZE], colors[MAX_SIZE];
        int actualSize = 0;

        for (int u = 0; u < frame->getWidth(); u++){
            for (int v = 0; v < frame->getHeight(); v++){
                if (frame->d(u, v) != MINF){
                    points[actualSize + 0] = u;
                    points[actualSize + 1] = v;
                    points[actualSize + 2] = 1.0;

                    colors[actualSize + 0] = 0.3;
                    colors[actualSize + 1] = 0;
                    colors[actualSize + 2] = 0;

                    actualSize += 3;
                }

            }
        }

        win.setup();

        // Move vertices and colors to appropriate buffers
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_SIZE * sizeof(GLfloat), points, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_SIZE * sizeof(GLfloat), colors, GL_DYNAMIC_DRAW);

        // Bind data stored in buffers to shader parameters
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
            0,         // attribute. No particular reason for 0, but must match the layout in the shader.
            3,         // size
            GL_FLOAT,  // type
            GL_FALSE,  // normalized?
            0,         // stride
            (void*)0   // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

        //Actually do the drawing
        glPointSize(5.0f);
        glDrawArrays(GL_POINTS, 0, actualSize / 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        win.update();
        if (win.shouldClose()) 
            break;
    }

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &colorBuffer);

    return 0;
}
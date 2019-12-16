#include <memory>
#include <vector>

#include <Eigen>

#include "sensor/Frame.h"

using namespace std;
using namespace Eigen;

void computeDepthMipMap(unique_ptr<Frame> &frame, vector<float> const &denoisedDepthMap, 
                        vector<vector<Vector3f>> &vertexMipMap, vector<vector<Vector3f>> &normalsMipMap){
    for (int u = 0; u < frame->getWidth(); u++){
        for (int v = 0; v < frame->getHeight(); v++){
            int i = v * frame->getWidth() + u;
            vertexMipMap[0][i] = Vector3f(0, 0, 0);
            normalsMipMap[0][i] = Vector3f(0, 0, 0);
        }
    }
}
#include <memory>
#include <vector>

#include "sensor/Frame.h"

#include <iostream>

using namespace std;

void denoiseDepthMap(unique_ptr<Frame> &frame, vector<float> &denoised){
    for (int u = 0; u < frame->getWidth(); u++)
        for (int v = 0; v < frame->getHeight(); v++)
            denoised[v * frame->getWidth() + u] = frame->d(u, v);
}
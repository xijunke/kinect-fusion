#pragma once

#include <string>
#include <memory>

#include "Sensor.h"

using namespace std;

class MockSensor : public Sensor { 
    private:
        enum class ImgType {RGBA, DEPTH};
        const string datasetRoot;

        Matrix3f intrinsics;
        unsigned int frameWidth, frameHeight;

        const unsigned int framesToCache;
        unsigned int currentFramePosition;

        vector<vector<float>> rgbCache;
        vector<vector<float>> depthCache;

        vector<string> loadListOfImageNames(string fileName);
        vector<float> loadImageFromFile(string fileName, ImgType type, unsigned int &width, unsigned int &height);
    public:
        MockSensor(string datasetRootPath, unsigned int framesToCache);
        virtual unique_ptr<Frame> getNextFrame();
};
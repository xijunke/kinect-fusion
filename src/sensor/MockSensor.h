#pragma once

#include <string>
#include <memory>

#include "Sensor.h"

using namespace std;

class MockSensor : public Sensor { 
    private:
        enum class ImgType {RGBA, DEPTH};
        const string datasetRoot;
        vector<string> colorImageNames;
        vector<string> depthImageNames;
        unsigned int currentFramePosition;

        void loadListOfNames(string fileName, vector<string> &list);
        unique_ptr<float[]> loadImageFromFile(string fileName, ImgType type, unsigned int &width, unsigned int &height);
    public:
        MockSensor(string datasetRootPath);
        virtual unique_ptr<Frame> getNextFrame();
};
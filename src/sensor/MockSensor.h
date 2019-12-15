#pragma once

#include <string>
#include <memory>

#include "Sensor.h"

using namespace std;

class MockSensor : public Sensor { 
    private:
        const string datasetRoot;
        vector<string> colorImageNames;
        vector<string> depthImageNames;
        unsigned int currentFramePosition;

        void loadListOfNames(string fileName, vector<string> &list);
    public:
        MockSensor(string datasetRootPath);
        virtual unique_ptr<Frame> getNextFrame();
};
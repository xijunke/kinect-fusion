#pragma once

#include <fstream>

#include "MockSensor.h"

#include <iostream>


MockSensor::MockSensor(string datasetRootPath)
        : datasetRoot(datasetRootPath), currentFramePosition(0) {
    this->loadListOfNames("rgb.txt", colorImageNames);
    this->loadListOfNames("depth.txt", depthImageNames);

    for (auto i = colorImageNames.begin(); i != colorImageNames.end(); ++i)
        cout << *i << ' ';
};

void MockSensor::loadListOfNames(string fileName, vector<string> &list){
    string tmp;
    double timestamp;
    ifstream file(datasetRoot + "/" + fileName, ios::in);

    if (!file.is_open())
        throw runtime_error("Cannot load RGB file list from dataset, path is '" + datasetRoot + "/" + fileName + "'");

    getline(file, tmp);
    getline(file, tmp);
    getline(file, tmp);

    while (file.good()) {
        file >> timestamp;
        file >> tmp;
        list.push_back(tmp);
    }

    file.close();
}

unique_ptr<Frame> MockSensor::getNextFrame(){
    const BYTE a[] = {0xff};
    const float d[] = {0.1f};
    const unsigned int i = 100;
    const Matrix3f I = Eigen::Matrix3f::Identity(3, 3);
    return make_unique<Frame>(Frame(a, a, a, d, i, i, I, I));
}
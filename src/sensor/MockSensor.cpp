#include <fstream>

#include "MockSensor.h"

#include <iostream>

MockSensor::MockSensor(string datasetRootPath)
        : datasetRoot(datasetRootPath), currentFramePosition(0) {
    this->loadListOfNames("rgb.txt", colorImageNames);
    this->loadListOfNames("depth.txt", depthImageNames);
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

    cout << "Reading frame " << currentFramePosition << "\n";

    unsigned int rgbWidth, rgbHeight;
    const string rgbFileName = datasetRoot + "/" + colorImageNames[currentFramePosition];
    auto rgb = loadImageFromFile(rgbFileName, ImgType::RGBA, rgbWidth, rgbHeight);

    unsigned int depthWidth, depthHeight;
    const string depthFileName = datasetRoot + "/" + depthImageNames[currentFramePosition]; 
    auto depth = loadImageFromFile(depthFileName, ImgType::DEPTH, depthWidth, depthHeight);

    for (int i = 0; i < depthWidth * depthHeight; ++i) {
        if (depth[i] == 0)
            depth[i] = -std::numeric_limits<float>::infinity();
        else
            depth[i] = (depth[i] * (256 * 256 - 1)) * 1.0f / 5000.0f;
    }

    Matrix3f intrinsics;
    intrinsics << 525.0f, 0.0f, 319.5f,
        0.0f, 525.0f, 239.5f,
        0.0f, 0.0f, 1.0f;

    currentFramePosition++;

    return make_unique<Frame>(Frame(move(rgb), move(depth), rgbWidth, rgbHeight, intrinsics, intrinsics));
}

vector<float> MockSensor::loadImageFromFile(string fileName, ImgType type, unsigned int &width, unsigned int &height){
    FreeImage_Initialise();

    if (!FreeImage_FIFSupportsReading(FIF_PNG))
        throw runtime_error("FreeImage does not support PNG? Something's wrong");

    FIBITMAP *dib = FreeImage_Load(FIF_PNG, fileName.c_str());

    if (dib == nullptr)
        throw runtime_error("Cannot load image at '" + fileName + "'");

    {
        FIBITMAP *hOldImage = dib;
        switch (type) {
            case ImgType::RGBA:
                dib = FreeImage_ConvertToRGBAF(hOldImage); // To RGBA float, 4 channels
                break;
            case ImgType::DEPTH:
                dib = FreeImage_ConvertToFloat(hOldImage); // To grayscale, 1 channel
                break;
        }
        FreeImage_Unload(hOldImage);
    }

    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);

    float *imageBits = (float *)FreeImage_GetBits(dib);

    int NUM_OF_CHANNELS;
    switch (type) {
        case ImgType::RGBA:
            NUM_OF_CHANNELS = 4;
            break;
        case ImgType::DEPTH:
            NUM_OF_CHANNELS = 1;
            break;
    }

    // Order in row-major. After this step, given:
    //  * x the horizontal coordinate of the pixel
    //  * y the vertical coordinate of the pixel
    //  * c the desired channel (c = 1 is G for example)
    // The corresponding image bit is at `data[(y * width + x) * NUM_OF_CHANNELS + c]`
    auto data = vector<float>(NUM_OF_CHANNELS * width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < NUM_OF_CHANNELS; ++c) {
                int i = (y * width + x) * NUM_OF_CHANNELS + c;
                int j = ((height - 1 - y) * width + x) * NUM_OF_CHANNELS + c;

                data[i] = imageBits[j];
            }
        }
    }

    FreeImage_Unload(dib);

    return data;
}
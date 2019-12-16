#include <fstream>

#include "MockSensor.h"

#include <iostream>

MockSensor::MockSensor(string datasetRootPath, unsigned int framesToCache)
        : datasetRoot(datasetRootPath), framesToCache(framesToCache), currentFramePosition(0) {

    // These are hardcoded and will always be the same for all frames.
    intrinsics << 525.0f, 0.0f, 319.5f,
        0.0f, 525.0f, 239.5f,
        0.0f, 0.0f, 1.0f;

    auto colorImageNames = loadListOfImageNames("rgb.txt");
    auto depthImageNames = loadListOfImageNames("depth.txt");

    if (colorImageNames.size() != depthImageNames.size())
        throw runtime_error("Found " + to_string(colorImageNames.size()) + " color frames and " + to_string(depthImageNames.size()) + " depth frame. They should be equal");

    if (framesToCache >= colorImageNames.size())
        throw runtime_error("Caching " + to_string(framesToCache) + " frames is not possible because there are only " + to_string(colorImageNames.size()) + " frames");

    // Load `frameToCache` frames in a local cache. In this way, we do all the disk accesses at the beggining
    // and aftewards the MockSensor is not bounded by disk latency anymore.
    rgbCache = vector<vector<float>>(framesToCache);
    depthCache = vector<vector<float>>(framesToCache);

    unsigned int width, height;

    for (int i = 0; i < framesToCache; i++){
        const string rgbFileName = datasetRoot + "/" + colorImageNames[i];
        auto rgb = loadImageFromFile(rgbFileName, ImgType::RGBA, width, height);

        const string depthFileName = datasetRoot + "/" + depthImageNames[i];
        auto depth = loadImageFromFile(depthFileName, ImgType::DEPTH, width, height);

        for (int j = 0; j < width * height; ++j)
        {
            if (depth[j] == 0)
                depth[j] = -std::numeric_limits<float>::infinity();
            else
                depth[j] = (depth[j] * (256 * 256 - 1)) * 1.0f / 5000.0f;
        }

        rgbCache[i] = rgb;
        depthCache[i] = depth;
    }

    // TODO FIXME
    // Assumption, all frames have the fame size. This should be enforced or improved.
    frameWidth = width;
    frameHeight = height;
};


unique_ptr<Frame> MockSensor::getNextFrame(){

    // Return frames 0 -> framesToCache and then framesToCache -> 0 and then repeat.
    // In this way we maintain a smooth kinect "movement".
    int t = currentFramePosition;

    currentFramePosition++;
    if (currentFramePosition == 2 * framesToCache)
        currentFramePosition = 0;

    t = t < framesToCache ? t : framesToCache - (t - framesToCache + 1);
    return make_unique<Frame>(Frame(rgbCache[t], depthCache[t], 640, 480, intrinsics, intrinsics));
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

vector<string> MockSensor::loadListOfImageNames(string fileName){
    vector<string> ans;
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
        ans.push_back(tmp);
    }

    file.close();
    return ans;
}
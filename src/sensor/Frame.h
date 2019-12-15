#pragma once

#include <memory>
#include <FreeImage.h>
#include <Eigen>

using namespace Eigen;
using namespace std;

class Frame{
    private:
        unique_ptr<float[]> colorsRGBA;
        unique_ptr<float[]> depth;
        const unsigned int width;
        const unsigned int height;
        const Matrix3f colorIntrinsics;
        const Matrix3f depthIntrinsics;

    public:
        Frame(unique_ptr<float[]> rgba, unique_ptr<float[]> d, const unsigned int w, const unsigned int h, const Matrix3f cInt, const Matrix3f dInt)
            : colorsRGBA(move(rgba)), depth(move(d)), width(w), height(h), colorIntrinsics(cInt), depthIntrinsics(dInt) {}
        
        const unsigned int getWidth()  { return width;  }
        const unsigned int getHeight() { return height; }

        const Matrix3f getColorIntrinsics() { return colorIntrinsics; }
        const Matrix3f getDepthIntrinsics() { return depthIntrinsics; }

        const float r(unsigned int x, unsigned int y) { return colorsRGBA[(y * width + x) * 4 + 0]; }
        const float g(unsigned int x, unsigned int y) { return colorsRGBA[(y * width + x) * 4 + 1]; }
        const float b(unsigned int x, unsigned int y) { return colorsRGBA[(y * width + x) * 4 + 2]; }
        const float d(unsigned int x, unsigned int y) { return depth[y * width + x]; }
};
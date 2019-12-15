#pragma once

#include <FreeImage.h>
#include <Eigen>

using namespace Eigen;

class Frame{
    private:
        const BYTE *red;
        const BYTE *green;
        const BYTE *blue;
        const float *depth;
        const unsigned int width;
        const unsigned int height;
        const Matrix3f colorIntrinsics;
        const Matrix3f depthIntrinsics;

    public:
        Frame(const BYTE *r, const BYTE *g, const BYTE *b, const float *d, const unsigned int w, const unsigned int h, const Matrix3f cInt, const Matrix3f dInt)
            : red(r), green(g), blue(b), depth(d), width(w), height(h), colorIntrinsics(cInt), depthIntrinsics(dInt) {}
        
        const unsigned int getWidth()  { return width;  }
        const unsigned int getHeight() { return height; }

        const Matrix3f getColorIntrinsics() {return colorIntrinsics; }
        const Matrix3f getDepthIntrinsics() {return depthIntrinsics; }

        const BYTE* getRed()    { return red; }
        const BYTE* getGreen()  { return green; }
        const BYTE* getBlue()   { return blue; }
        const float* getDepth() { return depth; }
};
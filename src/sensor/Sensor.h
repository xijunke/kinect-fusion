#pragma once

#include <memory>

#include <FreeImage.h>
#include "Frame.h"

class Sensor{
    public:
        virtual std::unique_ptr<Frame> getNextFrame() = 0;
};

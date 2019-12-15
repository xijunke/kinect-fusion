#include <iostream>
#include <memory>

#include "sensor/Sensor.h"
#include "sensor/MockSensor.h"

int testFunction();

int main(int argc, char * argv[]) {
    //testFunction();

    auto sensor = std::make_unique<MockSensor>(MockSensor("data/rgbd_dataset_freiburg1_xyz"));

    while(true){
        auto frame = sensor->getNextFrame();
    }

    return 0;
}
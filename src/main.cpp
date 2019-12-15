#include <iostream>
#include <memory>

#include "sensor/Sensor.h"
#include "sensor/MockSensor.h"

int testFunction();

int main(int argc, char * argv[]) {
    testFunction();

    auto sensor = std::make_unique<MockSensor>(MockSensor("data/rgbd_dataset_freiburg1_xyz", 30));

    while(true){
        auto frame = sensor->getNextFrame();
        cout << "frame->r(200, 100) = " << frame->r(200, 100) << endl;
        cout << "frame->g(200, 100) = " << frame->g(200, 100) << endl;
        cout << "frame->b(200, 100) = " << frame->b(200, 100) << endl;
        cout << "frame->d(200, 100) = " << frame->d(200, 100) << endl;
        cout << frame->getColorIntrinsics() << endl;
        
        cin.get();
    }

    return 0;
}
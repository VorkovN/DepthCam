#include <iostream>
#include "libobsensor/ObSensor.hpp"
#include "libobsensor/hpp/Error.hpp"

#include "../lib/window.h"

#include "libobsensor/hpp/Pipeline.hpp"

int main(int argc, char **argv) try {
    std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;
    std::cout << "SDK stage version: " << ob::Version::getStageVersion() << std::endl;

    // Create a Context.
    ob::Context ctx;
    auto devList = ctx.queryDeviceList();
    if(devList->deviceCount() == 0) {
        std::cerr << "Device not found!" << std::endl;
        return -1;
    }
    auto dev = devList->getDevice(0);
    auto devInfo = dev->getDeviceInfo();
    auto fwVer = devInfo->firmwareVersion();
    auto sn = devInfo->serialNumber();
    auto connectType = devInfo->connectionType();

    std::cout << "Device name: " << devInfo->name() << std::endl;
    std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;
    std::cout << "Firmware version: " << fwVer << std::endl;
    std::cout << "Serial number: " << sn << std::endl;
    std::cout << "ConnectionType: " << connectType << std::endl;

    std::cout << "Sensor types: " << std::endl;
    auto sensorList = dev->getSensorList();
    for(uint32_t i = 0; i < sensorList->count(); i++) {
        auto sensor = sensorList->getSensor(i);
        switch(sensor->type()) {
            case OB_SENSOR_COLOR:
                std::cout << "\tColor sensor" << std::endl;
                break;
            case OB_SENSOR_DEPTH:
                std::cout << "\tDepth sensor" << std::endl;
                break;
            case OB_SENSOR_IR:
                std::cout << "\tIR sensor" << std::endl;
                break;
            case OB_SENSOR_IR_LEFT:
                std::cout << "\tIR Left sensor" << std::endl;
                break;
            case OB_SENSOR_IR_RIGHT:
                std::cout << "\tIR Right sensor" << std::endl;
                break;
            case OB_SENSOR_GYRO:
                std::cout << "\tGyro sensor" << std::endl;
                break;
            case OB_SENSOR_ACCEL:
                std::cout << "\tAccel sensor" << std::endl;
                break;
            default:
                break;
        }
    }



    ob::Pipeline pipe;
    // By creating config to configure which streams to enable or disable for the pipeline, here the depth stream will be enabled
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableVideoStream(OB_STREAM_DEPTH);

    // Start the pipeline with config
    pipe.start(config);
    auto currentProfile = pipe.getEnabledStreamProfileList()->getProfile(0)->as<ob::VideoStreamProfile>();
    // Create a window for rendering, and set the resolution of the window
    Window app("DepthViewer", currentProfile->width(), currentProfile->height());

    while(app) {
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameSet = pipe.waitForFrames(100);
        if (frameSet == nullptr) {
            continue;
        }

        auto depthFrame = frameSet->depthFrame();

        // for Y16 format depth frame, print the distance of the center pixel every 30 frames
        if (depthFrame->index() % 30 == 0 && depthFrame->format() == OB_FORMAT_Y16) {
            uint32_t width = depthFrame->width();
            uint32_t height = depthFrame->height();
            float scale = depthFrame->getValueScale();
            uint16_t *data = (uint16_t *) depthFrame->data();

            // pixel value multiplied by scale is the actual distance value in millimeters
            float centerDistance = data[width * height / 2 + width / 2] * scale;

            // attention: if the distance is 0, it means that the depth camera cannot detect the object（may be out of detection range）
            std::cout << "Facing an object " << centerDistance << " mm away. " << std::endl;
        }

        // Render frame in the window
        app.addToRender(depthFrame);
    }
    pipe.stop();



    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
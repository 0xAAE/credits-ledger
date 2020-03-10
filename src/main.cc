#include <iostream>

#include "HidApi.h"

void hidApiErrorCb(HidError err);
void deviceAddedCb(int index, HidDevice dev);
void deviceRemovedCb(int index, HidDevice dev);
void hidDeviceErrorCb(HidDevice dev, HidError);

HidApi hidApi(&hidApiErrorCb);
HidDeviceList devList;

int main() {
    hidApi.registerApiErrorCallback(&hidApiErrorCb);
    hidApi.registerDeviceErrorCallback(&hidDeviceErrorCb);
    hidApi.registerDeviceAddCallback(&deviceAddedCb);
    hidApi.registerDeviceRemoveCallback(&deviceRemovedCb);

    devList = hidApi.scanDevices(0x2c97, ANY, ANY, ANY, ANY, ANY, ANY, ANY);

    std::cout << "Start nanos lookup..." << std::endl;

    for (size_t i = 0; i < devList.size(); ++i) {
        std::wcout << "DEVICE:" << std::endl
                   << "   Path        : " << devList[i].getPath().c_str()  << std::endl
                   << "   Manufacturer: " << devList[i].getManufacturer()  << std::endl
                   << "   Product     : " << devList[i].getProductString() << std::endl
                   << "   Serial      : " << devList[i].getSerial()        << std::endl
                   << "   Vendor Id   : " << devList[i].getVendorId()      << std::endl
                   << "   Product Id  : " << devList[i].getProductId()     << std::endl
                   << "   Release No  : " << devList[i].getRelease()       << std::endl
                   << "   Usage Page  : " << devList[i].getUsagePage()     << std::endl
                   << "   Usage       : " << devList[i].getUsage()         << std::endl
                   << "   Interface   : " << devList[i].getInterface()     << std::endl
                   << "--------------------------------------------------" << std::endl;
    }

    HidDevice nanos;
    std::string hidData;
    hidData.resize(9);
    hidData[0] = static_cast<char>(0xE0);
    hidData[1] = static_cast<char>(0x03);
    hidData[2] = static_cast<char>(0x00);
    hidData[3] = static_cast<char>(0x00);
    hidData[4] = static_cast<char>(0x04);
    hidData[5] = static_cast<char>(0x00);
    hidData[6] = static_cast<char>(0x00);
    hidData[7] = static_cast<char>(0x00);
    hidData[8] = static_cast<char>(0x00);

    if (devList.size()) {
        nanos = devList[0];
    }

    if (nanos.isInitialized()) {
        std::cout << "Nano S is initialized." << std::endl;
        if (nanos.open()) {
            std::cout << "Nano S is open." << std::endl;
            std::cout << "Write result " << nanos.write(hidData) << std::endl;
            auto s = nanos.read(-1);
            std::cout << "Read result: ";
            for (auto c : s) {
              std::cout << int(c) << " ";
            }
            std::cout << std::endl;
        }
    }
}

void hidApiErrorCb(HidError err) {
    std::cout << __func__ << ": " << err.getErrorString() << std::endl;
}


void deviceAddedCb(int index, HidDevice dev) {
    std::cout << __func__ << ": " << dev.getPath() << std::endl;
    devList = hidApi.getDeviceList();
}

void deviceRemovedCb(int index, HidDevice dev) {
    std::cout << __func__ << ": " << dev.getPath() << std::endl;
    devList = hidApi.getDeviceList();
}

void hidDeviceErrorCb(HidDevice dev, HidError err) {
    std::cout << __func__ << ": " << err.getErrorString()
              << " - " << dev.getPath() << std::endl;
}

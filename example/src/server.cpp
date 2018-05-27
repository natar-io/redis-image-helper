#include "RedisCameraServer.hpp"

int main(int argc, char** argv)
{
    RedisCameraServer server;
    server.setCameraKey("custom:image");

    std::string gstCommand = "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)I420, framerate=(fraction)120/1, queue-size=2, blockSize=16384, auto-exposure=1, scene-mode=1, flicker=0"
                             "! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

    if (!server.start())
    {
        std::cerr << "Failed to start camera server." << std::endl;
    }

    while (true)
    {
        server.pickUpCameraFrame();
    }
}

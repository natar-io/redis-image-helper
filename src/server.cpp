#include "RedisCameraServer.hpp"

int main(int argc, char** argv)
{
    RedisCameraServer server;
    server.setCameraKey("custom:image");

    if (!server.start())
    {
        std::cerr << "Failed to start camera server." << std::endl;
    }

    while (true)
    {
        server.pickUpCameraFrame();
    }
}

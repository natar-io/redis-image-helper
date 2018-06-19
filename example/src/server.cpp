#include "RedisCameraServer.hpp"

#include <cxxopts.hpp>
#include <string>

bool VERBOSE = false;
bool STREAM_MODE = true;
bool TEGRA = false;
std::string redisOutputKey = "custom:image";
std::string redisHost = "127.0.0.1";
int redisPort = 6379;
int cameraId = 0;

static int parseCommandLine(cxxopts::Options options, int argc, char** argv)
{
    auto result = options.parse(argc, argv);
    if (result.count("h")) {
        std::cout << options.help({"", "Group"});
        return EXIT_FAILURE;
    }

    if (result.count("v")) {
        VERBOSE = true;
        std::cerr << "Verbose mode enabled." << std::endl;
    }

    if (result.count("o")) {
        redisOutputKey = result["o"].as<std::string>();
        if (VERBOSE) {
            std::cerr << "Output key was set to `" << redisOutputKey << "`." << std::endl;
        }
    }
    else {
        if (VERBOSE) {
            std::cerr << "No output key was specified. Output key was set to default (" << redisOutputKey  << ")." << std::endl;
        }
    }

    if (result.count("tegra-camera")) {
        TEGRA = true;
        if (VERBOSE) {
            std::cerr << "NVIDIA Tegra support enabled." << std::endl;
        }
    }

    if (result.count("c")) {
        cameraId = result["c"].as<int>();
        if (VERBOSE) {
            std::cerr << "Camera device ID was set to " << cameraId << "." << std::endl;
        }
    }
    else {
        std::cerr << "No camera device ID specified. Camera device ID was set to default (" << cameraId << ")." << std::endl;
    }

    if (result.count("u")) {
        STREAM_MODE = false;
        if (VERBOSE) {
            std::cerr << "Unique mode enabled." << std::endl;
        }
    }

    if (result.count("redis-port")) {
        redisPort = result["redis-port"].as<int>();
        if (VERBOSE) {
            std::cerr << "Redis port set to " << redisPort << "." << std::endl;
        }
    }
    else {
        if (VERBOSE) {
            std::cerr << "No redis port specified. Redis port was set to " << redisPort << "." << std::endl;
        }
    }

    if (result.count("redis-host")) {
        redisHost = result["redis-host"].as<std::string>();
        if (VERBOSE) {
            std::cerr << "Redis host set to " << redisHost << "." << std::endl;
        }
    }
    else {
        if (VERBOSE) {
            std::cerr << "No redis host was specified. Redis host was set to " << redisHost << "." << std::endl;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    cxxopts::Options options("rcamserv", "Camera server example program.");
    options.add_options()
            ("redis-host", "The host adress to which the redis client should try to connect", cxxopts::value<std::string>())
            ("redis-port", "The port to which the redis client should try to connect.", cxxopts::value<int>())
            ("o, output", "The redis output key where data are going to be published/set.", cxxopts::value<std::string>())
            ("s, stream", "Activate stream mode. In stream mode the program will constantly process input data and publish output data. By default stream mode is enabled.")
            ("u, unique", "Activate unique mode. In unique mode the program will only read and output data one time.")
            ("v, verbose", "Enable verbose mode. This will print helpfull process informations on the standard error stream.")
            ("tegra-camera", "NVIDIA Tegra specific option. When this is set the program will try to access tegra camera using gstreamer command.")
            ("c, camera-id", "Set camera device id to get frames from. If tegra-camera is set this will have no effect.", cxxopts::value<int>())
            ("h, help", "Print this help message.");

    int retCode = parseCommandLine(options, argc, argv);
    if (retCode)
    {
        return EXIT_FAILURE;
    }

    RedisCameraServer server(redisHost, redisPort, redisOutputKey);

    std::string gstCommand = "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)I420, framerate=(fraction)120/1, queue-size=2, blockSize=16384, auto-exposure=1, scene-mode=1, flicker=0 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

    bool serverStarted;
    if (TEGRA) {
        serverStarted = server.start(gstCommand);
    }
    else {
        serverStarted = server.start(cameraId);
    }
    if (!serverStarted)
    {
        std::cerr << "Failed to start camera server." << std::endl;
        return EXIT_FAILURE;
    }

    // Set image format into redis
    server.setCameraParameters(redisOutputKey);

    if (STREAM_MODE) {
        while (true) {
            server.outputCameraFrame(true, redisOutputKey);
        }
    }
    else {
        server.outputCameraFrame(false, redisOutputKey);
    }
    return EXIT_SUCCESS;
}

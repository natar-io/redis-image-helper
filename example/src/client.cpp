#include <iostream>
#include <opencv2/opencv.hpp>

#include <RedisImageHelper.hpp>
#include <cxxopts.hpp>
#include "ImageUtils.hpp"

bool VERBOSE = false;
bool STREAM_MODE = true;
std::string redisInputKey = "custom:image";
std::string redisHost = "127.0.0.1";
int redisPort = 6379;

struct cameraParams {
    uint width;
    uint height;
    uint channels;
};

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

    if (result.count("i")) {
        redisInputKey = result["i"].as<std::string>();
        if (VERBOSE) {
            std::cerr << "Input key was set to `" << redisInputKey << "`." << std::endl;
        }
    }
    else {
        if (VERBOSE) {
            std::cerr << "No input key was specified. Input key was set to default (" << redisInputKey << ")." << std::endl;
        }
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

void onImagePublished(redisAsyncContext* c, void* data, void* privdata)
{
    struct cameraParams* cameraParams = static_cast<struct cameraParams*>(privdata);
    if (cameraParams == NULL) {
        if(VERBOSE) {
            std::cerr << "Could not read camera parameters from private data." << std::endl;
        }
        return;
    }
    uint width = cameraParams->width;
    uint height = cameraParams->height;
    uint channels = cameraParams->channels;

    redisReply *reply = (redisReply*) data;
    if  (reply == NULL)
    {
        return;
    }
    if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 3)
    {
        cv::Mat displayFrame;
        Image* cFrame = RedisImageHelper::dataToImage(reply->element[2]->str, width, height, channels, reply->element[2]->len);
        if (cFrame == NULL) {
            if (VERBOSE) {
                std::cerr << "Could not retrieve image from data." << std::endl;
            }
            return;
        }
        cv::Mat frame = cv::Mat(cFrame->height(), cFrame->width(), CV_8UC3, (void*)cFrame->data());
        cv::cvtColor(frame, displayFrame, CV_RGB2BGR);
        cv::imshow("frame", displayFrame);
        cv::waitKey(30);
    }
}

int main(int argc, char** argv)
{
    cxxopts::Options options("rcamcli", "Camera client sample program.");
    options.add_options()
            ("redis-host", "The host adress to which the redis client should try to connect", cxxopts::value<std::string>())
            ("redis-port", "The port to which the redis client should try to connect.", cxxopts::value<int>())
            ("i, input", "The redis input key where data are going to arrive.", cxxopts::value<std::string>())
            ("s, stream", "Activate stream mode. In stream mode the program will constantly process input data and publish output data. By default stream mode is enabled.")
            ("u, unique", "Activate unique mode. In unique mode the program will only read and output data one time.")
            ("v, verbose", "Enable verbose mode. This will print helpfull process informations on the standard error stream.")
            ("h, help", "Print this help message.");

    int retCode = parseCommandLine(options, argc, argv);
    if (retCode)
    {
        return EXIT_FAILURE;
    }

    if (STREAM_MODE) {
        RedisImageHelperAsync clientAsync(redisHost, redisPort, redisInputKey);
        if (!clientAsync.connect()) {
            std::cerr << "Could not connect to redis server." << std::endl;
            return EXIT_FAILURE;
        }

        RedisImageHelperSync clientSync(redisHost, redisPort, redisInputKey);
        if (!clientSync.connect()) {
            std::cerr << "Could not connect to redis server." << std::endl;
            return EXIT_FAILURE;
        }

        // When the camera server is started, it automatically set camera parameters.
        uint width = clientSync.getInt(redisInputKey + ":width");
        uint height = clientSync.getInt(redisInputKey + ":height");
        uint channels = clientSync.getInt(redisInputKey + ":channels");
        struct cameraParams cameraParams;
        cameraParams.width = width;
        cameraParams.height = height;
        cameraParams.channels = channels;
        clientAsync.subscribe(redisInputKey, onImagePublished, static_cast<void*>(&cameraParams));
        return EXIT_SUCCESS;
    }
    else {
        RedisImageHelperSync client(redisHost, redisPort, redisInputKey);
        if (!client.connect()) {
            std::cerr << "Could not connect to redis server." << std::endl;
            return EXIT_FAILURE;
        }

        cv::Mat displayFrame;
        Image* image = client.getImage();
        if (image == NULL) { std::cerr << "Error: Could not get camera frame, exiting..." << std::endl; return EXIT_FAILURE;}
        cv::Mat frame = cv::Mat(image->height(), image->width(), CV_8UC3, (void*)image->data());
        cv::cvtColor(frame, displayFrame, CV_RGB2BGR);
        cv::imshow("frame", displayFrame);
        cv::waitKey();
        delete image;

        return EXIT_SUCCESS;
    }
}

#include <iostream>
#include <opencv2/opencv.hpp>

#include "RedisCameraClient.hpp"
#include "ImageUtils.hpp"

int main(int argc, char** argv)
{
    RedisCameraClient client;
    if (!client.connect()) { std::cerr << "Client could not connect to redis server" << std::endl; }
    client.setCameraKey("custom:image");

    while (true)
    {
        CameraFrame* cFrame = client.getCameraFrame();
        cv::Mat frame(cFrame->height(), cFrame->width(), CV_8UC3, (void*)cFrame->data());
        cv::imshow("frame", frame);
        if (cv::waitKey(30) >= 0)
        {
            break;
        }
        delete cFrame;
    }
}

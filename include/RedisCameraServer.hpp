#ifndef REDISCAMERASERVER_H
#define REDISCAMERASERVER_H

#include <opencv2/opencv.hpp>
#include "RedisCameraClient.hpp"

class RedisCameraServer
{
private:
    RedisCameraClient* m_cameraClient;
    cv::VideoCapture* m_camera;
public:
    RedisCameraServer();
    bool start();
    void pickUpCameraFrame();
    void setCameraKey(std::string cameraKey) { m_cameraClient->setCameraKey(cameraKey); };
};

#endif // REDISCAMERASERVER_H

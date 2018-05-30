#ifndef REDISCAMERASERVER_H
#define REDISCAMERASERVER_H

#include <opencv2/opencv.hpp>
#include <RedisImageHelper.hpp>

class RedisCameraServer
{
private:
    RedisImageHelper* m_imageClient;
    cv::VideoCapture* m_camera;
public:
    RedisCameraServer();
    bool start(std::string command);
    bool start();
    void pickUpCameraFrame();
    void setCameraKey(std::string cameraKey) { m_imageClient->setCameraKey(cameraKey); };
};

#endif // REDISCAMERASERVER_H

#ifndef _CAMERASERVER_HPP_
#define _CAMERASERVER_HPP_

#include "RedisCameraClient.hpp"
#include <opencv2/opencv.hpp>

class CameraServer
{
private:
    RedisCameraClient m_redisClient;
public:
    void pickUpCameraFrame();
};

#endif


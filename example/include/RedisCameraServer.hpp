#ifndef REDISCAMERASERVER_H
#define REDISCAMERASERVER_H

#include <opencv2/opencv.hpp>
#include <RedisImageHelper.hpp>

class RedisCameraServer
{
private:
    RedisImageHelperSync* m_imageClient;
    cv::VideoCapture* m_camera;
public:
    RedisCameraServer();
    RedisCameraServer(std::string host, int port, std::string mainKey);
    ~RedisCameraServer() { delete m_imageClient; delete m_camera; }
    bool start(std::string command);
    bool start(int cameraId = 0);
    void setCameraParameters(std::string outputKey);
    void outputCameraFrame(bool publish, std::string outputKey);
    void setMainKey(std::string mainKey) { m_imageClient->setMainKey(mainKey); };
};

#endif // REDISCAMERASERVER_H

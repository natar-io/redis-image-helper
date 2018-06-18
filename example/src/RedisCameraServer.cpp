#include <RedisCameraServer.hpp>

RedisCameraServer::RedisCameraServer()
{
    m_imageClient = new RedisImageHelperSync();
}

RedisCameraServer::RedisCameraServer(std::string host, int port, std::string mainKey)
{
    m_imageClient = new RedisImageHelperSync(host, port, mainKey);
}

bool RedisCameraServer::start(std::string gstreamerCommand)
{
    if (!m_imageClient->connect())
    {
        std::cout << "Could not connect" << std::endl;
        return false;
    }

    m_camera = new cv::VideoCapture(gstreamerCommand.c_str());

    if (!m_camera->isOpened())
    {
        std::cout << "Could not open video capture device" << std::endl;
        return false;
    }

    return true;
}

bool RedisCameraServer::start(int cameraId)
{
    if (!m_imageClient->connect())
    {
        std::cout << "Could not connect" << std::endl;
        return false;
    }

    m_camera = new cv::VideoCapture(cameraId);

    if (!m_camera->isOpened())
    {
        std::cout << "Could not open video capture device" << std::endl;
        return false;
    }
    return true;
}

void RedisCameraServer::outputCameraFrame(bool publish, std::string outputKey)
{
    cv::Mat frame, RGBFrame;
    *m_camera >> frame;
    cv::cvtColor(frame, RGBFrame, CV_BGR2RGB);

    Image* image = new Image(RGBFrame.cols, RGBFrame.rows, RGBFrame.channels(), RGBFrame.data);
    if (publish)
        m_imageClient->publishImage(image, outputKey);
    else
        m_imageClient->setImage(image, outputKey);
    delete image;
}

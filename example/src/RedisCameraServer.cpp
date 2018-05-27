#include <RedisCameraServer.hpp>

RedisCameraServer::RedisCameraServer()
{
    m_cameraClient = new RedisCameraClient();
}

bool RedisCameraServer::start(std::string gstreamerCommand)
{
    if (!m_cameraClient->connect())
    {
        std::cout << "Could not connect" << std::endl;
        return false;
    }

    if (gstreamerCommand.empty())
    {
        m_camera = new cv::VideoCapture(0);
    }
    else {
        m_camera = new cv::VideoCapture(gstreamerCommand.c_str());
    }

    if (!m_camera->isOpened())
    {
        std::cout << "Could not open video capture device" << std::endl;
        return false;
    }

    return true;
}

bool RedisCameraServer::start()
{
    return this->start("");
}

void RedisCameraServer::pickUpCameraFrame()
{
    cv::Mat frame, RGBFrame;
    *m_camera >> frame;
    cv::cvtColor(frame, RGBFrame, CV_BGR2RGB);

    CameraFrame* cFrame = new CameraFrame(RGBFrame.cols, RGBFrame.rows, RGBFrame.channels(), RGBFrame.data);
    m_cameraClient->setCameraFrame(cFrame);
}

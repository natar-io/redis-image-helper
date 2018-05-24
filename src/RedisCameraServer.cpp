#include "RedisCameraServer.hpp"

RedisCameraServer::RedisCameraServer()
{
    m_cameraClient = new RedisCameraClient();
}

bool RedisCameraServer::start()
{
    if (!m_cameraClient->connect())
    {
        std::cout << "Could not connect" << std::endl;
        return false;
    }

    const char* gst = "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)I420, framerate=(fraction)120/1, queue-size=2, blockSize=16384, auto-exposure=1, scene-mode=1, flicker=0"
                      "! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    m_camera = new cv::VideoCapture(gst);
    if (!m_camera->isOpened())
    {
        std::cout << "Could not open video capture device" << std::endl;
        return false;
    }

    return true;
}

void RedisCameraServer::pickUpCameraFrame()
{
    cv::Mat frame, RGBFrame;
    *m_camera >> frame;
    cv::cvtColor(frame, RGBFrame, CV_BGR2RGB);

    CameraFrame* cFrame = new CameraFrame(RGBFrame.cols, RGBFrame.rows, RGBFrame.channels(), RGBFrame.data);
    m_cameraClient->setCameraFrame(cFrame);
}

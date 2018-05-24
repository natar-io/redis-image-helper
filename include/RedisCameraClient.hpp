#ifndef _REDIS_CAMERA_CLIENT_HPP_
#define _REDIS_CAMERA_CLIENT_HPP_

#include <hiredis/hiredis.h>
#include <string>

#include "CameraFrame.hpp"

class RedisCameraClient
{
private:
    redisContext* m_context;
    redisReply* m_reply;
    std::string m_hostname;
    std::string m_cameraKey;
    int m_port;

    int getInt(std::string intKey);
    char* getString(std::string stringKey, size_t& dataLength);
    void setInt(std::string intKey, int value);
    void setString(std::string stringKey, char* value);

public:
    RedisCameraClient() : m_hostname("127.0.0.1"), m_port(6379), m_cameraKey("nectar:jiii-mi:camera-server:camera#0") {};
    RedisCameraClient(std::string hostname, int port, std::string cameraKey) : m_hostname(hostname), m_port(port), m_cameraKey(cameraKey) {};

    ~RedisCameraClient() { redisFree(m_context); };

    bool connect();
    CameraFrame* getCameraFrame();
    void setCameraFrame(CameraFrame* frame);

    void setCameraKey(std::string cameraKey) { m_cameraKey = cameraKey; };
};

#endif

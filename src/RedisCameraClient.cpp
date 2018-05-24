#include "RedisCameraClient.hpp"

//TOREMOVE:
#include <iostream>

bool RedisCameraClient::connect()
{
    struct timeval timeout = {1, 500000};
    m_context = redisConnectWithTimeout(m_hostname.c_str(), m_port, timeout);
    if (m_context == NULL || m_context->err)
    {
        return false;
    }

    return true;
}

CameraFrame* RedisCameraClient::getCameraFrame()
{
    int width = getInt(m_cameraKey + ":width");
    if (width == -1) return nullptr;
    int height = getInt(m_cameraKey + ":height");
    if (height == -1) return nullptr;

    size_t dataLength;
    char* data = getString(m_cameraKey, dataLength);
    //WARN: We consider that the redis image is stored in RGB8 format
    if (data == nullptr || dataLength != width * height * 3) return nullptr;
    unsigned char* pixels = reinterpret_cast<unsigned char*>(data);
    return new CameraFrame(width, height, 3, pixels);
}

void RedisCameraClient::setCameraFrame(CameraFrame *frame)
{
    unsigned int width = frame->width();
    unsigned int height = frame->height();
    unsigned int channels = frame->channels();
    int size = width * height * channels;
    char* data = reinterpret_cast<char*>(frame->data());
    setInt(m_cameraKey + ":width", width);
    setInt(m_cameraKey + ":height", height);
    m_reply = (redisReply*)redisCommand(m_context, "SET %b %b", m_cameraKey.c_str(), (size_t)m_cameraKey.length(), data, size);
    // TODO: Redo that ?
    // Maybe we should not SET the image but we should publish it ?
    m_reply = (redisReply*)redisCommand(m_context, "PUBLISH cfr ready");
}

int RedisCameraClient::getInt(std::string intKey)
{
    m_reply = (redisReply*)redisCommand(m_context, "GET %s", intKey.c_str());
    if (m_reply == NULL || m_reply->type == REDIS_REPLY_NIL || m_reply->type != REDIS_REPLY_STRING)
    {
        return -1;
    }
    int value = atoi(m_reply->str);
    return value;
}

void RedisCameraClient::setInt(std::string intKey, int value)
{
    std::string command = "SET " + intKey + " " + std::to_string(value);
    m_reply = (redisReply*)redisCommand(m_context, command.c_str());
}

char* RedisCameraClient::getString(std::string stringKey, size_t& dataLength)
{
    m_reply = (redisReply*)redisCommand(m_context, "GET %s", stringKey.c_str());
    dataLength = m_reply->len;
    if (m_reply == NULL || m_reply->type == REDIS_REPLY_NIL || m_reply->type != REDIS_REPLY_STRING)
    {
        return nullptr;
    }
    return m_reply->str;
}

void RedisCameraClient::setString(std::string stringKey, char* value)
{
    m_reply = (redisReply*)redisCommand(m_context, "SET %s %s", stringKey, value);
}



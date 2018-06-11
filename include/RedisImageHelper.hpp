#ifndef _REDIS_IMAGE_HELPER_HPP_
#define _REDIS_IMAGE_HELPER_HPP_

#include <hiredis/hiredis.h>
#include <string>

#include "Image.hpp"

class RedisImageHelper
{
private:
    redisContext* m_context;
    redisReply* m_reply;
    std::string m_hostname;
    std::string m_mainKey;
    int m_port;

public:
    RedisImageHelper() : m_hostname("127.0.0.1"), m_port(6379), m_mainKey("nectar:jiii-mi:camera-server:camera#0") {};
    RedisImageHelper(std::string hostname, int port, std::string cameraKey) : m_hostname(hostname), m_port(port), m_mainKey(cameraKey) {};

    ~RedisImageHelper() { redisFree(m_context); };

    bool connect();

    Image*  getImage(std::string imageKey);
    Image*  getImage() { return getImage(m_mainKey); }
    int     getInt(std::string intKey);
    char*   getString(std::string stringKey, size_t& dataLength);

    void setImage(Image* image, std::string imageKey);
    void setImage(Image* image) { setImage(image, m_mainKey); }
    void setInt(int value, std::string intKey);
    void setString(char* value, std::string stringKey);

    void publishImage(Image* image, std::string publishKey);
    void publishInt(int value, std::string publishKey);
    void publishString(char* value, std::string publishKey);

    /*
    void publish(Image* image, std::string publishKey);
    void publish(int value, std::string publishKey);
    void publish(char* value, std:string publishKey);
    */

    void setMainKey(std::string mainKey) { m_mainKey = mainKey; };
};

#endif // REDIS_IMAGE_HELPER

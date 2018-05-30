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
    std::string m_cameraKey;
    int m_port;

    int getInt(std::string intKey);
    char* getString(std::string stringKey, size_t& dataLength);
    void setInt(std::string intKey, int value);
    void setString(std::string stringKey, char* value);

public:
    RedisImageHelper() : m_hostname("127.0.0.1"), m_port(6379), m_cameraKey("nectar:jiii-mi:camera-server:camera#0") {};
    RedisImageHelper(std::string hostname, int port, std::string cameraKey) : m_hostname(hostname), m_port(port), m_cameraKey(cameraKey) {};

    ~RedisImageHelper() { redisFree(m_context); };

    bool connect();

    Image* getImage();
    void setImage(Image* image, bool isOutput = false);

    void publishImage(Image* image, std::string publishKey);

    void setCameraKey(std::string cameraKey) { m_cameraKey = cameraKey; };
};

#endif // REDIS_IMAGE_HELPER

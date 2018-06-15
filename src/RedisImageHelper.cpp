#include "RedisImageHelper.hpp"
#include <iostream>
bool RedisImageHelperSync::connect()
{
    struct timeval timeout = {1, 500000};
    m_context = redisConnectWithTimeout(m_host.c_str(), m_port, timeout);
    if (m_context == NULL || m_context->err)
    {
        return false;
    }

    return true;
}

bool RedisImageHelperAsync::connect()
{
    m_context = redisAsyncConnect(m_host.c_str(), m_port);
    if (m_context == NULL || m_context->err)
    {
        return false;
    }
    m_event = event_base_new();
    redisLibeventAttach(m_context, m_event);
    return true;
}

void RedisImageHelperAsync::subscribe(std::string subscriptionKey, void (*callback)(redisAsyncContext *, void *, void *))
{
    std::string command = "SUBSCRIBE " + subscriptionKey;
    redisAsyncCommand(m_context, callback, (void*) NULL, command.c_str());
}

/*
Image* RedisImageHelper::getImage(std::string imageKey)
{
    int width = getInt(imageKey + ":width");
    if (width == -1) return NULL;
    int height = getInt(imageKey + ":height");
    if (height == -1) return NULL;
    int channels = getInt(imageKey + ":channels");
    if (channels == -1) return NULL;

    size_t dataLength;
    char* data = getString(imageKey, dataLength);
    if (data == NULL || dataLength != width * height * channels) return NULL;
    unsigned char* pixels = reinterpret_cast<unsigned char*>(data);
    return new Image(width, height, channels, pixels);
}

int RedisImageHelper::getInt(std::string intKey)
{
    m_reply = (redisReply*)redisCommand(m_context, "GET %s", intKey.c_str());
    if (m_reply == NULL || m_reply->type == REDIS_REPLY_NIL || m_reply->type != REDIS_REPLY_STRING)
    {
        return -1;
    }
    int value = atoi(m_reply->str);
    return value;
}

char* RedisImageHelper::getString(std::string stringKey, size_t& dataLength)
{
    m_reply = (redisReply*)redisCommand(m_context, "GET %s", stringKey.c_str());
    dataLength = m_reply->len;
    if (m_reply == NULL || m_reply->type == REDIS_REPLY_NIL || m_reply->type != REDIS_REPLY_STRING)
    {
        return NULL;
    }
    return m_reply->str;
}

void RedisImageHelper::setImage(Image* image, std::string imageKey)
{
    unsigned int width = image->width();
    unsigned int height = image->height();
    unsigned int channels = image->channels();
    int size = width * height * channels;
    char* data = reinterpret_cast<char*>(image->data());
    setInt(width, imageKey + ":width");
    setInt(height, imageKey + ":height");
    setInt(channels, imageKey + ":channels");
    m_reply = (redisReply*)redisCommand(m_context, "SET %b %b", imageKey.c_str(), (size_t)imageKey.length(), data, size);
}

void RedisImageHelper::setInt(int value, std::string intKey)
{
    std::string command = "SET " + intKey + " " + std::to_string(value);
    m_reply = (redisReply*)redisCommand(m_context, command.c_str());
}

void RedisImageHelper::setString(char* value, std::string stringKey)
{
    m_reply = (redisReply*)redisCommand(m_context, "SET %s %s", stringKey.c_str(), value);
}

void RedisImageHelper::publishImage(Image* image, std::string publishKey)
{
    unsigned int width = image->width();
    unsigned int height = image->height();
    unsigned int channels = image->channels();
    int size = width * height * channels;
    char* data = reinterpret_cast<char*>(image->data());
    publishString(data, publishKey);
    publishInt(width, publishKey + ":width");
    publishInt(height, publishKey + "height");
    publishInt(channels, publishKey + ":channels");
}

void RedisImageHelper::publishInt(int value, std::string publishKey)
{
    m_reply = (redisReply*)redisCommand(m_context, "PUBLISH %s %d", publishKey.c_str(), value);
}

void RedisImageHelper::publishString(char* value, std::string publishKey)
{
    m_reply = (redisReply*)redisCommand(m_context, "PUBLISH %s %s", publishKey.c_str(), value);
}*/


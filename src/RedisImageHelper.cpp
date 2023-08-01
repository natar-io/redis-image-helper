#include "RedisImageHelper.hpp"

#include <iostream>

Image* RedisImageHelper::dataToImage(char* data, uint width, uint height, uint channels, size_t dataLength)
{
    if (data == nullptr || dataLength != width * height * channels) {
        return nullptr;
    }
    unsigned char* pixels = reinterpret_cast<unsigned char*>(data);
    return new Image(width, height, channels, pixels);
}

bool RedisImageHelperSync::connect()
{
    struct timeval timeout = {1, 500000};
    m_context = redisConnectWithTimeout(m_host.c_str(), m_port, timeout);
    if (m_context == nullptr || m_context->err)
    {
        return false;
    }

    return true;
}

static void redisAsyncConnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        std::cerr << "Error: " << c->errstr << std::endl;
        return;
    }
}

bool RedisImageHelperAsync::connect()
{
    m_context = redisAsyncConnect(m_host.c_str(), m_port);
    if (m_context == nullptr || m_context->err)
    {
        return false;
    }
    redisLibevAttach(EV_DEFAULT_ m_context);
    redisAsyncSetConnectCallback(m_context, redisAsyncConnectCallback);
    return true;
}

void RedisImageHelperAsync::subscribe(std::string subscriptionKey, void (*callback)(redisAsyncContext *, void *, void *), void* privdata)
{
    std::string command = "SUBSCRIBE " + subscriptionKey;
    redisAsyncCommand(m_context, callback, privdata, command.c_str());
}

Image* RedisImageHelperSync::getImage(uint width, uint height, uint channels, std::string imageKey)
{
    size_t dataLength;
    char* data = getString(imageKey, dataLength);
    if (data == nullptr) {
        return nullptr;
    }
    Image* image = dataToImage(data, width, height, channels, dataLength);
    delete data;
    return image;
}

int RedisImageHelperSync::getInt(std::string intKey)
{
    m_reply = (redisReply*)redisCommand(m_context, "GET %s", intKey.c_str());
    if (m_reply == nullptr || m_reply->type == REDIS_REPLY_NIL || m_reply->type != REDIS_REPLY_STRING)
    {
        freeReplyObject(m_reply);
        return -1;
    }
    int value = atoi(m_reply->str);
    freeReplyObject(m_reply);
    return value;
}

char* RedisImageHelperSync::getString(std::string stringKey, size_t& dataLength)
{
    m_reply = (redisReply*)redisCommand(m_context, "GET %s", stringKey.c_str());
    dataLength = m_reply->len;
    if (m_reply == nullptr || m_reply->type == REDIS_REPLY_NIL || m_reply->type != REDIS_REPLY_STRING)
    {
        freeReplyObject(m_reply);
        return nullptr;
    }
    char* string = new char[dataLength];
    std::copy(m_reply->str, m_reply->str + dataLength * sizeof(char), string);
    freeReplyObject(m_reply);
    return string;
}

void RedisImageHelperSync::setImage(Image* image, std::string imageKey)
{
    unsigned int width = image->width(); setInt(width, imageKey + ":width");
    unsigned int height = image->height(); setInt(height, imageKey + ":height");
    unsigned int channels = image->channels(); setInt(channels, imageKey + ":channels");
    int size = width * height * channels;
    char* data = reinterpret_cast<char*>(image->data());
    m_reply = (redisReply*)redisCommand(m_context, "SET %b %b", imageKey.c_str(), (size_t)imageKey.length(), data, size);
}

void RedisImageHelperSync::setInt(int value, std::string intKey)
{
    std::string command = "SET " + intKey + " " + std::to_string(value);
    m_reply = (redisReply*)redisCommand(m_context, command.c_str());
}

void RedisImageHelperSync::setString(std::string value, std::string stringKey)
{
    m_reply = (redisReply*)redisCommand(m_context, "SET %s %s", stringKey.c_str(), value.c_str());
}

// Deprecated
void RedisImageHelperSync::publishImage(Image* image, std::string publishKey)
{
    unsigned int width = image->width();
    unsigned int height = image->height();
    unsigned int channels = image->channels();
    int size = width * height * channels;
    char* data = reinterpret_cast<char*>(image->data());
    m_reply = (redisReply*) redisCommand(m_context, "PUBLISH %b %b", publishKey.c_str(), (size_t)publishKey.length(), data, size);
}

void RedisImageHelperSync::publishInt(int value, std::string publishKey)
{
    m_reply = (redisReply*)redisCommand(m_context, "PUBLISH %s %d", publishKey.c_str(), value);
}

void RedisImageHelperSync::publishString(std::string value, std::string publishKey)
{
    m_reply = (redisReply*)redisCommand(m_context, "PUBLISH %s %s", publishKey.c_str(), value.c_str());
}

redisReply* RedisImageHelperSync::executeCommand(std::string command) {
    return (redisReply*) redisCommand(m_context, command.c_str());
}

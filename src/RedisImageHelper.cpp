#include "RedisImageHelper.hpp"

bool RedisImageHelper::connect()
{
    struct timeval timeout = {1, 500000};
    m_context = redisConnectWithTimeout(m_hostname.c_str(), m_port, timeout);
    if (m_context == NULL || m_context->err)
    {
        return false;
    }

    return true;
}

Image* RedisImageHelper::getImage()
{
    int width = getInt(m_cameraKey + ":width");
    if (width == -1) return NULL;
    int height = getInt(m_cameraKey + ":height");
    if (height == -1) return NULL;

    size_t dataLength;
    char* data = getString(m_cameraKey, dataLength);
    //WARN: We consider that the redis image is stored in RGB8 format
    if (data == NULL || dataLength != width * height * 3) return NULL;
    unsigned char* pixels = reinterpret_cast<unsigned char*>(data);
    return new Image(width, height, 3, pixels);
}

void RedisImageHelper::setImage(Image* image, bool isOutput)
{
    unsigned int width = image->width();
    unsigned int height = image->height();
    unsigned int channels = image->channels();
    int size = width * height * channels;
    char* data = reinterpret_cast<char*>(image->data());
    setInt(m_cameraKey + ":width", width);
    setInt(m_cameraKey + ":height", height);
    m_reply = (redisReply*)redisCommand(m_context, "SET %b %b", m_cameraKey.c_str(), (size_t)m_cameraKey.length(), data, size);
    // TODO: Redo that ?
    // Maybe we should not SET the image but we should publish it ?
    if (!isOutput)
    {
        m_reply = (redisReply*)redisCommand(m_context, "PUBLISH cfr ready");
    }
    else {
        m_reply = (redisReply*)redisCommand(m_context, "PUBLISH cft treated");
    }
}

void RedisImageHelper::publishImage(Image* image, std::string publishKey)
{
    unsigned int width = image->width();
    unsigned int height = image->height();
    unsigned int channels = image->channels();
    int size = width * height * channels;
    char* data = reinterpret_cast<char*>(image->data());
    m_reply = (redisReply*)redisCommand(m_context, "PUBLISH %s %s", publishKey.c_str(), data);
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

void RedisImageHelper::setInt(std::string intKey, int value)
{
    std::string command = "SET " + intKey + " " + std::to_string(value);
    m_reply = (redisReply*)redisCommand(m_context, command.c_str());
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

void RedisImageHelper::setString(std::string stringKey, char* value)
{
    m_reply = (redisReply*)redisCommand(m_context, "SET %s %s", stringKey.c_str(), value);
}



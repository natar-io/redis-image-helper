#ifndef _REDIS_IMAGE_HELPER_HPP_
#define _REDIS_IMAGE_HELPER_HPP_

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include <string>

#include "Image.hpp"

class RedisImageHelper {
protected:
    int m_port;
    std::string m_host;
    std::string m_mainKey;

public:
    RedisImageHelper() : m_host("127.0.0.1"), m_port(6379), m_mainKey("") {}
    RedisImageHelper(std::string host, int port, std::string mainKey = "") : m_host(host), m_port(port), m_mainKey(mainKey){}
    void setMainKey(std::string mainKey) { m_mainKey = mainKey; }

    virtual bool connect() = 0;
    //virtual void disconnect() = 0;
    virtual ~RedisImageHelper() {}
};

class RedisImageHelperSync : RedisImageHelper {
private:
    redisContext* m_context;
    redisReply* m_reply;

public:
    RedisImageHelperSync() : RedisImageHelper() {}
    RedisImageHelperSync(std::string host, int port, std::string mainKey) : RedisImageHelper(host, port, mainKey) {}

    bool connect() override = 0;
    ~RedisImageHelperSync() { redisFree(m_context); }

    /*
    Image*  getImage(std::string imageKey);
    Image*  getImage() { return getImage(m_mainKey); }
    int     getInt(std::string intKey);
    char*   getString(std::string stringKey, size_t& dataLength);

    void setImage(Image* image, std::string imageKey);
    void setImage(Image* image) { setImage(image, m_mainKey); }
    void setInt(int value, std::string intKey);
    void setString(char* value, std::string stringKey);
    */
};

class RedisImageHelperAsync : RedisImageHelper {
private:
    redisAsyncContext* m_context;
    struct event_base* m_event;

public:
    RedisImageHelperAsync() : RedisImageHelper() {}
    RedisImageHelperAsync(std::string host, int port, std::string mainKey) : RedisImageHelper(host, port, mainKey) {}

    bool connect() override = 0;
    ~RedisImageHelperAsync() { event_base_dispatch(m_event); redisAsyncFree(m_context);}

    void subscribe(std::string subscriptionKey, void(*callback)(redisAsyncContext*, void*, void*));
};

#endif //_REDIS_IMAGE_HELPER_HPP_

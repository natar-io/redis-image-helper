#ifndef _REDIS_IMAGE_HELPER_HPP_
#define _REDIS_IMAGE_HELPER_HPP_

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libev.h>
#include <string>

#include "Image.hpp"

/**
 * @brief The RedisImageHelper class is an abstract class used to
 * help manipulating images in redis.
 * It provides helper functions to deal with images, int and strings.
 */
class RedisImageHelper {
protected:
    int m_port;
    std::string m_host;
    std::string m_mainKey;

public:
    /**
     * @brief RedisImageHelper empty constructor.
     * By default a redis server is running on 127.0.0.1:6379.
     */
    RedisImageHelper() : m_host("127.0.0.1"), m_port(6379), m_mainKey("") {}

    /**
     * @brief RedisImageHelper constructor
     * @param host the host of the redis server.
     * @param port the port on which the redis server is running
     * @param mainKey the base key that will be used if no keys are specified during get/set/publish operations.
     */
    RedisImageHelper(std::string host, int port, std::string mainKey = "") : m_host(host), m_port(port), m_mainKey(mainKey){}

    /**
     * @brief setMainKey
     * @param mainKey the new mainKey to set.
     */
    void setMainKey(std::string mainKey) { m_mainKey = mainKey; }

    /**
     * @brief dataToImage Helper function that is used to convert raw redis data into an Image.
     * @param data the raw data got from redis.
     * @param width the width of the image.
     * @param height the height of the image.
     * @param channels the channel number of the image.
     * @param dataLength the length of the data array.
     * @return a new Image built from the given data.
     */
    static Image* dataToImage(char* data, uint width, uint height, uint channels, size_t dataLength);

    /**
     * @brief connect Pure virtual function that tries to connect to a redis server.
     * @return true if connected if not false.
     */
    virtual bool connect() = 0;

    virtual ~RedisImageHelper() {}
};

/**
 * @brief The RedisImageHelperSync class inherits from the RedisImageHelper class.
 * It is the implementation of a synchronous redis client.
 * It supports get/set/publish of images/int/strings into redis.
 */
class RedisImageHelperSync : public RedisImageHelper {
private:
    redisContext* m_context;
    redisReply* m_reply;

public:
    RedisImageHelperSync() : RedisImageHelper() {}
    RedisImageHelperSync(std::string host, int port, std::string mainKey) : RedisImageHelper(host, port, mainKey) {}

    /**
     * @brief connect Tries to connect synchronously to a redis server.
     * @return true if connect if not false
     */
    bool connect() override;
    ~RedisImageHelperSync() { redisFree(m_context); }

    /**
     * @brief getImage Gets image data from redis and tries to create an Image.
     * It uses redis BINARY GET command.
     * @param width the width of the image to get
     * @param height the height of the image to get
     * @param channels the channel number of the image to get.
     * @param imageKey the key where to get data in redis.
     * @return NULL if no data were found if not the created Image.
     */

    Image*  getImage(uint width, uint height, uint channels, std::string imageKey);

    /**
     * @brief getImage Gets image data from default key in redis and tries to create an Image.
     * @param width the width of the image to get
     * @param height the height of the image to get
     * @param channels the channel number of the image to get.
     * @return NULL if no data were found or data size does not match width * height * channels if not the created Image.
     */
    Image*  getImage(uint width, uint height, uint channels) { return getImage(width, height, channels, m_mainKey); }

    /**
     * @brief getInt Gets an int from redis.
     * It uses redis GET command.
     * @param intKey the key where to get the int in redis.
     * @return -1 if no int was found at given key if not the int.
     */
    int     getInt(std::string intKey);

    /**
     * @brief getInt Gets an int from redis at default key.
     * @return -1 if no int was found at given key if not the int.
     */
    int     getInt() { return getInt(m_mainKey); }

    /**
     * @brief getString Gets a string from redis.
     * It uses redis GET command.
     * @param stringKey the key where to get the string in redis.
     * @param dataLength a reference to a variable that will be filled with the length of the fetched string.
     * @return NULL if no string was found if not the string.
     */
    char*   getString(std::string stringKey, size_t& dataLength);

    /**
     * @brief getString Gets a string from redis at default key.
     * @param dataLength a reference to a variable that will be filled with the length of the fetched string.
     * @return NULL if no string was found if not the string.
     */
    char*   getString(size_t& dataLength) { return getString(m_mainKey, dataLength); }

    /**
     * @brief setImage Sets an Image into redis.
     * It uses redis BINARY SET command.
     * It also set width height and channels properties of the image into redis.
     * These properties are respectivly set at imageKey + ":width:", ":height", ":channels"
     * @param image the image to set into redis.
     * @param imageKey the key where to set the image.
     */
    void setImage(Image* image, std::string imageKey);

    /**
     * @brief setImage Sets an Image into redis at default key.
     * @param image the image to set into redis.
     */
    void setImage(Image* image) { setImage(image, m_mainKey); }

    /**
     * @brief setInt Sets an int into redis.
     * It uses redis SET command.
     * @param value the int to set into redis.
     * @param intKey the key where to set the int.
     */
    void setInt(int value, std::string intKey);

    /**
     * @brief setInt Sets an int into redis at default key.
     * @param value the int to set into redis.
     */
    void setInt(int value) { setInt(value, m_mainKey); }

    /**
     * @brief setString Sets a string into redis.
     * It uses redis SET command
     * @param value the string to set into redis.
     * @param stringKey the key where to set the string.
     */
    void setString(char* value, std::string stringKey);

    /**
     * @brief setString Sets a string into redis at default key.
     * @param value the string to set into redis.
     */
    void setString(char* value) { setString(value, m_mainKey); }

    /**
     * @brief publishImage Publishes an image into redis.
     * It uses redis BINARY PUBLISH command.
     * @param image the Image to publish.
     * @param publishKey the key where to publish the Image.
     */
    void publishImage(Image* image, std::string publishKey);


    /**
     * @brief publishImage Publishes an Image into redis.
     * @param image the Image to publish.
     * @param publishKey the key where to publish the Image.
     */
    void publishImage(Image* image) { publishImage(image, m_mainKey); }

    /**
     * @brief publishInt Publishes an int into redis.
     * It uses redis PUBLISH command.
     * @param value the int to publish into redis.
     * @param publishKey the key where to publish the int.
     */
    void publishInt(int value, std::string publishKey);

    /**
     * @brief publishInt Publishes an int into redis at default key.
     * @param value the int to publish into redis.
     */
    void publishInt(int value) { publishInt(value, m_mainKey); }

    /**
     * @brief publishString Publishes a string into redis.
     * It uses redis PUBLISH command.
     * @param value the string to publish.
     * @param publishKey the key where to publish the string.
     */
    void publishString(char* value, std::string publishKey);

    /**
     * @brief publishString Publishes a string into redis at default key.
     * @param value the string to publish.
     */
    void publishString(char* value) { publishString(value, m_mainKey); }
};

/**
 * @brief The RedisImageHelperAsync class inherits from the RedisImageHelper class.
 * It is the implementation of an asynchronous redis client.
 * It uses libev as event library.
 * It provides simple way to subscribe to redis channels and handle subscribtion events.
 */
class RedisImageHelperAsync : public RedisImageHelper {
private:
    redisAsyncContext* m_context;

public:
    RedisImageHelperAsync() : RedisImageHelper() {}
    RedisImageHelperAsync(std::string host, int port, std::string mainKey) : RedisImageHelper(host, port, mainKey) {}

    /**
     * @brief connect Tries to connect asynchronously to a redis server.
     * @return true if connect if not false
     */
    bool connect() override;
    ~RedisImageHelperAsync() {
        ev_loop(EV_DEFAULT_ 0);
    }

    /**
     * @brief subscribe Subscribes to a redis channel and triggers a callback each time a message is received
     * on the subscribed channels.
     * It uses redis SUBSCRIBE command.
     * @param subscriptionKey the key to subscribe to.
     * @param callback a function pointer to the callback. the function should have the following
     * definition void myfunc(redisAsyncContext*, void*, void*)
     * @param privdata extra data that are passed as the 3rd parameters to the callback function.
     */
    void subscribe(std::string subscriptionKey, void(*callback)(redisAsyncContext*, void*, void*), void* privdata);
};

#endif //_REDIS_IMAGE_HELPER_HPP_

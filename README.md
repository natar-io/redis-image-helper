# RedisImageHelper

**C++** library that provides a simple way to handle image manipulation in redis.
RedisImageHelper is built upon redis C client **hiredis**.

## Prerequisites
The following needs to be installed before you can begin using **RedisImageHelper**:
- [Redis](https://redis.io/) **available as a packet** `sudo apt install redis-server` or on arch `yaourt -S redis-server`
- [CMake](https://cmake.org) **available as a packet** `sudo apt install cmake` or on arch `yaourt -S cmake`
- [Hiredis](https://github.com/redis/hiredis) **available as a packet** `sudo apt install hiredis` or on arch `yaourt -S hiredis`
- [Libev](https://github.com/enki/libev) **available as a packet** `sudo apt install libev-dev` or on arch `yaourt -S libev`

## Compiling & Installing
Go into project folder and run the following:
```
mkdir build
cd build
cmake ..
make -j8
sudo make install
```
This should install redisimagehelper library under **/usr/local/lib** or **/usr/local/lib64**.

You can check that the installation worked properly by running ``` find /usr -name "*redisimagehelper*"```.

## Running the example

See the following to see different usage of **RedisImageHelper**
- [redis-camera-client-server]() C++ applications that uses RedisImageHelper to get webcam images, set/publish them and then get them back from redis.
- [markers-example]() C++ application that uses RedisImageHelper and ARToolKitPlus to detect and stream markers positions into redis.

## How to use
**RedisImageHelper** library provides 2 classes:

- RedisImageHelperSync that will allow you to perform synchronous operations such as `GET` `SET` and `PUBLISH` on various types.
- RedisImageHelperAsync that will allow you to perform asynchronous operations such as `SUBSCRIBE`.

Both works the same, you first need to initialize a RedisImageHelper object and connect it to a running redis server:

#### Synchronous
```cpp
#include <RedisImageHelper.hpp>
int main(int argc, char** argv)
{
    // Initialize helper object with running server informations
    RedisImageHelperSync helperSync ("127.0.0.1", 6379);
    if (!helperSync.connect()) {
        std::cerr << "Could not connect to redis server."
        << "Please make sure a redis server is up and running." 
        << std::endl;
        return EXIT_FAILURE;
    }

    /* CODE */
}
```
#### Asynchronous
```cpp
#include <RedisImageHelper.hpp>
int main(int argc, char** argv)
{
    // Initialize helper object with running server informations
    RedisImageHelperAsync helperAsync ("127.0.0.1", 6379);
    if (!helperAsync.connect()) {
        std::cerr << "Could not connect to redis server."
        << "Please make sure a redis server is up and running." 
        << std::endl;
        return EXIT_FAILURE;
    }

    /* CODE */
}
```

### Get an Image

Getting an image is for now only possible when using the **synchronous** class:
```cpp
#include <RedisImageHelper.hpp>
int main(int argc, char** argv)
{
    // Initialize helper object with running server informations
    RedisImageHelperSync helper ("127.0.0.1", 6379);
    if (!helper.connect()) {
        std::cerr << "Could not connect to redis server."
        << "Please make sure a redis server is up and running." 
        << std::endl;
        return EXIT_FAILURE;
    }
    // This specify where we should find image data in redis
    helper.setMainKey("imagekey");

    // Get image informations at specified key
    Image* image = helper.getImage();
    if (image == NULL) {
        std::cerr << "Could not get image data at " + key << std:endl;
        return EXIT_FAILURE;
    }

    uint width = image->width();
    uint height = image->height();
    uint channels = image->channels();
    unsigned char* data = image->data();

    /* CODE */
}
```
**Note:** If you do not want to specify a global key to get and set image to, you can pass the key as a parameter like so `RedisImageHelperSync::get***(type value, std::string key)`.


### Set an image:

```cpp
#include <RedisImageHelper.hpp>
int main(int argc, char** argv)
{
    // Initialize helper object with running server informations
    RedisImageHelperSync helper ("127.0.0.1", 6379);
    if (!helper.connect()) {
        std::cerr << "Could not connect to redis server."
        << "Please make sure a redis server is up and running." 
        << std::endl;
        return EXIT_FAILURE;
    }
    // This specify where we should set the image data in redis
    helper.setMainKey("custom:image");

    uint width = ..., height = ..., channels = ...;
    unsigned char* data = ...; 
    // Create image object to be pushed into redis
    Image* image = new Image(width, height, channels, data);
    // Push image data into redis.
    // Pushed data are: width, height, channels and image data
    helper.setImage(image)

    /* CODE */
}
```
**Note:** If you do not want to specify a global key to get and set image to, you can pass the key as a parameter like so `RedisImageHelperSync::set***(type value, std::string key)`.

### Publish an image:

```cpp
#include <RedisImageHelper.hpp>

int main(int argc, char** argv)
{
    // Initialize helper object with running server informations
    RedisImageHelperSync helper ("127.0.0.1", 6379);
    if (!helper.connect()) {
        std::cerr << "Could not connect to redis server."
        << "Please make sure a redis server is up and running." 
        << std::endl;
        return EXIT_FAILURE;
    }
    // This specify where we should set the image data in redis
    helper.setMainKey("custom:image");

    uint width = ..., height = ..., channels = ...;
    unsigned char* data = ...; 
    // Create image object to be published into redis
    Image* image = new Image(width, height, channels, data);
    // Publish image data into redis.
    // Published data are: width, height, channels and image data
    helper.publishImage(image);
}
```
**Note:** If you do not want to specify a global key to publish an image to, you can pass the key as a parameter like so `RedisImageHelperSync::publish***(type value, std::string key)`.

### Subscribe to a channel and retrieve an image

Subscribing only possible with the **asynchronous** class:

```cpp
#include <RedisImageHelper.hpp>
#include <hiredis/async.h>

uint width = 640;
uint height = 480;
uint channels = 3;

void callback(redisAsyncContext* context, void* rep, void* privdata)
{
    redisReply* reply = (redisReply*) rep;
    // If the reply is null or is not an array of size 3, then this is a bad reply format.
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY || reply->elements != 3) {
        return;
    }

    // Getting command data from reply
    char* cmd = reply->element[0]->str;
    char* channel = reply->element[1]->str;
    // Getting image data from reply
    char* data = reply->element[2]->str;
    int dataLength = reply->element[2]->len;

    // Building image using helper static function
    Image* image = RedisImageHelper::dataToImage(data, width, height, channels, dataLength);
    if (image == NULL) {
        std::cerr << "Could not retrieve image from data" << std::endl;
        return;
    }

    delete image;
}

int main(int argc, char** argv)
{
    // Initialize helper object with running server informations
    RedisImageHelperAsync helper ("127.0.0.1", 6379);
    if (!helper.connect()) {
        std::cerr << "Could not connect to redis server."
        << "Please make sure a redis server is up and running." 
        << std::endl;
        return EXIT_FAILURE;
    }

    helper.subscribe("subkey", callback, (void*)NULL)
}
```
**Note:** instead of `(void*) NULL`, you can pass usefull data through the privdata parameters to the callback function.

You can also use simple method to `GET`, `SET` and `PUBLISH` ints and strings from/to a redis server:
```cpp
int RedisImageHelperSync::getInt(std::string key);
char* RedisImageHelperSync::getString(std::string key, size_t& length);
void RedisImageHelperSync::setInt(int value, std::string key);
void RedisImageHelperSync::setString(char* value, std:string key);
void RedisImageHelperSync::publishString(char* value, std::string key);
void RedisImageHelperSync::publishInt(int value, std::string key);
```

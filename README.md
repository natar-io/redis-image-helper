# RedisImageHelper

**C++** library that provides a simple way to handle image manipulation in redis.
RedisImageHelper is built upon redis C client **hiredis**.

## Prerequisites

- [CMake](https://cmake.org)
- [Hiredis](https://github.com/redis/hiredis)
- [OpenCV](https://opencv.com) **/!\ Optional** (Used to display images in sample program)

Dependencies on arch:

`# pacman -S hiredis`

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
In the project example folder run the following to build examples. \
**Warning**: OpenCV is required to build the client example however you can still build the server example

```
mkdir build
cd build
cmake ..
make -j8
```

Once this is done you have 2 executables:

- **rcamserv:** Gets camera video output frames and transfer them into redis using **redisimagehelper** library.
- **rcamcli:** Gets image data from redis using **redisimagehelper** library and display them using **OpenCV**.


## How to use

Initialize a RedisImageHelper object and connect it to a running redis server:

```cpp
#include <RedisImageHelper.hpp>
int main(int argc, char** argv)
{
    // Initialize helper object with running server informations
    RedisImageHelper helper ("127.0.0.1", 6379);
    if (!helper.connect()) {
        std::cerr << "Could not connect to redis server."
        << "Please make sure a redis server is up and running." 
        << std::endl;
        return EXIT_FAILURE;
    }

    /* CODE */
}
```

Get image data from a redis server:
```cpp
#include <RedisImageHelper.hpp>
int main(int argc, char** argv)
{
    // Initialize helper object with running server informations
    RedisImageHelper helper ("127.0.0.1", 6379);
    if (!helper.connect()) {
        std::cerr << "Could not connect to redis server."
        << "Please make sure a redis server is up and running." 
        << std::endl;
        return EXIT_FAILURE;
    }
    // This specify where we should find image data in redis
    helper.setMainKey("custom:image");

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
**Note:** If you do not want to specify a global key to get and set image to, you can pass the key as a parameter like so `RedisImageHelper.get***(type value, std::string key)`.


Set image data into a redis server:

```cpp
#include <RedisImageHelper.hpp>
int main(int argc, char** argv)
{
    // Initialize helper object with running server informations
    RedisImageHelper helper ("127.0.0.1", 6379);
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
**Note:** If you do not want to specify a global key to get and set image to, you can pass the key as a parameter like so `RedisImageHelper.set***(type value, std::string key)`.

You can also use simple method to get/set int and strings from/to a redis server:
```cpp
void RedisImageHelper::setInt(int value, std::string key);
void RedisImageHelper::setString(std::string value, std:string key);
int RedisImageHelper::getInt(std::string key);
char* RedisImageHelper::getString(std::string key, size_t& length);
```

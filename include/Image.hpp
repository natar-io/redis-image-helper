#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_
#include <iostream>

/**
 * @brief The Image class is a data structure that represents an image.
 * An image is represented by the following properties: width, height, channels, and data
 */
class Image
{
private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_channels;
    unsigned int m_size;
    unsigned char* m_data;

public:
    Image(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data) : m_width(width), m_height(height), m_channels(channels)
    {
        // Copy the data removed. 

        // std::cout << "Image constructor called" << std::endl; 
        // m_data = new unsigned char[width * height * channels];
        // for (int i = 0 ; i < width * height * channels; i++)
        // {
        //     m_data[i] = data[i];
        // }

         m_data = data;
         m_size = width * height * channels;
       // std::cout << "Image constructor ended" << std::endl; 

    }

        Image(unsigned int size, unsigned char* data) : m_size(size)
    {
        // Copy the data removed. 

        // std::cout << "Image constructor called" << std::endl; 
        // m_data = new unsigned char[size];
        // for (int i = 0 ; i < size; i++)
        // {
        //     m_data[i] = data[i];
        // }
        // m_size = size;
         m_data = data;
       // std::cout << "Image constructor ended" << std::endl; 

    }

    //~Image() { delete[] m_data; }
    ~Image() {}

    const unsigned int width() { return m_width; }
    const unsigned int height() { return m_height; }
    const unsigned int channels() { return m_channels; }
    const unsigned int dataSize() { return m_size; }
    unsigned char* data() { return m_data; }
};

#endif // IMAGE


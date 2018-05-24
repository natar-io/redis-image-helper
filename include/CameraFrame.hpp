#ifndef _CAMERAFRAME_HPP_
#define _CAMERAFRAME_HPP_

class CameraFrame
{
private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_channels;
    unsigned char* m_data;

public:
    CameraFrame(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data) : m_width(width), m_height(height), m_channels(channels)
    {
        m_data = data;
    };

    const unsigned int width() { return m_width; };
    const unsigned int height() { return m_height; };
    const unsigned int channels() { return m_channels; };
    unsigned char* data() { return m_data; };
};

#endif // CAMERAFRAME


#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

class Image
{
private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_channels;
    unsigned char* m_data;

public:
    Image(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data) : m_width(width), m_height(height), m_channels(channels)
    {
        m_data = new unsigned char[width * height * channels];
        for (int i = 0 ; i < width * height * channels; i++)
        {
            m_data[i] = data[i];
        }

    };

    ~Image() { delete m_data; }

    const unsigned int width() { return m_width; };
    const unsigned int height() { return m_height; };
    const unsigned int channels() { return m_channels; };
    unsigned char* data() { return m_data; };
};

#endif // IMAGE


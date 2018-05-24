#ifndef _IMAGE_UTILS_HPP_
#define _IMAGE_UTILS_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

/**
    Reads a PPM file into a buffer
    @param file_name the ppm file to read from. Extension must be .ppm
    @param width a reference to the width of the loaded image.
    @param height a reference to the height of the loaded image.
    @return the data as un unsigned char buffer (8 bits)
*/
inline unsigned char* load_ppm(char* file_name, uint &width, uint &height)
{
  std::ifstream file(file_name);
  std::string magic_number;
  uint max_value;
  file >> magic_number >> width >> height >> max_value;
  if (magic_number != "P6")
    throw std::invalid_argument("Current PPM Image format is " + magic_number  + " and must be P6");
  if (max_value > 255)
    throw std::invalid_argument("Max value is " + std::to_string(max_value) + "but it should not be > 255");

  size_t size = width * height * 3;
  unsigned char* data = new unsigned char[size];
  file.read((char*)(&data[0]), size);
  return data;
}

/**
    Writes a PPM file from a buffer
    @param file_name the file to write in. Extension must be .ppm
    @param data the unsigned char* buffer that contains image data to write in the file.
    @param width the width of the image.
    @param height the height of the image.
*/
inline void write_ppm(char* file_name, unsigned char* data, int width, int height)
{
  std::ofstream file(file_name);
  file << "P6" << "\n"
	   << width << "\n"
	   << height << "\n"
	   << 255 << "\n";
  size_t size = width * height * 3;
  file.write((char*)(&data[0]), size);
}

/**
    Utils function to convert a uchar buffer into an float buffer
    @param data the buffer that contains image data
    @param size the total size of the buffer
    @return the buffer that contains the data converted from unsigned char to float.
*/
inline float* uchar_to_float(unsigned char* data, uint size)
{
  float* float_data = new float[size];
  for (uint i = 0 ; i < size ; ++i)
  {
	float_data[i] = static_cast<float>(data[i]);
  }
  return float_data;
}

/**
    Utils function to convert a float buffer into an uchar buffer
    @param data the buffer that contains image data
    @param size the total size of the buffer
    @return the buffer that contains the data converted from float to unsigned char.
*/
inline unsigned char* float_to_uchar(float* data, uint size)
{
  unsigned char* uchar_data = new unsigned char[size];
  for (uint i = 0 ; i < size ; i++)
  {
	float value = data[i];
	if (value < 0) value = 0;
	if (value > 255) value = 255;
	uchar_data[i] = static_cast<float>(data[i]);
  }
  return uchar_data;
}

/**
    Utils function to convert a RGB image to GRAYSCALE image
    @param data the buffer that contains image data
    @param width the width of the image
    @param height the height of the image
    @return the converted data buffer
*/
inline unsigned char* rgb_to_gray(unsigned char* data, uint width, uint height)
{
  unsigned char* data_gray = new unsigned char[width * height];
  int cpt = 0;
  for (uint i = 0 ; i < width * height * 3; i+=3)
  {
	data_gray[cpt++] = (data[i] + data[i+1] + data[i+2]) / 3;
  }
  
  return data_gray;
}

/**
    Utils function to convert a GRAYSCALE image to 3CHAN (RGB) image
    @param data the buffer that contains image data
    @param width the width of the image
    @param height the height of the image
    @return the converted data buffer
*/
inline unsigned char* gray_to_rgb(unsigned char* data, uint width, uint height)
{
  unsigned char* data_rgb = new unsigned char[width * height * 3];
  int cpt = 0;
  for (uint i = 0 ; i < width * height; ++i)
  {
	data_rgb[cpt++] = data[i];
	data_rgb[cpt++] = data[i];
	data_rgb[cpt++] = data[i];
  }
  
  return data_rgb;
}

inline float* generate_random_image(uint width, uint height, uint channel)
{
  float* img = new float[width * height * channel];
  for (int i = 0 ; i < width * height * channel ; ++i)
  {
    img[i] = rand()%256;
  }
  return img;
}

#endif

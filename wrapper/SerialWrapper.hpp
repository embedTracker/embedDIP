// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#pragma once

extern "C" {
#include "device/serial/serial.h"

#include "embedDIP_configs.h"
}

#include <cstdint>

#include "ImageWrapper.hpp"
namespace embedDIP
{

// Define class name based on platform
#if defined(ARDUINO_ARCH_ESP32)
    #define SERIAL_CLASS_NAME SerialDev
#else
    #define SERIAL_CLASS_NAME Serial
#endif

/*TODO: Class name is platform-dependent*/
/**
 * @brief Wrapper for serial communication device driver.
 * @see serial_t For the underlying C driver structure
 */
class SERIAL_CLASS_NAME
{
  public:
    /**
     * @brief Creates a serial wrapper around a C driver instance.
     * @param[in] driver Pointer to serial driver backend
     */
    explicit SERIAL_CLASS_NAME(serial_t *driver);

    /**
     * @brief Initializes serial interface.
     * @see serial_t::init For underlying C driver function
     */
    void init();
    /**
     * @brief Flushes pending serial buffers.
     * @see serial_t::flush For underlying C driver function
     */
    void flush();
    /**
     * @brief Receives an image from serial stream.
     * @param[out] img Destination image to receive data
     * @see serial_t::captureImage For underlying C driver function
     */
    void capture(Image &img);
    /**
     * @brief Sends raw image payload over serial.
     * @param[in] img Source image to transmit
     * @see serial_t::sendImage For underlying C driver function
     */
    void send(const Image &img);
    /**
     * @brief Sends image encoded as JPEG over serial.
     * @param[in] img Source image to encode and transmit
     * @see serial_t::sendImageJPEG For underlying C driver function
     */
    void sendJPEG(const Image &img);
    /**
     * @brief Sends typed 1D data buffer over serial.
     * @param[in] data Pointer to source buffer
     * @param[in] elem_size Size of one element in bytes
     * @param[in] length Number of elements to transmit
     * @param[in] type Data-type descriptor for receiver decoding
     * @see serial_t::send1DData For underlying C driver function
     */
    void send1D(const void *data, uint8_t elem_size, uint32_t length, Serial1DDataType type);

  private:
    serial_t *driver_;
};

}  // namespace embedDIP

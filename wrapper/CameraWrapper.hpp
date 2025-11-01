// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#pragma once

#include "device/camera/camera.h"  // camera_t, captureMode, etc.

#include "ImageWrapper.hpp"

namespace embedDIP
{

/**
 * @brief C++ RAII wrapper for camera device driver.
 * @see camera_t For the underlying C driver structure
 */
class Camera
{
  public:
    /**
     * @brief Creates a camera wrapper around a C driver instance.
     * @param[in] driver Pointer to camera driver backend
     */
    explicit Camera(camera_t *driver);

    /**
     * @brief Initializes camera with resolution and pixel format.
     * @param[in] res Target capture resolution
     * @param[in] format Target capture format (default: RGB565)
     * @return true on success, false otherwise
     * @see camera_t::init For underlying C driver function
     */
    bool init(ImageResolution res, ImageFormat format = IMAGE_FORMAT_RGB565);
    /**
     * @brief Captures one frame into the provided image.
     * @param[in] mode Capture mode (continuous, snapshot, etc.)
     * @param[out] img Destination image to receive frame data
     * @return true on success, false otherwise
     * @see camera_t::capture For underlying C driver function
     */
    bool capture(captureMode mode, Image &img);
    /**
     * @brief Stops camera capture stream.
     * @return true on success, false otherwise
     * @see camera_t::stop For underlying C driver function
     */
    bool stop();
    /**
     * @brief Updates camera capture resolution.
     * @param[in] res New target resolution
     * @return true on success, false otherwise
     * @see camera_t::setResolution For underlying C driver function
     */
    bool setRes(ImageResolution res);

  private:
    camera_t *driver_;
};

}  // namespace embedDIP

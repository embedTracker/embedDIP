// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#include "CameraWrapper.hpp"

namespace embedDIP
{

/**
 * @brief Constructs camera wrapper with provided driver backend.
 */
Camera::Camera(camera_t *driver) : driver_(driver) {}

/**
 * @brief Initializes camera through driver callback.
 */
bool Camera::init(ImageResolution res, ImageFormat format)
{
    if (driver_ && driver_->init) {
        return driver_->init(res, format) == 0;
    }
    return false;
}

/**
 * @brief Captures one frame using configured driver.
 */
bool Camera::capture(captureMode mode, Image &img)
{
    return driver_ && driver_->capture ? driver_->capture(mode, img.raw()) == 0 : false;
}

/**
 * @brief Stops camera stream.
 */
bool Camera::stop()
{
    return driver_ && driver_->stop ? driver_->stop() == 0 : false;
}

/**
 * @brief Updates camera resolution.
 */
bool Camera::setRes(ImageResolution res)
{
    return driver_ && driver_->setRes ? driver_->setRes(res) == 0 : false;
}

}  // namespace embedDIP

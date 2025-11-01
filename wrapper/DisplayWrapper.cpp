// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#include "DisplayWrapper.hpp"

namespace embedDIP
{

/**
 * @brief Constructs display wrapper with provided driver backend.
 */
Display::Display(display_t *driver) : driver_(driver) {}

/**
 * @brief Initializes display through driver callback.
 */
void Display::init()
{
    if (driver_ && driver_->init)
        driver_->init();
}

/**
 * @brief Deinitializes display through driver callback.
 */
void Display::deinit()
{
    if (driver_ && driver_->deinit)
        driver_->deinit();
}

/**
 * @brief Resets display through driver callback.
 */
void Display::reset()
{
    if (driver_ && driver_->reset)
        driver_->reset();
}

/**
 * @brief Clears display to a solid color.
 */
void Display::clear(displayColor color)
{
    if (driver_ && driver_->clear)
        driver_->clear(color);
}

/**
 * @brief Displays an image through driver callback.
 */
void Display::show(const Image &image)
{
    if (driver_ && driver_->show) {
        driver_->show(image.raw());
    }
}

}  // namespace embedDIP

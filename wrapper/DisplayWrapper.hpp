// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#pragma once

extern "C" {
#include "device/display/display.h"
}

#include "ImageWrapper.hpp"  // embedDIP::Image

namespace embedDIP
{

/**
 * @brief Wrapper for display device driver.
 * @see display_t For the underlying C driver structure
 */
class Display
{
  public:
    /**
     * @brief Creates a display wrapper around a C driver instance.
     * @param[in] driver Pointer to display driver backend
     */
    explicit Display(display_t *driver);

    /**
     * @brief Initializes display hardware.
     * @see display_t::init For underlying C driver function
     */
    void init();
    /**
     * @brief Deinitializes display hardware.
     * @see display_t::deinit For underlying C driver function
     */
    void deinit();
    /**
     * @brief Resets display hardware state.
     * @see display_t::reset For underlying C driver function
     */
    void reset();
    /**
     * @brief Clears screen with a uniform color.
     * @param[in] color Fill color (typically predefined displayColor value)
     * @see display_t::clear For underlying C driver function
     */
    void clear(displayColor color);
    /**
     * @brief Renders an image to the display.
     * @param[in] image Source image to display
     * @see display_t::show For underlying C driver function
     */
    void show(const Image &image);

  private:
    display_t *driver_;
};

}  // namespace embedDIP

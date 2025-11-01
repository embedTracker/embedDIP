// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#ifndef EMBED_DIP_COLOR_H
#define EMBED_DIP_COLOR_H

#include "board/common.h"
#include "core/error.h"
#include "core/image.h"
#include "core/memory_manager.h"

#include <math.h>

typedef enum {
    // RGB888 to others
    CVT_RGB888_TO_GRAYSCALE,
    CVT_RGB888_TO_RGB565,
    CVT_RGB888_TO_YUV,
    CVT_RGB888_TO_HSI,

    // RGB565 to others
    CVT_RGB565_TO_RGB888,
    CVT_RGB565_TO_GRAYSCALE,
    CVT_RGB565_TO_YUV,
    CVT_RGB565_TO_HSI,

    // GRAYSCALE to others
    CVT_GRAYSCALE_TO_RGB888,
    CVT_GRAYSCALE_TO_RGB565,
    CVT_GRAYSCALE_TO_YUV,
    CVT_GRAYSCALE_TO_HSI,

    // YUV to others
    CVT_YUV_TO_RGB888,
    CVT_YUV_TO_RGB565,
    CVT_YUV_TO_GRAYSCALE,
    CVT_YUV_TO_HSI,

    // HSI to others
    CVT_HSI_TO_RGB888,
    CVT_HSI_TO_RGB565,
    CVT_HSI_TO_GRAYSCALE,
    CVT_HSI_TO_YUV,

    // Identity (copy)
    CVT_COPY
} ColorConversionCode;

/**
 * @brief Converts the color format of the input image to a different format.
 *
 * Supports conversions between RGB888, RGB565, Grayscale, YUV, and HSI color spaces.
 * The output image format is automatically set based on the conversion code.
 *
 * @param src Pointer to the input image structure
 * @param dst Pointer to the output image structure
 * @param code Conversion code defined in ColorConversionCode enum
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t cvtColor(const Image *src, Image *dst, ColorConversionCode code);

/**
 * @brief Thresholds an HSI image based on hue range.
 *
 * Creates a binary mask where pixels within the specified hue range are set to 255,
 * and pixels outside the range are set to 0.
 *
 * @param src Pointer to the input HSI image
 * @param dst Pointer to the output binary mask image
 * @param min_hue Minimum hue value (in degrees, 0-360)
 * @param max_hue Maximum hue value (in degrees, 0-360)
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t hueThreshold(const Image *src, Image *dst, float min_hue, float max_hue);

/**
 * @brief Creates a binary mask for pixels within a specified color range.
 *
 * Tests each pixel to check if it falls within the specified lower and upper bounds
 * for all three color channels. Used for color-based segmentation.
 *
 * @param src Pointer to the input RGB image
 * @param mask Pointer to the output binary mask image
 * @param lower Array of 3 lower bound values [R_min, G_min, B_min]
 * @param upper Array of 3 upper bound values [R_max, G_max, B_max]
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t
inRange(const Image *src, Image *mask, const uint8_t lower[3], const uint8_t upper[3]);

#endif  // EMBED_DIP_COLOR_H
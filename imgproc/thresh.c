// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#include "thresh.h"

#include "core/error.h"
#include "core/memory_manager.h"

#include <math.h>
#include <stdint.h>

#include <board/common.h>

/**
 * @brief Applies negative transformation to an image.
 *
 * @param[in]  inImg   Pointer to the input image structure (source image).
 * @param[out] outImg  Pointer to the output image structure (resulting negative image).
 */
embeddip_status_t negative(const Image *src, Image *dst)
{
    uint8_t *src_data = src->pixels;
    uint8_t *dst_data = dst->pixels;

    for (uint32_t i = 0; i < src->size; ++i) {
        dst_data[i] = UINT8_MAX - src_data[i];
    }

    dst->log = IMAGE_DATA_PIXELS;
    return EMBEDDIP_OK;
}

/**
 * @brief Applies binary thresholding to a grayscale image.
 *
 * @param[in]  inImg      Pointer to the input image structure (grayscale image).
 * @param[out] outImg     Pointer to the output image structure (thresholded image).
 * @param[in]  threshold  Threshold value (in range 0–255).
 */
embeddip_status_t grayscaleThreshold(const Image *src, Image *dst, uint8_t threshold)
{
    uint8_t *src_data = src->pixels;
    uint8_t *dst_data = dst->pixels;

    for (uint32_t i = 0; i < src->size; ++i) {
        dst_data[i] = (src_data[i] >= threshold) ? 255 : 0;
    }

    dst->log = IMAGE_DATA_PIXELS;
    return EMBEDDIP_OK;
}

/**
 * @brief Computes Otsu's threshold for a grayscale image.
 *
 * @param[in] imgData  Pointer to grayscale pixel data.
 * @param[in] size     Total number of pixels in the image.
 * @return uint8_t     Computed Otsu threshold value.
 */
uint8_t OtsuThreshold(const uint8_t *imgData, int size)
{
    int hist[256] = {0};
    for (int i = 0; i < size; ++i)
        hist[imgData[i]]++;

    float sum = 0.0f;
    for (int t = 0; t < 256; ++t)
        sum += t * hist[t];

    float sumB = 0.0f;
    int wB = 0;
    int wF = 0;
    float maxVar = 0.0f;
    uint8_t threshold = 0;

    for (int t = 0; t < 256; ++t) {
        wB += hist[t];  // Weight Background
        if (wB == 0)
            continue;

        wF = size - wB;  // Weight Foreground
        if (wF == 0)
            break;

        sumB += (float)(t * hist[t]);

        float mB = sumB / wB;          // Mean Background
        float mF = (sum - sumB) / wF;  // Mean Foreground

        // Between Class Variance
        float varBetween = (float)wB * wF * (mB - mF) * (mB - mF);

        if (varBetween > maxVar) {
            maxVar = varBetween;
            threshold = (uint8_t)t;
        }
    }

    return threshold;
}

/**
 * @brief Applies Otsu's binary thresholding to a grayscale image.
 *
 * @param[in]  inImg   Pointer to input image (grayscale, 8-bit pixels).
 * @param[out] outImg  Pointer to output binary image.
 * @return EMBEDDIP_OK on success, error code otherwise.
 */
embeddip_status_t grayscaleOtsu(const Image *src, Image *dst)
{
    // Input validation
    if (!src || !dst)
        return EMBEDDIP_ERROR_NULL_PTR;

    if (!src->pixels || !dst->pixels)
        return EMBEDDIP_ERROR_NULL_PTR;

    if (src->format != IMAGE_FORMAT_GRAYSCALE || dst->format != IMAGE_FORMAT_GRAYSCALE)
        return EMBEDDIP_ERROR_INVALID_FORMAT;

    if (src->width != dst->width || src->height != dst->height)
        return EMBEDDIP_ERROR_INVALID_SIZE;

    const uint8_t *src_data = src->pixels;
    uint8_t *dst_data = dst->pixels;

    uint8_t threshold = OtsuThreshold(src_data, src->size);

    for (uint32_t i = 0; i < src->size; ++i) {
        dst_data[i] = (src_data[i] >= threshold) ? 255 : 0;
    }
    return EMBEDDIP_OK;
}

embeddip_status_t grayscaleThresholdLocalOtsu(const Image *src, Image *dst, int block_size)
{
    const int width = src->width;
    const int height = src->height;
    const uint8_t *src_data = src->pixels;
    uint8_t *dst_data = dst->pixels;

    for (int y = 0; y < height; y += block_size) {
        for (int x = 0; x < width; x += block_size) {
            int block_w = (x + block_size > width) ? (width - x) : block_size;
            int block_h = (y + block_size > height) ? (height - y) : block_size;

            // 1. Extract block into a temporary buffer
            uint8_t block_pixels[block_size * block_size];
            int count = 0;

            for (int j = 0; j < block_h; ++j) {
                for (int i = 0; i < block_w; ++i) {
                    block_pixels[count++] = src_data[(y + j) * width + (x + i)];
                }
            }

            // 2. Compute Otsu threshold for this block
            uint8_t threshold = OtsuThreshold(block_pixels, count);

            // 3. Apply threshold to each pixel in the block
            for (int j = 0; j < block_h; ++j) {
                for (int i = 0; i < block_w; ++i) {
                    int idx = (y + j) * width + (x + i);
                    dst_data[idx] = (src_data[idx] >= threshold) ? 255 : 0;
                }
            }
        }
    }
    return EMBEDDIP_OK;
}

/**
 * @brief Applies power-law (gamma) transformation.
 *
 * @param[in]  inImg   Pointer to the input image (supports GRAYSCALE or RGB).
 * @param[out] outImg  Pointer to the output image. Allocated `chals` field will be filled.
 * @param[in]  gamma   Gamma value (e.g., <1 brightens, >1 darkens).
 */
embeddip_status_t powerTransform(const Image *src, Image *dst, float gamma)
{
    // Validate input parameters
    if (src == NULL || dst == NULL) {
        return EMBEDDIP_ERROR_NULL_PTR;
    }

    if (src->pixels == NULL) {
        return EMBEDDIP_ERROR_NULL_PTR;
    }

    if (src->width == 0 || src->height == 0) {
        return EMBEDDIP_ERROR_INVALID_SIZE;
    }

    if (src->width != dst->width || src->height != dst->height) {
        return EMBEDDIP_ERROR_INVALID_SIZE;
    }

    // Ensure output has channels allocated
    if (isChalsEmpty(dst)) {
        embeddip_status_t status = createChals(dst, dst->depth);
        if (status != EMBEDDIP_OK) {
            return status;  // Memory allocation failed
        }
        dst->is_chals = 1;
    }

    // Safety check: ensure channel was actually allocated
    if (dst->chals == NULL || dst->chals->ch[0] == NULL) {
        return EMBEDDIP_ERROR_OUT_OF_MEMORY;
    }

    // NOTE: Channel operations work on width*height pixels, NOT width*height*depth
    // Each channel stores one value per pixel location
    // depth is only relevant for pixel buffer layout, not channel processing
    uint32_t num_pixels = src->width * src->height;

    // Check where the valid input data is located based on log state
    if (src->log == IMAGE_DATA_PIXELS) {
        // Valid data is in pixels buffer - read as uint8_t
        uint8_t *src_data = (uint8_t *)src->pixels;

        // Safety: verify we don't exceed allocated buffer
        size_t allocated_size = (size_t)dst->width * (size_t)dst->height;
        if (num_pixels > allocated_size) {
            return EMBEDDIP_ERROR_OUT_OF_RANGE;
        }

        for (uint32_t i = 0; i < num_pixels; ++i) {
            // Bounds check before read
            if (i >= src->size) {
                return EMBEDDIP_ERROR_OUT_OF_RANGE;
            }

            float norm = src_data[i] / 255.0f;

            // Check for valid gamma operation
            if (gamma < 0.0f || norm < 0.0f || norm > 1.0f) {
                dst->chals->ch[0][i] = 0.0f;
                continue;
            }

            float result = powf(norm, gamma);

            // Check for NaN or infinity
            if (!isfinite(result)) {
                result = 0.0f;
            }

            dst->chals->ch[0][i] = result;
        }
    } else {
        // Valid data is in chals - determine which channel based on log
        if (isChalsEmpty(src)) {
            embeddip_status_t status = createChals((Image *)src, src->depth);
            if (status != EMBEDDIP_OK) {
                return status;  // Memory allocation failed
            }
        }

        // Map log state to channel index
        int ch_idx = 0;
        if (src->log >= IMAGE_DATA_CH0 && src->log <= IMAGE_DATA_CH5) {
            ch_idx = src->log - IMAGE_DATA_CH0;
        } else if (src->log == IMAGE_DATA_MAGNITUDE || src->log == IMAGE_DATA_PHASE) {
            ch_idx = 0;  // MAGNITUDE and PHASE stored in ch[0]
        }

        // Safety check: ensure channel exists
        if (src->chals == NULL || src->chals->ch[ch_idx] == NULL) {
            return EMBEDDIP_ERROR_OUT_OF_MEMORY;
        }

        float *src_data = src->chals->ch[ch_idx];

        for (uint32_t i = 0; i < num_pixels; ++i) {
            dst->chals->ch[0][i] = powf(src_data[i], gamma);
        }
    }

    dst->log = IMAGE_DATA_CH0;
    return EMBEDDIP_OK;
}

/**
 * @brief Applies linear scaling and bias followed by absolute value and clamps to 8-bit.
 *
 * @param[in]  inImg   Pointer to the input image (supports GRAYSCALE or RGB, 8-bit).
 * @param[out] outImg  Pointer to the output image. Output must have pixels allocated.
 * @param[in]  alpha   Gain factor.
 * @param[in]  beta    Bias added after scaling.
 */
embeddip_status_t convertScaleAbs(const Image *src, Image *dst, float alpha, float beta)
{
    if (isChalsEmpty(dst)) {
        createChals(dst, dst->depth);
        dst->is_chals = 1;
    }

    if (isChalsEmpty(src)) {
        createChals((Image *)src, src->depth);

        uint8_t *src_data = (uint8_t *)src->pixels;

        for (uint32_t i = 0; i < src->width * src->height; ++i) {
            dst->chals->ch[0][i] = fabsf(alpha * (float)src_data[i] + beta);
        }
    } else {
        float *src_data = (float *)src->chals->ch[0];

        for (uint32_t i = 0; i < src->size; ++i) {
            dst->chals->ch[0][i] = fabsf(alpha * src_data[i] + beta);
        }
    }
    return EMBEDDIP_OK;
}

/**
 * @brief Applies piecewise linear transformation.
 *
 * @param[in]  src           Pointer to input image.
 * @param[out] dst           Pointer to output image.
 * @param[in]  breakpoints   Array of input breakpoint values (must be sorted).
 * @param[in]  values        Array of output values corresponding to breakpoints.
 * @param[in]  num_points    Number of points in breakpoints/values arrays.
 * @return EMBEDDIP_OK on success, error code otherwise.
 */
embeddip_status_t piecewiseTransform(const Image *src,
                                     Image *dst,
                                     const uint8_t *breakpoints,
                                     const uint8_t *values,
                                     int num_points)
{
    if (src->depth != 1) {
        // Only GRAYSCALE is supported
        return EMBEDDIP_ERROR_INVALID_FORMAT;
    }

    if (num_points < 2) {
        // Need at least 2 points for a valid transformation
        return EMBEDDIP_ERROR_INVALID_ARG;
    }

    if (isChalsEmpty(src)) {
        createChals((Image *)src, src->depth);
    }

    if (isChalsEmpty(dst)) {
        createChals(dst, dst->depth);
        dst->is_chals = 1;
    }

    uint8_t *src_data = (uint8_t *)src->pixels;
    float *dst_data = dst->chals->ch[0];
    int total_pixels = src->width * src->height;

    for (int i = 0; i < total_pixels; ++i) {
        uint8_t pixel = src_data[i];
        float output_value;

        // Handle pixels below the first breakpoint
        if (pixel <= breakpoints[0]) {
            output_value = values[0] / 255.0f;
        }
        // Handle pixels above the last breakpoint
        else if (pixel >= breakpoints[num_points - 1]) {
            output_value = values[num_points - 1] / 255.0f;
        }
        // Find the segment and interpolate
        else {
            int segment = 0;
            for (int j = 0; j < num_points - 1; ++j) {
                if (pixel >= breakpoints[j] && pixel <= breakpoints[j + 1]) {
                    segment = j;
                    break;
                }
            }

            // Linear interpolation within the segment
            float x0 = breakpoints[segment];
            float x1 = breakpoints[segment + 1];
            float y0 = values[segment] / 255.0f;
            float y1 = values[segment + 1] / 255.0f;

            output_value = y0 + ((pixel - x0) * (y1 - y0)) / (x1 - x0);
        }

        dst_data[i] = output_value;
    }

    dst->log = IMAGE_DATA_CH0;
    return EMBEDDIP_OK;
}

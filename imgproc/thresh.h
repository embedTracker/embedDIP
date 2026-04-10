// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#ifndef EMBEDDIP_THRESH_H
#define EMBEDDIP_THRESH_H

#include "core/error.h"
#include "core/image.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Applies negative transformation to an image.
 *
 * @param[in]  inImg   Pointer to the input image structure (source image).
 * @param[out] outImg  Pointer to the output image structure (resulting negative image).
 */
embeddip_status_t negative(const Image *src, Image *dst);

/**
 * @brief Applies binary thresholding to a grayscale image.
 *
 * @param[in]  inImg      Pointer to the input image structure (grayscale image).
 * @param[out] outImg     Pointer to the output image structure (thresholded image).
 * @param[in]  threshold  Threshold value (in range 0–255).
 */
embeddip_status_t grayscaleThreshold(const Image *src, Image *dst, uint8_t threshold);

/**
 * @brief Computes Otsu's threshold for a grayscale image.
 *
 * @param[in] imgData  Pointer to grayscale pixel data.
 * @param[in] size     Total number of pixels in the image.
 * @return uint8_t     Computed Otsu threshold value.
 */
uint8_t OtsuThreshold(const uint8_t *imgData, int size);

/**
 * @brief Applies Otsu's binary thresholding to a grayscale image.
 *
 * @param[in]  inImg   Pointer to input image (grayscale, 8-bit pixels).
 * @param[out] outImg  Pointer to output binary image.
 * @return EMBEDDIP_OK on success, error code otherwise.
 */
embeddip_status_t grayscaleOtsu(const Image *src, Image *dst);

/**
 * @brief Applies local Otsu thresholding with block-based approach.
 *
 * @param[in]  src         Pointer to input grayscale image.
 * @param[out] dst         Pointer to output binary image.
 * @param[in]  block_size  Size of local block for threshold computation.
 * @return EMBEDDIP_OK on success, error code otherwise.
 */
embeddip_status_t grayscaleThresholdLocalOtsu(const Image *src, Image *dst, int block_size);

/**
 * @brief Applies power-law (gamma) transformation.
 *
 * @param[in]  src    Pointer to input image.
 * @param[out] dst    Pointer to output image.
 * @param[in]  gamma  Gamma value for power transformation.
 * @return EMBEDDIP_OK on success, error code otherwise.
 */
embeddip_status_t powerTransform(const Image *src, Image *dst, float gamma);

/**
 * @brief Scales input and converts to absolute value.
 *
 * @param[in]  src    Pointer to input image.
 * @param[out] dst    Pointer to output image.
 * @param[in]  alpha  Scale factor.
 * @param[in]  beta   Offset value.
 * @return EMBEDDIP_OK on success, error code otherwise.
 */
embeddip_status_t convertScaleAbs(const Image *src, Image *dst, float alpha, float beta);

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
                                     int num_points);

#ifdef __cplusplus
}
#endif

#endif  // EMBEDDIP_THRESH_H

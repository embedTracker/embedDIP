// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "board/common.h"
#include "core/image.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Computes the histogram of a grayscale image.
 *
 * Calculates the frequency distribution of pixel intensities (0-255).
 * The histogram array must be pre-allocated with 256 elements.
 *
 * @param src Pointer to the input grayscale image
 * @param histogram Pointer to output histogram array (256 integers)
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t histForm(const Image *src, int *histogram);

/**
 * @brief Applies histogram equalization to enhance image contrast.
 *
 * Redistributes pixel intensities to utilize the full dynamic range (0-255),
 * improving contrast and making features more visible.
 *
 * @param src Pointer to the input grayscale image
 * @param dst Pointer to the output equalized image
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t histEq(const Image *src, Image *dst);

/**
 * @brief Applies histogram specification to match a target histogram.
 *
 * Transforms the input image histogram to match a specified target histogram
 * distribution, useful for matching lighting conditions between images.
 *
 * @param src Pointer to the input grayscale image
 * @param dst Pointer to the output transformed image
 * @param target_histogram Pointer to target histogram array (256 integers)
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t histSpec(const Image *src, Image *dst, const int *target_histogram);
#ifdef __cplusplus
}
#endif

#endif
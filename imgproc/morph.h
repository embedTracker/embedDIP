// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#ifndef EMBEDDIP_MORPH_H
#define EMBEDDIP_MORPH_H

#include "core/error.h"
#include "core/image.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create structuring element
 *
 * @param[out] kernel  Pointer to kernel structure
 * @param[in]  shape   Shape of structuring element
 * @param[in]  size    Size of structuring element
 */
embeddip_status_t getStructuringElement(Kernel *kernel, MorphShape shape, uint8_t size);

/**
 * @brief Performs morphological erosion on a binary image.
 *
 * @param[in]  src         Pointer to the source binary image.
 * @param[out] dst         Pointer to the destination image to store the eroded result.
 * @param[in]  kernel      Pointer to the structuring element (Kernel) used for erosion.
 * @param[in]  iterations  Number of times erosion is applied.
 */
embeddip_status_t erode(const Image *src, Image *dst, const Kernel *kernel, uint8_t iterations);

/**
 * @brief Performs morphological dilation on a binary image.
 *
 * @param[in]  src         Pointer to the source binary image.
 * @param[out] dst         Pointer to the destination image to store the dilated result.
 * @param[in]  kernel      Pointer to the structuring element (Kernel) used for dilation.
 * @param[in]  iterations  Number of times dilation is applied.
 */
embeddip_status_t dilate(const Image *src, Image *dst, const Kernel *kernel, uint8_t iterations);

/**
 * @brief Performs morphological opening on a binary image.
 *
 * @param[in]  inImg       Pointer to the input binary image.
 * @param[out] outImg      Pointer to the output image to store the result of opening.
 * @param[in]  kernel      Pointer to the structuring element (Kernel) used for the operation.
 * @param[in]  iterations  Number of erosion/dilation iterations.
 */
embeddip_status_t
opening(const Image *inImg, Image *outImg, const Kernel *kernel, uint8_t iterations);

/**
 * @brief Performs morphological closing on a binary image.
 *
 * @param[in]  inImg       Pointer to the input binary image.
 * @param[out] outImg      Pointer to the output image to store the result of closing.
 * @param[in]  kernel      Pointer to the structuring element (Kernel) used for the operation.
 * @param[in]  iterations  Number of dilation/erosion iterations.
 */
embeddip_status_t
closing(const Image *inImg, Image *outImg, const Kernel *kernel, uint8_t iterations);

#ifdef __cplusplus
}
#endif

#endif  // EMBEDDIP_MORPH_H

// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#ifndef EMBEDDIP_DRAWING_H
#define EMBEDDIP_DRAWING_H

#include "core/error.h"
#include "core/image.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Draw a line on an image
 *
 * @param[out] dst    Pointer to the destination image
 * @param[in]  x0     Starting X coordinate
 * @param[in]  y0     Starting Y coordinate
 * @param[in]  x1     Ending X coordinate
 * @param[in]  y1     Ending Y coordinate
 * @param[in]  color  Line color value
 */
embeddip_status_t
drawLine(Image *dst, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t color);

/**
 * @brief Draw a line on an image using polar coordinates
 *
 * @param[out] dst    Pointer to the destination image
 * @param[in]  rho    Distance from origin
 * @param[in]  theta  Angle in radians
 * @param[in]  color  Line color value
 */
embeddip_status_t drawLineOnImage(Image *dst, float rho, float theta, uint8_t color);

#ifdef __cplusplus
}
#endif

#endif  // EMBEDDIP_DRAWING_H

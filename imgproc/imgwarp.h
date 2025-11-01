// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#ifndef EMBEDDIP_IMGWARP_H
#define EMBEDDIP_IMGWARP_H

#include "core/error.h"
#include "core/image.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Resizes a single-channel image using nearest-neighbor interpolation.
 *
 * @param[in]  inImg      Pointer to the input image.
 * @param[out] outImg     Pointer to the output image. Must already be allocated
 *                        with dimensions (outWidth × outHeight).
 * @param[in]  outWidth   Desired width of the output image.
 * @param[in]  outHeight  Desired height of the output image.
 */
embeddip_status_t resize(Image *src, Image *dst, int width, int height);

#ifdef __cplusplus
}
#endif

#endif  // EMBEDDIP_IMGWARP_H

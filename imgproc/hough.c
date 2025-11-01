// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#include "hough.h"

#include <math.h>
#include <stdint.h>

embeddip_status_t houghTransform(const Image *src,
                                 int **accumulator,
                                 int num_rho,
                                 int num_theta,
                                 float rho_res,
                                 float theta_res)
{
    int width = src->width;
    int height = src->height;
    const uint8_t *pixels = src->pixels;

    float diag_len = sqrtf(width * width + height * height);
    int rho_max = (int)(diag_len / rho_res);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (pixels[y * width + x] == 255)  // edge pixel
            {
                for (int t = 0; t < num_theta; ++t) {
                    float theta = t * theta_res;
                    float rho = x * cosf(theta) + y * sinf(theta);
                    int r = (int)((rho + rho_max) / rho_res);

                    if (r >= 0 && r < num_rho) {
                        accumulator[r][t]++;
                    }
                }
            }
        }
    }
    return EMBEDDIP_OK;
}

embeddip_status_t extractLines(int **accumulator,
                               int num_rho,
                               int num_theta,
                               float rho_res,
                               float theta_res,
                               int threshold,
                               float rho_max,
                               Line *lines,
                               int max_lines)
{
    int count = 0;
    for (int r = 0; r < num_rho; ++r) {
        for (int t = 0; t < num_theta; ++t) {
            if (accumulator[r][t] >= threshold && count < max_lines) {
                lines[count].rho = r * rho_res - rho_max;
                lines[count].theta = t * theta_res;
                lines[count].votes = accumulator[r][t];
                count++;
            }
        }
    }
    return EMBEDDIP_OK;
}

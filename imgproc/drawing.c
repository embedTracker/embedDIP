// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#include "drawing.h"

#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

embeddip_status_t
drawLine(Image *dst, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t color)
{
    int dx = llabs((int64_t)x1 - (int64_t)x0);
    int dy = llabs((int64_t)y1 - (int64_t)y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (x0 < dst->width && y0 < dst->height) {
            ((uint8_t *)dst->pixels)[y0 * dst->width + x0] = color;
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
    return EMBEDDIP_OK;
}

embeddip_status_t drawLineOnImage(Image *dst, float rho, float theta, uint8_t color)
{
    float cosT = cosf(theta);
    float sinT = sinf(theta);

    float x0 = cosT * rho;
    float y0 = sinT * rho;

    int x1 = (int)(x0 + 1000 * (-sinT));
    int y1 = (int)(y0 + 1000 * (cosT));
    int x2 = (int)(x0 - 1000 * (-sinT));
    int y2 = (int)(y0 - 1000 * (cosT));

    drawLine(dst, x1, y1, x2, y2, color);
    return EMBEDDIP_OK;
}

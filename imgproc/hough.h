// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#ifndef EMBEDDIP_HOUGH_H
#define EMBEDDIP_HOUGH_H

#include "core/error.h"
#include "core/image.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Line candidate in Hough space.
 */
typedef struct {
    float rho;    ///< Distance from origin
    float theta;  ///< Angle in radians
    int votes;    ///< Accumulator vote count
} Line;

/**
 * @brief Detects lines in an edge image using Hough transform.
 *
 * Converts edge pixels to parameter space (rho, theta) and builds accumulator array.
 *
 * @param src Pointer to the input edge-detected image
 * @param accumulator Pointer to 2D accumulator array
 * @param num_rho Number of rho quantization levels
 * @param num_theta Number of theta quantization levels
 * @param rho_res Resolution of rho parameter
 * @param theta_res Resolution of theta parameter in radians
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t houghTransform(const Image *src,
                                 int **accumulator,
                                 int num_rho,
                                 int num_theta,
                                 float rho_res,
                                 float theta_res);

/**
 * @brief Extracts line parameters from Hough accumulator array.
 *
 * Identifies peaks in accumulator that correspond to detected lines.
 *
 * @param accumulator Pointer to 2D Hough accumulator array
 * @param num_rho Number of rho quantization levels
 * @param num_theta Number of theta quantization levels
 * @param rho_res Resolution of rho parameter
 * @param theta_res Resolution of theta parameter in radians
 * @param threshold Minimum vote threshold for line detection
 * @param rho_max Maximum rho value
 * @param lines Output array of detected lines
 * @param max_lines Maximum number of lines to extract
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t extractLines(int **accumulator,
                               int num_rho,
                               int num_theta,
                               float rho_res,
                               float theta_res,
                               int threshold,
                               float rho_max,
                               Line *lines,
                               int max_lines);

#ifdef __cplusplus
}
#endif

#endif  // EMBEDDIP_HOUGH_H

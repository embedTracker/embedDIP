// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#ifndef FFT_H
#define FFT_H

#ifndef __FPU_PRESENT
    #define __FPU_PRESENT 1
#endif

#include "core/error.h"
#include "core/image.h"

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FREQ_FILTER_IDEAL_LOWPASS,
    FREQ_FILTER_GAUSSIAN_LOWPASS,
    FREQ_FILTER_IDEAL_HIGHPASS,
    FREQ_FILTER_GAUSSIAN_HIGHPASS,
    FREQ_FILTER_IDEAL_BANDPASS,
    FREQ_FILTER_GAUSSIAN_BANDPASS
} FrequencyFilterType;

/**
 * @brief Computes the 2D Fast Fourier Transform of an image.
 * @param src Pointer to the input spatial-domain image
 * @param dst Pointer to the output frequency-domain image
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t fft(const Image *src, Image *dst);

/**
 * @brief Computes the inverse 2D FFT to convert frequency-domain back to spatial-domain.
 * @param src Pointer to the input frequency-domain image
 * @param dst Pointer to the output spatial-domain image
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t ifft(const Image *src, Image *dst);

/**
 * @brief Computes the magnitude (absolute value) of complex FFT data.
 * @param src Pointer to the input FFT image (complex)
 * @param dst Pointer to the output magnitude image
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t _abs_(const Image *src, Image *dst);

/**
 * @brief Computes the phase (angle) of complex FFT data.
 * @param src Pointer to the input FFT image (complex)
 * @param dst Pointer to the output phase image
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t _phase_(const Image *src, Image *dst);

/**
 * @brief Applies natural logarithm to image pixel values (for visualization).
 * @param img Pointer to the image (modified in-place)
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t _log_(Image *img);

/**
 * @brief Adds a scalar value to all pixels in the image.
 * @param img Pointer to the image (modified in-place)
 * @param value Scalar value to add to each pixel
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t _add_(Image *img, float value);

/**
 * @brief Shifts FFT quadrants to center the zero-frequency component.
 * @param img Pointer to the FFT image (modified in-place)
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t fftshift(Image *img);

/**
 * @brief Converts magnitude and phase to Cartesian complex representation.
 * @param mag_img Pointer to the magnitude image
 * @param phase_img Pointer to the phase image
 * @param dst Pointer to the output complex image
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t polarToCart(const Image *mag_img, const Image *phase_img, Image *dst);

/**
 * @brief Creates a frequency-domain filter mask.
 * @param filter_img Image to fill with filter values
 * @param filter_type Type of filter (lowpass, highpass, bandpass)
 * @param cutoff1 Primary cutoff radius in PIXELS from center
 * @param cutoff2 Secondary cutoff radius in PIXELS (bandpass only)
 * @return EMBEDDIP_OK on success, error code on failure
 * @note Cutoff is in PIXELS: distance from image center.
 *       For 256×256 image: center at (128,128), max corner distance ≈181 pixels.
 *       Example: cutoff1=30 passes frequencies within 30-pixel radius (~16.6%).
 */
embeddip_status_t
getFilter(Image *filter_img, FrequencyFilterType filter_type, float cutoff1, float cutoff2);

/**
 * @brief Applies a frequency-domain filter to an FFT image.
 * @param src_fft Pointer to the input FFT image
 * @param filter Pointer to the filter mask image
 * @param dst Pointer to the output filtered FFT image
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t ffilter2D(const Image *src_fft, const Image *filter, Image *dst);

/**
 * @brief Computes the element-wise difference between two images.
 * @param src1 Pointer to the first input image
 * @param src2 Pointer to the second input image
 * @param dst Pointer to the output difference image
 * @return EMBEDDIP_OK on success, error code on failure
 */
embeddip_status_t difference(const Image *src1, const Image *src2, Image *dst);

#ifdef __cplusplus
}
#endif

#endif  // FFT_H

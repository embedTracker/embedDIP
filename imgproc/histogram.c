// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#include <imgproc/histogram.h>

/**
 * @brief Forms a histogram from the grayscale values of the input image.
 *
 * @param[in]  inImg     Pointer to the input image.
 * @param[out] histogram Integer array of 256 elements to store the histogram.
 * @return Total number of pixels processed.
 */
embeddip_status_t histForm(const Image *src, int *histogram)
{
    if (!src || !src->pixels || src->depth != 1)
        return 0;  // Only supports 8-bit grayscale

    // Clear histogram
    for (int i = 0; i < 256; ++i)
        histogram[i] = 0;

    int totalPixels = src->width * src->height;
    uint8_t *data = (uint8_t *)src->pixels;

    for (int i = 0; i < totalPixels; ++i) {
        uint8_t pixel = data[i];
        histogram[pixel]++;
    }

    return totalPixels;
}

/**
 * @brief Performs histogram equalization and outputs normalized float values.
 *
 * @param[in]  src   Pointer to the input 8-bit grayscale image.
 * @param[out] outImg  Pointer to the output image with float channel data.
 * @return Total number of pixels processed, or 0 on failure.
 */
embeddip_status_t histEq(const Image *src, Image *dst)
{
    int totalPixels = src->width * src->height;

    // Ensure output has channels allocated
    if (isChalsEmpty(dst)) {
        createChals(dst, dst->depth);
        dst->is_chals = 1;
    }

    int histogram[256] = {0};
    int cdf[256];
    float cdfNormalized[256];

    // Check where the valid input data is located based on log state
    if (src->log == IMAGE_DATA_PIXELS) {
        // Valid data is in pixels buffer - read as uint8_t
        uint8_t *inputData = (uint8_t *)src->pixels;

        // Compute histogram
        for (int i = 0; i < totalPixels; ++i)
            histogram[inputData[i]]++;

        // Compute CDF
        cdf[0] = histogram[0];
        for (int i = 1; i < 256; ++i)
            cdf[i] = cdf[i - 1] + histogram[i];

        // Normalize CDF to [0.0, 1.0]
        float cdfMin = (float)cdf[0];
        float scale = 1.0f / (totalPixels - cdf[0]);  // avoid zero division
        for (int i = 0; i < 256; ++i)
            cdfNormalized[i] = (cdf[i] - cdfMin) * scale;

        // Apply equalization and store float output
        for (int i = 0; i < totalPixels; ++i) {
            uint8_t pixel = inputData[i];
            dst->chals->ch[0][i] = cdfNormalized[pixel];  // float value in [0.0, 1.0]
        }
    } else {
        // Valid data is in chals - determine which channel based on log
        if (isChalsEmpty(src)) {
            createChals((Image *)src, src->depth);
        }

        // Map log state to channel index
        int ch_idx = 0;
        if (src->log >= IMAGE_DATA_CH0 && src->log <= IMAGE_DATA_CH5) {
            ch_idx = src->log - IMAGE_DATA_CH0;
        } else if (src->log == IMAGE_DATA_MAGNITUDE || src->log == IMAGE_DATA_PHASE) {
            ch_idx = 0;  // MAGNITUDE and PHASE stored in ch[0]
        }

        float *inputData = (float *)src->chals->ch[ch_idx];

        // Compute histogram
        for (int i = 0; i < totalPixels; ++i)
            histogram[(uint8_t)inputData[i]]++;

        // Compute CDF
        cdf[0] = histogram[0];
        for (int i = 1; i < 256; ++i)
            cdf[i] = cdf[i - 1] + histogram[i];

        // Normalize CDF to [0.0, 1.0]
        float cdfMin = (float)cdf[0];
        float scale = 1.0f / (totalPixels - cdf[0]);  // avoid zero division
        for (int i = 0; i < 256; ++i)
            cdfNormalized[i] = (cdf[i] - cdfMin) * scale;

        // Apply equalization and store float output
        for (int i = 0; i < totalPixels; ++i) {
            uint8_t pixel = inputData[i];
            dst->chals->ch[0][i] = cdfNormalized[pixel];  // float value in [0.0, 1.0]
        }
    }

    dst->log = IMAGE_DATA_CH0;
    return totalPixels;
}

/**
 * @brief Perform histogram specification (matching) on a grayscale image.
 *
 * @param[in]  src            Pointer to the input grayscale image.
 * @param[out] dst           Pointer to the output image (float channel, same size).
 * @param[in]  targetHistogram  Pointer to the 256-bin target histogram array.
 * @return Total number of pixels processed, or 0 on failure.
 */
embeddip_status_t histSpec(const Image *src, Image *dst, const int *target_histogram)
{
    const int totalPixels = src->width * src->height;

    // Ensure output has channels allocated
    if (isChalsEmpty(dst)) {
        createChals(dst, dst->depth);
        dst->is_chals = 1;
    }

    float *outputData = dst->chals->ch[0];
    int inputHistogram[256] = {0};
    float inputCDF[256] = {0}, targetCDF[256] = {0};
    uint8_t mapping[256];

    // Check where the valid input data is located based on log state
    if (src->log == IMAGE_DATA_PIXELS) {
        // Valid data is in pixels buffer - read as uint8_t
        const uint8_t *inputData = (uint8_t *)src->pixels;

        // Step 1: Compute input histogram
        for (int i = 0; i < totalPixels; ++i)
            inputHistogram[inputData[i]]++;

        // Step 2: Compute input and target CDFs
        inputCDF[0] = (float)inputHistogram[0];
        targetCDF[0] = (float)target_histogram[0];
        for (int i = 1; i < 256; ++i) {
            inputCDF[i] = inputCDF[i - 1] + inputHistogram[i];
            targetCDF[i] = targetCDF[i - 1] + target_histogram[i];
        }

        for (int i = 0; i < 256; ++i) {
            inputCDF[i] /= totalPixels;
            targetCDF[i] /= targetCDF[255];  // normalize to 1.0
        }

        // Step 3: Build mapping from input levels to output levels
        for (int i = 0; i < 256; ++i) {
            float diffMin = 1.0f;
            int bestMatch = 0;
            for (int j = 0; j < 256; ++j) {
                float diff = fabsf(inputCDF[i] - targetCDF[j]);
                if (diff < diffMin) {
                    diffMin = diff;
                    bestMatch = j;
                }
            }
            mapping[i] = (uint8_t)bestMatch;
        }

        // Step 4: Apply mapping and normalize output to [0.0, 1.0]
        for (int i = 0; i < totalPixels; ++i) {
            uint8_t original = inputData[i];
            outputData[i] = mapping[original] / 255.0f;
        }
    } else {
        // Valid data is in chals - determine which channel based on log
        if (isChalsEmpty(src)) {
            createChals((Image *)src, src->depth);
        }

        // Map log state to channel index
        int ch_idx = 0;
        if (src->log >= IMAGE_DATA_CH0 && src->log <= IMAGE_DATA_CH5) {
            ch_idx = src->log - IMAGE_DATA_CH0;
        } else if (src->log == IMAGE_DATA_MAGNITUDE || src->log == IMAGE_DATA_PHASE) {
            ch_idx = 0;  // MAGNITUDE and PHASE stored in ch[0]
        }

        const float *inputData = (float *)src->chals->ch[ch_idx];

        // Step 1: Compute input histogram
        for (int i = 0; i < totalPixels; ++i)
            inputHistogram[(uint8_t)inputData[i]]++;

        // Step 2: Compute input and target CDFs
        inputCDF[0] = (float)inputHistogram[0];
        targetCDF[0] = (float)target_histogram[0];
        for (int i = 1; i < 256; ++i) {
            inputCDF[i] = inputCDF[i - 1] + inputHistogram[i];
            targetCDF[i] = targetCDF[i - 1] + target_histogram[i];
        }

        for (int i = 0; i < 256; ++i) {
            inputCDF[i] /= totalPixels;
            targetCDF[i] /= targetCDF[255];  // normalize to 1.0
        }

        // Step 3: Build mapping from input levels to output levels
        for (int i = 0; i < 256; ++i) {
            float diffMin = 1.0f;
            int bestMatch = 0;
            for (int j = 0; j < 256; ++j) {
                float diff = fabsf(inputCDF[i] - targetCDF[j]);
                if (diff < diffMin) {
                    diffMin = diff;
                    bestMatch = j;
                }
            }
            mapping[i] = (uint8_t)bestMatch;
        }

        // Step 4: Apply mapping and normalize output to [0.0, 1.0]
        for (int i = 0; i < totalPixels; ++i) {
            uint8_t original = inputData[i];
            outputData[i] = mapping[original] / 255.0f;
        }
    }

    dst->log = IMAGE_DATA_CH0;
    return totalPixels;
}
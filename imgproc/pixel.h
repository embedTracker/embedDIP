// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#ifndef PIXEL_H
#define PIXEL_H

#include "board/common.h"
#include "core/image.h"
#include "core/memory_manager.h"

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Include all imgproc sub-modules (OpenCV-style organization)
// ============================================================================

#include "color.h"                // Color space conversions
#include "connectedcomponents.h"  // Connected component analysis
#include "drawing.h"              // Drawing primitives
#include "fft.h"                  // FFT operations
#include "filter.h"               // Filtering operations
#include "histogram.h"            // Histogram operations
#include "hough.h"                // Hough transform
#include "imgwarp.h"              // Geometric transformations
#include "misc.h"                 // Arithmetic and logical operations
#include "morph.h"                // Morphological operations
#include "segmentation.h"         // Segmentation algorithms
#include "thresh.h"               // Thresholding and intensity transforms

#ifdef __cplusplus
}
#endif

#endif  // PIXEL_H

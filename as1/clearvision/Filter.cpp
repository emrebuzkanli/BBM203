#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // 1. Copy the original image for reference.
    GrayscaleImage original = image;
    int height = image.get_height(), width = image.get_width();

    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    int halfKernelSize = kernelSize / 2;
    int divider = kernelSize * kernelSize;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int sum = 0;


            for (int offsetRow = -halfKernelSize; offsetRow <= halfKernelSize; ++offsetRow) {
                for (int offsetCol = -halfKernelSize; offsetCol <= halfKernelSize; ++offsetCol) {
                    int neighborRow = row + offsetRow; // Neighbor row
                    int neighborCol = col + offsetCol; // Neighbor column


                    if (neighborRow >= 0 && neighborRow < height && neighborCol >= 0 && neighborCol < width) {
                        sum += original.get_pixel(neighborRow, neighborCol);
                    }
                }
            }

            // 3. Update each pixel with the computed mean.
            int mean = sum / divider;
            image.set_pixel(row, col, mean);
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // 1. Create a Gaussian kernel based on the given sigma value.
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double kernelSum = 0.0;
    int halfKernelSize = kernelSize / 2;
    double sigmaSquare = sigma * sigma;
    for (int offsetRow = -halfKernelSize; offsetRow <= halfKernelSize; ++offsetRow) {
        for (int offsetCol = -halfKernelSize; offsetCol <= halfKernelSize; ++offsetCol) {
            double gaussianValue = (1.0 / (2.0 * M_PI * sigmaSquare)) * exp(-(offsetRow * offsetRow + offsetCol * offsetCol) / (2 * sigmaSquare));
            kernel[offsetRow + halfKernelSize][offsetCol + halfKernelSize] = gaussianValue;
            kernelSum = kernelSum+ gaussianValue;
        }
    }

    // 2. Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] = kernel[i][j]/ kernelSum; // Normalize each kernel element
        }
    }

    int height = image.get_height(), width = image.get_width();
    GrayscaleImage originalImage = image; // Create a copy of the original image
    // 3. For each pixel, compute the weighted sum using the kernel.
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            double weightedSum = 0.0; // To hold the weighted sum for the pixel
            for (int kernelRow = -halfKernelSize; kernelRow <= halfKernelSize; ++kernelRow) {
                for (int kernelCol = -halfKernelSize; kernelCol <= halfKernelSize; ++kernelCol) {
                    int neighborRow = row + kernelRow; // Neighbor row
                    int neighborCol = col + kernelCol; // Neighbor column

                    // Handle boundary pixels by padding with black (0)
                    if (neighborRow >= 0 && neighborRow < height && neighborCol >= 0 && neighborCol < width) {
                        weightedSum += originalImage.get_pixel(neighborRow, neighborCol) * kernel[kernelRow + halfKernelSize][kernelCol + halfKernelSize];
                    }
                }
            }
            // 4. Update the pixel values with the smoothed results.
            image.set_pixel(row, col, static_cast<int>(weightedSum)); // Cast to int for pixel value
        }
    }
}



// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    GrayscaleImage originalImage = image;
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    GrayscaleImage blurredImage = image; // Create a copy of the original image for blurring
    apply_gaussian_smoothing(blurredImage, kernelSize, 1.0);

    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    int height = image.get_height(), width = image.get_width();

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            // Get the original and blurred pixel values
            int originalPixel = originalImage.get_pixel(row, col);
            int blurredPixel = blurredImage.get_pixel(row, col);

            int sharpenedPixel = static_cast<int>(originalPixel + amount * (originalPixel - blurredPixel));

            // 3. Clip values to ensure they are within a valid range [0-255].
            if (sharpenedPixel > 255) {
                sharpenedPixel = 255;
            } else if (sharpenedPixel < 0) {
                sharpenedPixel = 0;
            }
            image.set_pixel(row, col, sharpenedPixel);
        }
    }
}
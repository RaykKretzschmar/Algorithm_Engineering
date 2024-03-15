#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <iostream>
#include <cstdint>
#include <vector>

unsigned char* loadImage(const char* filename, int& width, int& height, int& channels);
void saveImage(const char* filename, unsigned char* img, int width, int height, int channels, int quality);

void convertToGrayscale(unsigned char* img, int width, int height, int channels, uint8_t* gray_img);

void primitiveThreshold(uint8_t* gray_img, uint8_t* MM_img, int width, int height, int gray_channels);
void adaptiveThreshold(uint8_t* gray_img, uint8_t* MM_img, int width, int height, int gray_channels);
void proposedThreshhold(uint8_t* gray_img, uint8_t* binarized_img, int width, int height, int gray_channels, double global_mean, double global_std_dev);
void localMinMax(uint8_t* gray_img, uint8_t* MM_img, int width, int height, int gray_channels);

double calculateGlobalMean(uint8_t* img, int width, int height);
double calculateGlobalStdDev(uint8_t* img, int width, int height, double mean);

void calculateContrastImage(uint8_t* img, float* contrastImg, int width, int height, int gray_channels);
float calculatePixelContrast(uint8_t* img, int x, int y, int width, int height);
void detectHighContrastPixels(const float* contrastImg, uint8_t* highContrastMask, int width, int height);
void estimateLocalThresholdsAndBinarize(uint8_t* gray_img, uint8_t* high_contrast_mask, uint8_t* binarized_img, int width, int height, int gray_channels);

#endif // IMAGE_PROCESSING_H

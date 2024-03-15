#include <iostream>
#include <cstdint>
#include <vector>
#include <omp.h>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <limits>
#include <climits>

#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include "../include/image_processing.h"

using namespace std;


unsigned char* loadImage(const char* filename, int& width, int& height, int& channels) {
    unsigned char* img = stbi_load(filename, &width, &height, &channels, 0);
    if (img == nullptr) {
        cerr << "Error loading image" << endl;
        return nullptr;
    }
    cout << "Loaded image with width of " << width << ", height of " << height << ", and " << channels << " channels." << endl;
    
    return img;
}

void convertToGrayscale(unsigned char* img, int width, int height, int channels, uint8_t* gray_img) {
    cout << "Start Convertation to Grayscale!" << endl;

    size_t img_size = width * height * channels;
    int gray_channels = channels == 4 ? 2 : 1;

    #pragma omp parallel for
    for (size_t i = 0; i < img_size; i += channels) {
        gray_img[i / channels] = (uint8_t)((img[i] + img[i + 1] + img[i + 2]) / 3.0);
        if (channels == 4) {
            gray_img[i / channels + 1] = img[i + 3];
        }
    }

    cout << "Ended Convertation to Grayscale!" << endl;
}

double calculateGlobalMean(uint8_t* img, int width, int height) {
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < width * height; ++i) {
        sum += img[i];
    }
    return sum / (width * height);
}

double calculateGlobalStdDev(uint8_t* img, int width, int height, double mean) {
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < width * height; ++i) {
        double val = img[i] - mean;
        sum += val * val;
    }
    return std::sqrt(sum / (width * height));
}

void primitiveThreshold(uint8_t* gray_img, uint8_t* MM_img, int width, int height, int gray_channels) {
    cout << "Start primitive Threshold!" << endl;

    size_t img_size = width * height * gray_channels;

    #pragma omp parallel for
    for (size_t i = 0; i < img_size; i += gray_channels) {
        if (gray_img[i] > 128) {
            MM_img[i] = 255;
        } else {
            MM_img[i] = 0;
        }

        if (gray_channels == 2) {
            MM_img[i + 1] = gray_img[i + 1];
        }
    }

    cout << "Ended primitive Threshold!" << endl;
}



void adaptiveThreshold(uint8_t* gray_img, uint8_t* MM_img, int width, int height, int gray_channels) {
    cout << "Start adaptive Threshold!" << endl;

    const int window = 11;
    const int winMinusTwo = window - 2;
    size_t gray_img_size = width * height * gray_channels;
    std::vector<int> v(window * window, 0);
    int localThreshold;

    for(unsigned char *p = gray_img, *pg = MM_img;
        p != gray_img + gray_img_size - (width * winMinusTwo + window * window - 1);
        p++, pg++) {

        int localMax = INT_MIN;
        int localMin = INT_MAX;

#pragma omp parallel for
        for(int i = 0; i < window * window; i++){
            int index = (i % window) + (((int)i / window) * width);
            v[i] = *(p + index);
        }

        int windowAverage = 0, threshAverage = 0, counter = 0, neighThreshold = 15;
        float stdSum = 0, stdd = 0;

#pragma omp parallel for reduction(+:windowAverage)
        for(int i = 0; i < v.size(); i++){
            windowAverage += v[i];
        }

        windowAverage = windowAverage / v.size();
        std::vector<int> w = {};
        for(int i = 0; i < v.size(); i++){
            if(v[i] > windowAverage){
                counter++;
                w.push_back(v[i]); // not thread safe
            }
        }

        if(counter > neighThreshold){
#pragma omp parallel for reduction(+:threshAverage)
            for(int i = 0; i < w.size(); i++){
                threshAverage += w[i];
            }
            threshAverage = threshAverage / w.size();

#pragma omp parallel for reduction(+:stdSum)
            for(int i = 0; i < w.size(); i++){
                stdSum += (v[0] - threshAverage)*(v[0] - threshAverage);
            }
            stdSum /= 2;
            stdd = sqrt(stdSum);

            if(v[0] <= ((threshAverage + stdd)/2)){
                *pg = 0;
            } else {
                *pg = 255;
            }
        } else {
#pragma omp parallel for reduction(max:localMax) reduction(min:localMin)
            for(int i = 0; i < v.size(); i++){
                if(v[i] > localMax){
                    localMax = v[i];
                }
                if(v[i] < localMin){
                    localMin = v[i];
                }
            }
            localThreshold = (localMax - localMin) / (localMax + localMin + 0.00000000001) * 255;

            if(*p < localThreshold){
                *pg = 0;
            } else{
                *pg = 255;
            }
        }
    }

    for(unsigned char *p = gray_img + gray_img_size - (width * winMinusTwo + window * window - 1), 
        *pg = MM_img + gray_img_size - (width * 9 + window * window - 1), 
        j = 0; 
        p < gray_img + gray_img_size; 
        p++, pg++, j++){

        int h = (int)j / width;
#pragma omp parallel for
        for(int i = 0; i < window * window; i++){
            int index = (i % window) + ((h * ((int)i / window)) * width);
            v[i] = *(p + index);
        }

        int localMax = INT_MIN;
        int localMin = INT_MAX;
        int windowAverage = 0, threshAverage = 0, counter = 0, neighThreshold = 25;
        float stdSum = 0, stdd = 0;

#pragma omp parallel for reduction(+:windowAverage)
        for(int i = 0; i < v.size(); i++){
            windowAverage += v[i];
        }
        windowAverage = windowAverage / v.size();

        std::vector<int> w = {};

        for(int i = 0; i < v.size(); i++){
            if(v[i] > windowAverage){
                counter++;
                w.push_back(v[i]); // not thread safe
            }
        }

        if(counter > neighThreshold){
#pragma omp parallel for reduction(+:threshAverage)
            for(int i = 0; i < w.size(); i++){
                threshAverage += w[i];
            }
            threshAverage = threshAverage / w.size();

#pragma omp parallel for reduction(+:stdSum)
            for(int i = 0; i < w.size(); i++){
                stdSum += (v[0] - threshAverage)*(v[0] - threshAverage);
            }
            stdSum /= 2;
            stdd = sqrt(stdSum);

            if(v[0] <= ((threshAverage + stdd)/2)){
                *pg = 0;
            } else {
                *pg = 255;
            }
        } else {
#pragma omp parallel for reduction(max:localMax) reduction(min:localMin)
            for(int i = 0; i < v.size(); i++){
                if(v[i] > localMax){
                    localMax = v[i];
                }
                if(v[i] < localMin){
                    localMin = v[i];
                }
            }
            localThreshold = (localMax - localMin) / (localMax + localMin + 0.00000000001) * 255;

            
            if(*p < localThreshold){
                *pg = 0;
            } else{
                *pg = 255;
            }
        }
    }

    cout << "Ended adaptive Threshold!" << endl;
}

double calculateLocalStdDev(uint8_t* gray_img, int width, int height, int x, int y, int window_size, double& local_mean) {
    int half_window = window_size / 2;
    double sum = 0.0, sq_sum = 0.0;
    int count = 0;
    
    #pragma omp parallel for reduction(+:sum, sq_sum, count) collapse(2)
    for (int wy = -half_window; wy <= half_window; ++wy) {
        for (int wx = -half_window; wx <= half_window; ++wx) {
            int cx = std::min(std::max(x + wx, 0), width - 1);
            int cy = std::min(std::max(y + wy, 0), height - 1);
            uint8_t pixel_value = gray_img[cy * width + cx];
            sum += pixel_value;
            sq_sum += pixel_value * pixel_value;
            count++;
        }
    }

    local_mean = sum / count;
    return std::sqrt((sq_sum - (sum * sum / count)) / count);
}

void calculateGlobalStdDevRange(uint8_t* gray_img, int width, int height, int window_size, double& global_std_dev_min, double& global_std_dev_max) {
    global_std_dev_min = std::numeric_limits<double>::max();
    global_std_dev_max = 0.0;
    
    #pragma omp parallel for reduction(min:global_std_dev_min) reduction(max:global_std_dev_max) collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double local_mean;
            double local_std_dev = calculateLocalStdDev(gray_img, width, height, x, y, window_size, local_mean);
            global_std_dev_min = std::min(global_std_dev_min, local_std_dev);
            global_std_dev_max = std::max(global_std_dev_max, local_std_dev);
        }
    }
}

void proposedThreshhold(uint8_t* gray_img, uint8_t* binarized_img, int width, int height, int gray_channels, double global_mean, double global_std_dev) {
    cout << "Start proposed threshold!" << endl;
    
    const int window_size = 15;
    const int half_window = window_size / 2;
    size_t img_size = width * height * gray_channels;
    double global_std_dev_min, global_std_dev_max;

    calculateGlobalStdDevRange(gray_img, width, height, window_size, global_std_dev_min, global_std_dev_max);

    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double sum = 0.0, sq_sum = 0.0;
            int count = 0;

            #pragma omp parallel for reduction(+:sum, sq_sum, count) collapse(2)
            for (int wy = -half_window; wy <= half_window; ++wy) {
                for (int wx = -half_window; wx <= half_window; ++wx) {
                    int cx = std::min(std::max(x + wx, 0), width - 1);
                    int cy = std::min(std::max(y + wy, 0), height - 1);
                    uint8_t pixel_value = gray_img[cy * width + cx];
                    sum += pixel_value;
                    sq_sum += pixel_value * pixel_value;
                    count++;
                }
            }
            
            double local_mean = sum / count;
            double std_dev_local = std::sqrt((sq_sum - (sum * sum / count)) / count);
            
            double std_dev_adaptiv = (std_dev_local - global_std_dev_min) / (global_std_dev_max - std_dev_local);
            
            double T = local_mean - ((local_mean*local_mean - std_dev_local) / ((global_mean + std_dev_local) * (std_dev_adaptiv + std_dev_local)));
            
            size_t index = y * width + x;
            binarized_img[index] = (gray_img[index] >= T) ? 255 : 0;
        }
    }
    
    cout << "Ended proposed threshold!" << endl;
}


float calculatePixelContrast(uint8_t* img, int x, int y, int width, int height) {
    uint8_t localMax = 0;
    uint8_t localMin = 255;
    const float epsilon = 1e-5f;

    #pragma omp parallel for collapse(2) reduction(max:localMax) reduction(min:localMin)
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newX = x + i;
            int newY = y + j;

            if (newX >= 0 && newY >= 0 && newX < width && newY < height) {
                uint8_t pixelValue = img[newY * width + newX];
                localMax = max(localMax, pixelValue);
                localMin = min(localMin, pixelValue);
            }
        }
    }

    float contrast = (localMax - localMin) / (float)(localMax + localMin + epsilon);
    return contrast;
}

void calculateContrastImage(uint8_t* img, float* contrastImg, int width, int height, int gray_channels) {
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            contrastImg[y * width + x] = calculatePixelContrast(img, x, y, width, height);
        }
    }
}

float otsuThreshold(const float* contrastImg, int width, int height) {
    int histogram[256] = {0};

    #pragma omp parallel for reduction(+:histogram[:256])
    for (int i = 0; i < width * height; ++i) {
        int pixelValue = static_cast<int>(contrastImg[i] * 255);
        histogram[pixelValue]++;
    }

    int total = width * height;
    float sum = 0;

    #pragma omp parallel reduction(+:sum)
    for (int t = 0; t < 256; t++) sum += t * histogram[t];

    float sumB = 0;
    int wB = 0;
    int wF = 0;
    float varMax = 0;
    float threshold = 0;

    for (int t = 0; t < 256; t++) {
        wB += histogram[t];
        if (wB == 0) continue;

        wF = total - wB;
        if (wF == 0) break;

        sumB += static_cast<float>(t * histogram[t]);

        float mB = sumB / wB;
        float mF = (sum - sumB) / wF;

        float varBetween = static_cast<float>(wB) * static_cast<float>(wF) * (mB - mF) * (mB - mF);

        if (varBetween > varMax) {
            varMax = varBetween;
            threshold = t;
        }
    }

    return threshold / 255.0f;
}

void detectHighContrastPixels(const float* contrastImg, uint8_t* highContrastMask, int width, int height) {
    float threshold = otsuThreshold(contrastImg, width, height);

    #pragma omp parallel for
    for (int i = 0; i < width * height; ++i) {
        highContrastMask[i] = contrastImg[i] >= threshold ? 255 : 0;
    }
}

void estimateLocalThresholdsAndBinarize(
    uint8_t* gray_img, 
    uint8_t* high_contrast_mask, 
    uint8_t* binarized_img, 
    int width, 
    int height, 
    int gray_channels) {
    const int windowSize = 9;
    const int halfWindow = windowSize / 2;
    const float stdFactor = 0.5;

    std::fill_n(binarized_img, width * height * gray_channels, uint8_t(0));

    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sumIntensity = 0;
            int sumHighContrast = 0;
            int countHighContrast = 0;

            for (int wy = -halfWindow; wy <= halfWindow; ++wy) {
                for (int wx = -halfWindow; wx <= halfWindow; ++wx) {
                    int new_x = x + wx;
                    int new_y = y + wy;

                    if (new_x >= 0 && new_y >= 0 && new_x < width && new_y < height) {
                        int newIndex = new_y * width + new_x;
                        sumIntensity += gray_img[newIndex];

                        if (high_contrast_mask[newIndex]) {
                            sumHighContrast += gray_img[newIndex];
                            countHighContrast++;
                        }
                    }
                }
            }

            if (countHighContrast > 0) {
                float meanHighContrast = static_cast<float>(sumHighContrast) / countHighContrast;
                binarized_img[y * width + x] = (gray_img[y * width + x] <= meanHighContrast - stdFactor * meanHighContrast) ? 0 : 255;
            }
            else {
                float meanIntensity = static_cast<float>(sumIntensity) / (windowSize * windowSize);
                binarized_img[y * width + x] = (gray_img[y * width + x] <= meanIntensity - stdFactor * meanIntensity) ? 0 : 255;
            }
        }
    }
}

void localMinMax(uint8_t* gray_img, uint8_t* MM_img, int width, int height, int gray_channels) {
    cout << "Start local MinMax!" << endl;

    auto* contrast_img = new float[width * height];
    calculateContrastImage(gray_img, contrast_img, width, height, gray_channels);

    auto* high_contrast_mask = new uint8_t[width * height];
    float threshold = 0.5f;
    detectHighContrastPixels(contrast_img, high_contrast_mask, width, height);

    estimateLocalThresholdsAndBinarize(gray_img, high_contrast_mask, MM_img, width, height, gray_channels);

    cout << "Ended local MinMax!" << endl;

    delete[] contrast_img;
    delete[] high_contrast_mask;
}


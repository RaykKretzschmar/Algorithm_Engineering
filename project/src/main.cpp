#include <iostream>
#include <cstdint>
#include <vector>
#include <omp.h>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <limits>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "../include/image_processing.h"

using namespace std;


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <image_path> <flags>" << endl;
        return -1;
    }
    const char* imagePath = argv[1];
    int width, height, channels, gray_channels;
    double start_time, end_time;
    size_t gray_img_size, img_size;
    unsigned char* img = loadImage(imagePath, width, height, channels);
    
    gray_channels = channels == 4 ? 2 : 1;
    gray_img_size = width * height * gray_channels;
    auto* gray_img = new uint8_t[gray_img_size];
    auto* prim_MM_img = new uint8_t[gray_img_size];
    auto* adaptiv_MM_img = new uint8_t[gray_img_size];
    auto* prop_MM_img = new uint8_t[gray_img_size];

    
    img_size = width * height * channels;

    {
        for (unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
            *pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
            if (channels == 4) {
                *(pg + 1) = *(p + 3);
            }
        }
    }

    start_time = omp_get_wtime();
    convertToGrayscale(img, width, height, channels, gray_img);
    end_time = omp_get_wtime();
    cout << "Time for grayscale conversion: " << end_time - start_time << " seconds." << endl;


    vector<string> flags;
    for (int i = 2; i < argc; ++i) {
        flags.emplace_back(argv[i]);
    }

    if (flags.empty()) {
        cerr << "No flags provided. Specify at least one flag to choose the method(s) to execute." << endl;
        return -1;
    }

    for (const auto& flag : flags) {
        if (flag == "-primitive") {
            start_time = omp_get_wtime();
            primitiveThreshold(gray_img, prim_MM_img, width, height, gray_channels);
            end_time = omp_get_wtime();
            cout << "Time for primitive threshold: " << end_time - start_time << " seconds." << endl;
            stbi_write_jpg("primitiveThreshold.ppm", width, height, gray_channels, prim_MM_img, 100);
        } else if (flag == "-adaptive") {
            start_time = omp_get_wtime();
            adaptiveThreshold(gray_img, adaptiv_MM_img, width, height, gray_channels);
            end_time = omp_get_wtime();
            cout << "Time for adaptive threshold: " << end_time - start_time << " seconds." << endl;
            stbi_write_jpg("adaptiveThreshold.ppm", width, height, gray_channels, adaptiv_MM_img, 100);
        } else if (flag == "-proposed") {
            start_time = omp_get_wtime();
            double global_mean = calculateGlobalMean(gray_img, width, height);
            double global_std_dev = calculateGlobalStdDev(gray_img, width, height, global_mean);
            proposedThreshhold(gray_img, prop_MM_img, width, height, gray_channels, global_mean, global_std_dev);
            end_time = omp_get_wtime();
            cout << "Time for proposed threshold: " << end_time - start_time << " seconds." << endl;
            stbi_write_jpg("proposedThreshold.ppm", width, height, 1, prop_MM_img, 100);
        } else if (flag == "-lmm") {
            start_time = omp_get_wtime();
            localMinMax(gray_img, prop_MM_img, width, height, gray_channels);
            end_time = omp_get_wtime();
            cout << "Time for local MinMax threshold: " << end_time - start_time << " seconds." << endl;
            stbi_write_jpg("localMinMax.ppm", width, height, 1, prop_MM_img, 100);
        } else {
            cerr << "Unknown flag: " << flag << ". Skipping..." << endl;
        }
    }

    stbi_image_free(img);
    delete[] gray_img;
    delete[] prim_MM_img;
    delete[] adaptiv_MM_img;
    delete[] prop_MM_img;

    return 0;
}

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE // for faster recompilation

#include "../catch/include/catch.hpp"
#include "../include/image_processing.h"  // Adjust the path to where your code resides

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include <iostream>

TEST_CASE("Image Loading", "[loadImage]") {
    int width, height, channels;
    auto img = loadImage("../pics/in.jpg", width, height, channels);

    REQUIRE(img != nullptr);
    REQUIRE(width > 0);
    REQUIRE(height > 0);
    REQUIRE(channels > 0);

    stbi_image_free(img);
}

TEST_CASE("Convert to Grayscale", "[convertToGrayscale]") {
    int width = 2, height = 2, channels = 3;
    unsigned char test_img[] = {
        255, 0, 0,   // Red
        0, 255, 0,   // Green
        0, 0, 255,   // Blue
        255, 255, 255 // White
    };
    unsigned char expected_gray[] = {
        85, // Red
        85, // Green
        85, // Blue
        255 // White
    };

    uint8_t gray_img[4]; // Assuming 1 channel for grayscale

    convertToGrayscale(test_img, width, height, channels, gray_img);

    for (int i = 0; i < 4; ++i) {
        REQUIRE(gray_img[i] == expected_gray[i]);
    }
}

TEST_CASE("Global Mean Calculation", "[calculateGlobalMean]") {
    int width = 2, height = 2;
    uint8_t test_img[] = {
        50, 100,
        150, 200
    };

    auto mean = calculateGlobalMean(test_img, width, height);

    REQUIRE(mean == Approx(125.0));
}

TEST_CASE("Primitive Threshold", "[primitiveThreshold]") {
    int width = 2, height = 2, channels = 1;
    uint8_t test_img[] = {
        50, 200,
        130, 70
    };
    uint8_t expected_img[] = {
        0, 255,
        255, 0
    };
    uint8_t MM_img[4];

    primitiveThreshold(test_img, MM_img, width, height, channels);

    for (int i = 0; i < 4; ++i) {
        REQUIRE(MM_img[i] == expected_img[i]);
    }
}

TEST_CASE("Global Standard Deviation Calculation", "[calculateGlobalStdDev]") {
    int width = 4, height = 4;
    uint8_t test_img[] = {
        10, 20, 30, 40,
        50, 60, 70, 80,
        90, 100, 110, 120,
        130, 140, 150, 160
    };

    double mean = calculateGlobalMean(test_img, width, height);
    double std_dev = calculateGlobalStdDev(test_img, width, height, mean);

    REQUIRE(mean == Approx(85.0));
    REQUIRE(std_dev == Approx(46.09).epsilon(0.01));
}

TEST_CASE("Contrast Image Calculation with 5x5 Image", "[calculateContrastImage]") {
    int width = 5, height = 5;
    uint8_t test_img[] = {
        80, 90, 100, 90, 80,
        90, 110, 120, 110, 90,
        100, 120, 130, 120, 100,
        90, 110, 120, 110, 90,
        80, 90, 100, 90, 80
    };
    
    float expected_contrast_img[25] = {
        0.1578, 0.2, 0.1428, 0.2, 0.1578,
        0.2, 0.2380, 0.1818, 0.2380, 0.2, 
        0.1428, 0.1818, 0.0833, 0.1818, 0.1428,
        0.2, 0.2380, 0.1818, 0.2380, 0.2, 
        0.1578, 0.2, 0.1428, 0.2, 0.1578
    };

    float* contrast_img = new float[width * height];
    calculateContrastImage(test_img, contrast_img, width, height, 1);

    for (int i = 0; i < height * width; i++) {
            REQUIRE(contrast_img[i] == Approx(expected_contrast_img[i]).epsilon(0.001));
    }

    delete[] contrast_img;
}


TEST_CASE("Pixel Contrast Calculation", "[calculatePixelContrast]") {
    int width = 3, height = 3;
    uint8_t test_img[] = {
        10, 20, 30,
        40, 50, 60,
        70, 80, 90
    };
    // central pixel  (1,1) with value 50
    int x = 1, y = 1;

    float contrast = calculatePixelContrast(test_img, x, y, width, height);

    // calculate the expected contrast
    // Maximaler Wert in der Umgebung = 90, minimaler Wert = 10
    // maximal value in the neighborhood = 90, minimal value = 10
    // contrast = (90 - 10) / (90 + 10 + epsilon) = 80 / 100 = 0.8
    float expected_contrast = 0.8;

    REQUIRE(contrast == Approx(expected_contrast).epsilon(0.001));
}



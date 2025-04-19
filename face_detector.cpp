#include "face_detector.h"
#include <iostream>
#include <algorithm>

FaceDetector::FaceDetector(const std::string& cascade_path) {
    if (!face_cascade.load(cascade_path)) {
        std::cerr << "Error loading cascade file: " << cascade_path << std::endl;
        loaded = false;
    } else {
        std::cout << "Face cascade loaded successfully from: " << cascade_path << std::endl;
        loaded = true;
    }
}

bool FaceDetector::isLoaded() const {
    return loaded;
}

std::vector<cv::Rect> FaceDetector::detect(const cv::Mat& input_image, cv::Mat& processed_image) {
    if (!loaded) {
        std::cerr << "Detector not loaded properly" << std::endl;
        return {};
    }
    if (input_image.empty()) {
         std::cerr << "Input image for detection is empty" << std::endl;
         return {};
    }

    double scale = 1.0;
    if (input_image.cols > max_dimension || input_image.rows > max_dimension) {
        scale = max_dimension / std::max(input_image.cols, input_image.rows);
    }

    cv::Size new_size(static_cast<int>(input_image.cols * scale), static_cast<int>(input_image.rows * scale));
    cv::resize(input_image, processed_image, new_size);
    std::cout << "Image resized for detection to: " << processed_image.cols << "x" << processed_image.rows << std::endl;

    cv::Mat gray_image;
    cv::cvtColor(processed_image, gray_image, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray_image, faces, detection_scale_factor, min_neighbors, 0, min_face_size);

    std::cout << "Detected faces: " << faces.size() << std::endl;

    return faces;
} 
#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class FaceDetector {
public:
    explicit FaceDetector(const std::string& cascade_path);
    std::vector<cv::Rect> detect(const cv::Mat& input_image, cv::Mat& processed_image);
    bool isLoaded() const;

private:
    cv::CascadeClassifier face_cascade;
    bool loaded = false; // Флаг для проверки, успешно ли загружен каскад
    double max_dimension = 1000.0;
    double detection_scale_factor = 1.1;
    int min_neighbors = 6;
    cv::Size min_face_size = cv::Size(30, 30);
}; 
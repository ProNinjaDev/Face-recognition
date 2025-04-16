#include <iostream>
#include <string>
#include <algorithm>
#include <opencv2/core/version.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    std::cout << "Hello, OpenCV!" << std::endl;
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    std::string image_path = "pfJbYwBxPb0.jpg";
    cv::Mat image = cv::imread(image_path);

    if (image.empty()) {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    std::cout << "Image loaded successfully: " << image_path << std::endl;
    std::cout << "Image size: " << image.cols << "x" << image.rows << std::endl;

    const double max_dimension = 1000.0; // Максимальная ширина или высота
    double scale = 1.0;

    if (image.cols > max_dimension || image.rows > max_dimension) {
        scale = max_dimension / std::max(image.cols, image.rows);
    }
    
    cv::Mat resized_image;
    cv::Size new_size(static_cast<int>(image.cols * scale), static_cast<int>(image.rows * scale));
    cv::resize(image, resized_image, new_size);    
    
    std::cout << "Resized size: " << resized_image.cols << "x" << resized_image.rows << std::endl;

    cv::imshow("Image", resized_image);

    std::string output_filename = "my_image.jpg";
    cv::imwrite(output_filename, resized_image);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
} 
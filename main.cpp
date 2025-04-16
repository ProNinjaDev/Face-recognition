#include <iostream>
#include <string>
#include <opencv2/core/version.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

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

    cv::imshow("Image", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
} 
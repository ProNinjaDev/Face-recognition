#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/core/version.hpp>
#include <opencv2/opencv.hpp>
#include "face_detector.h"

int main(int argc, char* argv[]) {
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    if (argc < 2) {
        std::cerr << "Usage: " << (argc > 0 ? argv[0] : "face_recognition_app")
                  << " <input_image_path> [output_image_path]" << std::endl;
        std::cerr << "  <input_image_path> : Path to the image file." << std::endl;
        std::cerr << "  [output_image_path]: Optional. Path to save the result."
                  << " Defaults to <input_image_name>_detected.<ext>" << std::endl;
        return 1;
    }

    std::string image_path = argv[1];
    cv::Mat image = cv::imread(image_path);

    if (image.empty()) {
        std::cerr << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    std::cout << "Image loaded successfully: " << image_path << std::endl;
    std::cout << "Original image size: " << image.cols << "x" << image.rows << std::endl;

    std::string cascade_path = "haarcascade_frontalface_default.xml";
    FaceDetector detector(cascade_path);

    if (!detector.isLoaded()) {
         std::cerr << "Failed to initialize face detector." << std::endl;
         std::cerr << "Ensure the cascade file '" << cascade_path << "' exists next to the executable." << std::endl;
         return 1;
    }

    cv::Mat processed_image;
    std::vector<cv::Rect> faces = detector.detect(image, processed_image);

    if (faces.empty()) {
        std::cout << "No faces detected in the image" << std::endl;
    }

    for (auto& face : faces) {
        cv::rectangle(processed_image, face, cv::Scalar(0, 0, 255), 2);
    }


    // cv::imshow("Detected Faces", processed_image);

    std::string output_filename;
    if (argc > 2) {
        output_filename = argv[2];
    } else {
        try {
            std::filesystem::path input_p(image_path);
            std::string stem = input_p.stem().string();
            std::string ext = input_p.extension().string();
            output_filename = stem + "_detected" + ext;
            if (ext.empty()) {
                output_filename += ".jpg";
            }
            if (input_p.has_parent_path()) {
                output_filename = (input_p.parent_path() / output_filename).string();
            }

        } catch (const std::exception& e) {
            std::cerr << "Error processing output path: " << e.what() << std::endl;
            output_filename = "output_detected.jpg";
        }
    }
    std::cout << "Saving result to: " << output_filename << std::endl;

    if (!cv::imwrite(output_filename, processed_image)) {
         std::cerr << "Could not save the image to: " << output_filename << std::endl;
    } else {
         std::cout << "Result saved successfully" << std::endl;
    }

    /*
    std::cout << "Press any key to exit" << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
    */

    return 0;
} 
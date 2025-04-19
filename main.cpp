#include <iostream>
#include <string>
#include <algorithm>
#include <opencv2/core/version.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <filesystem>

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

    cv::CascadeClassifier face_cascade;
    std::string cascade_path = "haarcascade_frontalface_default.xml";

    if (!face_cascade.load(cascade_path)) {
        std::cerr << "Error loading cascade file: " << cascade_path << std::endl;
        std::cerr << "Ensure the file exists next to the executable." << std::endl;
        return 1;
    }
    std::cout << "Face cascade loaded successfully from: " << cascade_path << std::endl;

    std::cout << "Image loaded successfully: " << image_path << std::endl;
    std::cout << "Image size: " << image.cols << "x" << image.rows << std::endl;

    const double max_dimension = 1000.0;
    double scale = 1.0;

    if (image.cols > max_dimension || image.rows > max_dimension) {
        scale = max_dimension / std::max(image.cols, image.rows);
    }
    
    cv::Mat resized_image;
    cv::Size new_size(static_cast<int>(image.cols * scale), static_cast<int>(image.rows * scale));
    cv::resize(image, resized_image, new_size);    
    
    std::cout << "Resized size: " << resized_image.cols << "x" << resized_image.rows << std::endl;

    cv::Mat gray_image;
    cv::cvtColor(resized_image, gray_image, cv::COLOR_BGR2GRAY);
    
    std::vector<cv::Rect> faces;
    
    // minNeighbors < 6 ищет на тестовой картинке лишние лица
    face_cascade.detectMultiScale(gray_image, faces, 1.1, 6, 0, cv::Size(30, 30)); // Не забыть посмотреть другие параметры

    std::cout << "Detect faces: " << faces.size() << std::endl;

    if (faces.empty()) {
        std::cerr << "No faces detected in the image" << std::endl;
    }

    for (auto& face : faces) {
        cv::rectangle(resized_image, face, cv::Scalar(0, 0, 255), 2);
    }

    cv::imshow("Image", resized_image);

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

        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Could not parse input path correctly" << std::endl;
            output_filename = "output_detected.jpg";
        }
    }
    std::cout << "Saving result to: " << output_filename << std::endl;

    if (!cv::imwrite(output_filename, resized_image)) {
         std::cerr << "Could not save the image to: " << output_filename << std::endl;
    }

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
} 
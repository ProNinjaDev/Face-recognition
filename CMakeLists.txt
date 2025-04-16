cmake_minimum_required(VERSION 3.15)

project(FaceRecognitionApp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)

add_executable(face_recognition_app main.cpp)

target_link_libraries(face_recognition_app PRIVATE ${OpenCV_LIBS})

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}") 
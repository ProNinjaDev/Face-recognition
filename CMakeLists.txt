cmake_minimum_required(VERSION 3.15)

project(FaceRecognitionApp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)

add_executable(face_recognition_app main.cpp)

set(CASCADE_SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/data/haarcascade_frontalface_default.xml")

if(NOT EXISTS "${CASCADE_SOURCE_FILE}")
    message(FATAL_ERROR "Cascade file not found at ${CASCADE_SOURCE_FILE}. Please copy it to the 'data' directory.")
endif()

add_custom_command(
    TARGET face_recognition_app
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CASCADE_SOURCE_FILE}" "$<TARGET_FILE_DIR:face_recognition_app>/"
    COMMENT "Copying cascade file to output directory..."
)

target_link_libraries(face_recognition_app PRIVATE ${OpenCV_LIBS})

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}") 
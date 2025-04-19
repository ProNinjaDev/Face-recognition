cmake_minimum_required(VERSION 3.15)

project(FaceRecognitionApp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OPENCV_ANDROID_SDK_PATH "D:/opencv_android/OpenCV-android-sdk/sdk" CACHE PATH "Path to the OpenCV Android SDK root folder")

if(NOT EXISTS "${OPENCV_ANDROID_SDK_PATH}/native/jni/include/opencv2")
    message(FATAL_ERROR "OpenCV Android SDK not found or invalid at specified path: ${OPENCV_ANDROID_SDK_PATH}")
endif()

set(OpenCV_INCLUDE_DIRS "${OPENCV_ANDROID_SDK_PATH}/native/jni/include")
include_directories(${OpenCV_INCLUDE_DIRS})

if(NOT DEFINED ANDROID_ABI)
    message(WARNING "ANDROID_ABI is not defined. Using default 'arm64-v8a' for library search. Ensure you are using the NDK toolchain file for actual builds.")
    set(ANDROID_ABI "arm64-v8a")
endif()

# Отладочный вывод
set(LIBS_SEARCH_PATH "${OPENCV_ANDROID_SDK_PATH}/native/libs/${ANDROID_ABI}")
message(STATUS "Searching for OpenCV libs in: ${LIBS_SEARCH_PATH}")
file(GLOB LIBS_IN_PATH "${LIBS_SEARCH_PATH}/libopencv_java*.so")
message(STATUS "Files found in search path matching libopencv_java*.so: ${LIBS_IN_PATH}")

# Проверяем, найдена ли ровно одна библиотека
list(LENGTH LIBS_IN_PATH LIBS_COUNT)
if(LIBS_COUNT EQUAL 1)
    list(GET LIBS_IN_PATH 0 OpenCV_LIBS_FOUND)
    # Используем найденный путь напрямую
    set(OpenCV_LIBS ${OpenCV_LIBS_FOUND} CACHE FILEPATH "Path to found OpenCV library" FORCE)
    message(STATUS "Using OpenCV library found by GLOB: ${OpenCV_LIBS}")
else()
    message(WARNING "Found ${LIBS_COUNT} libraries matching libopencv_java*.so in ${LIBS_SEARCH_PATH}. Falling back to find_library.")
    # Возвращаемся к find_library
    find_library(OpenCV_LIBS
                 NAMES opencv_java4
                 PATHS "${LIBS_SEARCH_PATH}"
                 NO_DEFAULT_PATH)
endif()

if(NOT OpenCV_LIBS)
    message(FATAL_ERROR "Could not find OpenCV library (.so) for ABI '${ANDROID_ABI}' in ${OPENCV_ANDROID_SDK_PATH}/native/libs/${ANDROID_ABI}. Check the ABI and SDK path.")
endif()

message(STATUS "OpenCV library status (Android SDK):")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "    library: ${OpenCV_LIBS} (for ABI: ${ANDROID_ABI})")


add_library(face_detector_lib face_detector.cpp face_detector.h)

target_include_directories(face_detector_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Линкуем face_detector_lib с OpenCV
target_link_libraries(face_detector_lib PUBLIC ${OpenCV_LIBS})

add_executable(face_recognition_app main.cpp)

# Линкуем основное приложение с face_detector_lib
target_link_libraries(face_recognition_app PRIVATE face_detector_lib)

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
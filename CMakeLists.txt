cmake_minimum_required(VERSION 3.20)

project(
    DonutMaskerCPP
    VERSION 1.0
    DESCRIPTION "Donut & Hole Mask Generation using Classical Image Processing"
    LANGUAGES CXX
)

# ==========================================================
# C++ Standard
# ==========================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================================================
# OpenCV
# ==========================================================

set(OpenCV_DIR "C:/Users/MOHSIE/Downloads/opencv/build/x64/vc16/lib")

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV Version   : ${OpenCV_VERSION}")
message(STATUS "OpenCV Includes  : ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV Libraries : ${OpenCV_LIBS}")

# ==========================================================
# Include Directories
# ==========================================================

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# ==========================================================
# Source Files
# ==========================================================




set(SOURCES
    src/main.cpp
    src/ImageLoader.cpp
    src/BatchProcessor.cpp
    src/Preprocessor.cpp
    src/DonutDetector.cpp
)

# ==========================================================
# Executable
# ==========================================================

add_executable(
    donut_masker
    ${SOURCES}
)

# ==========================================================
# Link OpenCV
# ==========================================================

target_link_libraries(
    donut_masker
    PRIVATE
    ${OpenCV_LIBS}
)

# ==========================================================
# Compiler Warnings
# ==========================================================

if(MSVC)
    target_compile_options(
        donut_masker
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        donut_masker
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ==========================================================
# Output Directory
# ==========================================================

set_target_properties(
    donut_masker
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/output_samples
)

# Copy OpenCV runtime DLLs to the executable output folder so the built EXE can
# run without requiring the user to manually add OpenCV's bin folder to PATH.
if(WIN32 AND EXISTS "${OpenCV_DIR}/../bin")
    file(GLOB OPENCV_RUNTIME_DLLS "${OpenCV_DIR}/../bin/*.dll")
    if(OPENCV_RUNTIME_DLLS)
        foreach(_dll ${OPENCV_RUNTIME_DLLS})
            add_custom_command(TARGET donut_masker POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_dll}"
                "$<TARGET_FILE_DIR:donut_masker>"
            )
        endforeach()
    endif()
endif()

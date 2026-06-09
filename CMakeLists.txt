cmake_minimum_required(VERSION 3.20)

project(LC_OCR_CPP VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(LCOCR
    src/main.cpp
    src/app/Pipeline.cpp
    src/core/DocumentLoader.cpp
    src/core/OCRRunner.cpp
    src/core/LineUnwrapper.cpp
    src/core/FieldExtractor.cpp
    src/io/CSV.cpp
    src/io/ConfigLoader.cpp
    src/io/FileUtil.cpp
    src/io/Logger.cpp
)

target_include_directories(LCOCR PRIVATE src)

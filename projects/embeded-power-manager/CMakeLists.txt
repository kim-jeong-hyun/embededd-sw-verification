# cmake 최소 버전
cmake_minimum_required(VERSION 3.20)

# 프로젝트 명
project(EmbeddedPowerManager)

# C++ 버전(반드시 C++17 이상 필요)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 실행파일 생성
# cpp 컴파일 -> EmbeddedPowerManager.exe 생성
add_executable(
    EmbeddedPowerManager
    src/main.cpp
    src/PowerManager.cpp
)
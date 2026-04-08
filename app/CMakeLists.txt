list(APPEND BOARD_ROOT F:/swaroopb_zephyr/zephyr-course)

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS F:/swaroopb_zephyr/zephyr-course/zephyr)
project(app)
target_sources(app PRIVATE src/main.cpp)
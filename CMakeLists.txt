cmake_minimum_required(VERSION 3.16)
project(youtubeplayer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure LVGL - use lv_conf.h from configs/
set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/configs/lv_conf.h" CACHE PATH "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/lvgl)

add_executable(youtubeplayer
    src/main.cpp
    src/backend/video_backend.cpp
    src/view/main_view.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0)

target_include_directories(youtubeplayer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backend
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controller
    ${CMAKE_CURRENT_SOURCE_DIR}/src/model
    ${CMAKE_CURRENT_SOURCE_DIR}/src/view
    ${CMAKE_CURRENT_SOURCE_DIR}/configs
    ${GST_INCLUDE_DIRS}
)

target_link_libraries(youtubeplayer lvgl ${GST_LIBRARIES} pthread)

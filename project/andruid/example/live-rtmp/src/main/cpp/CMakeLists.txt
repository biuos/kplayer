
cmake_minimum_required(VERSION 3.10.2)
project("BBLive")


add_subdirectory(librtmp)


add_library(
        BBLive SHARED

        ScreenLive_jni.cpp
)


target_link_libraries(
        BBLive
        # android system library
        log
        # static RTMP library
        RTMP
)

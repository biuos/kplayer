cmake_minimum_required(VERSION 3.5)

# operator system check
if (IOS)
    MESSAGE(DEBUG "platform: iOS")
elseif (ANDROID)
    MESSAGE(DEBUG "platform: android")
else ()
    MESSAGE(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif ()


SET(CMAKE_CXX_STANDARD          17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(PROJECT_NAME          ktplayer)
SET(SRCS                        "")


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/play/include
)

if (IOS)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/platform/ios/include)
elseif (ANDROID)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/platform/android/include)
endif ()

# src
FILE(GLOB_RECURSE src_core_c  ${CMAKE_CURRENT_SOURCE_DIR}/core/*.c)
FILE(GLOB_RECURSE src_core_cc ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp)
SET (SRCS ${SRCS} ${src_core_c} ${src_core_cc})

FILE(GLOB_RECURSE src_core_c  ${CMAKE_CURRENT_SOURCE_DIR}/play/*.c)
FILE(GLOB_RECURSE src_core_cc ${CMAKE_CURRENT_SOURCE_DIR}/play/*.cpp)
SET (SRCS ${SRCS} ${src_core_c} ${src_core_cc})


if (ANDROID)
    FILE(GLOB_RECURSE android_src_c  ${CMAKE_CURRENT_SOURCE_DIR}/platform/android/*.c)
    FILE(GLOB_RECURSE android_src_cc ${CMAKE_CURRENT_SOURCE_DIR}/platform/android/*.cpp)
    SET(SRCS ${SRCS} ${android_src_c} ${android_src_cc})

elseif (IOS)
    FILE(GLOB_RECURSE ios_src_c  ${CMAKE_CURRENT_SOURCE_DIR}/platform/ios/*.c)
    FILE(GLOB_RECURSE ios_src_cc ${CMAKE_CURRENT_SOURCE_DIR}/platform/ios/*.cpp)
    SET(SRCS ${SRCS} ${ios_src_c} ${ios_src_cc})

endif ()


add_library(${PROJECT_NAME} SHARED ${SRCS})

if (ANDROID)
    target_link_libraries(${PROJECT_NAME} PUBLIC
            log
            android
            OpenSLES)
elseif (IOS) # APPLE IOS
    ## xxx todo for IOS
endif ()

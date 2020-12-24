cmake_minimum_required(VERSION 3.5)

# operator system check
if (IOS)
    MESSAGE( DEBUG "------------------------------" )
    MESSAGE( DEBUG "platform: iOS" )
    MESSAGE( DEBUG "------------------------------" )
elseif (ANDROID)
    MESSAGE( DEBUG "------------------------------" )
    MESSAGE( DEBUG "platform: android" )
    MESSAGE( DEBUG "------------------------------" )
else ()
    MESSAGE(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif ()


SET(CMAKE_CXX_STANDARD          17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(PROJECT_NAME           kplayer)
SET(SRCS                        "")


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
)

if (IOS)
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/platform/ios/include )
elseif (ANDROID)
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/platform/android/include )
endif ()

# src
FILE(GLOB_RECURSE src_core_c ${CMAKE_CURRENT_SOURCE_DIR}/core/*.c)
SET (SRCS ${SRCS} ${src_core_c})

FILE(GLOB_RECURSE src_core_cc ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp)
SET (SRCS ${SRCS} ${src_core_cc})







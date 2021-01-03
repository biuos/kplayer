
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)


# close openssl
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_CRYPTO")

FILE(GLOB srcs *.c)

# build static library
ADD_LIBRARY(RTMP STATIC ${srcs})

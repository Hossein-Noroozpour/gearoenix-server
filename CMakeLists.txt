CMAKE_MINIMUM_REQUIRED(VERSION 3.4 FATAL_ERROR)

PROJECT(SLXS)

SET(PROJECT_DESCRIPTION "Standard Letters XML Service.")

SET(GX_VERSION_MAJOR 1)
SET(GX_VERSION_MINOR 0)
SET(GX_VERSION_PATCH 0)
SET(GX_VERSION "${GX_VERSION_MAJOR}.${GX_VERSION_MINOR}.${GX_VERSION_PATCH}")

MESSAGE(STATUS "SLXS is going to be compiled for ${CMAKE_SYSTEM_NAME}")

# Language level
IF (MSVC)
    SET(CMAKE_CXX_FLAGS "/std:c++latest ${CMAKE_CXX_FLAGS}")
ELSE ()
    SET(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS}")
ENDIF ()
IF (APPLE)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
ENDIF ()

# Warning generation
IF (MSVC)
    STRING(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS_NEW ${CMAKE_CXX_FLAGS})
    IF (${CMAKE_CXX_FLAGS_NEW} STREQUAL ${CMAKE_CXX_FLAGS})
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    ELSE ()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_NEW}")
    ENDIF ()
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
ELSE ()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
ENDIF (MSVC)

SET(SLXS_SDK_PATH ${CMAKE_SOURCE_DIR}/sdk)
MESSAGE(STATUS "SLXS SDK path is '${SLXS_SDK_PATH}'.")

INCLUDE_DIRECTORIES(${SLXS_SDK_PATH}/boost)
INCLUDE_DIRECTORIES(${SLXS_SDK_PATH}/oracle-sdk/sdk/include)

LINK_DIRECTORIES(${SLXS_SDK_PATH}/oracle)

ADD_EXECUTABLE(SLXS
        ${CMAKE_SOURCE_DIR}/src/connection/slxs-conn-keys.cpp
        ${CMAKE_SOURCE_DIR}/src/connection/slxs-conn-keys.hpp
        ${CMAKE_SOURCE_DIR}/src/connection/slxs-conn-listener.cpp
        ${CMAKE_SOURCE_DIR}/src/connection/slxs-conn-listener.hpp
        ${CMAKE_SOURCE_DIR}/src/connection/slxs-conn-session.cpp
        ${CMAKE_SOURCE_DIR}/src/connection/slxs-conn-session.hpp
        ${CMAKE_SOURCE_DIR}/src/core/slxs-core-configurations.cpp
        ${CMAKE_SOURCE_DIR}/src/core/slxs-core-configurations.hpp
        ${CMAKE_SOURCE_DIR}/src/database/oracle/slxs-db-ora-connection-manager.cpp
        ${CMAKE_SOURCE_DIR}/src/database/oracle/slxs-db-ora-connection-manager.hpp
        ${CMAKE_SOURCE_DIR}/src/database/slxs-db-connection-manager.cpp
        ${CMAKE_SOURCE_DIR}/src/database/slxs-db-connection-manager.hpp
        ${CMAKE_SOURCE_DIR}/src/log/slxs-log.cpp
        ${CMAKE_SOURCE_DIR}/src/log/slxs-log.hpp
        ${CMAKE_SOURCE_DIR}/src/slxs-main.cpp)

IF (MSVC)
ELSE ()
    TARGET_LINK_LIBRARIES(SLXS pthread crypto ssl occi clntsh nnz19 clntshcore)
ENDIF ()
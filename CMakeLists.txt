cmake_minimum_required(VERSION 2.6)
project(rpitank)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Threads)
find_package(pigpio REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${pigpio_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(RPiTank_SRCS RPiTank.cpp HttpProvider.cpp GPIO.cpp main.cpp RPiTank.qrc)

add_executable(rpitank ${RPiTank_SRCS})
target_link_libraries(rpitank Qt5::Core Qt5::Network ${pigpio_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS rpitank RUNTIME DESTINATION bin)

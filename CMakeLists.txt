cmake_minimum_required(VERSION 3.0.2)
project(msg_downsampler)

add_compile_options(-std=c++14 -O2 -g -Wall)

find_package(catkin REQUIRED
    roscpp
    message_filters
)

################################################
## Declare ROS messages, services and actions ##
################################################

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

catkin_package(
)

###########
## Build ##
###########

include_directories(
    ${catkin_INCLUDE_DIRS}
)

add_executable(compressed_image_downsampler src/compressed_image_downsampler.cpp)
target_link_libraries(compressed_image_downsampler
    ${catkin_LIBRARIES}
)

#############
## Install ##
#############

#############
## Testing ##
#############

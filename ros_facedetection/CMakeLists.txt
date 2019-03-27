cmake_minimum_required(VERSION 2.8.3)
project(ros_facedetection)

find_package(catkin REQUIRED COMPONENTS roscpp image_transport cv_bridge)
find_package(OpenCV REQUIRED)

catkin_package(
  CATKIN_DEPENDS roscpp opencv2 image_transport cv_bridge
)

include_directories(${catkin_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)

add_executable(ros_facedetection src/ros_facedetection.cpp)
add_dependencies(ros_facedetection ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(ros_facedetection ${catkin_LIBRARIES} facedetection ${OpenCV_LIBS})


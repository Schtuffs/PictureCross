cmake_minimum_required(VERSION 3.10)
project(PictureCross CXX)

set(SRC ${CMAKE_CURRENT_LIST_DIR}/src)
set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set (CMAKE_CXX_STANDARD 23)

add_executable(PictureCross ${SRC}/main.cpp ${SRC}/Grid.cpp ${SRC}/HeaderInfo.cpp ${SRC}/Line.cpp ${SRC}/Solver.cpp ${SRC}/Timer.cpp)
target_include_directories(PictureCross PRIVATE ${INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE "-lstdc++exp")

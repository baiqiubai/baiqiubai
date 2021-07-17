
cmake_minimum_required(VERSION 3.5)

project (ReactorTcp)


include_directories(${PROJECT_SOURCE_DIR}/include)
#包含头文件路径

file(GLOB Tcp_SRCS "src/*.cpp") 
#Tcp_SRCS为src/下所有.cpp 文件

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#二进制代码存放在顶层CMALISTS的bin目录下
add_library(ReactorTcp ${Tcp_SRCS})
#制作静态库
target_link_libraries(ReactorTcp pthread) 
#链接

aux_source_directory(${PROJECT_SOURCE_DIR}/src/tests mains)
#获取tests下所有文件

foreach(mainfile IN LISTS mains)
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
    message(STATUS "name = ${mainname}")
    target_link_libraries(${mainname} ReactorTcp)
endforeach()





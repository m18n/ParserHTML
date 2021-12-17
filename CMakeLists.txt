cmake_minimum_required(VERSION 3.5)
project(parserHTML)
file(GLOB CPPS "*.cpp")
add_executable(${PROJECT_NAME} ${CPPS} )


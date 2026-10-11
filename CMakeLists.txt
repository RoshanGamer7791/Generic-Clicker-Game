cmake_minimum_required(VERSION 3.12)

project(ClickerGame CXX)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(nlohmann_json)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets nlohmann_json::nlohmann_json)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/upgrades.json"
        "${CMAKE_CURRENT_BINARY_DIR}/upgrades.json"
    COMMENT "Copying upgrades to build folder"
)
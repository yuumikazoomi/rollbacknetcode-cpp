cmake_minimum_required(VERSION 3.0)

project(rollback)

add_executable(rollback main.cpp entity.cpp gamestate.cpp netinterface.cpp)

target_include_directories(rollback PUBLIC "include")

add_subdirectory(SDL)

target_include_directories(rollback PUBLIC SDL/include)

target_link_libraries(rollback SDL2)

target_compile_features(rollback PRIVATE cxx_std_17)
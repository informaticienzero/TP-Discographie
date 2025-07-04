cmake_minimum_required(VERSION 4.0.0)

project(Discographie)

add_executable(Discographie main.cpp)
# add_library(DiscographieModules)
target_sources(Discographie
    PUBLIC
    FILE_SET CXX_MODULES FILES
    DiscographieTests.ixx String.ixx Discographie.ixx 
)
target_compile_features(Discographie PUBLIC cxx_std_23)

# target_link_libraries(Discographie PRIVATE DiscographieModules)
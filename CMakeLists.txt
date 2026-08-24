cmake_minimum_required(VERSION 3.10)
project(SkillPathV2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(skillpath_v2
    src/main.cpp
    src/SkillGraph.cpp
    src/PracticeScheduler.cpp
    src/SkillTypes.cpp
)

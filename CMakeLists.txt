cmake_minimum_required(VERSION 3.15)
project(mcpico2at)

find_package(ROOT REQUIRED COMPONENTS RIO Tree)
include(${ROOT_USE_FILE})
include(FetchContent)

FetchContent_Declare(
        AnalysisTree
        GIT_REPOSITORY https://github.com/HeavyIonAnalysis/AnalysisTree
        GIT_TAG        v2.2.3
        CMAKE_ARGS
        "-CMAKE_CXX_STANDARD=17"
)
FetchContent_MakeAvailable(AnalysisTree)

add_executable(convert src/convert.cc src/mc_pico_chain.cc src/out_tree_manager.cc src/converter.cc)
target_link_libraries(convert ROOT::RIO ROOT::Tree AnalysisTreeBase AnalysisTreeInfra)
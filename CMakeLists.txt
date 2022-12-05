cmake_minimum_required(VERSION 3.15)
project(mcpico2at)

find_package(ROOT REQUIRED COMPONENTS RIO Tree)
find_package(Boost REQUIRED COMPONENTS program_options)
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

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${ROOT_INCLUDE_DIRS}
)
set(SOURCES
        src/unigen_format/UEvent.cxx
        src/unigen_format/UParticle.cxx
        src/unigen_format/URun.cxx
        )

set(HEADERS
        src/unigen_format/UEvent.h
        src/unigen_format/UParticle.h
        src/unigen_format/URun.h
        )

set(PCM_FILE_NAME libUnigen)
ROOT_GENERATE_DICTIONARY(G__Unigen ${HEADERS}
        LINKDEF src/unigen_format/UniGenLinkDef.h
        OPTIONS
        -I ${CMAKE_SOURCE_DIR})
add_library(Unigen SHARED ${SOURCES} G__Unigen.cxx)
target_link_libraries(Unigen PUBLIC ROOT::RIO ROOT::Core ROOT::Tree)

add_executable(convert
        src/convert.cc
        src/converter.cc
        src/input_chain.cc
        src/mc_pico_chain.cc
        src/unigen_chain.cc
        src/out_tree_manager.cc)
target_link_libraries(convert ROOT::RIO ROOT::Tree AnalysisTreeBase AnalysisTreeInfra Boost::program_options Unigen)

add_executable(cut_equal_bins macro/cut_equal_bins.cc)
target_link_libraries(cut_equal_bins ROOT::RIO ROOT::ProofBench)

add_subdirectory(qnanalysis/flow)
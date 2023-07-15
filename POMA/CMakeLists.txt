cmake_minimum_required(VERSION 3.8)
project(motif_src)

include_directories(include)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 11)

string(REPLACE "-O3" "-Os" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

set(SOURCE_FILES
        Graph.h
        Graph.cpp
        main.cpp
        SetTrieNode.cpp
        SetTrieNode.h
        subgraph_match.cpp
        subgraph_match.h
        small-graph.h
        small-graph.cpp
        ./include global-variable.h global-variable.cpp maximal-motif-clique.h maximal-motif-clique.cpp)

add_executable(motif_src ${SOURCE_FILES})
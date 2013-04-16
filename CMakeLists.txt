cmake_minimum_required (VERSION 2.6)
project (powerline-zsh)

set( CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++ -lc++abi")
add_subdirectory(ctoml)

find_package( Boost )
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
   add_executable(powerline-zsh powerline-zsh.cpp powerline.cpp parser.cpp)
   LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/ctoml/build/)
   target_link_libraries(powerline-zsh ${Boost_FILESYSTEM_LIBRARY} ctoml)
endif()

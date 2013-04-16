cmake_minimum_required (VERSION 2.6)
project (powerline-zsh)

find_package( Boost )
          if(Boost_FOUND)
             include_directories(${Boost_INCLUDE_DIRS})
             add_executable(powerline-zsh powerline-zsh.cpp powerline.cpp segment.cpp)
          endif()


link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ctoml/build)
target_link_libraries(powerline-zsh
  ${Boost_FILESYSTEM_LIBRARY}
)

set( CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++ -lc++abi")

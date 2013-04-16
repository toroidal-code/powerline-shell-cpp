cmake_minimum_required (VERSION 2.6)
project (powerline-zsh)

find_package( Boost )
          if(Boost_FOUND)
             include_directories(${Boost_INCLUDE_DIRS})
             add_executable(powerline-zsh powerline.cpp powerline-zsh.cpp)
          endif()

target_link_libraries(powerline-zsh
  ${Boost_FILESYSTEM_LIBRARY}
)

set( CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++ -lc++abi")

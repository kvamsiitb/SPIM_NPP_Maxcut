cmake_minimum_required(VERSION 3.10)

project(ising_machine C CXX)

set(ising_machine_VERSION_MAJOR 0)
set(ising_machine_VERSION_MINOR 1)

foreach (lang CXX)
  set(CMAKE_${lang}_STANDARD 14)
  set(CMAKE_${lang}_STANDARD_REQUIRED ON)
  set(CMAKE_${lang}_EXTENSIONS OFF)
endforeach()


set(HOLOEYE_TOOLCHAIN_DIR "" CACHE STRING "" FORCE)

add_subdirectory(slm_target)

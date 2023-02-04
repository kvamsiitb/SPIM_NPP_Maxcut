set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/slm_display/cmake/;${CMAKE_SOURCE_DIR}/cmake")
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(SLMDISPLAY REQUIRED)
find_package(Pylon REQUIRED)
message(  ${PYLON_INCLUDE_DIR} )
message( ${PYLON_INCLUDE_DIRS} )
message( ${PYLON_GCBASE_LIBRARY} )
message( ${PYLON_GENAPI_LIBRARY} )

message( ${PYLON_BASE_LIBRARY} )
message( ${PYLON_UTILITY_LIBRARY} )
message(${SLMDISPLAY_LIBRARY})


###############################
##
################################
find_package(OpenCV REQUIRED COMPONENTS core)
###############################
##
################################
message("OpenCV_LIBS ", ${OpenCV_LIBS})

set(TARGET "SPIM")
set(sources ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/sources.cpp)

set(headers ${CMAKE_CURRENT_SOURCE_DIR}/include/slm_target/getopt.h)

#set(sources ${CMAKE_CURRENT_SOURCE_DIR}/src/phasefunction_axicon.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/sources.cpp)

add_executable(${TARGET} ${sources})

target_compile_options(${TARGET} PUBLIC -std=c++11)

target_link_libraries(${TARGET} PUBLIC ${SLMDISPLAY_LIBRARY} ${PYLON_LIBRARIES} ${OpenCV_LIBS} ${CUDA_curand_LIBRARY})

target_include_directories(${TARGET} PUBLIC ${SLMDISPLAY_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${PYLON_INCLUDE_DIRS})

install(TARGETS ${TARGET} DESTINATION "${CMAKE_SOURCE_DIR}/bin")
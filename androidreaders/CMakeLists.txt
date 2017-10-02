cmake_minimum_required(VERSION 2.6)

project(androidreaders)

add_definitions(-DUNIX)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions" )

file(GLOB SRCS *.cpp)
file(GLOB include *.hpp)

file(GLOB include_readercardadapters ${CMAKE_CURRENT_SOURCE_DIR}/readercardadapters/*.hpp)
SOURCE_GROUP("Header Files\\readercardadapters" FILES ${include_readercardadapters})

file(GLOB source_readercardadapters ${CMAKE_CURRENT_SOURCE_DIR}/readercardadapters/*.cpp)
SOURCE_GROUP("Source Files\\readercardadapters" FILES ${source_readercardadapters})

set(INCLUDE_LIB ${include} ${include_readercardadapters})
set(SOURCE_LIB ${SRCS} ${source_readercardadapters})

find_library(log-lib log)

add_library(
	androidreaders
	SHARED
	${SOURCE_LIB}
	${INCLUDE_LIB}
	${resourcerc}
  )
  
target_link_libraries(
	androidreaders
	${log-lib}
	logicalaccess-cryptolib
	iso7816readers
	desfirecards
	iso7816cards
	${LIBLOGICALACCESS_LIBRARY}
	${OPENSSL_LIBRARIES}
)

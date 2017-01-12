cmake_minimum_required(VERSION 2.6)

project(androidreaders)

set(INCLUDE_FOLDER  ${CMAKE_CURRENT_SOURCE_DIR}/../../pluginsreaderproviders/iso7816
					${CMAKE_CURRENT_SOURCE_DIR}/../../pluginscards/iso7816
					${CMAKE_CURRENT_SOURCE_DIR}/../../pluginscards/desfire)

INCLUDE_DIRECTORIES(${INCLUDE_FOLDER})

file(GLOB SRCS *.cpp)
file(GLOB include *.hpp)

set(INCLUDE_LIB ${include} ${include_readercardadapters})
set(SOURCE_LIB ${SRCS} ${source_readercardadapters})

add_library(
	androidreaders
	SHARED
	${SOURCE_LIB}
	${INCLUDE_LIB}
	${resourcerc}
  )
  
target_link_libraries(
	androidreaders
	logicalaccess-cryptolib
	iso7816readers
	desfirecards
	iso7816cards
	${LIBLOGICALACCESS_LIBRARY}
	${OPENSSL_LIBRARIES}
)

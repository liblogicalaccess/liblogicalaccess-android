cmake_minimum_required(VERSION 3.4.1)
project(androidreaders)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions" )

set(INCLUDES
androidNDKHelper.hpp
androidreaderunitconfiguration.hpp
androidreaderprovider.hpp
androidreaderunit.hpp
androidsupportcontext.hpp
morphosamreaderprovider.hpp
morphosamreaderunit.hpp
jnihelper.h)
set(INCLUDE_COMMANDS commands/MifareAndroidCommands.hpp)
set(INCLUDE_RCA readercardadapters/javabridgedatatransport.hpp)

file(GLOB_RECURSE SRCS *.cpp)
set(SOURCE_LIB ${SRCS} ${source_readercardadapters} ${source_commands})

find_library(log-lib log)

add_library(
	androidreaders
	SHARED
	${SOURCE_LIB}
	${INCLUDES}
	${resourcerc}
  )

target_compile_definitions(androidreaders PRIVATE -DUNIX)

# Work around a boost / clang issue with include to string/view.
# See https://github.com/boostorg/asio/issues/82
target_compile_definitions(androidreaders PUBLIC BOOST_ASIO_HAS_STD_STRING_VIEW=1)

target_link_libraries(androidreaders PUBLIC
  ${log-lib}
  CONAN_PKG::LogicalAccessPrivate
  CONAN_PKG::boost)

install(FILES ${INCLUDES} DESTINATION include/logicalaccess/plugins/readers/android/)
install(FILES ${INCLUDE_COMMANDS} DESTINATION include/logicalaccess/plugins/readers/android/commands)
install(FILES ${INCLUDE_RCA} DESTINATION include/logicalaccess/plugins/readers/android/readercardadapters)
install(TARGETS androidreaders RUNTIME DESTINATION bin/${LIB_SUFFIX} ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)

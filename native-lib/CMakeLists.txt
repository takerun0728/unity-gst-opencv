cmake_minimum_required(VERSION 3.22)

project(native-lib LANGUAGES CXX)

if (WIN32)
    message("Building for Windows")
elseif (ANDROID)
    message("Building for Android")
endif()

message("${PKG_CONFIG_PATH}")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-app-1.0)

add_library(NativeGstOpenCV SHARED native_lib.cpp)

target_include_directories(NativeGstOpenCV PRIVATE ${GST_INCLUDE_DIRS})
target_link_libraries(NativeGstOpenCV PRIVATE ${GST_LIBRARIES})
target_link_directories(NativeGstOpenCV PRIVATE ${GST_LIBRARY_DIRS})
target_compile_options(NativeGstOpenCV PRIVATE ${GST_CFLAGS_OTHER})

install(TARGETS NativeGstOpenCV
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} #for Android
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} #for Windows
)

#Only for Windows debug mode
file(GLOB PDB_FILES "${CMAKE_CURRENT_BINARY_DIR}/*.pdb")
foreach(PDB ${PDB_FILES})
    install(FILES ${PDB} DESTINATION ${CMAKE_INSTALL_PREFIX})
endforeach()
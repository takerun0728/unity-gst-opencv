cmake_minimum_required(VERSION 3.22)

project(native-lib LANGUAGES CXX)
set(LIB_NAME NativeGstOpenCV)

add_library(${LIB_NAME} SHARED native_lib.cpp)

if (WIN32)
    message("Building for Windows")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        install(FILES "${OpenCV_DIR}/x64/vc16/bin/opencv_world4110d.dll" DESTINATION ${CMAKE_INSTALL_PREFIX})
    else()
        install(FILES "${OpenCV_DIR}/x64/vc16/bin/opencv_world4110.dll" DESTINATION ${CMAKE_INSTALL_PREFIX})
    endif()
    set(GST_PLUGIN_TARGETS)
elseif (ANDROID)
    message("Building for Android")
    set(GSTREAMER_PLUGINS coreelements app videotestsrc)
    set(GST_PLUGIN_TARGETS GStreamer::coreelements GStreamer::app GStreamer::videotestsrc)
    target_compile_definitions(${LIB_NAME} PRIVATE "OPENCV_STATIC")
endif()

find_package(OpenCV REQUIRED)
set(WIN32 TRUE) #For inappropriate description of multiple paths in FindGstreamer.cmake
set(GStreamer_EXTRA_DEPS glib-2.0 gmodule-2.0 gstreamer-app-1.0 libffi libpcre2-8)
find_package(GStreamer REQUIRED)
set(WIN32 FALSE)
message("${PC_GStreamer_LIBRARY_DIRS}")

target_include_directories(${LIB_NAME} PRIVATE ${PC_GStreamer_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
target_link_directories(${LIB_NAME} PRIVATE ${PC_GStreamer_LIBRARY_DIRS} ${OpenCV_DIR})
target_link_libraries(${LIB_NAME} PRIVATE ${PC_GStreamer_LIBRARIES}  ${OpenCV_LIBS} ${GST_PLUGIN_TARGETS})
target_compile_options(${LIB_NAME} PRIVATE ${GST_CFLAGS_OTHER})

install(TARGETS ${LIB_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} #for Android
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} #for Windows
)

#Only for Windows debug mode
file(GLOB PDB_FILES "${CMAKE_CURRENT_BINARY_DIR}/*.pdb")
foreach(PDB ${PDB_FILES})
    install(FILES ${PDB} DESTINATION ${CMAKE_INSTALL_PREFIX})
endforeach()
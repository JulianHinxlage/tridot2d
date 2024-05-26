cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
set(SOLUTION_NAME "Tridot2D")

#general settings
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(CMAKE_DEBUG_POSTFIX "")
    set(CMAKE_RELWITHDEBINFO_POSTFIX "")
    set(CMAKE_IMPORT_LIBRARY_SUFFIX ".lib")
    set(USE_MSVC_RUNTIME_LIBRARY_DLL 1)
    #set(CMAKE_WIN32_EXECUTABLE true)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    add_link_options(-no-pie)
endif()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_FOLDER Libs)


function(assign_source_group)
    foreach(source IN LISTS ${ARGN})
        file(RELATIVE_PATH source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${source}")
        get_filename_component(source_path "${source_rel}" PATH)
        string(REPLACE "/" "\\" source_path_msvc "${source_path}")
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()
endfunction(assign_source_group)

file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
assign_source_group(ALL_SOURCES)



project(tridot2d)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/tridot2d/*.cpp src/tridot2d/*.h)
file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS libs/imgui/*.cpp libs/imgui/misc/cpp/imgui_stdlib.cpp libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_opengl3.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${IMGUI_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC src src/tridot2d)
add_subdirectory(libs/glm)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glew/build/cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE AL_LIBTYPE_STATIC)
set(LIBTYPE "STATIC")
add_subdirectory(libs/openal)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} glm glfw opengl32 glew_s imm32 OpenAL)
else()
    target_link_libraries(${PROJECT_NAME} glm glfw GL glew pthread stdc++fs OpenAL)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC libs libs/glm libs/glfw/include libs/glew/include libs/imgui libs/imgui/backends libs/stb extern/openal/include)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${SOLUTION_NAME})


project(test)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/test/*.cpp src/test/*.h)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} tridot2d)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${SOLUTION_NAME})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})


set_target_properties(glfw uninstall update_mappings PROPERTIES FOLDER Libs)
project(${SOLUTION_NAME})
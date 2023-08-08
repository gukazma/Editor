set(TARGET_NAME "EditorMesh")

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE HPP_FILES "*.hpp")
file(GLOB_RECURSE CPP_FILES "*.cpp")
set(CMAKE_MODULE_PATH "$ENV{THIRDPARTY}/cmake" ${CMAKE_MODULE_PATH})

find_package(OpenSceneGraph 2.0.0 REQUIRED osgViewer osgDB osgUtil osgGA)
find_package(VCG REQUIRED)
find_package(Boost COMPONENTS filesystem system thread REQUIRED)
find_package(Eigen3 REQUIRED HINTS "$ENV{THIRDPARTY}/Eigen3")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${HPP_FILES} ${CPP_FILES})

add_library(${TARGET_NAME} ${HEADER_FILES} ${CPP_FILES} ${HEADER_FILES} ${HPP_FILES})
add_library(Editor::Mesh ALIAS ${TARGET_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Modules)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

# Link dependencies    
target_link_libraries(${TARGET_NAME} 
PUBLIC
Boost::system
Boost::filesystem
Boost::dynamic_linking
Boost::thread
Eigen3::Eigen
${OPENSCENEGRAPH_LIBRARIES}
)

target_include_directories(
${TARGET_NAME} 
PUBLIC 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
${OPENSCENEGRAPH_INCLUDE_DIRS}
${VCG_INCLUDE_DIRS}
)
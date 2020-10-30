cmake_minimum_required(VERSION 3.1)

#set project name and language
project(vf-export LANGUAGES CXX)

#Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#C++ 11 Flag
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Set project Version
set(VfExport_VERSION_MAJOR "0")
set(VfExport_VERSION_MINOR "0")
set(VfExport_VERSION_PATCH "1")

set(PROJECT_VERSION "${VfExport_VERSION_MAJOR}.${VfExport_VERSION_MINOR}.${VfExport_VERSION_PATCH}")

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

#Find dependecies
find_package(Qt5 REQUIRED COMPONENTS Core CONFIG  )
find_package(VfHelpers REQUIRED)
find_package(VfEvent REQUIRED)
find_package(VfComponent REQUIRED)
#find_package(VfQml REQUIRED)
find_package(VfStorageHash REQUIRED)
find_package(VfCpp REQUIRED)
find_package(PythonLibs 3.8 REQUIRED)
find_package(PythonScriptingInterface)

#sum up project Files 


file(GLOB SOURCES
${PROJECT_SOURCE_DIR}/src/*.cpp
    )


file(GLOB HEADER
${PROJECT_SOURCE_DIR}/src/*.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    )


include_directories(
    ${PYTHON_INCLUDE_DIRS}
    )


#Exclude psql files
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/vl_postgresdatabase.cpp")


#create library 
add_library(VfExport SHARED
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    )



#link dependencies
target_link_libraries(VfExport 
    PRIVATE
    Qt5::Core
    ${PYTHON_LIBRARIES}
    VeinMeta::VfHelpers
    VeinMeta::VfEvent
    VeinMeta::VfComponent
    #VeinMeta::VfQml
    VeinMeta::VfStorageHash
    VeinMeta::VfCpp
    VeinMeta::PythonScriptingInterface
    )

#set target Version
set_target_properties(VfExport PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfExport PROPERTIES SOVERSION ${VfExport_VERSION_MAJOR})


#generate export header
include(GenerateExportHeader)
generate_export_header(VfExport)

#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfexport_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfExport
    )

# announce headers - target perspective
target_include_directories(VfExport
    PUBLIC
    $<INSTALL_INTERFACE:include/VfExport>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

# install library
install(TARGETS VfExport
    EXPORT VfExportExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    VfExportConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
    )

# configure *Config.cmake
configure_file(VfExportConfig.cmake.in VfExportConfig.cmake @ONLY)



# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfExportConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfExportConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfExport
    )

# install targets cmake-files
install(EXPORT VfExportExport
    FILE VfExportTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfExport
    )

# install public headers
install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfExport
    )
    

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)



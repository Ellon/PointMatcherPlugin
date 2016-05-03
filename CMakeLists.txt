if(NOT ParaView_SOURCE_DIR)
  cmake_minimum_required(VERSION 2.8)
  set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")
  find_package(ParaView REQUIRED)
  include(${PARAVIEW_USE_FILE})
  find_package(PythonLibs REQUIRED)
endif()

find_package(libpointmatcher 1.2.3 REQUIRED)
include_directories("${libpointmatcher_INCLUDE_DIRS}")
message(STATUS "Using libpointmatcher version ${libpointmatcher_VERSION}")

include_directories(${VTK_INCLUDE_DIRS})
include_directories(${VTK_PYTHON_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(gui_sources)
set(gui_interfaces)
set(gui_xml)
set(gui_deps)
set(servermanager_sources)
set(servermanager_xml)
set(server_deps)

list(APPEND servermanager_sources
  vtkPMConversions.cxx
  vtkPMFixStepSamplingDataPointsFilter.cxx
  vtkPMSurfaceNormalDataPointsFilter.cxx
  vtkPMMaxDistDataPointsFilter.cxx
  vtkPMMaxDensityDataPointsFilter.cxx
  )

list(APPEND servermanager_xml
  xml/vtkPMFixStepSamplingDataPointsFilter.xml
  xml/vtkPMSurfaceNormalDataPointsFilter.xml
  xml/vtkPMMaxDistDataPointsFilter.xml
  xml/vtkPMMaxDensityDataPointsFilter.xml
  )

list(APPEND server_deps
  ${VTK_LIBRARIES}
  ${libpointmatcher_LIBRARIES}
  )

set(library_name vtkPMFilters)
include(GenerateExportHeader)

add_paraview_plugin(${library_name} "1.0"
  GUI_INTERFACES ${gui_interfaces}
  # GUI_RESOURCE_FILES ${gui_xml}
  GUI_SOURCES ${gui_sources}
  SERVER_MANAGER_XML ${servermanager_xml}
  SERVER_MANAGER_SOURCES ${servermanager_sources}
  )
generate_export_header(${library_name} EXPORT_FILE_NAME ${library_name}Module.h)


target_link_libraries(${library_name} LINK_PUBLIC ${gui_deps} ${server_deps})


# if(VTK_WRAP_PYTHON)
#   include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/wrap-python.cmake)
#   wrap_python(${library_name} "${servermanager_sources}" ${VTK_LIBRARIES})
# endif()


# if(BUILD_TESTING)
#   add_subdirectory(Testing)
# endif()

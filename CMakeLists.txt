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
  vtkPMIcpFilter.cxx
  )

list(APPEND servermanager_xml
  xml/vtkPMFixStepSamplingDataPointsFilter.xml
  xml/vtkPMSurfaceNormalDataPointsFilter.xml
  xml/vtkPMMaxDistDataPointsFilter.xml
  xml/vtkPMMaxDensityDataPointsFilter.xml
  xml/vtkPMObservationDirectionDataPointsFilter.xml
  xml/vtkPMOrientNormalsDataPointsFilter.xml
  xml/vtkPMIcpFilter.xml
  )

option(ENABLE_AUTOGEN "If we should auto-generate for filters from a yaml file" ON)
if(ENABLE_AUTOGEN)
  # include file to perform autogen
  include(cmake/vtkPMAutoGen.cmake)
endif()

list(APPEND server_deps
  ${VTK_LIBRARIES}
  ${libpointmatcher_LIBRARIES}
  )

set(library_name vtkPMFilters)
include(GenerateExportHeader)

# Custom property widget for IcpFilter
if (PARAVIEW_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(widget_moc_srcs
    ui/pqIcpPropertyWidget/pqIcpPropertyWidget.h
    # ui/pqIcpPropertyWidget/impossibleoptionexception.h
    ui/pqIcpPropertyWidget/outlierfilteroptions.h)
  qt5_wrap_ui(widget_form_headers
    ui/pqIcpPropertyWidget/pqIcpPropertyWidget.ui)
else ()
  qt4_wrap_cpp(widget_moc_srcs
    ui/pqIcpPropertyWidget/pqIcpPropertyWidget.h
    # ui/pqIcpPropertyWidget/impossibleoptionexception.h
    ui/pqIcpPropertyWidget/outlierfilteroptions.h)
  qt4_wrap_ui(widget_form_headers
    ui/pqIcpPropertyWidget/pqIcpPropertyWidget.ui)
endif ()

set (widget_interfaces)
set (widget_sources)
add_paraview_property_widget(widget_interfaces widget_sources
  TYPE "pqIcpPropertyWidget"
  CLASS_NAME pqIcpPropertyWidget)

list(APPEND gui_interfaces ${widget_interfaces})
list(APPEND gui_sources
  ${widget_sources}
  ${widget_moc_srcs}
  ${widget_form_headers}
  ui/pqIcpPropertyWidget/pqIcpPropertyWidget.cxx
  # ui/pqIcpPropertyWidget/impossibleoptionexception.cpp
  ui/pqIcpPropertyWidget/outlierfilteroptions.cpp
  )
include_directories(ui/pqIcpPropertyWidget)

# Create the plugin
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

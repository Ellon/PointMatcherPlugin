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

option(ENABLE_AUTOGEN "If we should auto-generate for filters from a yaml file" OFF)
if(ENABLE_AUTOGEN)
  # The autogen is composed of the following phases, all done at the configuration time:
  #   0) Create directories used by autogen
  #   1) Generate yaml file from pmicp -l (optional)
  #   2) Get output names using autogen_get_filenames.py
  #   3) Generate the .h, .cxx and .xml files from the template and the yaml file
  include(cmake/find_python_module.cmake)
  find_python_module(yaml REQUIRED)
  find_python_module(mako REQUIRED)
  ### 0) Create directories to be used by autogen:
  #        ${CMAKE_CURRENT_BINARY_DIR}/autogen_files -> directory where the auto generated files will be outputed
  #        ${CMAKE_CURRENT_BINARY_DIR}/autogen_files/dummy -> dummy directory used to trigger autoreconfiguration when some files changes
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/autogen_files/dummy)
  ### 1) Generate yaml file
  option(ENABLE_AUTOGEN_YAML "If we want to auto generate the filters definition file from pmicp -l" ON)
  if(ENABLE_AUTOGEN_YAML)
    set(AUTOGEN_YAML_FILE "${CMAKE_BINARY_DIR}/autogen_files/filters.yaml" CACHE STRING "Filters definition file to be auto-generated" FORCE)
    # set dependency to reconfigure on file change
    configure_file(${CMAKE_SOURCE_DIR}/autogen/autogen_generate_yaml.py ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
    # Generate filters.yaml from pmicp -l
    execute_process( COMMAND ${CMAKE_COMMAND} -E echo_append "-- AUTOGEN: Generating yaml with filter definitions..." )
    execute_process( COMMAND pmicp -l
                     COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/autogen/autogen_generate_yaml.py ${CMAKE_CURRENT_SOURCE_DIR}/xml ${AUTOGEN_YAML_FILE})
    execute_process( COMMAND ${CMAKE_COMMAND} -E echo "done" )
  else()
    set(AUTOGEN_YAML_FILE "${CMAKE_CURRENT_SOURCE_DIR}/autogen/filters.yaml" CACHE STRING "Filters definition file to be used by auto-generation")
    # set dependency to reconfigure on file change
    configure_file(${AUTOGEN_YAML_FILE} ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
  endif()
  ### 2) Get filenames to be auto-generated
  # set dependency to reconfigure on file change
  configure_file(${CMAKE_SOURCE_DIR}/autogen/autogen_get_filenames.py ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
  # get filenames
  execute_process( COMMAND ${CMAKE_COMMAND} -E echo_append "-- AUTOGEN: Parsing generated filenames..." )
  execute_process( COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/autogen/autogen_get_filenames.py ${AUTOGEN_YAML_FILE} xml ${CMAKE_BINARY_DIR}/autogen_files
                   OUTPUT_VARIABLE AUTOGEN_XML_FILES )
  execute_process( COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/autogen/autogen_get_filenames.py ${AUTOGEN_YAML_FILE} h ${CMAKE_BINARY_DIR}/autogen_files
                   OUTPUT_VARIABLE AUTOGEN_HEADER_FILES )
  execute_process( COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/autogen/autogen_get_filenames.py ${AUTOGEN_YAML_FILE} cxx ${CMAKE_BINARY_DIR}/autogen_files
                   OUTPUT_VARIABLE AUTOGEN_CXX_FILES )
  execute_process( COMMAND ${CMAKE_COMMAND} -E echo "done" )
  ### 3) auto generate files
  # set dependencies to reconfigure on file changes
  configure_file(${CMAKE_SOURCE_DIR}/autogen/autogen_generate_files.py ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
  configure_file(${CMAKE_SOURCE_DIR}/autogen/vtkPM___DataPointsFilter.cxx.template ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
  configure_file(${CMAKE_SOURCE_DIR}/autogen/vtkPM___DataPointsFilter.h.template ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
  configure_file(${CMAKE_SOURCE_DIR}/autogen/vtkPM___DataPointsFilter.xml.template ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
  # generate files
  execute_process( COMMAND ${CMAKE_COMMAND} -E echo_append "-- AUTOGEN: Generating files..." )
  execute_process( COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/autogen/autogen_generate_files.py ${AUTOGEN_YAML_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/autogen ${CMAKE_BINARY_DIR}/autogen_files )
  execute_process( COMMAND ${CMAKE_COMMAND} -E echo "done" )
  # Add directory with generated headers as include directory
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/autogen_files)
endif()

set(gui_sources)
set(gui_interfaces)
set(gui_xml)
set(gui_deps)
set(servermanager_sources)
set(servermanager_xml)
set(server_deps)

list(APPEND servermanager_sources
  vtkPMConversions.cxx
  )

list(APPEND servermanager_xml
  xml/vtkPMFixStepSamplingDataPointsFilter.xml
  xml/vtkPMSurfaceNormalDataPointsFilter.xml
  xml/vtkPMMaxDistDataPointsFilter.xml
  xml/vtkPMMaxDensityDataPointsFilter.xml
  xml/vtkPMObservationDirectionDataPointsFilter.xml
  xml/vtkPMOrientNormalsDataPointsFilter.xml
  )

if(ENABLE_AUTOGEN)
  list(APPEND servermanager_sources ${AUTOGEN_CXX_FILES})
  list(APPEND servermanager_xml ${AUTOGEN_XML_FILES})
endif()

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

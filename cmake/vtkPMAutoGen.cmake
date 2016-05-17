# The autogen is composed of the following phases, all done at the configuration time:
#   0) Create directories used by autogen
#   1) Generate yaml file from pmicp -l (optional)
#   2) Get output names using autogen_get_filenames.py
#   3) Generate the .h, .cxx and .xml files from the template and the yaml file
#   4) Add the auto-generated files to servermanager lists

include(cmake/find_python_module.cmake)
find_python_module(yaml REQUIRED)
find_python_module(mako REQUIRED)
### 0) Create directories to be used by autogen:
#        ${CMAKE_BINARY_DIR}/autogen_files -> directory where the auto generated files will be outputed
#        ${CMAKE_BINARY_DIR}/autogen_files/dummy -> dummy directory used to trigger autoreconfiguration when some files changes
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/autogen_files/dummy)
### 1) Generate yaml file
option(ENABLE_AUTOGEN_YAML "If we want to auto generate the filters definition file from pmicp -l" ON)
if(ENABLE_AUTOGEN_YAML)
  set(AUTOGEN_YAML_FILE "${CMAKE_BINARY_DIR}/autogen_files/filters.yaml" CACHE STRING "Filters definition file to be auto-generated" FORCE)
  # set dependency to reconfigure on file change
  configure_file(${CMAKE_SOURCE_DIR}/autogen/autogen_generate_yaml.py ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
  # Generate filters.yaml from pmicp -l
  message(STATUS "AUTOGEN: Generating yaml with filter definitions")
  execute_process( COMMAND pmicp -l
                   COMMAND python ${CMAKE_SOURCE_DIR}/autogen/autogen_generate_yaml.py ${CMAKE_SOURCE_DIR}/xml ${AUTOGEN_YAML_FILE})
  message(STATUS "AUTOGEN: Generating yaml with filter definitions - DONE")
else()
  set(AUTOGEN_YAML_FILE "${CMAKE_SOURCE_DIR}/autogen/filters.yaml" CACHE STRING "Filters definition file to be used by auto-generation")
  # set dependency to reconfigure on file change
  configure_file(${AUTOGEN_YAML_FILE} ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
endif()
### 2) Get filenames to be auto-generated
# set dependency to reconfigure on file change
configure_file(${CMAKE_SOURCE_DIR}/autogen/autogen_get_filenames.py ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
# get filenames
message(STATUS "AUTOGEN: Parsing generated filenames")
execute_process( COMMAND python ${CMAKE_SOURCE_DIR}/autogen/autogen_get_filenames.py ${AUTOGEN_YAML_FILE} xml ${CMAKE_BINARY_DIR}/autogen_files
                 OUTPUT_VARIABLE AUTOGEN_XML_FILES )
execute_process( COMMAND python ${CMAKE_SOURCE_DIR}/autogen/autogen_get_filenames.py ${AUTOGEN_YAML_FILE} h ${CMAKE_BINARY_DIR}/autogen_files
                 OUTPUT_VARIABLE AUTOGEN_HEADER_FILES )
execute_process( COMMAND python ${CMAKE_SOURCE_DIR}/autogen/autogen_get_filenames.py ${AUTOGEN_YAML_FILE} cxx ${CMAKE_BINARY_DIR}/autogen_files
                 OUTPUT_VARIABLE AUTOGEN_CXX_FILES )
message(STATUS "AUTOGEN: Parsing generated filenames - DONE")
### 3) auto generate files
# set dependencies to reconfigure on file changes
configure_file(${CMAKE_SOURCE_DIR}/autogen/autogen_generate_files.py ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/autogen/vtkPM___DataPointsFilter.cxx.template ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/autogen/vtkPM___DataPointsFilter.h.template ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/autogen/vtkPM___DataPointsFilter.xml.template ${CMAKE_BINARY_DIR}/autogen_files/dummy COPYONLY)
# generate files
message(STATUS "AUTOGEN: Generating files")
execute_process( COMMAND python ${CMAKE_SOURCE_DIR}/autogen/autogen_generate_files.py ${AUTOGEN_YAML_FILE} ${CMAKE_SOURCE_DIR}/autogen ${CMAKE_BINARY_DIR}/autogen_files )
message(STATUS "AUTOGEN: Generating files - DONE")
# Add directory with generated headers as include directory
include_directories(${CMAKE_BINARY_DIR}/autogen_files)
### 4) Add the auto-generated files to servermanager lists
list(APPEND servermanager_sources ${AUTOGEN_CXX_FILES})
list(APPEND servermanager_xml ${AUTOGEN_XML_FILES})

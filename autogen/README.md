Auto-generation of Filter Source Files
======================================

Wrapping libpointmatcher filters to paraview has been shown to be a tedious task
since most of the code are the same for each filter, appart from small changes
here and there. This python-based auto-generation tool was created to easy the
wrapping of the filters. It uses python's yaml and mako modules to do the job.
The generated files are based on the template files 
[autogen/vtkPM___DataPointsFilter.cxx.template](https://github.com/Ellon/PointMatcherPlugin/blob/master/autogen/vtkPM___DataPointsFilter.cxx.template), 
[autogen/vtkPM___DataPointsFilter.h.template](https://github.com/Ellon/PointMatcherPlugin/blob/master/autogen/vtkPM___DataPointsFilter.h.template) and 
[autogen/vtkPM___DataPointsFilter.xml.template](https://github.com/Ellon/PointMatcherPlugin/blob/master/autogen/vtkPM___DataPointsFilter.xml.template).

Filters Definition File
-----------------------

You will need to write a filters definition file, that will be used to generate
the the .h, .cxx and .xml files for each defined filter. An example of this
file can be seen at [`autogen/filters.yaml`](https://github.com/Ellon/PointMatcherPlugin/blob/master/autogen/filters.yaml).
Basically, a filter is defined using the following yaml structure:
```
<filterName>:
  properties:
    - [<property1Name>, <property1Type>, <property1DefaultValue>, <property1MinValue>, <property1MaxValue>]
    - [<property2Name>, <property2Type>, <property2DefaultValue>, <property2MinValue>, <property2MaxValue>]
    - ...
  xml_info:
    short_help: <short help text>
    long_help: <long help text>
```
where: 

- `<filterName>` should match the filter name on libpointmatcher;
- each filter property is a list, where the two last elements (min and max
  value) are optional. If provided they are used in the XML file generation to
  set limits in the ParaView GUI. All properties can be easily recovered using
  the command `pmicp -l` from libpointmatcher;
- `xml_info` is also optional. If not supplied the filter `.xml` file is not
  going to be auto-generated.

How to use it
-------------

You can enable the auto-generation in the build process to wrap filters
automatically from the yaml file, or as stand-alone scripts to get a first
version of the files needed to wrap a filter.

### In the building process

To use with CMake you need to set the `ENABLE_AUTOGEN` flag using `ccmake` or
with `-DENABLE_AUTOGEN=ON` when calling `cmake`. You may also set
`AUTOGEN_YAML_FILE` variable to the path to the yaml file that will be used to
generate the filters (it defaults to [`autogen/filters.yaml`](https://github.com/Ellon/PointMatcherPlugin/blob/master/autogen/filters.yaml)).
The generated files (.h, .cxx and .xml) will be written to
`${CMAKE_BINARY_DIR}/autogen_files`, from where you can inspect them if you
want.

### As a stand-alone script

Just execute the script passing the yaml file as argument to generate the
files in the current working directory. If the template files are not in the
current working directory, you can pass the template directory as the second
argument. You can also inform the output directory to write the files as a
third argument.
```
./autogen_generate_files.py <filters.yaml> [template-dir] [output-dir]
```

Personalizing the XML File
--------------------------

The auto-generated XML file is quite simple: it just allows to set the filter
properties from the filter's property tab on ParaView. But ParaView has better
widgets that can be used to input property values for a filter. If you find a
better widget to use for your filter GUI you can use the following workflow to
personalize your XML file:

1. Add your filter to [`autogen/filters.yaml`](https://github.com/Ellon/PointMatcherPlugin/blob/master/autogen/filters.yaml);
2. Build the plugin with auto-generation enabled;
3. *Move* the generated XML file to [xml/](https://github.com/Ellon/PointMatcherPlugin/tree/master/xml);
4. Remove or comment out the `xml_info` section of your filter from the yaml file;
5. Edit the generated XML file to use some awesome widgets;
6. Add the path to the XML file to the servermanager_xml list on [CMakeLists.txt](https://github.com/Ellon/PointMatcherPlugin/blob/master/CMakeLists.txt#L60);
7. Recompile the plugin.

You can find some examples of how to personalize your XML file in the 
[ParaView Plugin HowTo](http://www.paraview.org/Wiki/ParaView/Plugin_HowTo#Adding_GUI_Parameters).
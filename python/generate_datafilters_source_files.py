#!/usr/bin/env python
import sys
import yaml
from mako.template import Template

# Load filters config file as dict
filters_config_filename = sys.argv[1]
with open(filters_config_filename, 'r') as stream:
    filter_dict = yaml.load(stream)

# Read templates
header_template = Template(filename="vtkPM___DataPointsFilter.h.template")
cxx_template = Template(filename="vtkPM___DataPointsFilter.cxx.template")

# Generate files from template
for filterName in filter_dict:
	rendered_header = header_template.render(filterName=filterName,filterProperties=filter_dict[filterName])
	rendered_cxx = cxx_template.render(filterName=filterName,filterProperties=filter_dict[filterName])

	with open("vtkPM" + filterName + ".h",'w') as output_file:
		output_file.write(rendered_header)

	with open("vtkPM" + filterName + ".cxx",'w') as output_file:
		output_file.write(rendered_cxx)

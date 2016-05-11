#!/usr/bin/env python
# usage: ./generate_datafilters_source_files.py <filters.yaml> [template-dir] [output-dir]
import sys
import os
import yaml
from mako.template import Template

# Load filters config file as dict
filters_config_filename = sys.argv[1]
with open(filters_config_filename, 'r') as stream:
    filter_dict = yaml.load(stream)

# Define template dir (optional argument)
if len(sys.argv) < 3:
	template_dir = '.'
else:
	template_dir = sys.argv[2]

# Define output dir (optional argument)
if len(sys.argv) < 4:
	output_dir = '.'
else:
	output_dir = sys.argv[3]

# Read templates
header_template = Template(filename=os.path.join(template_dir,"vtkPM___DataPointsFilter.h.template"))
cxx_template = Template(filename=os.path.join(template_dir,"vtkPM___DataPointsFilter.cxx.template"))
xml_template = Template(filename=os.path.join(template_dir,"vtkPM___DataPointsFilter.xml.template"))

# Generate files from template
for filterName in filter_dict:
	rendered_header = header_template.render(filterName=filterName,filterProperties=filter_dict[filterName]['properties'])
	with open(os.path.join(output_dir,"vtkPM" + filterName + ".h"),'w') as output_file:
		output_file.write(rendered_header)

	rendered_cxx = cxx_template.render(filterName=filterName,filterProperties=filter_dict[filterName]['properties'])
	with open(os.path.join(output_dir,"vtkPM" + filterName + ".cxx"),'w') as output_file:
		output_file.write(rendered_cxx)

	if 'xml_info' in filter_dict[filterName]:
		rendered_xml = xml_template.render(filterName=filterName,filterProperties=filter_dict[filterName]['properties'],filterXmlInfo=filter_dict[filterName]['xml_info'])
		with open(os.path.join(output_dir,"vtkPM" + filterName + ".xml"),'w') as output_file:
			output_file.write(rendered_xml)

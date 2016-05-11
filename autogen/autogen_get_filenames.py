#!/usr/bin/env python
# usage: ./generate_datafilters_source_files.py <filters.yaml> <h|cxx|xml> [base-directory]
from __future__ import print_function
import sys
import os
import yaml


# Load filters config file as dict
filters_config_filename = sys.argv[1]
with open(filters_config_filename, 'r') as stream:
    filter_dict = yaml.load(stream)

# Get extension from arguments
extension = sys.argv[2]

# Get base directory if supplied
if len(sys.argv) < 4:
	base_dir = ''
else:
	base_dir = sys.argv[3]

filenames = []
for filterName in filter_dict:
	if (extension == 'xml' and 'xml_info' in filter_dict[filterName]) or extension != 'xml':
		filenames.append(os.path.join(base_dir,'vtkPM'+filterName+'.'+extension))

print(';'.join(filenames), end="")
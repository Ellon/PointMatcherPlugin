#!/usr/bin/env python
# usage: ./autogen_generate_yaml [<handwritten_xmls_dir> [<output> [<list_from_pmicp>] ] ]
# if the input/output parameter is not supplied the standard input/output is used instead.

import sys
import re
import yaml
import os

# Parse arguments
if len(sys.argv) < 2:
	xml_list = []
else:
	xml_list = []
	for xml in os.listdir(sys.argv[1]):
		xml_list.append(xml[5:-4])

if len(sys.argv) < 3:
	output_file = ''
else:
	output_file = sys.argv[2]

if len(sys.argv) < 4:
	text = sys.stdin.read()
else:
	with open(sys.argv[3],'r') as f:
		text = f.read()

# Define RE's used 
datafilters = re.compile('\*\ DataPointsFilter\ \*\n\n([^*]+)\n\n\*\ Matcher\ \*')
datafilter = re.compile('(\w+\n[^\n]+\n(?:-\ [^\n]+\n)*)')
prop_name_and_default = re.compile('- (\w+) \(default: ((?:[-+]?[0-9]*\.?[0-9]+(?:[eE][-+]?[0-9]+)?)|\w+)\) - ')
prop_min_max = re.compile('- min: ((?:[-+]?[0-9]*\.?[0-9]+(?:[eE][-+]?[0-9]+)?)|\w+) - max: ((?:[-+]?[0-9]*\.?[0-9]+(?:[eE][-+]?[0-9]+)?)|\w+)')

# extract part of the listing that contains datafilters
datafilters_text = datafilters.search(text).group(1)

# put each filter text as a list element
datafilter_text_list = datafilter.findall(datafilters_text)

# Build the filter dictionary
filters_dict = {}
for datafilter_text in datafilter_text_list:
	datafilter_text = datafilter_text.splitlines()
	filter_name = datafilter_text[0]
	filter_help = datafilter_text[1]
	filters_dict[filter_name] = {'properties': []}
	if filter_name not in xml_list:
		filters_dict[filter_name]['xml_info'] = {}
		filters_dict[filter_name]['xml_info']['short_help'] = re.sub('[<>]', '', filter_help)
		filters_dict[filter_name]['xml_info']['long_help'] = re.sub('[<>]', '', filter_help)

	for property_text in datafilter_text[2:]:
		m = prop_name_and_default.match(property_text)
		prop_name, prop_default = m.groups()
		# Oh noooo... just realized pmicp -l does not print information about
		# the property type :-(((
		prop_list = [prop_name, '<???>', prop_default]
		m = prop_min_max.search(property_text)
		if m:
			prop_min, prop_max = m.groups()
			prop_list.extend([prop_min,prop_max])
		filters_dict[filter_name]['properties'].append(prop_list)
		# We're trying to guess the good value here.
		# prop_list = [prop_name, ??prop_type??, prop_default]
		if len(prop_list) == 3:
			try:
				prop_default = int(prop_list[2])
				# If we got here, the type is int, because the propert has no min/max
				prop_list[1] = 'int'
				prop_list[2] = prop_default
			except ValueError:
				try:
					# cast to int didn't work. Try float.
					# if prop_default != '-inf' and prop_default != 'inf':
					# 	prop_default = float(prop_list[2])
					prop_default = float(prop_list[2])
					# If we got here, the type is double.
					prop_list[1] = 'double'
					prop_list[2] = prop_default
				except ValueError:
					# cast to float also didn't work. This should be a string
					prop_list[1] = 'string'
		else:
			try:
				prop_default = int(prop_list[2])
				prop_min = int(prop_list[3])
				prop_max = int(prop_list[4])
				# If we got here, the type is bool, int or insigned
				if prop_min == 0 and prop_max == 1:
					prop_list[1] = 'bool'
				elif prop_min < 0 or prop_max < 0:
					prop_list[1] = 'int'
				else:
					prop_list[1] = 'unsigned'
				prop_list[2] = prop_default
				prop_list[3] = prop_min
				prop_list[4] = prop_max
			except ValueError:
				try:
					# cast to int didn't work. Try float.
					# prop_default = float(prop_list[2])
					# if prop_min != '-inf' and prop_min != 'inf':
					# 	prop_min = float(prop_list[3])
					# if prop_max != '-inf' and prop_max != 'inf':
					# 	prop_max = float(prop_list[4])

					prop_default = float(prop_list[2])
					prop_min = float(prop_list[3])
					prop_max = float(prop_list[4])

					# If we got here, the type is double.
					prop_list[1] = 'double'
					prop_list[2] = prop_default
					prop_list[3] = prop_min
					prop_list[4] = prop_max
				except ValueError:
					# cast to float also didn't work. Check for inf
					# if prop_default == '-inf' or prop_default == 'inf' or \
					#    prop_min == '-inf' or prop_min == 'inf' or \
					#    prop_max == '-inf' or prop_max == 'inf':
					# 	# No way to tell the type :-(. Go with double
					# 	prop_list[1] = 'double'
					# else:
					# 	# This should be a string
					# 	prop_list[1] = 'string'
					prop_list[1] = 'string'


# Save dictionary as yaml file
if output_file:
	with open(output_file,'w') as stream:
		yaml.dump(filters_dict, stream)
else:
	sys.stdout.write(yaml.dump(filters_dict))

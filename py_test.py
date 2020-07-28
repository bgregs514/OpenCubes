import requests
import json

def get_config():
	configs = {};
	config = open("cubes.conf");
	for line in config:
		if ('=' in line):
			key, val = line.split("=");
			configs[key] = val.rstrip();
	return configs;

def create_url(param, configs):
	URL = "http://" + configs["IP"] + ":" + configs["PORT"] + "/api/v1/" + param + "?$select=Name";
	return URL;

def get_names(param):
	configs = get_config();
	URL = create_url(param, configs);
	response = requests.get(url = URL, auth=(configs["USER"], configs["PASSWORD"]));
	data_json = response.json();
	d = data_json["value"];
	name_list = [];
	for item in d:
		name_list.append(item["Name"]);
		#print(item);
	return name_list;

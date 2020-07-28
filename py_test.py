import requests
import json

# sample URL for reference
#URL = "http://192.168.0.17:8880/api/v1/" + param + "?$select=Name";

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
#get_cubes("Cubes");

def get_proc():
	URL = "http://192.168.0.17:8880/api/v1/Processes?$select=Name";

	response = requests.get(url = URL, auth=("ben", ""));
	data_json = response.json();
	d = data_json["value"];
	name_list = [];
	for proc in d:
		name_list.append(proc["Name"]);
	return name_list;

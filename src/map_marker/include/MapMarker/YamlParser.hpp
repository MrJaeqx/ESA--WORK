
#ifndef YAML_PARSER_H
#define YAML_PARSER_H

#include <string>
#include <vector>
#include <ros/ros.h>

extern "C" {
#include "yaml.h"
}

typedef struct{
	std::string key;
	std::vector<std::string> data;
}KeyDataPair;

class YamlParser
{
private:
	std::vector<KeyDataPair> parsedYaml;
	void parseData(yaml_parser_t parser, yaml_token_t token, yaml_token_type_t previousType, KeyDataPair* k);
public:
	YamlParser();
	void loadYaml(std::string fileName);
	void printYaml(std::string fileName);
	std::vector<KeyDataPair> GetParsedYaml();
};

#endif // YAML_PARSER_H


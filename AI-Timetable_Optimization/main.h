#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;
//https://github.com/nlohmann/json


void jsonUseExample(json& lessons, json& teachers, std::map<int, std::string>& lessonsIDs);
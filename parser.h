#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "ham.h"

bool Parse(std::vector<std::string> args);
bool Parse(int argc, char** argv);
void arch_name_and_files_find(std::vector<std::string> args, int& i, std::string& arch_name, std::vector<std::string>& files);
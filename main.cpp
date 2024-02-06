#include <functional>
#include <iostream>
#include <numeric>
#include "ham.h"
#include "parser.h"
#include <fstream>

int main(int argc, char** argv) {
  setlocale(LC_ALL, "ru");
  Parse(argc, argv);
//  std::string aname = "A.txt";
//  std::string papa = "papa.txt";
//  std::string mama = "papa.txt";
//  std::vector<std::string> g;
//////
//  g.push_back(papa);
//  g.push_back(mama);
//  ham::haming::create_arch(g, aname);
//  ham::haming::append_file_to_arch(mama, aname);
//  ham::haming::delete_file_from_arch(mama, aname);
  return 0;
}

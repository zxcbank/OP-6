#include "parser.h"


bool Parse(std::vector<std::string> args) {
  int i = 0;
  while (i < args.size()) {
    if (args[i] == "--create" || args[i] == "-c"){
      std::string arch_name;
      std::vector<std::string> files;
//      while (i < args.size() && (args[i].find("-c") != args[i].npos || args[i].find("-l") != args[i].npos) || args[i].find("-e") != args[i].npos || args[i].find("-a") != args[i].npos || args[i].find("-d") != args[i].npos || args[i].find("-x") != args[i].npos || args[i].find("-A") != args[i].npos)
      i++;
      arch_name_and_files_find(args, i, arch_name, files);
      ham::haming::create_arch(files, arch_name);
    } else if (args[i] == "--extract" || args[i] == "-x"){
      std::string arch_name;
      std::vector<std::string> files;
      i++;
      arch_name_and_files_find(args, i, arch_name, files);
      if (files.size() != 0)
        ham::haming::extract_arch(files, arch_name);
      else
        ham::haming::extract_arch(arch_name);
    } else if (args[i].substr(0, 3) == "--c" || args[i] == "-A") {
      std::string arch_name;
      std::vector<std::string> files;
      i++;
      arch_name_and_files_find(args, i, arch_name, files);
      ham::haming::concat(files[0], files[1], arch_name);
    } else if (args[i] == "-l" || args[i] == "--list"){
      std::string arch_name;
      i++;
      if (args[i] == "-f"){
        i++;
        arch_name = args[i];
      } else {
        arch_name = std::string_view(args[i]).substr(7, args[i].length()-7);
      }
      ham::haming::show_list_arch(arch_name);
    } else if (args[i] == "-d" || args[i].substr(0, 3) == "--d") {
      std::string arch_name;
      std::string del_file;
      i++;
      while (i < args.size()){
        if (args[i] == "-f"){
          i++;
          arch_name = args[i];
        } else if (args[i].substr(0, 3) == "--f") {
          arch_name = std::string_view(args[i]).substr(7, args[i].length()-7);
        } else {
          del_file = args[i];
          i++;
        }
      }

      ham::haming::delete_file_from_arch(del_file, arch_name);
    } else if (args[i] == "-a" || args[i].substr(0, 3) == "--a") {
      std::string arch_name;
      std::string app_file;
      i++;
      while (i < args.size()){
        if (args[i] == "-f"){
          i++;
          arch_name = args[i];
        } else if (args[i].substr(0, 3) == "--f") {
          arch_name = std::string_view(args[i]).substr(7, args[i].length()-7);
        } else {
          app_file = args[i];
          i++;
        }
      }
      ham::haming::append_file_to_arch(app_file, arch_name);
    }
    i++;
  }
  return true;
}
bool Parse(int argc, char** argv) {
  std::vector<std::string> params;
  for (int i = 1; i < argc; i++){
    params.push_back(std::string(argv[i]));
  }

  return Parse(params);
}

void arch_name_and_files_find(std::vector<std::string> args, int& i, std::string& arch_name, std::vector<std::string>& files){
  while (i < args.size()){
    if (args[i] == "-f"){
      i++;
      arch_name = args[i];
      i++;
    }
    else if (std::string_view(args[i]).substr(0, 6) == "--file="){
      arch_name = std::string_view(args[i]).substr(7, args[i].length()-7);
    } else {
      files.push_back(args[i]);
      i++;
    }
  }
}
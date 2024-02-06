
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <cmath>
#include <filesystem>

#ifndef LABWORK4_BIN_HAM_H_
#define LABWORK4_BIN_HAM_H_

namespace ham {
  class haming{
   private:

    std::vector <std::string> names;
    std::string final_name;
    static std::bitset<12> byte_ham(std::bitset<8>& data);
    static std::bitset<8> byte_reham(std::bitset<12>& encoded);
    static unsigned char Reverse(unsigned char byte);

    class file {
      private:
        static int get_digit(const std::string& header, size_t& index_of_size, char sep);
     public:
        std::string name;
        int offset_tila;
        int encoded_size;
        file(const std::string& header,const std::string& filename);

    };

   public:
    static void create_arch(std::vector<std::string>& user_files_names_, std::string& archive_name_);
    static std::vector<std::string> list_arch(std::string& arch_name_);
    static void show_list_arch(std::string& arch_name_);
    static void extract_arch(std::vector<std::string>& files_in_arch_, std::string& arch_file_name);
    static void extract_arch(std::string& archive_name_);
    static void append_file_to_arch(std::string& file_name_, std::string& arch_name_);
    static void delete_file_from_arch(std::string& file_name_, std::string& arch_name_);
    static void concat(std::string& file_name_1_, std::string& file_name_2_, std::string& final_file_name);
  };

};


#endif //LABWORK4_BIN_HAM_H_

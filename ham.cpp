//
// Created by Святослав on 09.01.2024.
//

#include "ham.h"

using namespace ham;

void haming::create_arch(std::vector<std::string>& user_files_names_, std::string& archive_name_) {
  int greate_size = 0;
  std::string header;
  std::ofstream final_file(archive_name_, std::ios::binary);
  for (const std::string& fn : user_files_names_){
    header += fn;
    std::ifstream tmp(fn);
    int size;
    tmp.seekg(0, std::ios::end);
    size = ceil(tmp.tellg() * 1.5);
    header += "_";
    header += std::to_string(size); // name_size_shift
    header += "_";
    header +=  std::to_string((int)ceil(greate_size) + 1);
    header += "|";
    greate_size += (size);
  }
  final_file << header << "`";
  for (const std::string& fn : user_files_names_) {
    std::ifstream archiving_file(fn, std::ios::binary);
    if (!archiving_file.is_open()){
      std::cout << "Archiving file " << fn << " is not open.";
      return;
    }
    char ch;
    while (archiving_file.get(ch)) {
      std::bitset<8> coding_byte(ch);
      const std::bitset<12> coded_byte1 = byte_ham(coding_byte);
      if (archiving_file.get(ch)) {
        std::bitset<8> coding_byte2(ch);
        const std::bitset<12> coded_byte2 = byte_ham(coding_byte2);
        unsigned char ch1 = 0, ch2 = 0, ch3 = 0;
        for (int i = 0; i < 8; i++){
          ch1 <<= 1;
          ch1 |= coded_byte1[i] & 1;
        }
        for (int i = 0; i < 4; i++){
          ch2 <<= 1;
          ch2 |= coded_byte1[i + 8] & 1;
        }
        for (int i = 0; i < 4; i++){
          ch2 <<= 1;
          ch2 |= coded_byte2[i] & 1;
        }
        for (int i = 0; i < 8; i++){
          ch3 <<= 1;
          ch3 |= coded_byte2[i + 4] & 1;
        }
        final_file << ch1 << ch2 << ch3;
      } else {
        unsigned char ch1 = 0, ch2 = 0;
        for (int i = 0; i < 8; i++){
          ch1 <<= 1;
          ch1 |= coded_byte1[i] & 1;
        }
        for (int i = 0; i < 4; i++){
          ch2 <<= 1;
          ch2 |= coded_byte1[i+8] & 1;
        }
        ch2 <<= 4;
        final_file << (ch1) << (ch2);
      }
    }
  }
}

void haming::extract_arch(std::vector<std::string>& files_in_arch_, std::string& arch_file_name){
  std::ifstream arch(arch_file_name,std::ios::binary);
  std::vector<file> files;

  arch.seekg(0, std::ios::beg);
  std::string header;
  char ch;

  while (arch.get(ch) && ch != '`'){
    header += ch;
  }
  int begin = arch.tellg();
  for (const std::string& fn : files_in_arch_){
      std::ofstream output("_generated_" + fn, std::ios::binary);
      file tmp(header, fn);
      arch.seekg(begin + tmp.offset_tila-1, std::ios::beg);
      while (arch.get(ch) && tmp.encoded_size > 0){
        std::bitset<12> first, second;
         unsigned char ch1, ch2, ch3;
        ch1 = (ch);
        if (arch.get(ch)) {
          ch2 = (ch);
          tmp.encoded_size-=2;
          if (!arch.eof() && arch.get(ch) && tmp.encoded_size > 0) {
            ch3 = (ch);
            tmp.encoded_size--;
            for (int i = 0; i < 8; i++){
              first[i] = ch1 & (1 << (7-i));
            }
            for (int i = 0; i < 4; i++){
              first[i+8] = ch2 & (1 << (7 - i));
              second[i] = ch2 & (1 << (3 - i));
            }
            for (int i = 0; i < 8; i++){
              second[i+4] = ch3 & (1 << (7 - i));
            }
            std::bitset<8> byte1 = byte_reham(first);
            std::bitset<8> byte2 = byte_reham(second);
            ch1 = 0, ch2 = 0;
            for (int i = 0; i < 8; i++){
              ch1 <<= 1;
              ch1 |= byte1[i];
              ch2 <<= 1;
              ch2 |= byte2[i];
            }
            output << Reverse(ch1);
            output << Reverse(ch2);
          } else {
            for (int i = 0; i < 8; i++){
              first[i] = ch1 & (1 << (7-i));
            }
            for (int i = 0; i < 4; i++){
              first[i+8] = ch2 & (1 << (7-i));
            }
            std::bitset<8> byte1 = byte_reham(first);
            ch1 = 0;
            for (int i = 0; i < 8; i++){
              ch1 <<= 1;
              ch1 |= byte1[i];
            }
            output << Reverse(ch1);
          }
        }

      }
  }
}

void haming::extract_arch(std::string& archive_name_) {
  std::vector<std::string> tmp = list_arch(archive_name_);
  extract_arch(tmp, archive_name_);
}

unsigned char haming::Reverse(unsigned char byte){
  unsigned char new_byte = 0;
  int base = 256;
  while (byte != 0){
    new_byte += (byte & 1) * (base >>= 1);
    byte >>= 1;
  }
  return new_byte;
}

std::bitset<12> haming::byte_ham(std::bitset<8>& data) {
  std::bitset<12> coded_byte;
  coded_byte[0] = false;
  coded_byte[1] = false;
  coded_byte[2] = data[0];
  coded_byte[3] = false;
  coded_byte[4] = data[1];
  coded_byte[5] = data[2];
  coded_byte[6] = data[3];
  coded_byte[7] = false;
  coded_byte[8] = data[4];
  coded_byte[9] = data[5];
  coded_byte[10] = data[6];
  coded_byte[11] = data[7];
  coded_byte[0] = coded_byte[0] ^ coded_byte[2] ^ coded_byte[4] ^ coded_byte[6] ^ coded_byte[8] ^ coded_byte[10];
  coded_byte[1] = coded_byte[1] ^ coded_byte[2] ^ coded_byte[5] ^ coded_byte[6] ^ coded_byte[9] ^ coded_byte[10];
  coded_byte[3] = coded_byte[3] ^ coded_byte[4] ^ coded_byte[5] ^ coded_byte[6] ^ coded_byte[11];
  coded_byte[7] = coded_byte[7] ^ coded_byte[8] ^ coded_byte[9] ^ coded_byte[10] ^ coded_byte[11];
  return coded_byte;
}

std::bitset<8> haming::byte_reham(std::bitset<12>& encoded) {
  std::bitset<8> decoded_byte;
  bool c0 = (encoded[0] ^ encoded[2] ^ encoded[4] ^ encoded[6] ^ encoded[8] ^ encoded[10]);
  bool c1 = (encoded[1] ^ encoded[2] ^ encoded[5] ^ encoded[6] ^ encoded[9] ^ encoded[10]);
  bool c3 = (encoded[3] ^ encoded[4] ^ encoded[5] ^ encoded[6] ^ encoded[11]);
  bool c7 = (encoded[7] ^ encoded[8] ^ encoded[9] ^ encoded[10] ^ encoded[11]);

  int error_position = c0 + c1 * 2 + c3 * 4 + c7 * 8;

  if (error_position < 0 || error_position > 12) {
//    std::cout << "Too much bits loss.\n";
//    std::cout << encoded << "\n";
    return decoded_byte;
  }
  if (error_position != 0) {
//    std::cout << "loss corrected\n";
    encoded.flip(error_position-1);
  }
  decoded_byte[0] = encoded[2];
  decoded_byte[1] = encoded[4];
  decoded_byte[2] = encoded[5];
  decoded_byte[3] = encoded[6];
  decoded_byte[4] = encoded[8];
  decoded_byte[5] = encoded[9];
  decoded_byte[6] = encoded[10];
  decoded_byte[7] = encoded[11];

  return decoded_byte;
}
std::vector<std::string> haming::list_arch(std::string& arch_name_) {
  std::ifstream f(arch_name_, std::ios::binary);
  std::vector<std::string>ans;
  char ch;

  while (f.get(ch) && ch != '`'){
    std::string tmp;
    tmp += ch;
    while (f.get(ch) && ch != '_' && ch != '`')
      tmp += ch;
    ans.push_back(tmp);
    while (ch != '|' && ch != '`')
      f.get(ch);
  }
  return ans;
}
void haming::show_list_arch(std::string& arch_name_) {
  std::vector<std::string> tmp = list_arch(arch_name_);
  for (std::string& a : tmp)
    std::cout << a << "\n";
}

void haming::delete_file_from_arch(std::string& file_name_, std::string& arch_name_) {
    std::ifstream arch(arch_name_, std::ios::binary);
    std::string header, new_header;
    char ch;

    std::vector<std::string> files_in_arch = list_arch(arch_name_);
    while (arch.get(ch) && ch != '`'){
      header += ch;
    }

    int begin = arch.tellg();
    file del_file(header, file_name_);
    int greate_size = 0;
    std::ofstream final_file("1" + arch_name_, std::ios::binary);
    for (const std::string& fn : files_in_arch){
      if (fn != file_name_){
        new_header += fn;
        file tmp(header, fn);
        new_header += "_";
        new_header += std::to_string(tmp.encoded_size); // name_size_shift
        new_header += "_";
        new_header +=  std::to_string((int)ceil(greate_size) + 1);
        new_header += "|";
        greate_size += (tmp.encoded_size);
      }
    }
    final_file << new_header << "`";
  for (const std::string& fn : files_in_arch) {
    if (fn != file_name_) {
      file tmp(header, fn);
      arch.seekg(begin + tmp.offset_tila-1, std::ios::beg);
      while (arch.get(ch) && tmp.encoded_size > 0){
        final_file << ch;
      }
    }
  }

  arch.close();
  final_file.close();
  std::filesystem::remove(arch_name_);
  std::filesystem::rename("1" + arch_name_, arch_name_);
}

void haming::append_file_to_arch(std::string& file_name_, std::string& arch_name_) {
  std::ifstream arch(arch_name_, std::ios::binary);
  std::string header, new_header;
  char cht;

  std::vector<std::string> files_in_arch = list_arch(arch_name_);
  files_in_arch.push_back(file_name_);

  while (arch.get(cht) && cht != '`'){
    header += cht;
  }
  int begin = arch.tellg();

  int greate_size = 0;

  std::ofstream final_file("1" + arch_name_, std::ios::binary);
  for (const std::string& fn : files_in_arch){
    if (fn != file_name_){
      new_header += fn;
      file tmp(header, fn);
      new_header += "_";
      new_header += std::to_string(tmp.encoded_size); // name_size_shift
      new_header += "_";
      new_header +=  std::to_string((int)ceil(greate_size) + 1);
      new_header += "|";
      greate_size += (tmp.encoded_size);
    } else {
      new_header += file_name_;
      std::ifstream tmp(file_name_);
      int size;
      tmp.seekg(0, std::ios::end);
      size = ceil(tmp.tellg() * 1.5);
      new_header += "_";
      new_header += std::to_string(size); // name_size_shift
      new_header += "_";
      new_header +=  std::to_string((int)ceil(greate_size) + 1);
      new_header += "|";
      greate_size += (size);
    }
  }
  final_file << new_header << "`";

  for (const std::string& fn : files_in_arch) {
    if (fn != file_name_) {
      file tmp(header, fn);
      arch.seekg(begin + tmp.offset_tila-1, std::ios::beg);
      while (arch.get(cht) && tmp.encoded_size > 0){
        final_file << cht;
      }
    } else {
      char ch;
      std::ifstream archiving_file(file_name_);
      while (archiving_file.get(ch)) {
        std::bitset<8> coding_byte(ch);
        const std::bitset<12> coded_byte1 = byte_ham(coding_byte);
        if (archiving_file.get(ch)) {
          std::bitset<8> coding_byte2(ch);
          const std::bitset<12> coded_byte2 = byte_ham(coding_byte2);
          unsigned char ch1 = 0, ch2 = 0, ch3 = 0;
          for (int i = 0; i < 8; i++){
            ch1 <<= 1;
            ch1 |= coded_byte1[i] & 1;
          }
          for (int i = 0; i < 4; i++){
            ch2 <<= 1;
            ch2 |= coded_byte1[i + 8] & 1;
          }
          for (int i = 0; i < 4; i++){
            ch2 <<= 1;
            ch2 |= coded_byte2[i] & 1;
          }
          for (int i = 0; i < 8; i++){
            ch3 <<= 1;
            ch3 |= coded_byte2[i + 4] & 1;
          }
          final_file << ch1 << ch2 << ch3;
        } else {
          unsigned char ch1 = 0, ch2 = 0;
          for (int i = 0; i < 8; i++){
            ch1 <<= 1;
            ch1 |= coded_byte1[i] & 1;
          }
          for (int i = 0; i < 4; i++){
            ch2 <<= 1;
            ch2 |= coded_byte1[i+8] & 1;
          }
          ch2 <<= 4;
          final_file << (ch1) << (ch2);
        }
      }
    }
  }
  arch.close();
  final_file.close();
  std::filesystem::remove(arch_name_);

  std::filesystem::rename("1" + arch_name_, arch_name_);
}
void haming::concat(std::string& file_name_1_, std::string& file_name_2_, std::string& final_file_name) {
    std::ifstream a1(file_name_1_, std::ios::binary);
    std::ifstream a2(file_name_2_, std::ios::binary);
    std::vector<std::string> files_in_arch1 = list_arch(file_name_1_);
    std::vector<std::string> files_in_arch2 = list_arch(file_name_2_);
    std::ofstream final_file(final_file_name, std::ios::binary);

    std::string header1, header2, new_header;
    char ch;

    while (a1.get(ch) && ch != '`'){
      header1 += ch;
    }
    int begin1 = a1.tellg();
    while (a2.get(ch) && ch != '`'){
      header2 += ch;
    }
    int begin2 = a2.tellg();
    int greate_size = 0;
    for (const std::string& fn : files_in_arch1){
        new_header += fn;
        file tmp(header1, fn);
        new_header += "_";
        new_header += std::to_string(tmp.encoded_size); // name_size_shift
        new_header += "_";
        new_header +=  std::to_string((int)ceil(greate_size) + 1);
        new_header += "|";
        greate_size += (tmp.encoded_size);
    }
    for (const std::string& fn : files_in_arch2){
      new_header += fn;
      file tmp(header2, fn);
      new_header += "_";
      new_header += std::to_string(tmp.encoded_size); // name_size_shift
      new_header += "_";
      new_header +=  std::to_string((int)ceil(greate_size) + 1);
      new_header += "|";
      greate_size += (tmp.encoded_size);
    }
    final_file << new_header << "`";

    char tmp_ch;
    for (const std::string& fn : files_in_arch1) {
      file tmp(header1, fn);
      a1.seekg(begin1 + tmp.offset_tila - 1, std::ios::beg);
      while (a1.get(tmp_ch) && tmp.encoded_size > 0) {
        final_file << tmp_ch;
      }
    }
    for (const std::string& fn : files_in_arch2) {
      file tmp(header2, fn);
      a1.seekg(begin2 + tmp.offset_tila - 1, std::ios::beg);
      while (a2.get(tmp_ch) && tmp.encoded_size > 0) {
        final_file << tmp_ch;
      }
    }
}

haming::file::file(const std::string& header, const std::string& filename) {
    name = filename;
    size_t index_of_size = header.find(filename) + filename.length() + 1;
    encoded_size = get_digit(header, index_of_size, '_');
    offset_tila = get_digit(header, index_of_size, '|');

}
int haming::file::get_digit(const std::string& header, size_t& index_of_size, char sep) {
  std::string ans;
  while (header[index_of_size] != sep) {
    ans += header[index_of_size];
    index_of_size++;
  }
  index_of_size++;
  return std::stoi(ans);
}
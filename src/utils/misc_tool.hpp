#ifndef TETHYS_SCE_VS_TOOL_HPP
#define TETHYS_SCE_VS_TOOL_HPP

#include <string>
#include <algorithm>
#include <map>
#include <chrono>
#include <cctype>
#include <locale>

#include <botan-2/botan/base58.h>
#include <botan-2/botan/base64.h>
#include <botan-2/botan/exceptn.h>
#include <botan-2/botan/secmem.h>

#include "date.hpp"

class tt {
public:

#if 0
  static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
      return !std::isspace(ch);
    }));
  }

  static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
      return !std::isspace(ch);
    }).base(), s.end());
  }

  static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
  }
#endif

  static std::string& ltrim(std::string& str) {
    const std::string chars = "\t\n\v\f\r ";
    str.erase(0, str.find_first_not_of(chars));
    return str;
  }

  static std::string& rtrim(std::string& str) {
    const std::string chars = "\t\n\v\f\r ";
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
  }

  static std::string& trim(std::string& str) {
    return ltrim(rtrim(str));
  }

  static std::string& toLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
  }

  static std::string& toUpper(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
  }

  static std::string toLower(std::string_view str) {
    std::string dst(str);
    std::transform(str.begin(), str.end(), dst.begin(), ::tolower);
    return dst;
  }

  static std::string toUpper(std::string_view str) {
    std::string dst(str);
    std::transform(str.begin(), str.end(), dst.begin(), ::toupper);
    return dst;
  }

  static uint64_t timestr2timestamp(std::string_view time_str, int offset_min = 0) {

  std::istringstream in;
  date::sys_time<std::chrono::milliseconds> time_point;

  std::string iso_time{time_str};

  if(iso_time.empty())
  return 0;

  if(iso_time.back() == 'Z' || iso_time.back() == 'z'){
    iso_time = iso_time.substr(0,iso_time.size() - 1);
  }

  if(iso_time.size() < 10)
    return 0;

  if(iso_time.size() > 10) {
    iso_time[10] = 'T';
  }

  if(iso_time.size() == 10) {
    std::string add_str = "T00:00:00+00:00";
    in.str(iso_time + add_str);
  } else if(iso_time.size() == 19) {
    std::string add_str = "+00:00";
    in.str(iso_time + add_str);
  } else if(iso_time.size() == 25){
    in.str(iso_time);
  } else {
    return 0;
  }

  in >> date::parse("%FT%T%Ez", time_point);

  if (in.fail())
    return 0;

  return static_cast<uint64_t>(time_point.time_since_epoch().count() - offset_min * 60000);

  }

  template <typename T>
  inline static std::string encodeBase64(T &&t) {
    try {
      return Botan::base64_encode(std::vector<uint8_t>(begin(t), end(t)));
    } catch (...) {
      return {};
    }
  }

  template <typename T>
  inline static std::vector<std::byte> decodeBase64(T &&input) {
    try {
      auto s_vector = Botan::base64_decode(input);
      return std::vector<std::byte>(s_vector.begin(), s_vector.end());
    } catch (...) {
      return {};
    }
  }

  template <typename T>
  inline static std::string encodeBase58(T &&t) {
    try {
      return Botan::base58_encode(std::vector<uint8_t>(begin(t), end(t)));
    } catch (...) {
      return {};
    }
  }

  template <typename T>
  inline static std::vector<std::byte> decodeBase58(T &&input) {
    try {
      auto s_vector = Botan::base58_decode(input);
      return std::vector<std::byte>(s_vector.begin(), s_vector.end());
    } catch (...) {
      return {};
    }
  }

  template <typename S1 = std::string, typename S2 = std::string>
  static std::vector<std::string> split (S1 &&str, S2 &&delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = str.find (delimiter, pos_start)) != std::string::npos) {
      token = str.substr (pos_start, pos_end - pos_start);
      pos_start = pos_end + delim_len;
      res.emplace_back(token);
    }

    res.emplace_back(str.substr(pos_start));
    return res;
  }

  template <typename T, typename S = std::string>
  static T str2num (S &&s) {
    T ret_val = 0;
    if(!s.empty()) {
      try{
        ret_val = (T) std::stoll(s);
      }
      catch (...){
        ret_val = 0;
      }
    }

    return ret_val;
  }

  static bool isDigit(std::string_view data) {
    return std::all_of(data.begin(), data.end(), ::isdigit);
  }

  static bool inArray(std::string_view data, const std::vector<std::string_view> &array) {
    for(auto &item : array) {
      if(item == data)
        return true;
    }

    return false;
  }

  static bool inArray(const std::vector<std::string_view> &array, std::string_view data) {
    return inArray(data,array);
  }

};

#endif //GRUUTSCE_VS_TOOL_HPP

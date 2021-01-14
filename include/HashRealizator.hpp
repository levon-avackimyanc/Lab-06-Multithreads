//
// Copyright 2021 Levon Avakimyanc levon.avakimyanc.01@mail.ru
//

#ifndef INCLUDE_MULTITHREADS_HASHREALIZATOR_HPP
#define INCLUDE_MULTITHREADS_HASHREALIZATOR_HPP
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

using json = nlohmann::json;
const char LastHashPos[] = "0000";
const size_t SizeOfPos = 4;
void SetLogs();

class JsonPrinter {
 public:
  void NewHash(const std::string& SomeStr, const std::string& hash,
               std::time_t timestamp);
  friend std::ostream& operator<<(std::ostream&, const JsonPrinter&);
  json GetJson() const { return J_Arr; };
  std::mutex& GetMut() const { return mut; };

 private:
  json J_Arr;
  mutable std::mutex mut;
};
#endif  // INCLUDE_MULTITHREADS_HASHREALIZATOR_HPP

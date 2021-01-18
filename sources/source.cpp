//
// Copyright 2021 Levon Avakimyanc levon.avakimyanc.01@mail.ru
//
#include <picosha2.h>

#include <atomic>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <thread>

#include "HashRealizator.hpp"

std::atomic<bool> WorksContinue = true;
std::atomic<bool> DoJson = false;

void Stop(int S) {
  if (S == SIGINT) {
    WorksContinue = false;
  }
}

void HashAlg(JsonPrinter& JP) {
  while (WorksContinue) {
    std::string randStr = std::to_string(std::rand());
    std::string hash = picosha2::hash256_hex_string(randStr);
    std::time_t timestamp(std::time(nullptr));
    std::string lastSymb = hash.substr(hash.size() - SizeOfPos);
    if (lastSymb == LastHashPos) {
      BOOST_LOG_TRIVIAL(info) << "0000 founded in hash '" << hash
                              << "' of string '" << randStr << "'";
      if (DoJson) {
        JP.NewHash(randStr, hash, timestamp);
      }
    } else {
      BOOST_LOG_TRIVIAL(trace)
          << "Hash:" << hash << " From string:" << randStr << ";";
    }
  }
}
void Hashing(const int& argc, char* argv[]) {
  unsigned int ThreadsNumb;
  std::string j_path;
  std::srand(time(nullptr));
  switch (argc) {
    case 1:
      ThreadsNumb = std::thread::hardware_concurrency();
      break;
    case 2:
      ThreadsNumb = std::atoi(argv[1]);
      if (ThreadsNumb == 0 ||
          ThreadsNumb > std::thread::hardware_concurrency()) {
        throw std::out_of_range(" Wrong number of threads!");
      }
      break;
    case 3:
      ThreadsNumb = std::atoi(argv[1]);
      if (ThreadsNumb == 0 ||
          ThreadsNumb > std::thread::hardware_concurrency()) {
        throw std::out_of_range(" Wrong number of threads!");
      }
      j_path = argv[2];
      DoJson = true;
      break;
    default:
      throw std::out_of_range(" Wrong number of arguments!");
  }
  SetLogs();
  boost::log::add_common_attributes();
  std::vector<std::thread> ThreadsArr;
  JsonPrinter JP;
  ThreadsArr.reserve(ThreadsNumb);
  std::signal(SIGINT, Stop);

  for (size_t i = 0; i < ThreadsNumb; ++i) {
    ThreadsArr.emplace_back(HashAlg, std::ref(JP));
  }
  for (auto& thread : ThreadsArr) {
    thread.join();
  }
  if (DoJson) {
    std::ofstream fileout{j_path};
    fileout << JP;
  }
}

int main(int argc, char* argv[]) {
  Hashing(argc, argv);
  return 0;
}

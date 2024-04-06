#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include "clerk.h"
#include "util.h"

std::vector<std::string> split(std::string& str, char delimiter) {
  std::vector<std::string> tokens;
  std::istringstream iss(str);
  std::string token;
  while (std::getline(iss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

int main() {
  Clerk client;
  client.Init("test.conf");
  // auto start = now();
  int count = 100;
  int tmp = count;

  auto start = std::chrono::high_resolution_clock::now();

  // std::random_device rd;
  // std::mt19937 gen(rd());
  // int minNumber = 1;
  // int maxNumber = 100;
  // std::uniform_int_distribution<int> dis(minNumber, maxNumber);
  // while (tmp--) {
  //   int randomNumber = dis(gen);
  //   client.Put(std::to_string(randomNumber), "1");
  //   std::cout << tmp << std::endl;
  //   // std::string get1 = client.Get("x");
  //   // std::printf("get return :{%s}\r\n", get1.c_str());
  // }

  while (true) {
    std::string op;
    getline(cin, op);
    std::vector<std::string> ops = split(op, ' ');
    if (ops[0] == "set") {
      client.Put(ops[1], ops[2]);
    } else if (ops[0] == "get") {
      std::string rtn = client.Get(ops[1]);
      std::cout << ops[1] << ": " << rtn << std::endl;
    } else if (ops[0] == "exit") {
      break;
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> usetime = end - start;
  cout << std::chrono::duration<double, std::milli>(end - start).count();

  return 0;
}
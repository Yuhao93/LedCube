#include "flags.h"
#include <map>
#include <stdio.h>
#include <string>
#include <utility>

FlagException duplicateFlagException("Flag with same name was already added.");
FlagException requiredFlagNotFoundException(
    "At least one required flag not found.");

std::map<std::string, Flags::Flag*> Flags::mappingReq;
std::map<std::string, Flags::Flag*> Flags::mappingNoReq;

std::string Flags::Flag::get() {
  return val;
}

void Flags::Flag::set(std::string value) {
  val = value;
}

Flags::Flag* Flags::registerFlag(std::string name, bool required) {
  if (mappingReq.find(name) != mappingReq.end()
      || mappingNoReq.find(name) != mappingNoReq.end()) {
    throw duplicateFlagException; 
  }

  Flag* flag = new Flag();
  if (required) {
    mappingReq.insert(std::pair<std::string, Flag*>(name, flag));
  } else {
    mappingNoReq.insert(std::pair<std::string, Flag*>(name, flag));
  }

  return flag;
}

void Flags::parseFlags(int argc, char** argv) {
  for (int i = 1; i < argc - 1; i++) {
    std::string arg = std::string(argv[i]);
    if (arg.size() > 1 && arg.at(0) == '-') {
      std::string key = arg.substr(1, arg.size() - 1);
      auto itReq = mappingReq.find(key);
      auto itNoReq = mappingNoReq.find(key);
      if (itReq != mappingReq.end()) {
        itReq->second->set(std::string(argv[++i]));
        mappingReq.erase(itReq);
      } else if (itNoReq != mappingNoReq.end()) {
        itNoReq->second->set(std::string(argv[++i]));
        mappingNoReq.erase(itNoReq);
      }
    }
  }

  if (mappingReq.size() > 0) {
    for (auto it = mappingReq.begin();
        it != mappingReq.end(); it++) {
      printf("\"%s\" Flag not found\n", it->first.c_str());
    }
    throw requiredFlagNotFoundException;
  }
}


#ifndef FLAGS_H
#define FLAGS_H

#include <exception>
#include <map>
#include <string>

class FlagException : public std::exception {
public: 
  FlagException(const char* str) : str(str) { }
  
  virtual const char* what() const throw() {
    return str;
  }

private:
  const char* str;    
};

class Flags {
public:
  class Flag {
    public: 
      std::string get();
      void set(std::string val);
    private:
      std::string val;
  };

  static Flag* registerFlag(std::string name, bool required);
  static void parseFlags(int argc, char** argv);

private:
  Flags() { }
  static std::map<std::string, Flag*> mappingReq;
  static std::map<std::string, Flag*> mappingNoReq;
};

#endif

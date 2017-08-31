#ifndef HYPER_PARAM_H_
#define HYPER_PARAM_H_

#include "log.h"

#include <vector>
#include <string>
#include <map>

#define PARAM_INT 1
#define PARAM_FLOAT 2
#define PARAM_STRING 3

class HyperParam {
 public:
  HyperParam() {
    params_.clear();
    v_int_.reserve(20);
    v_float_.reserve(20);
    v_str_.reserve(20);
  }
  void Load(FILE* file) {
    std::string param_name = read_str(file);
    while (param_name.compare("end") != 0) {
      int type;
      CHECK_EQ(fread(&type, sizeof(int), 1, file), 1); 
      if (type == PARAM_INT) {
        InsertInt(param_name, read_int(file));
      }
      else if (type == PARAM_FLOAT) {
        InsertFloat(param_name, read_float(file));
      }
      else if (type == PARAM_STRING) {
        InsertString(param_name, read_str(file));
      }
      param_name = read_str(file);
    }
  }
  void ToBinaryFile(FILE* file) {
    
  }
  ~HyperParam() {
    params_.clear();
    v_int_.clear();
    v_float_.clear();
    v_str_.clear();
  }
  bool has_param(std::string param_name) {
    return params_.count(param_name) != 0;
  }
  void* param(std::string param_name) {
    if (!has_param(param_name)) {
      LOG(ERROR) << "Param name " << param_name << " not exists.";
    }
    return params_[param_name];
  }
  void InsertInt(const std::string& key, const int value) {
    if (params_.count(key) != 0) {
      LOG(ERROR) << "Param name " << key << " already exists.";
      exit(0);
    }
    v_int_.push_back(value);
    params_[key] = &(v_int_.back());
    LOG(INFO) << key << ": " << value;
  }
  void InsertFloat(const std::string& key, const float value) {
    if (params_.count(key) != 0) {
      LOG(ERROR) << "Param name " << key << " already exists.";
      exit(0);
    }
    v_float_.push_back(value);
    params_[key] = &(v_float_.back());
    LOG(INFO) << key << ": " << value;
  }
  void InsertString(const std::string& key, const std::string& value) {
    if (params_.count(key) != 0) {
      LOG(ERROR) << "Param name " << key << " already exists.";
      exit(0);
    }
    v_str_.push_back(value);
    params_[key] = &(v_str_.back());
    LOG(INFO) << key << ": " << value;
  }
 private:
  std::string read_str(FILE* file) {
    int len;
    CHECK_EQ(fread(&len, sizeof(int), 1, file), 1);
    if (len <= 0) return "";
    char* c_str = new char[len + 1];
    CHECK_EQ(fread(c_str, sizeof(char), len, file), len);
    c_str[len] = '\0';
    std::string str(c_str);
    delete []c_str;
    return str;
  }
  int read_int(FILE* file) {
    int i;
    CHECK_EQ(fread(&i, sizeof(int), 1, file), 1);
    return i;
  }
  float read_float(FILE* file) {
    float f;
    CHECK_EQ(fread(&f, sizeof(int), 1, file), 1);
    return f;
  }
  int num_;
  std::map<std::string, void*> params_;
  std::vector<int> v_int_;
  std::vector<float> v_float_;
  std::vector<std::string> v_str_;
};

#endif //HYPER_PARAM_H_

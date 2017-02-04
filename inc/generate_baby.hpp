#ifndef H_GENERATE_BABY
#define H_GENERATE_BABY

#include <vector>
#include <set>
#include <string>

class Variable{
public:
  Variable():
    type_(""),
    name_(""){
  }

  Variable(const std::string &type,
           const std::string &name):
    type_(type),
    name_(name){
  }

  bool operator<(const Variable& var) const{
    return type_<var.type_ || (type_==var.type_ && name_<var.name_);
  }

  std::string type_, name_;
};

bool Contains(const std::string &text, const std::string &pattern);

void WritePlusHeader(const std::set<Variable> &full_vars, const std::set<Variable> &new_vars);
void WritePlusSource(const std::set<Variable> &full_vars, const std::set<Variable> &new_vars);

void WriteCorrHeader(const std::set<Variable> &corr_vars, const std::set<Variable> &new_vars);
void WriteCorrSource(const std::set<Variable> &corr_vars, const std::set<Variable> &new_vars);

#endif

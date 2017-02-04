#include "generate_baby.hpp"

#include <cstring>

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <set>

#include <unistd.h>

using namespace std;

string ToCaps(string str){
  for(string::iterator it = str.begin();
      it != str.end();
      ++it){
    *it = toupper(*it);
  }
  return str;
}

string execute(const string &cmd){
  FILE *pipe = popen(cmd.c_str(), "r");
  if(!pipe) throw runtime_error("Could not open pipe.");
  const size_t buffer_size = 128;
  char buffer[buffer_size];
  string result = "";
  while(!feof(pipe)){
    if(fgets(buffer, buffer_size, pipe) != NULL) result += buffer;
  }

  pclose(pipe);
  return result;
}

vector<string> Tokenize(const string& input,
                        const string& tokens = " "){
  char* ipt(new char[input.size()+1]);
  memcpy(ipt, input.data(), input.size());
  ipt[input.size()]=static_cast<char>(0);
  char* ptr(strtok(ipt, tokens.c_str()));
  vector<string> output(0);
  while(ptr!=NULL){
    output.push_back(ptr);
    ptr=strtok(NULL, tokens.c_str());
  }
  return output;
}

string FixName(string name){
  //Variable names can have alphanumeric characters and underscores only
  string allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_";

  //Remove illegal characters
  size_t pos = name.find_first_not_of(allowed);
  while(pos < name.size()){
    name.at(pos) = '_';
    pos = name.find_first_not_of(allowed);
  }

  //Replace double underscore with single underscore
  pos = name.rfind("__");
  while(pos < name.size()){
    name.replace(pos, 2, "_");
    pos = name.rfind("__");
  }

  //Remove leading and trailing spaces
  pos = 0;
  for(pos = 0; pos < name.size(); ++pos){
    if(name.at(pos) != ' ') break;
  }
  size_t endpos = name.size();
  for(endpos = name.size(); endpos != 0; --endpos){
    if(name.at(endpos-1) != ' ') break;
  }

  return name.substr(pos, endpos-pos);
}

set<Variable> GetVariables(const string &file_name){
  string allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_";
  set<Variable> vars;

  ifstream infile(("variables/"+file_name).c_str());
  string line;
  while(getline(infile, line)){
    size_t start = line.find_first_not_of(" ");
    if(start >= line.size() || line.at(start) == '#' || line.at(start) == '/') continue;

    //Replace double space with single space
    size_t pos = line.rfind("  ");
    while(pos < line.size()){
      line.replace(pos, 2, " ");
      pos = line.rfind("  ");
    }
    size_t end = line.find_last_of(allowed)+1;
    size_t split = line.rfind(' ', end)+1;

    vars.insert(Variable(line.substr(start, split-start),
                         line.substr(split, end-split)));
  }
  infile.close();

  return vars;
}

int main(){
  set<Variable> corr_vars = GetVariables("corr");
  set<Variable> new_corr_vars = GetVariables("new_corr");
  set<Variable> full_vars = GetVariables("full");
  set<Variable> new_full_vars = GetVariables("new_full");

  WritePlusHeader(full_vars, new_full_vars);
  WritePlusSource(full_vars, new_full_vars);
  WriteCorrHeader(corr_vars, new_corr_vars);
  WriteCorrSource(corr_vars, new_corr_vars);

}

bool Contains(const string &text, const string &pattern){
  return text.find(pattern) != string::npos;
}

void WritePlusHeader(const set<Variable> &full_vars, const set<Variable> &new_vars){


  set<Variable> all_vars = full_vars;
  for (auto &ivar: new_vars) all_vars.insert(ivar);

  ofstream file("inc/baby_plus.hpp");

  file << "// baby_plus: base class to handle reduced tree ntuples\n";
  file << "// File generated with generate_baby.exe\n\n";

  file << "#ifndef H_BABY_PLUS\n";
  file << "#define H_BABY_PLUS\n\n";

  file << "#include <vector>\n";
  file << "#include <string>\n";
  file << "#include <cmath>\n\n";
  file << "#include \"TTree.h\"\n";
  file << "#include \"TFile.h\"\n\n";
  file << "#include \"TChain.h\"\n\n";
  file << "#include \"TString.h\"\n\n";
  //  file << "#include \"TTreeFormula.h\"\n\n";

  file << "class baby_plus{\n";
  file << "public:\n";
  file << "  baby_plus(TString inputs, TString outname = \"\"); // Constructor to read tree\n\n";

  file << "  long GetEntries() const;\n";
  file << "  void GetEntry(const long entry);\n";

  file << "  void Fill();\n";
  file << "  void Write();\n\n";

  file << "  bool readOnly_;\n\n";
  file << "  double bad_val_;\n\n";

  file << "  ~baby_plus();\n\n";

  for(set<Variable>::const_iterator var = full_vars.begin(); var != full_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->type_ << " " << var->name_ << "();\n";
    } else {
      file << "  " << var->type_ << " " << var->name_ << "();\n";
    }
  }
  file << '\n';

  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->type_ << " & out_" << var->name_ << "();\n";
    } else {
      file << "  " << var->type_ << " & out_" << var->name_ << "();\n";      
    }
  }
  file << '\n';

  file << "  TFile* outfile_;\n";
  file << "  TChain* intree_;\n";
  file << "  TTree* outtree_;\n";
  file << '\n';

  file << "protected:\n";

  file << "private:\n";
  file << "  class VectorLoader{\n";
  file << "  public:\n";
  file << "    VectorLoader();\n";
  file << "  private:\n";
  file << "    static bool loaded_;\n";
  file << "  };\n\n";

  file << "  static VectorLoader vl_;\n";
  for(set<Variable>::const_iterator var = full_vars.begin(); var != full_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->type_ << ' ' << var->name_ << "_;\n";
      file << "  " << var->type_ << " *p_" << var->name_ << "_;\n";
    } else { 
      file << "  " << var->type_ << ' ' << var->name_ << "_;\n";
    }
    file << "  TBranch *b_" << var->name_ << "_;\n";
    file << "  mutable bool c_" << var->name_ << "_;\n";
  }

  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->type_ << " out_" << var->name_ << "_;\n";
      file << "  " << var->type_ << " *p_out_" << var->name_ << "_;\n";
    } else { 
      file << "  " << var->type_ << " out_" << var->name_ << "_;\n";
    }
    file << "  mutable bool c_out_" << var->name_ << "_;\n";
  }

  file << "  long entry_;\n";

  file << "};\n\n";

  file << "#endif" << endl;

  file.close();
}

void WritePlusSource(const set<Variable> &full_vars,
                     const set<Variable> &new_vars){

  set<Variable> all_vars = full_vars;
  for (auto &ivar: new_vars) all_vars.insert(ivar);

  ofstream file("src/baby_plus.cpp");

  file << "// baby_plus: base class to handle reduce tree ntuples\n";
  file << "//File generated with generate_baby.exe\n\n";

  file << "#include \"baby_plus.hpp\"\n\n";

  file << "#include <stdexcept>\n";
  file << "#include <string>\n";
  file << "#include <iostream>\n";
  file << "#include <vector>\n\n";

  file << "#include \"TROOT.h\"\n";
  file << "#include \"TTree.h\"\n";
  file << "#include \"TBranch.h\"\n";
  file << "#include \"TChain.h\"\n";
  file << "#include \"TString.h\"\n";
  //  file << "#include \"TTreeFormula.h\"\n\n";

  file << "using namespace std;\n\n";

  file << "#define ERROR(x) do{throw std::runtime_error(string(\"Error in file \")+__FILE__+\" at line \"+to_string(__LINE__)+\" (in \"+__func__+\"): \"+x);}while(false)\n";
  file << "#define DBG(x) do{std::cerr << \"In \" << __FILE__ << \" at line \" << __LINE__ << \" (in function \" << __func__ << \"): \" << x << std::endl;}while(false)\n\n";

  file << "bool baby_plus::VectorLoader::loaded_ = false;\n\n";

  file << "baby_plus::VectorLoader baby_plus::vl_ = baby_plus::VectorLoader();\n\n";

  file << "baby_plus::VectorLoader::VectorLoader(){\n";
  file << "  if(!loaded_){\n";
  file << "    gROOT->ProcessLine(\"#include <vector>\");\n";
  file << "    loaded_ = true;\n";
  file << "  }\n";
  file << "}\n\n";

  file << "baby_plus::baby_plus(TString inputs, TString outname):\n";
  file << "  readOnly_(outname==\"\"),\n";
  file << "  bad_val_(-999.),\n";

  for(set<Variable>::const_iterator var = full_vars.begin(); var != full_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->name_ << "_(0),\n";
    }else if(Contains(var->type_, "tring")){
      file << "  " << var->name_ << "_(\"\"),\n";
    }else{
      file << "  " << var->name_ << "_(static_cast<" << var->type_ << ">(bad_val_)),\n";
    }
    if(Contains(var->type_, "vector")){
      file << "  p_" << var->name_ << "_(&" << var->name_ << "_),\n";
    }
    file << "  b_" << var->name_ << "_(NULL),\n";
    file << "  c_" << var->name_ << "_(false),\n";
  }

  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  out_" << var->name_ << "_(0),\n";
    }else if(Contains(var->type_, "tring")){
      file << "  out_" << var->name_ << "_(\"\"),\n";
    }else{
      file << "  out_" << var->name_ << "_(static_cast<" << var->type_ << ">(bad_val_)),\n";
    }
    if(Contains(var->type_, "vector")){
      file << "  p_out_" << var->name_ << "_(&out_" << var->name_ << "_),\n";
    }
    file << "  c_out_" << var->name_ << "_(false),\n";
  }
  file << "  entry_(0){\n";

  file << "  if (inputs!=\"\") {\n";
  file << "    intree_ = new TChain(\"tree\");\n";
  file << "    intree_->Add(inputs);\n\n";
  file << "  }\n\n";
  file << "  if (!readOnly_) {\n";
  file << "    outfile_ = new TFile(outname, \"recreate\");\n";
  file << "    if(!outfile_->IsOpen()) ERROR(\"Could not open output file \"+outname.Data());\n";
  file << "    outfile_->cd();\n";
  file << "    outtree_ = intree_->CloneTree(0);\n";
  file << "  }\n\n";

  for(set<Variable>::const_iterator var = full_vars.begin(); var != full_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  intree_->SetBranchAddress(\"" << var->name_ << "\", &p_" << var->name_ << "_, &b_" << var->name_ << "_);\n";
    }else{
      file << "  intree_->SetBranchAddress(\"" << var->name_ << "\", &" << var->name_ << "_, &b_" << var->name_ << "_);\n";
    }
  }
  file << "  if (!readOnly_) {\n";
  for(set<Variable>::const_iterator var = full_vars.begin(); var != full_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "    outtree_->SetBranchAddress(\"" << var->name_ << "\", &p_out_" << var->name_ << "_);\n";
    }else{
      file << "    outtree_->SetBranchAddress(\"" << var->name_ << "\", &out_" << var->name_ << "_);\n";
    }
  }
  file << "  //New branches from \"extra\" list\n";
  for(set<Variable>::const_iterator var = new_vars.begin(); var != new_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "    outtree_->Branch(\"" << var->name_ << "\", &p_out_" << var->name_ << "_);\n";
    }else{
      file << "    outtree_->Branch(\"" << var->name_ << "\", &out_" << var->name_ << "_);\n";
    }
  }
  file << "  }\n\n";
  file << "}\n\n";

  file << "void baby_plus::Fill(){\n";
  file << "  //Loading unused branches so their values are copied to the new tree\n";
  for(set<Variable>::const_iterator var = full_vars.begin(); var != full_vars.end(); ++var){
    file << "  if (!readOnly_ && !c_"+var->name_+"_ && !c_out_"+var->name_+"_) " << var->name_ << "();\n";
  }
  file << "  outtree_->Fill();\n";

  file << "  //Resetting variables\n";
  for(set<Variable>::const_iterator var = full_vars.begin(); var != full_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->name_ << "_.clear();\n";
    }else if(Contains(var->type_, "tring")){
      file << "  " << var->name_ << "_ = \"\";\n";
    }else{
      file << "  " << var->name_ << "_ = static_cast<" << var->type_ << ">(bad_val_);\n";
    }
  }
  file << "  if (!readOnly_) {\n";
  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "    out_" << var->name_ << "_.clear();\n";
    }else if(Contains(var->type_, "tring")){
      file << "    out_" << var->name_ << "_ = \"\";\n";
    }else{ 
      file << "    out_" << var->name_ << "_ = static_cast<" << var->type_ << ">(bad_val_);\n";
    }
  }

  file << "  // Untick output branches\n";
  for(set<Variable>::const_iterator var = all_vars.begin(); var!= all_vars.end(); ++var){
    file << "    c_out_" << var->name_ << "_ = false;\n";
  }
  file << "  }\n";
  file << "}\n\n";

  file << "void baby_plus::Write(){\n";
  file << "  outtree_->Write();\n";
  file << "}\n\n";

  file << "baby_plus::~baby_plus(){\n";
  file << "  if (!readOnly_) outfile_->Close();\n";
  file << "}\n\n";
  
  file << "long baby_plus::GetEntries() const{\n";
  file << "  return intree_->GetEntries();\n";
  file << "}\n\n";

  file << "void baby_plus::GetEntry(const long entry){\n";

  for(set<Variable>::const_iterator var = full_vars.begin(); var!= full_vars.end(); ++var){
    file << "  c_" << var->name_ << "_ = false;\n";
  }
  file << "  entry_ = intree_->LoadTree(entry);\n";
  file << "}\n\n";

  for(set<Variable>::const_iterator var = full_vars.begin(); var != full_vars.end(); ++var){
    file << var->type_ << " baby_plus::" << var->name_ << "(){\n";
    file << "  if(!c_" << var->name_ << "_ && b_" << var->name_ <<"_){\n";
    file << "    b_" << var->name_ << "_->GetEntry(entry_);\n";
    if(Contains(var->type_, "vector")){
      if (!Contains(var->type_, "tring") && !Contains(var->type_, "bool")){
        file << "    for (unsigned i(0); i<" << var->name_ << "_.size(); i++) {\n";
        file << "      if (isnan(" << var->name_ << "_[i]) || isinf(" << var->name_ << "_[i])) {\n";
        file << "        cout<<\"Variable " << var->name_ << " at idx \"<<i<<\" is Nan or Inf.\"<<endl;\n";
        file << "        "<< var->name_ << "_[i] = 1;\n";
        file << "      }\n";
        file << "    }\n";
      }
    } else if(!Contains(var->type_, "tring") && !Contains(var->type_, "bool")){
      file << "    if (isnan(" << var->name_ << "_) || isinf(" << var->name_ << "_)){\n";
      file << "      cout<<\"Variable " << var->name_ << " is Nan or Inf.\"<<endl;\n";
      file << "      "<< var->name_ << "_ = 1;\n";
      file << "    }\n";
    }
    file << "    if (!readOnly_ && !c_out_" << var->name_ << "_) out_" << var->name_ << "_ = " << var->name_ << "_;\n";
    file << "    c_" << var->name_ << "_ = true;\n";
    file << "  }\n";
    file << "  return " << var->name_ << "_;\n";
    file << "}\n\n";
  }

  for(set<Variable>::const_iterator var = full_vars.begin(); var != full_vars.end(); ++var){
    file << var->type_ << " & baby_plus::out_" << var->name_ << "(){\n";
    file << "  if (!c_out_" << var->name_ << "_ && !c_" << var->name_ << "_) " << var->name_ << "();\n";
    file << "  c_out_" << var->name_ << "_ = true;\n";
    file << "  return out_" << var->name_ << "_;\n";
    file << "}\n\n";
  }

  for(set<Variable>::const_iterator var = new_vars.begin(); var != new_vars.end(); ++var){
    file << var->type_ << " & baby_plus::out_" << var->name_ << "(){\n";
    file << "  c_out_" << var->name_ << "_ = true;\n";
    file << "  return out_" << var->name_ << "_;\n";
    file << "}\n\n";
  }

  file.close();
}

void WriteCorrHeader(const set<Variable> &corr_vars, const set<Variable> &new_vars){

  set<Variable> all_vars = corr_vars;
  for (auto &ivar: new_vars) all_vars.insert(ivar);

  ofstream file("inc/baby_corr.hpp");

  file << "// baby_corr: base class to handle reduced tree ntuples\n";
  file << "// File generated with generate_baby.exe\n\n";

  file << "#ifndef H_BABY_CORR\n";
  file << "#define H_BABY_CORR\n\n";

  file << "#include <vector>\n";
  file << "#include <string>\n\n";

  file << "#include \"TTree.h\"\n";
  file << "#include \"TFile.h\"\n\n";
  file << "#include \"TChain.h\"\n\n";
  file << "#include \"TString.h\"\n\n";
  //  file << "#include \"TTreeFormula.h\"\n\n";

  file << "class baby_corr{\n";
  file << "public:\n";
  file << "  baby_corr(TString inputs, TString outname = \"\"); // Constructor to read tree\n\n";

  file << "  long GetEntries() const;\n";
  file << "  void GetEntry(const long entry);\n";

  file << "  void Fill();\n";
  file << "  void Write();\n\n";

  file << "  bool readOnly_;\n\n";
  file << "  double init_val_;\n\n";

  file << "  ~baby_corr();\n\n";

  for(set<Variable>::const_iterator var = corr_vars.begin(); var != corr_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->type_ << " " << var->name_ << "();\n";
    } else {
      file << "  " << var->type_ << " " << var->name_ << "();\n";
    }
  }
  file << '\n';

  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      if(Contains(var->type_, "float")) file << "  std::vector<double> & out_" << var->name_ << "();\n";
      else file << "  " << var->type_ << " & out_" << var->name_ << "();\n";
    } else {
      if(Contains(var->type_, "float")) file << "  double & out_" << var->name_ << "();\n";
      else file << "  " << var->type_ << " & out_" << var->name_ << "();\n";      
    }
  }
  file << '\n';

  file << "  TFile* outfile_;\n";
  file << "  TChain* intree_;\n";
  file << "  TTree* outtree_;\n";
  file << '\n';

  file << "protected:\n";

  file << "private:\n";
  file << "  class VectorLoader{\n";
  file << "  public:\n";
  file << "    VectorLoader();\n";
  file << "  private:\n";
  file << "    static bool loaded_;\n";
  file << "  };\n\n";

  file << "  static VectorLoader vl_;\n";
  for(set<Variable>::const_iterator var = corr_vars.begin(); var != corr_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->type_ << ' ' << var->name_ << "_;\n";
      file << "  " << var->type_ << " *p_" << var->name_ << "_;\n";
    } else { 
      file << "  " << var->type_ << ' ' << var->name_ << "_;\n";
    }
    file << "  TBranch *b_" << var->name_ << "_;\n";
    file << "  mutable bool c_" << var->name_ << "_;\n";
  }

  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      if(Contains(var->type_, "float")) file << "  std::vector<double> dbl_" << var->name_ << "_;\n";
      file << "  " << var->type_ << " out_" << var->name_ << "_;\n";
      file << "  " << var->type_ << " *p_out_" << var->name_ << "_;\n";
    } else { 
      if(Contains(var->type_, "float")) file << "  double dbl_" << var->name_ << "_;\n";
      file << "  " << var->type_ << " out_" << var->name_ << "_;\n";
    }
  }

  file << "  long entry_;\n";

  file << "};\n\n";

  file << "#endif" << endl;

  file.close();
}

void WriteCorrSource(const set<Variable> &corr_vars, const set<Variable> &new_vars){

  set<Variable> all_vars = corr_vars;
  for (auto &ivar: new_vars) all_vars.insert(ivar);

  ofstream file("src/baby_corr.cpp");

  file << "// baby_corr: base class to handle reduce tree ntuples\n";
  file << "//File generated with generate_baby.exe\n\n";

  file << "#include \"baby_corr.hpp\"\n\n";

  file << "#include <stdexcept>\n";
  file << "#include <string>\n";
  file << "#include <iostream>\n";
  file << "#include <vector>\n\n";

  file << "#include \"TROOT.h\"\n";
  file << "#include \"TTree.h\"\n";
  file << "#include \"TBranch.h\"\n";
  file << "#include \"TChain.h\"\n";
  file << "#include \"TString.h\"\n";
  //  file << "#include \"TTreeFormula.h\"\n\n";

  file << "using namespace std;\n\n";

  file << "#define ERROR(x) do{throw std::runtime_error(string(\"Error in file \")+__FILE__+\" at line \"+to_string(__LINE__)+\" (in \"+__func__+\"): \"+x);}while(false)\n";
  file << "#define DBG(x) do{std::cerr << \"In \" << __FILE__ << \" at line \" << __LINE__ << \" (in function \" << __func__ << \"): \" << x << std::endl;}while(false)\n\n";

  file << "bool baby_corr::VectorLoader::loaded_ = false;\n\n";

  file << "baby_corr::VectorLoader baby_corr::vl_ = baby_corr::VectorLoader();\n\n";

  file << "baby_corr::VectorLoader::VectorLoader(){\n";
  file << "  if(!loaded_){\n";
  file << "    gROOT->ProcessLine(\"#include <vector>\");\n";
  file << "    loaded_ = true;\n";
  file << "  }\n";
  file << "}\n\n";

  file << "baby_corr::baby_corr(TString inputs, TString outname):\n";
  file << "  readOnly_(outname==\"\"),\n";
  file << "  init_val_(0.),\n";

  for(set<Variable>::const_iterator var = corr_vars.begin(); var != corr_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->name_ << "_(0),\n";
    }else if(Contains(var->type_, "tring")){
      file << "  " << var->name_ << "_(\"\"),\n";
    }else{
      file << "  " << var->name_ << "_(static_cast<" << var->type_ << ">(init_val_)),\n";
    }
    if(Contains(var->type_, "vector")){
      file << "  p_" << var->name_ << "_(&" << var->name_ << "_),\n";
    }
    file << "  b_" << var->name_ << "_(NULL),\n";
    file << "  c_" << var->name_ << "_(false),\n";
  }

  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  out_" << var->name_ << "_(0),\n";
    }else if(Contains(var->type_, "tring")){
      file << "  out_" << var->name_ << "_(\"\"),\n";
    }else{
      if(Contains(var->type_, "float")) file << "  dbl_" << var->name_ << "_(init_val_),\n";
      file << "  out_" << var->name_ << "_(static_cast<" << var->type_ << ">(init_val_)),\n";
    }
    if(Contains(var->type_, "vector")){
      file << "  p_out_" << var->name_ << "_(&out_" << var->name_ << "_),\n";
    }
  }
  file << "  entry_(0){\n";

  file << "  if (inputs!=\"\") {\n";
  file << "    intree_ = new TChain(\"tree\");\n";
  file << "    intree_->Add(inputs);\n\n";
  file << "  }\n\n";
  file << "  if (!readOnly_) {\n";
  file << "    outfile_ = new TFile(outname, \"recreate\");\n";
  file << "    if(!outfile_->IsOpen()) ERROR(\"Could not open output file \"+outname.Data());\n";
  file << "    outfile_->cd();\n";
  file << "    outtree_ = new TTree(\"tree\",\"tree\");\n";
  file << "  }\n\n";

  file << "  if (readOnly_ && inputs!=\"\") {\n";
  for(set<Variable>::const_iterator var = corr_vars.begin(); var != corr_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  intree_->SetBranchAddress(\"" << var->name_ << "\", &p_" << var->name_ << "_, &b_" << var->name_ << "_);\n";
    }else{
      file << "  intree_->SetBranchAddress(\"" << var->name_ << "\", &" << var->name_ << "_, &b_" << var->name_ << "_);\n";
    }
  }
  file << "  } else {\n";
  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "    outtree_->Branch(\"" << var->name_ << "\", &p_out_" << var->name_ << "_);\n";
    }else{
      file << "    outtree_->Branch(\"" << var->name_ << "\", &out_" << var->name_ << "_);\n";
    }
  }
  file << "  }\n\n";
  file << "}\n\n";

  file << "void baby_corr::Fill(){\n";
  file << "  //Loading unused branches so their values are copied to the new tree\n";
  file << "  if (!readOnly_) {\n";
  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "float")) {
      if (Contains(var->type_, "vector")) {
        file << "    out_" << var->name_ << "_ = std::vector<float>(dbl_" << var->name_ << "_.begin(), dbl_" << var->name_ << "_.end());\n";
      } else { 
        file << "    out_" << var->name_ << "_ = static_cast<float>(dbl_" << var->name_ << "_);\n";
      }
    }
  }
  file << "  }\n";
  file << "  outtree_->Fill();\n";

  file << "  //Resetting variables\n";
  file << "  if (readOnly_) {\n";
  for(set<Variable>::const_iterator var = corr_vars.begin(); var != corr_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->name_ << "_.clear();\n";
    }else if(Contains(var->type_, "tring")){
      file << "  " << var->name_ << "_ = \"\";\n";
    }else{
      file << "  " << var->name_ << "_ = static_cast<" << var->type_ << ">(init_val_);\n";
    }
  }
  file << "  } else {\n";
  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "    out_" << var->name_ << "_.clear();\n";
      if(Contains(var->type_, "float")) file << "    dbl_" << var->name_ << "_.clear();\n";
    }else if(Contains(var->type_, "tring")){
      file << "    out_" << var->name_ << "_ = \"\";\n";
    }else{ 
      file << "    out_" << var->name_ << "_ = static_cast<" << var->type_ << ">(init_val_);\n";
      if(Contains(var->type_, "float")) file << "    dbl_" << var->name_ << "_ = init_val_;\n";
    }
  }

  file << "  }\n";
  file << "}\n\n";

  file << "void baby_corr::Write(){\n";
  file << "  if (!readOnly_) outtree_->Write();\n";
  file << "}\n\n";

  file << "baby_corr::~baby_corr(){\n";
  file << "  if (!readOnly_) outfile_->Close();\n";
  file << "}\n\n";
  
  file << "long baby_corr::GetEntries() const{\n";
  file << "  if (readOnly_) return intree_->GetEntries();\n";
  file << "  else return -1;\n";
  file << "}\n\n";

  file << "void baby_corr::GetEntry(const long entry){\n";

  for(set<Variable>::const_iterator var = corr_vars.begin(); var!= corr_vars.end(); ++var){
    file << "  c_" << var->name_ << "_ = false;\n";
  }
  file << "  entry_ = intree_->LoadTree(entry);\n";
  file << "}\n\n";

  for(set<Variable>::const_iterator var = corr_vars.begin(); var != corr_vars.end(); ++var){
    file << var->type_ << " baby_corr::" << var->name_ << "(){\n";
    file << "  if(readOnly_ && !c_" << var->name_ << "_ && b_" << var->name_ <<"_){\n";
    file << "    b_" << var->name_ << "_->GetEntry(entry_);\n";
    file << "    c_" << var->name_ << "_ = true;\n";
    file << "  }\n";
    file << "  return " << var->name_ << "_;\n";
    file << "}\n\n";
  }

  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(Contains(var->type_, "float")) {
      if(Contains(var->type_, "vector")) 
        file << "std::vector<double> & baby_corr::out_" << var->name_ << "(){\n";
      else
        file << "double & baby_corr::out_" << var->name_ << "(){\n";
      file << "  return dbl_" << var->name_ << "_;\n";
      file << "}\n\n";
    } else { 
      file << var->type_ << " & baby_corr::out_" << var->name_ << "(){\n";
      file << "  return out_" << var->name_ << "_;\n";
      file << "}\n\n";
    }
  }

  file.close();
}




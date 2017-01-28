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
  vector<string> file_names = Tokenize(execute("ls variables/ 2> /dev/null"), "\n");

  vector<pair<string, set<Variable> > > sep_vars(file_names.size());
  vector<string> fixed_names(file_names.size());
  for(size_t ifile = 0; ifile < file_names.size(); ++ifile){
    fixed_names.at(ifile) = FixName(file_names.at(ifile));
    sep_vars.at(ifile).first = fixed_names.at(ifile);
    sep_vars.at(ifile).second = GetVariables(file_names.at(ifile));
  }

  set<Variable> all_vars;
  for(size_t ifile = 0; ifile < sep_vars.size(); ++ifile){
    for(set<Variable>::const_iterator var = sep_vars.at(ifile).second.begin();
        var != sep_vars.at(ifile).second.end();
        ++var){
      all_vars.insert(*var);
    }
  }

  set<Variable> com_vars;
  if(sep_vars.size()){
    for(set<Variable>::const_iterator var = sep_vars.at(0).second.begin();
        var != sep_vars.at(0).second.end();
        ++var){
      bool found_in_all = true;
      for(size_t ifile = 1; found_in_all && ifile < sep_vars.size(); ++ifile){
        if(sep_vars.at(ifile).second.find(*var) == sep_vars.at(ifile).second.end()){
          found_in_all = false;
        }
      }
      if(found_in_all){
        com_vars.insert(*var);
      }
    }
    for(set<Variable>::const_iterator var = com_vars.begin();
        var != com_vars.end();
        ++var){
      for(size_t ifile = 0; ifile < sep_vars.size(); ++ifile){
        sep_vars.at(ifile).second.erase(*var);
      }
    }
  }

  WriteBaseHeader(all_vars, com_vars);
  WriteBaseSource(all_vars, com_vars);

}

bool Contains(const string &text, const string &pattern){
  return text.find(pattern) != string::npos;
}

void WriteBaseHeader(const set<Variable> &all_vars,
                     const set<Variable> &com_vars){

  ofstream file("inc/baby_base.hpp");

  file << "// baby_base: base class to handle reduced tree ntuples\n";
  file << "// File generated with generate_baby.exe\n\n";

  file << "#ifndef H_BABY_BASE\n";
  file << "#define H_BABY_BASE\n\n";

  file << "#include <vector>\n";
  file << "#include <string>\n\n";

  file << "#include \"TTree.h\"\n";
  file << "#include \"TFile.h\"\n\n";
  file << "#include \"TString.h\"\n\n";
  //  file << "#include \"TTreeFormula.h\"\n\n";

  file << "class baby_base{\n";
  file << "public:\n";
  file << "  baby_base(TString filename); // Constructor to read tree\n\n";

  file << "  long GetEntries() const;\n";
  file << "  void GetEntry(const long entry);\n";

  file << "  void Fill();\n";
  file << "  void Write();\n\n";

  file << "  static const double bad_val_;\n\n";

  file << "  ~baby_base();\n\n";

  for(set<Variable>::const_iterator var = com_vars.begin();
      var != com_vars.end();
      ++var){
    if(Contains(var->type_, "vector")){
      file << "  std::" << var->type_ << " & " << var->name_ << "();\n";
      file << "  std::" << var->type_ << " & new_" << var->name_ << "();\n";
    } else {
      file << "  " << var->type_ << " & " << var->name_ << "();\n";
      file << "  " << var->type_ << " & new_" << var->name_ << "();\n";      
    }
  }
  file << '\n';

  for(set<Variable>::const_iterator var = all_vars.begin();
      var != all_vars.end();
      ++var){
    if(com_vars.find(*var) != com_vars.end()) continue;
    file << "  __attribute__((noreturn)) "
         << var->type_ << " & " << var->name_ << "();\n";
  }
  file << "  TFile* infile_;\n";
  file << "  TFile* outfile_;\n";
  file << "  TTree* intree_;\n";
  file << "  TTree* outtree_;\n";
  file << "  long entry_;\n";
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
  for(set<Variable>::const_iterator var = com_vars.begin();
      var != com_vars.end();
      ++var){
    if(Contains(var->type_, "vector")){
      file << "  std::" << var->type_ << ' ' << var->name_ << "_;\n";
      file << "  std::" << var->type_ << " new_" << var->name_ << "_;\n";
      file << "  std::" << var->type_ << " *p_" << var->name_ << "_;\n";
      file << "  std::" << var->type_ << " *p_new_" << var->name_ << "_;\n";
    } else { 
      file << "  " << var->type_ << ' ' << var->name_ << "_;\n";
      file << "  " << var->type_ << " new_" << var->name_ << "_;\n";
    }
    file << "  TBranch *b_" << var->name_ << "_;\n";
    file << "  mutable bool c_" << var->name_ << "_;\n";
  }
  file << "};\n\n";

  file << "#endif" << endl;

  file.close();
}

void WriteBaseSource(const set<Variable> &all_vars,
                     const set<Variable> &com_vars){
  ofstream file("src/baby_base.cpp");

  file << "// baby_base: base class to handle reduce tree ntuples\n";
  file << "//File generated with generate_baby.exe\n\n";

  file << "#include \"baby_base.hpp\"\n\n";

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

  file << "bool baby_base::VectorLoader::loaded_ = false;\n\n";

  file << "baby_base::VectorLoader baby_base::vl_ = baby_base::VectorLoader();\n\n";

  file << "baby_base::VectorLoader::VectorLoader(){\n";
  file << "  if(!loaded_){\n";
  file << "    gROOT->ProcessLine(\"#include <vector>\");\n";
  file << "    loaded_ = true;\n";
  file << "  }\n";
  file << "}\n\n";

  file << "const double baby_base::bad_val_ = -999.;\n\n";

  file << "baby_base::baby_base(TString filename):\n";
  file << "  entry_(0),\n";
  if(com_vars.size()){
    const set<Variable>::const_iterator com_end_2 = --com_vars.end();
    for(set<Variable>::const_iterator var = com_vars.begin();
        var != com_end_2;
        ++var){
      if(Contains(var->type_, "vector")){
        file << "  " << var->name_ << "_(0),\n";
        file << "  new_" << var->name_ << "_(0),\n";
      }else if(Contains(var->type_, "tring")){
        file << "  " << var->name_ << "_(\"\"),\n";
        file << "  new_" << var->name_ << "_(\"\"),\n";
      }else{
        file << "  " << var->name_ << "_(static_cast<" << var->type_ << ">(bad_val_)),\n";
        file << "  new_" << var->name_ << "_(static_cast<" << var->type_ << ">(bad_val_)),\n";
      }
      if(Contains(var->type_, "vector")){
        file << "  p_" << var->name_ << "_(&" << var->name_ << "_),\n";
        file << "  p_new_" << var->name_ << "_(&" << var->name_ << "_),\n";
      }
      file << "  b_" << var->name_ << "_(NULL),\n";
      file << "  c_" << var->name_ << "_(false),\n";
    }
    if(Contains(com_end_2->type_, "vector")){
      file << "  " << com_end_2->name_ << "_(0),\n";
      file << "  new_" << com_end_2->name_ << "_(0),\n";
    }else if(Contains(com_end_2->type_, "tring")){
      file << "  " << com_end_2->name_ << "_(\"\"),\n";
      file << "  new_" << com_end_2->name_ << "_(\"\"),\n";
    }else{
      file << "  " << com_end_2->name_ << "_(static_cast<" << com_end_2->type_ << ">(bad_val_)),\n";
      file << "  new_" << com_end_2->name_ << "_(static_cast<" << com_end_2->type_ << ">(bad_val_)),\n";
    }
    if(Contains(com_end_2->type_, "vector")){
      file << "  p_" << com_end_2->name_ << "_(&" << com_end_2->name_ << "_),\n";
      file << "  p_new_" << com_end_2->name_ << "_(&" << com_end_2->name_ << "_),\n";
    }
    file << "  b_" << com_end_2->name_ << "_(NULL),\n";
    file << "  c_" << com_end_2->name_ << "_(false){\n";
  }else{
    file << "  {\n";
  }
  file << "  infile_ = new TFile(filename, \"read\");\n";
  file << "  if(!infile_->IsOpen()) ERROR(\"Could not open input file \"+filename);\n";
  file << "  intree_ = static_cast<TTree*>(infile_->Get(\"tree\"));\n";
  file << "  if(intree_ == nullptr) ERROR(\"No tree in input file:\" +filename);\n";
  file << "    \n";
  file << "  outfile_ = new TFile(filename.ReplaceAll(\".root\",\"_renorm.root\"), \"recreate\");\n";
  file << "  if(!outfile_->IsOpen()) ERROR(\"Could not open output file \"+filename);\n";
  file << "  outfile_->cd();\n";
  file << "  outtree_ = intree_->CloneTree(0);\n";
  file << "  intree_->CopyAddresses(outtree_);\n";
  for(set<Variable>::const_iterator var = com_vars.begin(); var != com_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  intree_->SetBranchAddress(\"" << var->name_ << "\", &p_" << var->name_ << "_, &b_" << var->name_ << "_);\n";
      file << "  outtree_->SetBranchAddress(\"" << var->name_ << "\", &p_new_" << var->name_ << "_);\n";
    }else{
      file << "  intree_->SetBranchAddress(\"" << var->name_ << "\", &" << var->name_ << "_, &b_" << var->name_ << "_);\n";
      file << "  outtree_->SetBranchAddress(\"" << var->name_ << "\", &new_" << var->name_ << "_);\n";
    }
  }
  file << "}\n\n";

  file << "void baby_base::Fill(){\n";
  file << "  //Loading unused branch so their values are copied to the new tree\n";
  for(set<Variable>::const_iterator var = com_vars.begin(); var != com_vars.end(); ++var){
    file << "  if (!c_"+var->name_+"_) " << var->name_ << "();\n";
  }
  file << "  outtree_->Fill();\n";

  file << "  //Resetting variables\n";
  for(set<Variable>::const_iterator var = com_vars.begin(); var != com_vars.end(); ++var){
    if(Contains(var->type_, "vector")){
      file << "  " << var->name_ << "_.clear();\n";
      file << "  new_" << var->name_ << "_.clear();\n";
    }else if(Contains(var->type_, "tring")){
      file << "  " << var->name_ << "_ = \"\";\n";
      file << "  new_" << var->name_ << "_ = \"\";\n";
    }else{
      file << "  " << var->name_ << "_ = static_cast<" << var->type_ << ">(bad_val_);\n";
      file << "  new_" << var->name_ << "_ = static_cast<" << var->type_ << ">(bad_val_);\n";
    }
  }
  file << "}\n\n";

  file << "void baby_base::Write(){\n";
  file << "  outtree_->Write();\n";
  file << "}\n\n";

  file << "baby_base::~baby_base(){\n";
  file << "  infile_->Close();\n";
  file << "  outfile_->Close();\n";
  file << "}\n\n";
  
  file << "long baby_base::GetEntries() const{\n";
  file << "  return intree_->GetEntries();\n";
  file << "}\n\n";

  file << "void baby_base::GetEntry(const long entry){\n";

  for(set<Variable>::const_iterator var = com_vars.begin(); var!= com_vars.end(); ++var){
    file << "  c_" << var->name_ << "_ = false;\n";
  }
  file << "  entry_ = intree_->LoadTree(entry);\n";
  file << "}\n\n";

  for(set<Variable>::const_iterator var = com_vars.begin(); var != com_vars.end(); ++var){
    file << var->type_ << " & baby_base::" << var->name_ << "(){\n";
    file << "  if(!c_" << var->name_ << "_ && b_" << var->name_ <<"_){\n";
    file << "    b_" << var->name_ << "_->GetEntry(entry_);\n";
    file << "    new_" << var->name_ << "_ = " << var->name_ <<"_;\n";
    file << "    c_" << var->name_ << "_ = true;\n";
    file << "  }\n";
    file << "  return " << var->name_ << "_;\n";
    file << "}\n\n";

    file << var->type_ << " & baby_base::new_" << var->name_ << "(){\n";
    file << "  return new_" << var->name_ << "_;\n";
    file << "}\n\n";
  }

  for(set<Variable>::const_iterator var = all_vars.begin(); var != all_vars.end(); ++var){
    if(com_vars.find(*var) != com_vars.end()) continue;
    file << var->type_ << " & baby_base::" << var->name_ << "(){\n";
    file << "  throw std::logic_error(\"" << var->name_
         << " does not exist in this baby_base version.\");\n";
    file << "}\n\n";
  }

  file.close();
}



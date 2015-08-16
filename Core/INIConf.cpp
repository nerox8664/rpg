#include "INIConf.hpp"

#ifdef __INI_SINGLETON__
  INIConf* INIConf::instance = nullptr;
  INIConf* INIConf::GetInstance() {
    return instance;
  };
#endif
    
//PARSER

std::string INIConf::stringToString(std::string value) {
  return value;
}

float INIConf::stringToFloat(std::string value) {
  return atof (value.c_str());
}

int INIConf::stringToInt(std::string value) {
  return atoi (value.c_str());
}

bool INIConf::stringToBool(std::string value) {
  if(value == "" ) {
    return false;
  } else {
    bool numeric = 1;
    for(int i = 0; i< value.size(); i++) {
      if(!isdigit(value[i])) numeric = 0;
    }
    if(numeric) {
      return atoi (value.c_str());
    } else {
      if(value.find("true")!=std::string::npos) return true;
      return false;
    }
  }
}

void INIConf::setUpdate(std::string section, std::string key, std::string value) {
  INIConfEntity e;
  e.svalue = value;
  e.ivalue = stringToInt(value);
  e.lvalue = stringToBool(value);
  e.fvalue = stringToFloat(value);
  iniMap[section][key] = e;
}

void INIConf::parse ( std::string line ) {
  std::string key = "";
  std::string value = "";
  char openingChar;
  bool isValue = 0;
  for(int i = 0; i < line.size(); i++) {
    switch(line[i]) {
    case ';': // beginning of comment
        if(section.size() && key.size()) {
            setUpdate(section, key, value);

#ifdef __DEBUG_OUTPUT__
            std::cout<<"Add pair : "<<section<<" : "<<key<<" : "<<value<<std::endl;
#endif

            return;
        }
        break;
    case ' ':
        break;
    case '{':
        i++;
        if(isValue) {
            for(; line[i]!='}'; i++) {
                if(i>=line.size() ) {

                    std::cout<<"Error. Where is string closing symbol?"<<std::endl;

                    break;
                }
                value+=line[i];
            }
        }
        break;
    case '=':
        isValue = 1;
        break;
    case '[': // beginning of section
        section="";
        i++;
        for(; line[i]!=']'; i++) {
            if(i>=line.size() ) {
                std::cout<<"Error. Where is closing bracket?"<<std::endl;
                break;
            }
            section+=line[i];
        }
        break;
    case '\'':
    case '"':
    case '`':
        openingChar = line[i++];
        if(isValue) {
            for(; line[i]!=openingChar; i++) {
                if(i>=line.size() ) {
                    std::cout<<"Error. Where is string closing symbol?"<<std::endl;
                    break;
                }
                value+=line[i];
            }
        }
        else {
            for(; line[i]!=openingChar; i++) {
                if(i>=line.size() ) {
                    std::cout<<"Error. Where is string closing symbol?"<<std::endl;
                    break;
                }
                key+=line[i];
            }
        }
        break;
    default:
        if(isValue) value+=line[i];
        else key+=line[i];
    }
  }

  if(key.size()  && value.size() && section.size() > 0) {
      setUpdate(section, key, value);
      //iniMap[section][key] = value;
#ifdef __DEBUG_OUTPUT__
      std::cout<<"Add pair : "<<section<<" : "<<key<<" : "<<value<<std::endl;
#endif
  }

  isLoadedFlag = 1;

  return;
}

bool INIConf::isLoaded() {
  return isLoadedFlag;
}

bool INIConf::isSet(std::string section, std::string key) {
  if(iniMap.find(section)!=iniMap.end() && iniMap[section].find(key)!=iniMap[section].end()) {
    return true;
  } else {
    return false;
  }
}

void INIConf::load ( std::string filename ) {
  std::ifstream inputFile(filename.c_str());
  std::string line;
  while(!inputFile.eof() && inputFile.good()) {
    std::getline(inputFile,line);
    parse(line);
  }
  inputFile.close();
  return;
}

void INIConf::save ( std::string filename ) {
  std::ofstream outputFile(filename.c_str());
  for( auto i = iniMap.begin(); i!= iniMap.end(); i++) {
    outputFile<<"["<<i->first<<"]"<<std::endl;
    for(  auto j = i->second.begin(); j!=  i->second.end(); j++) {
      outputFile<<j->first<<" = "<<j->second.svalue<<std::endl;
    }
    outputFile<<std::endl;
  }
  outputFile.close();
  return;
}

void INIConf::merge(INIConf *another) {
  iniMap.insert(another->iniMap.begin(), another->iniMap.end());
}

INIConf::INIConf():isLoadedFlag(false), section("") {
#ifdef __INI_SINGLETON__
  instance = this;
#endif
}

INIConf::INIConf ( std::string filename ) {
  load(filename);
#ifdef __INI_SINGLETON__
  instance = this;
#endif
}

INIConf::~INIConf () {
}

//CONVERTOR

std::vector<std::string> INIConf::split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}


int INIConf::getInt( std::string section, std::string key) {
  if (isSet(section, key)) {
      return iniMap[section][key].ivalue;
  } else {
    return -1;
  }
}

float INIConf::getFloat( std::string section, std::string key) {
  if (isSet(section, key)) {
      return iniMap[section][key].fvalue;
  } else {
    return -1;
  }
}

bool INIConf::getBool( std::string section, std::string key) {
  if (isSet(section, key)) {
    return iniMap[section][key].lvalue;
  } else {
    return false;
  }
        
}

std::string INIConf::getString( std::string section, std::string key) {
  if (isSet(section, key)) {
      return iniMap[section][key].svalue;
  } else {
    return "";
  }
}

void INIConf::setInt( std::string section, std::string key, int value) {
  setUpdate(section, key, std::to_string(value) );
}

void INIConf::setFloat( std::string section, std::string key, float value) {
  setUpdate(section, key, std::to_string(value) );
}

void INIConf::setBool( std::string section, std::string key, bool value) {
  setUpdate(section, key, std::to_string((int)value) );
}

void INIConf::setString( std::string section, std::string key, std::string value) {
  setUpdate(section, key, value );
}

template<typename T>
std::vector<T> INIConf::getArray(std::string section, std::string key, T (*conv)(std::string) , char delim) {
  std::string imp_value = getString(section, key);
  std::vector<std::string> ex_values = split(imp_value, delim);
  std::vector<T> result(ex_values.size());
  for(int i = 0; i<ex_values. size(); i++) {
    result[i] = conv(ex_values[i]);
  }
  return result;
}

template<typename T>
int INIConf::setArray(std::string section, std::string key, std::vector<T> array , std::string (*conv)(T) , char delim) {
  std::string result;
  for(int i =0; i<array.size(); i++) {
    result+=conv(array[i]);
    if(i!=array.size()-1) {
      result+=delim;
    }
  }
  setUpdate(section, key, result );
}

std::vector<int> INIConf::getIntArray(std::string section, std::string key, char delim) {
  return getArray<int> (section,  key, &INIConf::stringToInt, delim);
}


std::vector<std::string> INIConf::getStringArray(std::string section, std::string key, char delim) {
  return getArray<std::string> (section,  key, &INIConf::stringToString, delim);
}


std::vector<float> INIConf::getFloatArray(std::string section, std::string key, char delim) {
  return getArray<float> (section,  key, &INIConf::stringToFloat, delim);
}
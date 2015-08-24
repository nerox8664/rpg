#ifndef __RESOURCE__
#define __RESOURCE__

#include "Core/Types.hpp"

class Resource {

public:
  int32_t usage;
  int64_t id;
  
  std::string path;
  
  enum ResourceType{
    SOUND,
    TEXTURE,
    TEXT
  };
  
  ResourceType type;

  virtual ~Resource() {};

};

#endif
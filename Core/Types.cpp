#include <Core/Types.hpp>

bool operator==(const Event_t& x, const Event_t& y) {
  return (int)x==(int)y || (int)x == (int)EVENT_ANY || (int)y == (int)EVENT_ANY;
}

bool operator==(const Event_sub_t& x, const Event_sub_t& y) {
  return (int)x==(int)y || (int)x == (int)EVENT_ANY_ANY || (int)y == (int)EVENT_ANY_ANY;
}

bool isPartOf(Event& part, Event& base) {
  if (part.type != base.type || part.subtype != base.subtype) {
    return false;
  }

  for (std::map<std::string, Any>::iterator i = part.data.begin(); i!= part.data.end(); i++) {
    if (base.data.find(i->first) == base.data.end()) {
      return false;
    }
    if (!(i->second == base.data[(*i).first])) {

      return false;
    }
  }
  return true;
}

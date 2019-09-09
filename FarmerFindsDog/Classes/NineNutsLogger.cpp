#include "NineNutsLogger.h"


using namespace std;
using namespace cocos2d;

#include <sstream> // ostringstream

// =============================================================================


string NineNutsLogger::value2str(const Value& value) {
  //
  ostringstream ss;

  switch (value.getType()) {
  case Value::Type::NONE:
    ss << "NONE type";
    break;

  case Value::Type::BYTE:
    ss << value.asByte();
    break;

  case Value::Type::INTEGER:
    ss << value.asInt();
    break;

  case Value::Type::FLOAT:
    ss << value.asFloat();
    break;

  case Value::Type::DOUBLE:
    ss << value.asDouble();
    break;

  case Value::Type::BOOLEAN:
    ss << value.asByte() ? "TRUE" : "FALSE";
    break;

  case Value::Type::STRING:
    ss << value.asString();
    break;


  case Value::Type::VECTOR: {
    const ValueVector vv = value.asValueVector();
    ss << "Vector of " << vv.size() << " elements" << "\n";

    for (auto v: vv) {
      ss << NineNutsLogger::value2str(v) << "\n";
    }
  };
    break;

  case Value::Type::MAP: {
    const ValueMap vmap = value.asValueMap();
    ss << "Value Map of " << vmap.size() << " elements" << "\n";

    for (auto v: vmap) {
      ss << v.first << " : " << NineNutsLogger::value2str(v.second) << "\n";
    }
  };
    break;

  case Value::Type::INT_KEY_MAP: {
    const ValueMapIntKey vmap = value.asIntKeyMap();
    ss << "Integer Map of " << vmap.size() << " elements" << "\n";

    for (auto v: vmap) {
      ss << v.first << " : " << NineNutsLogger::value2str(v.second) << "\n";
    }
  };
    break;

  default:
    ss << "Failed to recognize type";
  }

  return ss.str();
}

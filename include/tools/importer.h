#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>
#include "../utilities.h"

class Importer {
public:
    Scene importXml(const std::string &filepath);
};

#endif

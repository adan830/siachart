#pragma once

#include "siapi.h"
#include "unpacker.h"

namespace sia {
namespace resy {

// �ṩԭʼ����Դ����ӿ�
class RESY_API Asset {
public:
    static bool read(const std::string& file, std::string& cont);
};

}
}

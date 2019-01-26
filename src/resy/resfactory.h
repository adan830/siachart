#pragma once

#include "siapi.h"
#include "unpacker.h"
#include "resitem.h"

namespace sia {
namespace resy {

class RESY_API IResFactory {
public:
    IResFactory(const SkString& res_type)
        :res_type_(res_type) {
        assert(!res_type_.isEmpty());
    }

    virtual my_sp<ResItem> parseRes(const SkString& group_name, const SkString& key, const SkString& val) = 0;

    const SkString& resType()const {
        return res_type_;
    }

protected:
    // ��������Դ���ͣ���ʶ�磺font/image/color�ȣ�
    SkString res_type_;
};

}
}

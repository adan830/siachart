#include "resitem.h"
#include "resman.h"
#include <string>

namespace sia {
namespace resy {

ResItem::ResItem(int16_t typ, const SkString& styp, const SkString& id)
    :kTyp_(typ), sTyp_(styp), key_(id) {
    // id �����Ǵ�����Դ��ʶ�ģ�
    // assert(id.contains('/'));
    if (!id.contains('/')) {
        // ����Ҫ������key�������Դ����
        key_ = resTypeStr();
        key_.append("/");
        key_.append(id);
    }
}

// str�������ƣ�
//      @global/color/border
//      @global/border  <-- ���ʱ����봫����Դ����
bool extractResPath(const SkString& str, const SkString& restyp, SkString& out) {
    int idx = str.find("/");
    if (idx > 0 && idx < (str.size() - 1)) {    // �Ҳ������һ���ַ�
        out = SkString(str.c_str() + idx + 1);
        if (!out.contains('/')) {
            // û�д���Դ���ͣ�
            SkString lead = restyp;
            lead.append("/");
            out.prepend(lead);
            return true;
        }

        // ������Դ������
        if (!out.startsWith(restyp.c_str())) {
            // ��Դ��ͬ����
            return false;
        }
        return true;
    }
    return false;
}

my_sp<ResItem> ResItem::commonRefrence(const SkString& id) {
    if (!id.startsWith("@common")) {
        return nullptr;
    }

    // ������
    SkString item;
    if (cmn_res_ && extractResPath(id, resTypeStr(), item)) {
        return cmn_res_->rawRes(item);
    }
    return nullptr;
}

my_sp<ResItem> ResItem::globalRefrence(const SkString& id) {
    if (!id.startsWith("@global")) {
        return nullptr;
    }

    // ������
    SkString item;
    if (global_res_ && extractResPath(id, resTypeStr(), item)) {
        return global_res_->rawRes(item);
    }
    return nullptr;
}

bool ResItem::detectRefrence(const SkString& resval) {
    auto setRef = [&](my_sp<ResItem> p) -> bool {
        if (p && p->resType() == resType()) {
            flags_ |= Flags::kRefRes;
            res_ref_ = p;
            return true;
        } else {
            flags_ &= (~Flags::kValid);
            return false;
        }
    };

    if (resval.startsWith("@common")) {
        setRef(commonRefrence(resval));
        return true;
    }

    if (resval.startsWith("@global")) {
        setRef(globalRefrence(resval));
        return true;
    }
    return false;
}

}
}

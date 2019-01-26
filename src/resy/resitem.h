#pragma once

#include "resyapi.h"

namespace sia {
namespace resy {

enum /*class*/ RESY_API ResType : int16_t {
    kResInvalid = 0,
    //! ���壺��������ɫ
    kResFont = 1,
    //! ͼ��
    kResImage,
    //! ��ɫ
    kResColor,
    //! �߶�
    kResStroke,
    //! Ч�������߶εķ��dashed ...��
    kResEffect,

    // ����resy�����ĵײ�Ӧ�ÿ�ܿ��Ի��ڴ�ֵ�ۼ�
    kFramework = 100,

    // ���ߵ�Ӧ�ò���ڴ�ֵ�ۼ�
    kApplication = 1000,
};

class RESY_API ResGroup;
class RESY_API ResItem {
    enum Flags {
        kRefRes = 0x1,
        kValid = 0x2,
    };

public:
    ResItem() = delete;
    virtual ~ResItem() {
    }

    /*! \brief ���ͱ������ڴ�����ʱ���ȷ�������ǲ��ܸ��ĵģ�
     *
     *    @param    typ ���ε����ͱ�ʶ
     *              styp    ������ʶ������ͣ����׶���
     *              res_path  ��Դ��·�������߽�id��
     */
    explicit ResItem(int16_t typ, const SkString& styp, const SkString& res_path);

    /*! \brief ������Դ
     *
     *    @param    res ���ַ�����ʶ����Դ
     *              Ҫ������������֪����������ݣ���������һ������ȷ����)
     *    @return
     */
    virtual bool parse(const SkString& res) {
        return false;
    }

    void releaseRes() {
        cmn_res_ = nullptr;
        global_res_ = nullptr;
        res_ref_ = nullptr;
    }

    inline SkString resTypeStr()const {
        return sTyp_;
    }

    void setCommonResRef(my_sp<ResGroup> cmn_res, my_sp<ResGroup> global_res) {
        cmn_res_ = cmn_res;
        global_res_ = global_res;
    }

    inline SkString respath()const {
        return respath_;
    }

    inline SkString key()const {
        return key_;
    }

    inline bool isRefrence()const {
        return flags_ & Flags::kRefRes;
    }

    inline int16_t resType()const {
        return kTyp_;
    }

    inline bool isValid()const {
        return is_valid_;
    }

protected:
    // ����Ƿ������õ���Դ����
    bool detectRefrence(const SkString& id);
    my_sp<ResItem> commonRefrence(const SkString& id);
    my_sp<ResItem> globalRefrence(const SkString& id);

protected:
    //! �����ǲ��ܸ��ĵ�
    const int16_t kTyp_ /*= ResType::kResInvalid*/;
    int16_t flags_ = Flags::kValid;
    const SkString sTyp_;

    // �������� @global/font/f1
    // ������Դ���ƣ���#ff00ff�� / "bold 15 ����"
    SkString respath_;
    SkString key_;

    // ���õı𴦵���Դ
    my_sp<ResItem> res_ref_ = nullptr;

private:
    bool is_ref_res_ = false;
    bool is_valid_ = true;
    my_sp<ResGroup> cmn_res_ = nullptr;
    my_sp<ResGroup> global_res_ = nullptr;
};

}
}

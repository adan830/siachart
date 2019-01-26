#pragma once

#include "graphyapi.h"

// string
// HASH_WORK_AROUND(SkString);

const SkScalar INVALID_SCALAR_VAL = std::numeric_limits<float>::max();
const SkScalar MAX_SCALAR = std::numeric_limits<float>::max();
const SkScalar ZERO_SCALAR = SkIntToScalar(0);

namespace sia {
namespace graphy {
class Widget;
class GRAPHY_API Layout {
    friend class Widget;
public:
    virtual ~Layout();

    inline Widget* decoratedWidget()const {
        return decorated_;
    }

    SkRect geometry()const;

    SkRect rect()const;

protected:
    virtual bool parse(const SkString& layout) {
        return true;
    }

    virtual void doLayout() = 0;

    /*! \brief ������ҪӦ�ô˲��ֵĿؼ�
     *		���õ�ͬʱ���ڲ����Զ�����һ��doLayout���ϲ���Բ����ٴ˵���doLayout
     *    @param
     *    @return
     */
    void setDecoratedWidget(Widget* w);

    Widget* widget(const char* name)const {
        return widget(SkString(name));
    }

    Widget* widget(const SkString& name)const;

    std::vector<Widget*> childrenWidget()const;

protected:
    Widget* decorated_ = nullptr;
};

class GRAPHY_API Measure {
    enum class MeasureType {
        kInvalid = 0,
        kFixed = 1,
        kPercent,
        kAll,	// ռ��ʣ�����еĿռ�
    };

public:
    bool parse(const SkString& s);
    void set(SkScalar v, MeasureType t);

    // ��ȿ�������ȷ��
    bool isImdiateSure()const {
        return isFixed() || isPercent();
    }

    bool isPercent()const {
        return type_ == MeasureType::kPercent;
    }

    bool isFixed()const {
        return type_ == MeasureType::kFixed;
    }

    SkScalar calc(SkScalar all, SkScalar max_v, SkScalar min_v)const;

    inline MeasureType typ()const {
        return type_;
    }

    inline SkScalar val()const {
        return val_;
    }

private:
    SkScalar val_ = 0.f;
    MeasureType type_ = MeasureType::kInvalid;
};

}
}

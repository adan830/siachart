#pragma once

#include "defines.h"
#include "graphy/tooltip.h"
#include "visualitem.h"

namespace sia {
namespace charty {


class ViewItemTooltip : public VisualItem {
public:
    ViewItemTooltip();

    void setTitle(const SkString& title);

    void addBodyItem(const SkString& cont, SkColor color = Sk_InvalidColor);
    void setBodyItems(const SkTArray<SeriesTooltipData::Item>& item);

    void reset();

    virtual void preparePaints() override;

    SkColor bodyTextColor()const;

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    /*! \brief ���cont��ռ�ÿ�͸ߣ��Զ��ӵ���ǰ�Ŀؼ���С�ϡ�
     *
     *    @param	cont	��Ҫ�����ַ���
     *    @return
     */
    void addTextRect(const SkString& cont) ;

private:
    SkString title_;
    SkTArray<SeriesTooltipData::Item> body_;

    SkScalar padding_ = SkIntToScalar(5);

    SkPaint* bk_ = nullptr;
    SkPaint* border_ = nullptr;
    SkPaint* text_ = nullptr;
};

}
}

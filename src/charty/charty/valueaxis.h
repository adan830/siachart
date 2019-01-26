#pragma once

#include "axis.h"

namespace sia {
namespace charty {
/*!
 * \class VisualItem
 *
 * \brief ͼ�������пɼ�������Ԫ�ض�Ҫ�̳��ڴ���
 *
 */
class ValueAxis : public Axis {
public:
    ValueAxis(Panel* parent = nullptr);

protected:
    virtual void onDraw(SkCanvas* canvas) override;
    virtual void preparePaints() override;

private:
    SkPaint* background_ = nullptr;
    SkPaint* text_ = nullptr;
    SkPaint* above_ = nullptr;
    SkPaint* below_ = nullptr;
    SkPaint* border_ = nullptr;
};
}
}

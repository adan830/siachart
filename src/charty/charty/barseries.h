#pragma once

#include "series.h"

namespace sia {
namespace charty {
/*!
 * \class Series
 *
 * \brief ��������չʾͼ�����������
 *
 */
class BarSeries : public Series {
public:
    BarSeries(const SkString& series_name);

protected:
    virtual void onDraw(SkCanvas* canvas) override;
    virtual void preparePaints() override;

private:
    SkPaint pat_;
};

}
}

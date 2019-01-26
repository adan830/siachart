#include "updownseries.h"
#include "SkPath.h"
#include "graphy/scene.h"
#include "mainview.h"

namespace sia {

namespace charty {

UpdownSeries::UpdownSeries(const SkString& series_name)
    :Series(SkString("updown_series"), series_name, SeriesType::kUpdownBar) {
    setZIndex(VisualItemZValue::kUpDownSeries);
}

void UpdownSeries::onDraw(SkCanvas* canvas) {
    if (!mainView() || !ready() || !calcPoints()) {
        return;
    }

    // ��Ҫ���ҵ�0ֵ����y����
    const SkScalar CONT_HEIGHT = contentHeight();
    const SkScalar Y_0 = mainView()->yPosOfValue(0);
    const SkScalar bar_width = barWidth(bar_width_);
    const SkScalar offset_x = bar_width / 2;
    for (auto& dp : datapos_) {
        auto& pts = dp.points;
        int idx = 0;
        for (const SkPoint& pt : pts) {
            if (!isValidPoint(pt)) {
                idx++;
                continue;
            }

            // ��ʵ���ֻ��Ҫ�ж�����ֵ�ͺ��ˣ���������ȥ��valueȥ�ж��ˡ�
            // ��Ϊ����0�ģ�y����һ����С��Y_0��(������ϵ�������valueֵ��С)

            // ��һ������
            SkRect rc;
            SkPaint* p_pat = nullptr;
            if (pt.y() >= Y_0) {    // value < 0
                // ��Y_0���»�
                rc = SkRect::MakeXYWH(pt.x() - offset_x, Y_0, bar_width, pt.y() - Y_0);
                p_pat = &down_pat_;
            } else {    // value > 0
                // ��Y_0���ϻ�
                rc = SkRect::MakeXYWH(pt.x() - offset_x, pt.y(), bar_width, Y_0 - pt.y());
                p_pat = &up_pat_;
            }

            canvas->drawRect(rc, *p_pat);
            setSensorArea(idx, rc);
            idx++;
        }
    }
}

void UpdownSeries::preparePaints() {
    auto& ss = style();

    up_pat_.setColor(ss.up_color);
    up_pat_.setStyle(SkPaint::kFill_Style);
    up_pat_.setStrokeWidth(ss.stroke_width);

    down_pat_ = up_pat_;
    down_pat_.setColor(ss.down_color);
}

}
}


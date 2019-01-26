#include "viewitem.h"
#include "mainview.h"
#include "chart.h"

namespace sia {
namespace charty {

ViewItem::ViewItem(ViewItemType typ, const SkString& name, MainView* parent)
    :SensorWidget(name, parent),vtype_(typ) {

}

ViewItem::~ViewItem() {
    SkEvent evt(ChartEventType::kViewItemDeleted);
    evt.setPtr("item", this);
    if (auto ctx = chartCtx()) {
        ctx->emitEvent(evt);
    }
}

SkScalar ViewItem::contentWidth() const {
    if (auto view = mainView()) {
        return view->contentWidth();
    }
    return SkIntToScalar(0);
}

SkScalar ViewItem::contentHeight() const {
    if (auto view = mainView()) {
        return view->contentHeight();
    }
    return SkIntToScalar(0);
}

bool ViewItem::rectOfKey(chart_key_t key, SkRect& rc_out) const {
    int idx = mainView()->visualIndexOfKey(key);
    if (-1 == idx) {
        return false;
    }

    rc_out = sensorArea(idx);
    return !rc_out.isEmpty();
}

void ViewItem::select(bool sel) {
    auto ctx = chartCtx();
    if (!ctx->functionEnabled(Chart::kFunctionSelectSeries)) {
        return;
    }

    flags_.set(ViewItemFlag::kSelected, sel);

    if (sel) {
        SkEvent evt(ChartEventType::kViewItemSeletChanged);
        evt.setPtr("item", this);

        // �����ź�
        chartCtx()->emitEvent(evt);
    }
}

bool ViewItem::selected() const {
    if (flags_.test(ViewItemFlag::kSelected)) {
        if (auto ctx = chartCtx()) {
            return ctx->functionEnabled(Chart::kFunctionSelectSeries);
        }
    }
    return false;
}

void ViewItem::onDraw(SkCanvas* canvas) {
    if (!selected()) {
        return;
    }

    // ������sensor�������������һ������
    static const int kHandleSize = 6;
    static SkPaint pat;
    pat.setColor(SK_ColorWHITE);

    // һ���������ܹ���25��handle���㹻�ˣ�
    // ���ǵ�һ�������һ��һ��Ҫ��
    int n_sensor = sensorCount();
    int span = std::max(1,  n_sensor / 25);
    forEachSensor([=](int idx, const SkRect& r)->bool {
        if ((idx % span == 0) || (idx == (n_sensor-1))) {
            // ÿ3����һ��handle
            SkRect d_r = SkRect::MakeXYWH(r.centerX() - kHandleSize / 2, r.centerY() - kHandleSize / 2, kHandleSize, kHandleSize);
            canvas->drawRect(d_r, pat);
        }
        return true;
    });

    SensorWidget::onDraw(canvas);
}

void ViewItem::onClickSensor(int idx, const SkPoint& pt) {
    select(true);
}

}

}


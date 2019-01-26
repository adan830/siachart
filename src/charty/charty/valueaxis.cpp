#include "valueaxis.h"
#include "mainview.h"
#include "panel.h"

namespace sia {

namespace charty {

ValueAxis::ValueAxis(Panel* parent)
    :Axis(SkString("value_axis"), parent) {

}

void ValueAxis::preparePaints() {
    background_ = declarePaint(SkString("background"));
    text_ = declarePaint(SkString("text"), SK_ColorWHITE, true);
    above_ = declarePaint(SkString("above_text"), SK_ColorRED, true);
    below_ = declarePaint(SkString("below_text"), SK_ColorGREEN, true);
    border_ = declarePaint(SkString("border"), SK_ColorRED);

    SkRect rc;
    if (getConfigGeometry(rc)) {
        setWidth(rc.width());
    }
}

void ValueAxis::onDraw(SkCanvas* canvas) {
    auto rc = rect();
    canvas->drawRect(rc, *background_);
    // �ұ߽�
    canvas->drawLine(rc.right() - 1, rc.top(), rc.right() - 1, rc.bottom(), *border_);
    // �ϱ߽�
    if (panel()->hasItem(PanelItemType::kPanelTitle)) {
        SkScalar y = border_->getStrokeWidth() / 2;
        canvas->drawLine(0, y, width(), y, *border_);
    }

    Title *title = panel()->title();
    const SkScalar y_offset = title ? title->height() : SkIntToScalar(0);	// Ҫ����title�ĸ߶�
    SkScalar text_offset_y = y_offset + textVerticalMiddle(*text_);// +contentRect().top();

    // ���ϵ��»���λ��Ϣ
    auto& vs = mainView()->valSlice();
    auto middle_val = vs.middleVal();
    static const SkScalar PADDING_RIGHT = SkIntToScalar(10);
    if (middle_val == chartbase::kInvalidDataValue) {
        for (const ValueSlice::PosVal& pv : vs.valPos()) {
            SkTextUtils::DrawString(canvas, pv.sval, width() - PADDING_RIGHT, pv.ypos + text_offset_y, *text_, SkTextUtils::kRight_Align);
        }
    } else {
        SkPaint* tmp = nullptr;
        for (const ValueSlice::PosVal& pv : vs.valPos()) {
            // ���ʹ���Ǹ�paint����
            if (pv.val > middle_val) {
                tmp = above_;
            } else if (pv.val < middle_val) {
                tmp = below_;
            } else {
                tmp = text_;
            }
            SkTextUtils::DrawString(canvas, pv.sval, width() - PADDING_RIGHT, pv.ypos + text_offset_y, *tmp, SkTextUtils::kRight_Align);
        }
    }

    // û�������ߵ�ʱ��Ż�����
    auto &gd = panel()->grid();
    if (!gd.hasHorizontalLine()) {
        SkPaint line_paint;
        line_paint.setColor(gd.gridColor());
        for (const ValueSlice::PosVal& pv : vs.valPos()) {
            SkScalar y = pv.ypos + y_offset; // Ҫ����title�ĸ߶�����Ӧƫ��
            canvas->drawLine(width() - 5, y, width() - 2, y, line_paint);
        }
    }
}

}

}


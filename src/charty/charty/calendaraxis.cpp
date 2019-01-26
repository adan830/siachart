#include "calendaraxis.h"
#include "mainview.h"
#include <ctime>

namespace sia {

namespace charty {

CalendarAxis::CalendarAxis(Panel* parent)
    :Axis(SkString("calendar"), parent) {

}

void CalendarAxis::onDraw(SkCanvas* canvas) {
    canvas->drawRect(rect(), *bk_);

    auto* ctx = chartCtx();
    SkTArray<chartbase::data_key_t> keys;
    if (!datasource()->keys(ctx->indexBegin(), ctx->indexEnd(), keys)) {
        return;
    }

    // �����ܹ��ж�����������Ҫ��ʾ��
    auto& xpos = mainView()->indexXPos();
    auto pos_count = xpos.count();
    if (pos_count != keys.count()) {
        return;
    }

    // text y pos
    SkScalar hh = height();
    SkScalar text_offset_y = textVerticalMiddle(*text_, hh);

    // �����ÿ����Ŀ��ʾռ�õĿ��
    SkString demo_cal = ctx->formatCalendar(20181018232912000);
    SkScalar w_item = text_->measureText(demo_cal.c_str(), demo_cal.size()) + 20; // spacing;
    // һ���ܻ����ٸ����ݣ�
    int max_item = std::min(pos_count, (int)(width() / w_item));

    // ������ٸ����л�һ����
    int pos_step = (pos_count / max_item) + 1;
    SkScalar x_offset = contentRect().left();
    const SkScalar border_offset = border_->getStrokeWidth() / 2;
    // ��ʼ��
    canvas->drawLine(0, border_offset, width(), border_offset, *border_);
    for (int i = 0; i < pos_count; i+=pos_step) {
        canvas->drawLine(xpos[i] + x_offset, 0, xpos[i] + x_offset, hh, *text_);
        canvas->drawString(ctx->formatCalendar(keys[i]), xpos[i] + x_offset + 5, text_offset_y, *text_);
    }
}

void CalendarAxis::preparePaints() {
    bk_ = declarePaint(SkString("background"));
    text_ = declarePaint(SkString("text"), SK_ColorRED, true);
    border_ = declarePaint(SkString("border"), SK_ColorRED);

    SkRect rc;
    if (getConfigGeometry(rc)) {
        setHeight(rc.height());
    }
}

}

}


#include "panelayout.h"
#include <iostream>
#include "graphy/widget.h"
#include "panel.h"

namespace sia {
namespace charty {

void dumpRect(const char* name, const SkRect& r);

void PanelLayout::setPanel(Panel* p) {
    panel_ = p;
}

bool PanelLayout::parse(const SkString& layout) {
    return true;
}

void PanelLayout::doLayout() {
    using namespace sia::graphy;
    SkRect rc = geometry();

    // ��mainview �Լ�value/ratio �������һ��padding
    static const SkScalar PADDING_BOTTOM = 10;

    PanelItem* value_axis = reinterpret_cast<PanelItem*>(widget("value_axis"));
    const SkScalar value_axis_width = value_axis ? value_axis->width() : SkIntToScalar(0);

    PanelItem* ratio_axis = reinterpret_cast<PanelItem*>(widget("ratio_axis"));
    const SkScalar ratio_width = ratio_axis ? ratio_axis->width() : SkIntToScalar(0);

    PanelItem* title = reinterpret_cast<PanelItem*>(widget("title"));
    const SkScalar title_height = title ? title->height() : SkIntToScalar(0);

    PanelItem* calendar = reinterpret_cast<PanelItem*>(widget("calendar"));
    const SkScalar calendar_height = calendar ? calendar->height() : SkIntToScalar(0);

    const SkScalar MAIN_VIEW_WIDTH = rc.width() - value_axis_width - ratio_width;
    const SkScalar MAIN_VIEW_HEIGHT = rc.height() - title_height - calendar_height;
    // ��ʼ����
    // title
    //! ע�⣺ContentArea һ��Ҫ��setGeometry��Ч֮ǰ���ã��������������
    // ��Ϊgeometry�ı䵼�¿ؼ������ػ档��chart�Ŀؼ��ػ�ʹ�õ�contentArea��С��
    // ���ʱ��contentArea�����úþͼ��������
    SkRect rc_w = SkRect::MakeXYWH(value_axis_width, 0, MAIN_VIEW_WIDTH, title_height);
    if (title) {
        title->setContentArea(SkRect::MakeXYWH(0, 0, rc_w.width(), rc_w.height()));
        title->setGeometry(rc_w);
    }
    // title->setContentArea(SkRect::MakeXYWH(value_axis_width, 0, MAIN_VIEW_WIDTH, title_height));
    dumpRect("title", rc_w);

    // value_axis
    rc_w = SkRect::MakeXYWH(0, 0, value_axis_width, rc.height() - calendar_height);
    if (value_axis) {
        value_axis->setContentArea(SkRect::MakeXYWH(0, title_height, rc_w.width(), rc_w.height() - PADDING_BOTTOM));
        value_axis->setGeometry(rc_w);
    }
    dumpRect("value_axis", rc_w);

    // ratio_axis
    rc_w = SkRect::MakeXYWH(rc.width() - ratio_width, 0, ratio_width, rc.height() - calendar_height);
    if (ratio_axis) {
        ratio_axis->setContentArea(SkRect::MakeXYWH(0, title_height, rc_w.width(), rc_w.height() - PADDING_BOTTOM));
        ratio_axis->setGeometry(rc_w);
    }
    dumpRect("ratio_axis", rc_w);

    // calendar
    rc_w = SkRect::MakeXYWH(0, rc.height() - calendar_height, rc.width(), calendar_height);
    if (calendar) {
        calendar->setContentArea(SkRect::MakeXYWH(value_axis_width, 0, MAIN_VIEW_WIDTH, calendar_height));
        calendar->setGeometry(rc_w);
    }
    dumpRect("calendar", rc_w);

    // mainview ������series֮ǰ�������ô�С��
    // ����series������λ����ʾ����
    PanelItem* mainview = reinterpret_cast<PanelItem*>(widget("mainview"));
    rc_w = SkRect::MakeXYWH(value_axis_width, title_height, MAIN_VIEW_WIDTH, MAIN_VIEW_HEIGHT);

    SkRect rc_cont = SkRect::MakeXYWH(0, 0, MAIN_VIEW_WIDTH, MAIN_VIEW_HEIGHT - PADDING_BOTTOM);
    if (mainview) {
        mainview->setContentArea(rc_cont);
        mainview->setGeometry(rc_w);
    }

    if (panel_) {
        // ���еĸ�ͼ����������ͼ֮�ϣ�
        for (auto p : panel_->subViews()) {
            p->setContentArea(rc_cont);
            p->setGeometry(rc_w);
        }
    }
    dumpRect("mainview", rc_w);
}

}
}

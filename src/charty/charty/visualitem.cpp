#include "visualitem.h"
#include "graphy/paintman.h"
#include "chartman.h"

namespace sia {

namespace charty {

VisualItem::VisualItem(const SkString& name, graphy::Widget* parent)
    :graphy::Widget(name, parent) {

    setVisibleP(true);
}

bool VisualItem::ready() const {
    if (!paint_ready_) {
        return false;
    }

    if (auto ds = datasource()) {
        return ds->ready();
    }
    return false;
}

void VisualItem::setChartContext(ChartContext* t) {
    ctx_ = t;
    forVisualChildren([=](VisualItem* c) {
        c->setChartContext(t);
        return true;
    });
    refreshPaints();
}

Dict* VisualItem::dictionary() const {
    return ChartMan::instance()->dict();
}

void VisualItem::preparePaints() {
//     forVisualChildren([](VisualItem* c) {
//         c->preparePaints();
//         return true;
//     });
}

SkScalar VisualItem::textVerticalMiddle(const SkPaint& paint, SkScalar height) {
    SkRect rc = textRect(SkString(u8"159����"), paint);
    return (height + rc.height()) / 2;
}

SkRect VisualItem::textRect(const SkString& str, const SkPaint& paint,
                            const SkPoint& left_top /*= SkPoint::Make(SkIntToScalar(0), SkIntToScalar(0))*/) {
    SkRect rc;
    paint.measureText(str.c_str(), str.size(), &rc);
    rc.offsetTo(left_top.x(), left_top.y());
    return rc;
}

void VisualItem::drawMultilineText(SkCanvas* canvas, const SkPaint& paint,
                                   const SkString& text, const SkRect& rc, SkScalar spacing) {
    SkTArray<SkString> lines;
    SkStrSplit(text.c_str(), "\n", SkStrSplitMode::kStrict_SkStrSplitMode, &lines);

    if (lines.empty()) {
        return;
    }

    // ���߶�
    const SkScalar LINE_MIDDLE = textVerticalMiddle(paint);
    const SkScalar LINE_HEIGHT = LINE_MIDDLE * 2 + spacing;

    SkScalar y = LINE_HEIGHT + rc.top();
    for (SkString& line : lines) {
        canvas->drawString(line, rc.left() + spacing, y, paint);
        y += LINE_HEIGHT;
    }
}

void VisualItem::forVisualChildren(std::function<bool(VisualItem*)> cb) {
    if (!cb) {
        return;
    }

    for (auto *w : children()) {
        if (VisualItem* v = dynamic_cast<VisualItem*>(w)) {
            if (!cb(v)) {
                break;
            }
        }
    }
}

SkPaint* VisualItem::declarePaint(const SkString& name, SkColor def_color /*= SK_ColorWHITE*/,
                                  bool antialias /*= false*/) {
    if (painters_) {
        if (auto ret = painters_->getPaint(name)) {
            return ret;
        }
    }

    // �����ǵĻ����в���һ��ͬ����
    auto i = paints_.find(name);
    if (i != paints_.end()) {
        return i->second.get();
    }

    // �´���һ��
    my_sp<SkPaint> new_p = std::make_shared<SkPaint>();
    new_p->setColor(def_color);
    new_p->setAntiAlias(antialias);
    // new_p->setTextAlign(textAlign);
    paints_.insert({name, new_p});
    return new_p.get();
}

bool VisualItem::getConfigGeometry(SkRect& rc) {
    if (painters_) {
        return painters_->getGeometry(rc);
    }
    return false;
}

bool VisualItem::onEvent(const SkEvent& evt) {
    bool handled = false;
    if (evt.isType(ChartEventType::kSiaSeriesStyleChanged)
            || evt.isType(ChartEventType::kSiaThemeChanged)) {
        refreshPaints();
        handled = true;
    }
    return handled;
}

void VisualItem::refreshPaints() {
    // һ��Ҫ�Ƚ�֮ǰ������������������ͷ��ڴ棡
    painters_ = nullptr;

    // ��������׼��paint
    paint_ready_ = false;
    // ���֮ǰ��������ʱpaint
    paints_.clear();

    // �ҵ��Լ���Ӧ��painter��
    if (auto man = sia::graphy::PaintMan::instance()) {
        SkString group_name("@paint/");
        group_name.append(name());
        painters_ = man->paintGroup(group_name, false);
    }

    // ��������׼���Լ���paints
    preparePaints();

    // ��ʱ���ſ��������Ŀ�ʼ����Ļ���
    paint_ready_ = true;
}

}
}


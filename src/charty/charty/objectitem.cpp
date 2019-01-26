#include "objectitem.h"
#include "mainview.h"
#include "viewitemtooltip.h"
#include "graphy/scene.h"

namespace sia {
namespace charty {

ObjectItem::ObjectItem(const SkString& name, MainView* parent)
    :ViewItem(ViewItemType::kViewObject, name, parent) {
    setZIndex(VisualItemZValue::kObjects);
}

ObjectItem::~ObjectItem() {

}

void ObjectItem::setParam(ObjectParam para, my_sp<IVisualObject> obj) {
    para_ = para;
    obj_ = obj;

    switch (para.postyp) {
    case PosType::kAttached:
        pos_calc_.setAttached(mainView()->viewItem(para.attach), para.x);
        break;

    case PosType::kRelated:
        pos_calc_.setRelatePos(para.relpos);
        break;

    case PosType::kValued:
        pos_calc_.setDataValue(para.val);
        break;
    }

    if (obj_) {
        obj_->buildPaint();
    }
    reCalc();
}

bool ObjectItem::isAttachedTo(ViewItem* item) const {
    if (para_.postyp == PosType::kAttached) {
        return (pos_calc_.attachedView() == item);
    }
    return false;
}

void ObjectItem::onDraw(SkCanvas* canvas) {
    reCalc();
    // draw
    if (obj_ && !object_visual_rc_.isEmpty()) {

        if (para_.postyp == PosType::kAttached) {
            //! ��attached�� series��λ�ã���PosType::kAttached��ʱ����Ч
            SkRect attached_rc;
            SkPoint attached_pt;
            SkPoint object_pt;
            pos_calc_.getAttachedItem(attached_rc, attached_pt, object_pt);

            // for debug
            SkPaint paint;
            paint.setColor(SK_ColorWHITE);
//             canvas->drawRect(attached_rc, paint);

            // ��ע������
            canvas->drawLine(attached_pt, object_pt, paint);
        }

        // ��object
        obj_->draw(canvas, object_visual_rc_);
    }
    ViewItem::onDraw(canvas);
}

void ObjectItem::onSizeChange() {
    reCalc();

    ViewItem::onSizeChange();
}

void ObjectItem::reCalc() {
    if (pos_calc_.calc(mainView(), para_.sz, object_visual_rc_)) {
        // ��Ӧ����һ������򣬿�visual index
        setOnePieceSensor(object_visual_rc_);
    } else {
        // �����Լ�Ϊ�գ������ײ��⵽�Ͳ����ٵ��û�����
    }
}

void ObjectItem::onSensed(int idx, const SkPoint& pt) {
    // ��ʾtooltip
    if (auto scene = dynamic_cast<graphy::Scene*>(root())) {
        if (ViewItemTooltip* tip = mainView()->seriesTip()) {
            if (!para_.tooltip.isEmpty()) {
                SkPoint pt_win = clientToWindow(pt);
                tip->moveTo(pt_win.x(), pt_win.y());
                tip->setTitle(SkString(""));
                tip->addBodyItem(para_.tooltip);
                scene->requestTooltip(tip);
            } else {
                scene->hideTooltip();
            }
        }
    }
}

}
}


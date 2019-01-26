#include "widget.h"
#include "layout.h"

namespace sia {
namespace graphy {

Widget::Widget(const SkString& name/* = ""*/, Widget* parent /*= nullptr*/) {
    // һ��Ҫ���úã�����debugģʽ�ᵼ��layout���㲻��ȷ
    pos_.set(0, 0);

    setName(name);
    setParent(parent);
}

Widget::~Widget() {
    // ���Ƴ������еĺ��ӽڵ�
    for (auto ch : children_) {
        ch->setParent(nullptr);
    }
    children_.clear();

    // �ٴӸ��׽ڵ����Ƴ�
    if (Widget* p = parent()) {
        p->removeChild(this);
    }
}

void Widget::addChild(Widget* w) {
    if (!w || childExist(w)) {
        return;
    }

    children_.push_back(w);
    w->setParent(this);
    w->onAttach();
    sortChildren();

    relayout();
}

void Widget::setLayout(Layout* l) {
    layout_ = l;
    relayout();
}

void Widget::relayout() {
    if (layout_) {
        if (layout_->decoratedWidget() != this) {
            // ��������ڲ��Ѿ�������doLayout��
            layout_->setDecoratedWidget(this);
        } else {
            layout_->doLayout();
        }
        onLayoutChange();
    }
}

void Widget::setName(const SkString& name) {
    name_ = name;
}

void Widget::forEachChild(std::function<bool(Widget*)> onChild) const {
    if (!onChild) {
        return;
    }

    for (Widget* w : children_) {
        if (!onChild(w)) {
            break;
        }
    }
}

void Widget::removeChild(Widget* ch) {
    if (!ch) {
        return;
    }

    for (auto i = children_.begin(); i != children_.end(); ++i) {
        if (*i == ch) {
            children_.erase(i);
            ch->setParent(nullptr);
            relayout();
            break;
        }
    }
}

Widget* Widget::root() const {
    Widget *p = const_cast<Widget*>(this);
    while (true) {
        if (!p->parent()) {
            return p;
        }
        p = p->parent();
    }
    return nullptr;
}

void Widget::onChildZIndexChange(Widget* w) {
    sortChildren();
}

void Widget::broadcast(const SkEvent& evt) {
    doEvent(evt);

    forEachChild([&](Widget* c)->bool {
        c->broadcast(evt);
        return true;
    });
}

void Widget::setParent(Widget* pa) {
    if (parent_ == pa) {
        return;
    }

    // ֪ͨԭ���ĸ��׽ڵ�ɾ������
    if (parent_ && parent_->childExist(this)) {
        parent_->removeChild(this);
    }

    if (pa) {
        // ֪ͨ���ڵĸ�������µĺ���
        pa->addChild(this);
    }

    // ��¼�Լ��ĸ��׽ڵ�
    parent_ = pa;
}

SkRect Widget::rect() const {
    return SkRect::MakeXYWH(0, 0, width(), height());
}

SkRect Widget::geometry() const {
    return SkRect::MakeXYWH(pos_.x(), pos_.y(), width(), height());
}

SkRect Widget::windowGeometry() const {
    SkPoint pt = clientToWindow(SkPoint::Make(0, 0));
    return SkRect::MakeXYWH(pt.x(), pt.y(), width(), height());
}

SkPoint Widget::windowToClient(SkPoint pt) const {
    const Widget* p = this;
    do {
        SkPoint of = p->pos();
        pt.offset(-of.x(), -of.y());

        // һֱ�ҵ����ϲ㣨������Ϊ���ϲ����window����
        p = p->parent();
    } while (p);
    return pt;
}

SkPoint Widget::clientToWindow(SkPoint pt) const {
    const Widget* p = this;
    do {
        SkPoint of = p->pos();
        pt.offset(of.x(), of.y());

        p = p->parent();
    } while (p);
    return pt;
}

void Widget::setGeometry(const SkRect& r) {
    moveTo(r.x(), r.y());
    setSize(r.width(), r.height());
}

void Widget::moveTo(SkScalar x, SkScalar y) {
    pos_.fX = x;
    pos_.fY = y;
}

void Widget::setZIndex(int16_t index) {
    z_index_ = index;

    if (parent_) {
        parent_->onChildZIndexChange(this);
    }
}

void Widget::onDestroy() {
    if (parent_) {
// 		SkEvent evt;
// 		parent_->doEvent(evt);
    }
}

void Widget::onSizeChange() {
    relayout();
}

void Widget::onDraw(SkCanvas* canvas) {
    for (Widget* c : children_) {
        // ����Ҫ���������ƫ�Ƶļ���
        c->draw(canvas, c->pos());
    }
    SkView::onDraw(canvas);
}

Widget* Widget::hitTest(int x, int y) {
    // �ȼ�ʵ��һ����ʹ�ñ����ķ���
    // TODO: ʹ��BSP ������
    // ���Ӱ���zindex��С�����˳���������Ժ�����������棬�����ȴ�����Ϣ
    Widget* target = nullptr;
    for (auto i = children_.rbegin(); i != children_.rend(); ++i) {
        target = *i;
        if (target->geometry().contains(SkIntToScalar(x), SkIntToScalar(y))) {
            return target;
        }
    }
    return nullptr;
}

bool Widget::onMouse_internal(int x, int y, InputState state, uint32_t modifiers) {
    if (onMouse(x, y, state, modifiers)) {
        return true;
    }

    // ����Լ���������ô�͸������Ǵ���
    if (Widget* w = hitTest(x, y)) {
        SkPoint pt = w->pos();
        return w->onMouse_internal(x - SkScalarFloorToInt(pt.x()), y - SkScalarFloorToInt(pt.y()), state, modifiers);
    }
    return false;
}

bool Widget::onMouseWheel_internal(float delta, uint32_t modifiers) {
    if (onMouseWheel(delta, modifiers)) {
        return true;
    }

    // ����Լ���������ô�͸������Ǵ���
//     if (Widget* w = hitTest(x, y)) {
//         SkRect rc = w->geometry();
//         return w->onMouseWheel_internal(delta, modifiers);
//     }
    return false;
}

void Widget::sortChildren() {
    std::stable_sort(children_.begin(), children_.end(), [](Widget* w1, Widget* w2)->bool {
        return w1->zindex() < w2->zindex();
    });
}

bool Widget::childExist(Widget* w) const {
    return std::find(children_.begin(), children_.end(), w) != children_.end();
}

}
}

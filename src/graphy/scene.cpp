#include "scene.h"
#include <cassert>

namespace sia {
namespace graphy {

// void drawTooltip(SkCanvas* canvas, const SkString& text, const SkRect& rc) {
//     SkPaint p1;
//     p1.setColor(SkColorSetARGB(0xAA, 0xff, 0xff, 0xff));
//     canvas->drawRect(rc, p1);
//
//     SkPaint p2;
//     p2.setColor(SK_ColorBLACK);
//     canvas->drawString(text, rc.left(), rc.top() + 10, p2);
// }

Scene::Scene(const SkString& name /*= SkString()*/)
    :Widget(name) {

}

void Scene::paint(SkCanvas* canvas) {
    // canvas->clear(SK_ColorWHITE);
    if (!canvas) {
        return;
    }

    // ���Լ��ĺ��ӽڵ�
    draw(canvas);

    // ��������еĲ�֮�ϣ�����Tooltip
    // drawTooltip(canvas, SkString(u8"Hello ����"), SkRect::MakeXYWH(_mouse_x, _mouse_y, 100, 100));
    if (tooltip_ && !(tooltip_->parent())) {
        tooltip_->draw(canvas, tooltip_->pos());
    }
}

bool Scene::onMouse_internal(int x, int y, InputState state, uint32_t modifiers) {
    bool handled = false;
    SkPoint pt = SkPoint::Make(SkIntToScalar(x), SkIntToScalar(y));
    switch (state) {
    case InputState::kDown_InputState:
        tmd_.record(pt);
        if (tmd_.totalDuration() >= clickDuration()) {
            tmd_.reset();
            tmd_.record(pt);
        }
        break;

    case InputState::kUp_InputState:
        // ��¼
        tmd_.record(pt);

        // �ȷ�һ��up��Ϣ
        handled = Widget::onMouse_internal(x, y, state, modifiers);

        // �����Ƿ���Ҫ��һ��click��Ϣ
        if (tmd_.recordCount() == 2) {
            this->onMouse_internal(x, y, InputState::kClick_InputState, modifiers);
        }

        // �ٿ����ǲ�����Ҫ��һ��double click��Ϣ
        if (tmd_.recordCount() >= 4) {
            if (tmd_.totalDuration() <= clickDuration()) {
                this->onMouse_internal(x, y, InputState::kDoubleClick_InputState, modifiers);
            } else {
                // �����double click����ô��Ҫ��һ��������Ϣ
                this->onMouse_internal(x, y, InputState::kClick_InputState, modifiers);
            }
            tmd_.reset();
        }
        break;
    }

    if (state != InputState::kUp_InputState) {
        return Widget::onMouse_internal(x, y, state, modifiers);
    }
    // scene ���Ա���Ϊ�����ϲ㣬������ΪֻҪ��Ϣ�����������һ����������ˡ����������´����ˣ�
    return handled;
}

bool Scene::onMouseWheel_internal(float delta, uint32_t modifiers) {
    return Widget::onMouseWheel_internal(delta, modifiers);
}

bool Scene::onKey(sia::KeyBoard k, InputState state, uint32_t modif) {
    return Widget::onKey(k, state, modif);
}

void Scene::requestTooltip(Widget* tip) {
    if (tip->parent()) {
        // tooltip��ֹ���ø��ף�
        assert(false);
        return;
    }

    tooltip_ = tip;
}

void Scene::hideTooltip() {
    tooltip_ = nullptr;
}

void Scene::setSurface(sk_sp<SkSurface> surf) {
    surf_ = surf;

    onSurfaceChanged();
}

}
}
#include "sensorwidget.h"
#include "mainview.h"

namespace sia {
namespace charty {

SensorWidget::SensorWidget(const SkString& name, MainView* parent)
    :VisualItem(name, (Widget*)parent) {
}

MainView* SensorWidget::mainView() const {
    return dynamic_cast<MainView*>(parent());
}

Panel* SensorWidget::panel() const {
    if (auto *view = mainView()) {
        return view->parent();
    }
    return nullptr;
}

void SensorWidget::forEachSensor(std::function<bool(int, const SkRect&)> func) const {
    if (!func) {
        return;
    }

    int n = area_.count();
    for (int i = 0; i < n; ++i) {
        if (!func(i, area_.get(i))) {
            break;
        }
    }
}

bool SensorWidget::onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
    if (state != InputState::kMove_InputState
            && state != InputState::kClick_InputState) {
        return false;
    }

    // ��������ȫ���ǵ�MainView�ϵģ����Բ��ý�������ת����
    int idx = mainView()->visualIndexOfPos(SkIntToScalar(x));
    if (idx < 0) {
        return false;
    }

    // �Ƿ��ڸ�Ӧ���ڣ�
    SkPoint pt = SkPoint::Make(SkIntToScalar(x), SkIntToScalar(y));
    if (!inSensorArea(idx, pt)) {
        return false;
    }

    std::cout << "sensor widget move:{" << x << "," << y << "}:" << idx << std::endl;

    // �ڸ�Ӧ���Ļ�����Ҫ��ʾ��
    // �����Ȳ�Ҫ����ˢ�¸�Ч���ֱ����ʾ�ͺ��ˣ�
    if (InputState::kMove_InputState == state) {
        onSensed(idx, pt);
    } else if (InputState::kClick_InputState == state) {
        onClickSensor(idx, pt);
    }
    return true;
}

}

}


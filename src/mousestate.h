#pragma once

#include "bitflag.h"
#include "graphy/event.h"

namespace util {

/*!
 * \class MouseStateMachine
 *
 * \brief ������״̬���򻯳����з�ɢ���߼��ж�
 *		�ݲ�֧���������Ҽ��������ж�
 */
class MouseStateMachine {
    enum Flags {
        //! ��갴��
        kButtonPressed = 1 << 1,

        //! ��ק�У���갴�����ƶ�
        kDragging = 1 << 2,

        //! ��ק��������ק�У���갴��״̬����
        kDragEnd = 1 << 3,
    };

public:
    void onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
        switch (state) {
        case sia::InputState::kDoubleClick_InputState:
        case sia::InputState::kClick_InputState:
            flags.clear();
            break;

        case sia::InputState::kDown_InputState:
            flags.set(Flags::kButtonPressed, true);
            break;

        case sia::InputState::kMove_InputState:
            if (flags.test(Flags::kButtonPressed)) {
                flags.set(Flags::kDragging, true);
            }
            break;

        case sia::InputState::kUp_InputState:
            if (dragging()) {
                flags.set(Flags::kDragEnd, true);
            }
            // �����϶��Ͱ���״̬��
            flags.set(Flags::kButtonPressed | Flags::kDragging, false);
            break;
        }
    }

    inline bool dragged()const {
        return flags.test(Flags::kDragEnd);
    }

    inline bool dragging()const {
        return flags.test(Flags::kDragging);
    }

    inline bool pressing()const {
        return flags.test(Flags::kButtonPressed);
    }

private:
    BitFlags flags;
};

}


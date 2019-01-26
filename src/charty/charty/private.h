#pragma once

#include "chart.h"
#include "chartlayout.h"
#include "crosshair.h"

namespace sia {
namespace charty {

struct ChartPrivate {
private:
    struct ViewCache {
        sk_sp<SkImage> snap;
        sk_sp<SkSurface> surf;

        // �ĸ���
        bool snapshot(SkCanvas* canvas);

        // ����
        bool draw(SkCanvas* canvas);

        inline bool hasSnap()const {
            return !!snap;
        }
    };

public:
    enum Flags : int32_t {
        //! star
        kMinFlagShift = Chart::FunctionType::kFunctionMaxShift,

        //! �������ˣ�Ҫ�����µ�ͼ�Σ���ȫ���ػ�
        kDataDirty = 1 << (kMinFlagShift + 1),

        //! ��ͼ���ˣ�ֻ��Ҫ�ػ���ͼ������ǣ�����
        kViewDirty = 1 << (kMinFlagShift + 2),

        //! �������ˣ�Ҫ�����µ�ͼ�Σ���ȫ���ػ�
        kDomainChanged = 1 << (kMinFlagShift + 3),
    };

    // ���ֹ���
    ChartLayout layout;

    // ÿ��ͼ���ʵ������һ��id
    // id�ľ��庬����ҵ�����ȷ��
    std::string id;

    // ÿ��ͼ�����һ������panel
    // ͼ�����˳������
    std::vector<Panel*> panels;

    // ������
    ChartContext ctx;

    //! ��־
    BitFlags flags;

    //! ʮ����
    CrossHair cross;

    //! ��ǰѡ�е�ͼԪ
    ViewItem* selected = nullptr;

    //! ���״̬���
    util::MouseStateMachine mouse_state;

    //! ��ͼ����
    ViewCache vcache;
};

}
}

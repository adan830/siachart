#pragma once

#include "siapi.h"
#include "widget.h"
#include "deps/util.h"
#include "SkSurface.h"

// Scene ���ڹ�������������ͼ
// ����ϵͳSurface/Window������
namespace sia {
namespace graphy {

class Tooltip;
class GRAPHY_API Scene : public Widget {
public:
    Scene(const SkString& name = SkString());

    virtual void paint(SkCanvas* canvas);

    // ���ǵ�widget��ͬ���������Ա�����ṩ���õĺ���
    bool onMouse_internal(int x, int y, InputState state, uint32_t modifiers);
    bool onMouseWheel_internal(float delta, uint32_t modifiers);

    bool onKey(sia::KeyBoard k, InputState state, uint32_t modif);

    /*! \brief �����������������ڴ�ֵ��������Ϊ��˫��
     *
     *    @return
     */
    inline int32_t clickDuration()const {
        return 500; // msec
    }

    /*! \brief ������ʾһ��Tooltip
     *	     scene������һ���������ڵ����׶λ������Tooltip����ȷ��������������Ԫ��֮�ϡ�
     *       ���Ƶ�λ����Ҫʵ�����ú�Tooltip��geometry����Scene����Ϊ���գ�
     *    @param
     *    @return
     */
    void requestTooltip(Widget* tip);

    /*! \brief ����tooltip
     *
     *    @param
     *    @return
     */
    void hideTooltip();

    void setSurface(sk_sp<SkSurface> surf);

    sk_sp<SkSurface> surface()const {
        return surf_;
    }

protected:
    virtual void onSurfaceChanged() {};

private:
    sk_sp<SkSurface> surf_;

    util::StopWatch<SkPoint> tmd_;
    Widget* tooltip_ = nullptr;
};
}
}

#pragma once

#include "visualitem.h"
#include "mousestate.h"

namespace sia {
namespace charty {
class Panel;
class CrosshairDetail;

/*!
 * \class CrossHair
 *
 * \brief ͼ�����ʮ������ʾ
 *      һ��Ҫ�ѱ�ʵ����ȫ���ǵ�ͼ���ϲſ�����ȷ����
 *
 *	ʹ��key�����ҷ������������ʱ��ʮ����ʼ������������panel�ϣ�
 */
class CrossHair : public VisualItem {
public:
    enum Flags {
        //! ��ʾʮ����
        kHasCross = 0x4,

        //! ��ʾ��ѡ
        kHasRubber = 0x8,
    };

    CrossHair(/*Chart* parent = nullptr*/);

    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

    virtual bool onKey(sia::KeyBoard key, sia::InputState state, uint32_t modifiers) override;

    void setPanel(Panel* main_p, Panel* cur_p);

    bool curKeyVal(int& idx, SkString& key, SkString& val);

    /*! \brief �ƶ�ʮ����
     *
     *    @param    offset    >0 �����ƶ��� <0 �����ƶ�, abs(offset) �ƶ���index����
     *    @return false �Ѿ��޷��ƶ��ˣ�
     */
    bool move(int offset);

    /*! \brief ʮ�����ƶ���ʱ��Ӧ�����ϵ��ĸ������ϣ�
     *
     *    @param
     *    @return
     */
    void attachDataname(const SkString& name) {
        dataname_ = name;
    }

    void setDetail(CrosshairDetail* d) {
        cross_detail_ = d;
    }

    void hide() {
        flags_.clear();
    }

    //! CrossHair::Flags
    void show(Bits bits);

    inline bool flag(Bits b)const {
        return flags_.test(b);
    }

    inline bool showing()const {
        return flags_.test(Flags::kHasCross | Flags::kHasRubber);
    }

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    virtual void preparePaints() override;

    void doRubberSelect();

    void drawTag(SkCanvas* canvas, const SkRect& rc, const SkString& str) const;

    void drawLineDetail(SkCanvas* canvas, int idx);

private:
    SkPaint *line_ = nullptr;
    SkPaint *rubber_ = nullptr;
    SkPaint *tag_back_ = nullptr;
    SkPaint *tag_text_ = nullptr;
    SkPaint *tag_border_ = nullptr;

    SkPaint *detail_border_ = nullptr;
    SkPaint *detail_background_ = nullptr;
    SkPaint *detail_title_ = nullptr;
    SkPaint *detail_value_ = nullptr;

    // detail ���Ĵ�С
    SkSize detail_size_;

    bool pressed_ = false;
    SkPoint pt_lt_;
    SkPoint pt_cur_;

    BitFlags flags_;

    //! ��ǰʮ�������ĸ�panel��
    Panel* cur_panel_ = nullptr;

    //! chart����panel
    Panel* main_panel_ = nullptr;

    //! ʮ�������ҵ�����ʱ�����ϵ��ĸ�data��
    SkString dataname_;

    //! ʮ��������λ�õ���ϸ����
    CrosshairDetail* cross_detail_ = nullptr;

    // ���״̬���
    util::MouseStateMachine mouse_state_;
};
}
}

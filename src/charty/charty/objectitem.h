#pragma once

#include "viewitem.h"
#include "viewobject.h"
#include "objectpos.h"

namespace sia {
namespace charty {

/*!
 * \class ObjectItem
 *
 * \brief ϡ����ͼ���
 *
 */
class ObjectItem : public ViewItem {
public:
    ObjectItem(const SkString& name, MainView* parent);

    virtual ~ObjectItem();

    void setParam(ObjectParam para, my_sp<IVisualObject> obj);

    /*! \brief ��object�Ƿ�������������viewitem��
     *
     *    @param	item �Ƿ�����
     *    @return
     */
    bool isAttachedTo(ViewItem* item)const;

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    virtual void onSizeChange() override;

    void reCalc();

    virtual void onSensed(int idx, const SkPoint& pt) override;

private:
    // �����λ����Ϣ
    ObjectParam para_;

    //! �����Ļ��ƶ���
    my_sp<IVisualObject> obj_ = nullptr;

    //! λ�ü���
    PositionCalc pos_calc_;

    //! ��Ч��
    SkRect object_visual_rc_;
};

}
}

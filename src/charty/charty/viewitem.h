#pragma once

#include "sensorwidget.h"

namespace sia {
namespace charty {

enum class ViewItemType {
    kNotSet,
    kViewSeries,
    kViewObject,
};

/*!
 * \class ViewItem
 *
 * \brief ��ȫ������MainView�ϵ���ͼ
 *
 *
 */
class ViewItem : public SensorWidget {
protected:
    enum ViewItemFlag {
        kSelected = 1 << 1,

        // �˴������õ�flag����
        kMaxFlagShift = 8,
    };
public:
    ViewItem(ViewItemType typ, const SkString& name, MainView* parent);

    virtual ~ViewItem();

    SkScalar contentWidth()const;

    SkScalar contentHeight()const;

    inline ViewItemType viewType()const {
        return vtype_;
    }

    /*! \brief ��ȡkey���е�ǰ��View�е���Ч��������Ŀǰ�õ���Sensor����
     *
     *    @param    key ����key
     *              rc_out sensor��
     *
     *    @return   ���key�ڵ�ǰ������û����ʾ���򷵻�false
     */
    virtual bool rectOfKey(chart_key_t key, SkRect& rc_out) const;

    /*! \brief ѡ��Series
     *
     *    @param	sel		ѡ�����
     *    @return
     */
    void select(bool sel);

    inline bool selected()const;

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    virtual void onClickSensor(int idx, const SkPoint& pt) override;

protected:
    BitFlags flags_;

private:
    ViewItemType vtype_ = ViewItemType::kNotSet;
};
}
}

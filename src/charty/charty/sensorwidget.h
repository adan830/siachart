#pragma once

#include "visualitem.h"
#include "validarea.h"

namespace sia {
namespace charty {

class MainView;
class Panel;

/*!
 * \class SensorWidget
 *
 * \brief �ܹ���֪�����ͣ�Ŀؼ����������MainView�У��ұ�����ȫ����MainView�ſ��ԣ�
 *
 */
class SensorWidget : public VisualItem {
    friend class MainView;
    enum _SensorFlags {
        //! �Ƿ����ڸ�Ӧ����
        kInSensing = 0x1,

        // �Ƿ�����ʾ��Ӧ
        kSensorActived = 0x2,
    };

public:
    SensorWidget(const SkString& name, MainView* parent);

    inline void resetSensorAreas(int n) {
        area_.reset(n);
    }

    inline void setSensorArea(int idx, const SkRect& rc) {
        area_.set(idx, rc);
    }

    inline void setSensorArea(int idx, const SkPoint& pt_center, SkScalar width, SkScalar height) {
        auto rc = SkRect::MakeXYWH(pt_center.x() - width / 2, pt_center.y() - height / 2, width, height);
        setSensorArea(idx, rc);
    }

    inline const SkRect& sensorArea(int idx)const {
        return area_.get(idx);
    }

    inline void setRegion(const SkRegion& pa) {
        r1_ = pa;
    }

    inline void setOnePieceSensor(const SkRect& rc) {
        area_.setOne(rc);
    }

    inline bool inSensorArea(int idx, const SkPoint& pt) const {
        // return r1_.contains(pt.x(), pt.y());
        return area_.contains(idx, pt);
    }

    /*! \brief ÿ��indexռ�õĿ��
     *
     *    @return
     */
    inline SkScalar indexWidth()const {
        return index_width_;
    }

    inline void setIndexWidth(SkScalar w) {
        index_width_ = w;
    }

    MainView* mainView() const;

    Panel* panel() const;

    void forEachSensor(std::function<bool(int, const SkRect&)> func)const;

    inline int sensorCount()const {
        return area_.count();
    }

protected:
    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

    /*! \brief ���������Ӧ���ڵ�ʱ�����ô���Ϣ
     *
     *    @param
     *    @return
     */
    virtual void onSensed(int idx, const SkPoint& pt) {

    }

    /*! \brief �ڸ�Ӧ���ڵ��
     *
     *    @param
     *    @return
     */
    virtual void onClickSensor(int idx, const SkPoint& pt) {

    }

private:
    //! ������ͼ����ȱ��ȷ֣�ÿһ��ռ�õĿ�ȴ�С
    SkScalar index_width_ = 1.f;

    // ��index��¼��Ч��
    ValidArea area_;

    // �Ƿ��Ѿ���ʾ��Ӧ��
    // BitFlags sflags_;
    SkRegion r1_;
};

}
}

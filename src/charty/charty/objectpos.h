#pragma once

#include "viewobject.h"

namespace sia {
namespace charty {
class MainView;
class ViewItem;

/*!
 * \class ObjectPosCalc
 *
 * \brief �����������
 *
 *
 */
class PositionCalc {
public:
    /*! \brief �����������ʵ�geometry��
     *	    ��֧����Ը������ļ���
     *
     *    @param    view    ����
     *    @param    sz  ������С�����Դ����ֵ���ó����Լ��жϺ��ʴ�С
     *    @param    object_rect  �������geometry
     *
     *    @return ������󣬻���λ�ó���view��ʾ��Χ������false
     */
    bool calc(MainView* view, SkSize sz, SkRect& object_rect);

    void setRelatePos(const RelPos& p) {
        pos_ = PosType::kRelated;
        rel_ = p;
    }

    void setAttached(ViewItem* item, chart_key_t x);

    void setDataValue(chart_val_t val) {
        pos_ = PosType::kValued;
        val_ = val;
    }

    /*! \brief �ο�item_point_/item_rc_��ע�ͣ�
     *
     *    @param    rc [out]
     *    @param    pt [out]
     *    @return
     */
    void getAttachedItem(SkRect& rc, SkPoint& attach_pt, SkPoint& object_pt) {
        rc = item_rc_;
        attach_pt = item_point_;
        object_pt = object_point_;
    }

    inline ViewItem* attachedView()const {
        return attach_;
    }

protected:
    // ���λ��
    SkRect calcRelate();

    /*! \brief ����attach����
     *
     *    @return   object��λ��
     */
    SkRect calcAttach();

    // ֵ
    SkRect calcDataVal();

    bool validCoord(SkScalar coo) const;

private:
    PosType pos_ = PosType::kNotSet;

    //! ������С
    SkSize sz_;

    //! ����
    MainView* view_ = nullptr;

    //! �������
    RelPos rel_;

    //! attach�������
    ViewItem* attach_ = nullptr;

    //! x�����λ�ã�attach��ʱ����Ч��
    chart_key_t x_ = chartbase::kInvalidDataKey;

    //! ����value����
    chart_val_t val_ = chartbase::kInvalidDataValue;

    // �������
    //! ��ע�Ļ���Ҫ��һ��ֱ�ߣ����ñ�ע�������Ҫ֪�����ĸ��ط���ʼ���ߵ����
    //  �����Ǳ���ע��item��sensor rect���ʵĽǵ�
    // ֻ��PosType::kAttached��ʱ����Ч
    SkPoint item_point_;

    // �������attached object ���ʵı�ע�ߣ�������
    SkPoint object_point_;

    //! ����ע��item��rect���� bar / line / candle���ڵ�ǰkey�ϵ�λ�ã�
    // ֻ��PosType::kAttached��ʱ����Ч
    SkRect item_rc_;
};

}
}

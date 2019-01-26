#pragma once

#include "panelitem.h"
#include "precision.h"

namespace sia {
namespace charty {

class ValueSlice {
public:
    struct PosVal {
        PosVal(SkScalar y = 0.f, double v = 0.f)
            :ypos(y), val(v) {

        }

        // y ����
        SkScalar ypos;

        // ���괦��Ӧ��ֵ
        double val;

        //! val��format���ֵ
        SkString sval;

        //! �Ƿ����м�ֵ
        bool middle = false;
    };

    void setSlice(int count);

    /*! \brief �����ڴ�ֱ������ [0��height]�ϣ�������ֵ��ƽ���ȷ�λ��
     *      �����ϵ�������ֵ���󣬶���Ӧ����ֵ��С��
     *
     *    @param    prec	��ֵת���Լ�format
     *    @param    height	�ܹ�ʹ�õĴ�ֱ�ռ�
     *    @return
     */
    void calc(const Precision& prec, SkScalar height);

    const SkTArray<PosVal>& valPos()const {
        return valpos_;
    }

    const SkTArray<PosVal>& ratioPos()const {
        return ratiopos_;
    }

    inline chart_val_t middleVal()const {
        return middle_val_;
    }

    void setMiddleVal(chart_val_t val);

    void setMaxMin(chart_val_t min_y, chart_val_t max_y);

    inline chart_val_t minY()const {
        return min_y_;
    }

    inline chart_val_t maxY()const {
        return max_y_;
    }

    double valueOfYPos(SkScalar y, SkScalar height) const;

    SkScalar yPosOfValue(chart_val_t v, SkScalar height) const;

protected:
    void adjustSliceCount();

private:
    // �������
    int16_t slice_ = 4;

    //! �м�ֵ�����ڱ��
    chart_val_t middle_val_ = chartbase::kInvalidDataValue;

    // �����Сֵ
    chart_val_t max_y_ = chartbase::kInvalidDataValue;
    chart_val_t min_y_ = chartbase::kInvalidDataValue;

private:
    // ����������
    // value axis
    SkTArray<PosVal> valpos_;

    // ratio axis
    SkTArray<PosVal> ratiopos_;
};

}
}

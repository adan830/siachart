#pragma once

#include "panelitem.h"

namespace sia {
namespace charty {

/*!
 * \class Precision
 *
 * \brief ���ڼ���ͼ�����ʾ���ȣ��������ӵ�
 *
 */
class Precision {
public:
    Precision() {
        // Ĭ����ʾ��λС�����ޱ�����
        setPrecision(2, 1);
    }

    void setPrecision(int16_t precision, int32_t divisor) {
        precision_ = precision;
        divisor_ = divisor;
        precision_format_ = SkStringPrintf("%%.%dlf", precision);
    }

    inline int16_t precision()const {
        return precision_;
    }

    inline int32_t divisor()const {
        return divisor_;
    }

    /*! \brief �����趨�ľ��ȸ�ʽ��
     *
     *    @param	val	��Ҫ��ʽ���ĸ�����
     *    @return
     */
    inline SkString dataFormat(double val)const {
        if (precision_ <= 0) {
            return SkStringPrintf("%lld", (int64_t)val);
        }
        return SkStringPrintf(precision_format_.c_str(), val);
    }

    /*! \brief ���ݳ��Գ������Ӻ��ٸ�ʽ��
     *
     *    @param	val	�������setPrecision�����õ�divisor��Ȼ�����dataFormat
     *    @return
     */
    inline SkString chartValFormat(chart_val_t val)const {
        if (!chartbase::validVal(val)) {
            return SkString("-");
        }
        return dataFormat(util::divide((double)val, (double)divisor_));
    }

private:
    // ��ʾ����
    int16_t precision_ = 2;
    SkString precision_format_;

    // ��������
    int32_t divisor_ = 1;
};

}
}

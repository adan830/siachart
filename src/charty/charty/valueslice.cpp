#include "valueslice.h"

namespace sia {

namespace charty {

void ValueSlice::setSlice(int count) {
    if (count < 4) {
        count = 4;
    }
    slice_ = count;
    adjustSliceCount();
}

void ValueSlice::calc(const Precision& prec, SkScalar height) {
    valpos_.reset(slice_);
    ratiopos_.reset(slice_);

    if (!chartbase::validVal(maxY()) || !chartbase::validVal(minY())) {
        return;
    }

    bool has_middle = chartbase::validVal(middleVal());
    double slice_val = util::divide((double)(maxY() - minY()), (double)(slice_ - 1));
    // �����м�ֵ�ģ�����ҲҪ�ʵ�����ʧһ��content�ռ䣬��ȷ�����м�ֵǡ�÷ŵ�ͼ������м䡣
    SkScalar slice_h = util::divide(height, SkScalar(slice_ - 1));  // // n����λ��n+1����5
    for (int i = 0; i < slice_; ++i) {
        PosVal pv_val;
        pv_val.middle = (has_middle && (i == slice_ / 2));
        if (pv_val.middle) {
            pv_val.ypos = yPosOfValue(middleVal(), height);
            pv_val.val = middleVal();
        } else {
            // �ڽ����ϣ���û�а�����صġ������������ϣ����Ƕ�ת��Ϊ�������㡣
            pv_val.ypos = SkIntToScalar(int(slice_h * i));
            pv_val.val = valueOfYPos(pv_val.ypos, height);
        }
        pv_val.sval = prec.chartValFormat(pv_val.val);
        valpos_[i] = pv_val;

        if (chartbase::validVal(middleVal())) {
            PosVal pv_ratio;
            pv_ratio.middle = pv_val.middle;
            pv_ratio.ypos = pv_val.ypos;
            pv_ratio.val = (pv_val.val - middleVal()) / middleVal() * 100;
            pv_ratio.sval = prec.dataFormat(abs(pv_ratio.val));
            pv_ratio.sval.append("%");
            ratiopos_[i] = pv_ratio;
        }
    }
}

void ValueSlice::setMiddleVal(chart_val_t val) {
    middle_val_ = val;

    setMaxMin(minY(), maxY());
    adjustSliceCount();
}

void ValueSlice::setMaxMin(chart_val_t min_y, chart_val_t max_y) {
    max_y_ = max_y;
    min_y_ = min_y;

    if (chartbase::validVal(middle_val_)) {
        chart_val_t offset = std::max(max_y_ - middle_val_, middle_val_ - min_y_);
        max_y_ = middle_val_ + offset;
        min_y_ = middle_val_ - offset;
    }
}

double ValueSlice::valueOfYPos(SkScalar y, SkScalar height) const {
    // double slice_val = util::divide((double)(maxY() - minY()), (double)(slice_ - 1));

    const double v_per_dpi = util::divide((double)(maxY() - minY()), (double)height);
    // ������ֵ�Ǵ��ϵ��¼�С�����������Ǵ��ϵ�������ģ�
    if (y >= 0) {
        return maxY() - y * v_per_dpi;
    }
    return y * v_per_dpi;
}

SkScalar ValueSlice::yPosOfValue(chart_val_t v, SkScalar height) const {
    // const double height = contentHeight();
    // ÿ1ֵռ�õĸ߶�
    const double dy = util::divide((double)height, (double)(maxY() - minY()));
    return (SkScalar)((double)height - (v - minY()) * dy);
}

void ValueSlice::adjustSliceCount() {
    // ��������м�ֵ�Ļ�������Ҫ��λ��������������
    if (chartbase::validVal(middle_val_)) {
        if (slice_ % 2 == 0) {
            slice_ += 1;
        }
    }
}

}

}


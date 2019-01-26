#pragma once

#include "valueslice.h"

namespace sia {
namespace charty {
/*!
 * \class GridLine
 *
 * \brief ��屳����������
 *
 *
 */
class GridLine {
    enum Flags {
        //! ��ʾ��ֱ��
        kShowVertical = 0x1,

        //! ��ʾˮƽ��
        kShowHorizontal = 0x2,

        //! kVerticalLineEqualSlice, kVerticalLineIndexSlice, kVerticalLineKeySlice ������־�ǻ���ġ�
        //! ��ֱ��ʹ�õȷ�
        kVerticalLineEqualSlice = 0x4,

        //! ��ֱ��ʹ��visual index
        kVerticalLineIndexSlice = 0x8,

        //! ��ֱ��ʹ��key
        kVerticalLineKeySlice = 0x10,
    };

public:
    GridLine();

    void draw(SkCanvas* canvas, MainView* view, SkScalar width, SkScalar height, const SkTArray<ValueSlice::PosVal>& hlines);
    void setPaints(SkPaint* grid);

    void showGrid(bool horizontal, bool vertical);
    void setGridVerticalSlice(int line_count);
    void setGridVerticalSlice(const SkTArray<chart_key_t>& keys_slice, chart_key_t middle_key);
    void setGridVerticalSlice(const SkTArray<int>& visual_indexes, int middle_index);

    inline bool hasHorizontalLine() const {
        return flags_.test(Flags::kShowHorizontal);
    }

    SkColor gridColor()const;

private:
    void setGridVerticalFlag(Flags f);

private:
    BitFlags flags_;

    //! ��visual index�Ϸָ����
    SkTArray<int> v_index_slice_;
    int middle_index_ = -1;

    //! ʹ��key�ָ����
    SkTArray<chart_key_t> keys_slice_;
    chart_key_t middle_key_ = chartbase::kInvalidDataKey;

    //! ʹ�õȷ�
    int16_t v_slice_count_;

    // paints
    SkPaint* grid_ = nullptr;
    SkPaint middle_grid_;
};
}
}

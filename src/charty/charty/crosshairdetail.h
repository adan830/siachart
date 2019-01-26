#pragma once

#include "siapi.h"
#include "defines.h"
#include "chart.h"

namespace sia {
namespace charty {

/*!
 * \class CrosshairDetail
 *
 * \brief ʮ������������������ɡ�
 *	�̳д��࣬��дdetailItems���������������б�DetailItem��
 */
class CHARTY_API CrosshairDetail : public ChartFormater {
    friend class CrossHair;
public:
    struct DetailItem {
        DetailItem() = default;
        DetailItem(const SkString& stitle, const SkString& sval, SkColor color_val = SK_ColorRED)
            :title(stitle), val(sval), val_color(color_val) {
        }

        SkColor val_color = SK_ColorRED;
        SkString title;
        SkString val;
    };

public:
    /*! \brief ʮ����detail�������ʾ����Ŀ��Ϣ
     *
     *    @param	data	��ǰʮ��������index����������
     *    @return	detail�������Ҫ��ʾ����Ŀ
     */
    virtual std::vector<DetailItem> detailItems(chartbase::data_key_t k, const std::map<SkString, chartbase::data_val_t>& data) = 0;
};

}
}

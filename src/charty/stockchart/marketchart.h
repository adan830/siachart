#pragma once

#include "chartbase.h"
#include "bitflag.h"

namespace sia {
namespace stockchart {
/*!
 * \class Axis
 *
 * \brief ͼ�����������
 *
 *
 */
class MarketChart : public StockChartBase {
    enum Flags {
    };

public:
    //! ������
    static const SkString PRICE_LINE;
    static const SkString AVG_PRICE_LINE;
    static const SkString VOL_BAR;

    MarketChart();

    /*! \brief ���ÿ��̼�
     *
     *    @param	price	��Ʊ�۸�
     *    @return
     */
    void setOpenPrice(chartbase::data_val_t price);

    /*! \brief �۸���ʾ��λ
     *
     *    @param	count	��ʾ�ȷֵļ۸�ĵ�λ
     *    @return
     */
    void setValueSlice(int count);

    HPANEL mainPanel()const {
        return main_panel_;
    }

    HPANEL volPanel()const {
        return vol_panel_;
    }

protected:
    void initPanels();

private:
    HPANEL main_panel_ = nullptr;
    HPANEL vol_panel_ = nullptr;
    BitFlags flags_;
};
}
}

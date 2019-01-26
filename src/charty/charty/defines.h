#pragma once

#include "chartapi.h"
#include "bitflag.h"
#include "../base/datasource.h"
#include "../formula/taparam.h"

//! panel ���handle
typedef struct __charty_panel {}*HPANEL;

namespace sia {
namespace charty {

using chart_key_t = chartbase::data_key_t;
using chart_val_t = chartbase::data_val_t;
using SeriesType = chartbase::SeriesType;

/*! \brief ��chart_key_t��Ϊʱ�����
 *
 *    @param	d	������ʽ��ʱ��ֵ���磺��20181008162430435 = 2018-10-08 16:24:30.435��
 *    @param	dt	�����������
 *    @return
 */
CHARTY_API void siaParseChartKey(chart_key_t d, std::tm& dt);

/*! \brief ����chart_key_t
 *
 *    @param	����/ʱ��
 *    @return	chart_key_t
 */
CHARTY_API chart_key_t siaMakeChartKey(const std::tm& dt);

/*!
 * \enum VisualItemZValue
 *      ͼ��ȵ�Z��˳��涨
 */
enum CHARTY_API VisualItemZValue : int16_t {
    kPanel = 1,
    kBackground,
    kTitle,
    kAxis,
    kBarSeries,
    kUpDownSeries,
    kCandleSeries,
    //! line ���е���ʾ����������֮��
    kLineSeries,
    kSideBoard,

    // Ƕ�����
    kObjects = 100,

    //! �û���ͼ
    kTrendObjects = 300,

    // tooltip
    kTooltip,

    // ʮ����
    kCrossHair,

    // ��ѡ
    kRubberband,

    // ��ʾͼ����ϸ��Ϣ����ͼ
    kCrossHairDetail,
};

/*!
 * \enum ObjectType
 *  �Զ������ObjectItem������
 */
enum class CHARTY_API ObjectType : uint8_t {
    kNotSet = 0,

    //! ˮƽ��
    kHLine = 1,

    //! ����
    kText = 2,

    //! ͼ��
    kPicture = 3,

    // ��̬ͼƬ
    kMovie = 4,

    //! ͼ�Ļ���
    kRichText = 5,

    //! �û��Զ��壬��Ҫ�û��ṩ��ͼ����
    Custom = 0xFF,
};

/*!
 * \enum ObjectPosition
 *
 *   �Զ��������λ��
 */
enum class CHARTY_API PosType {
    kNotSet = 0,

    //! ��ͼ��MainView�е��������λ�ã�ֻ֧������ڸ�������MainView����
    // �ο�CSS��position:relate
    kRelated,

    //! ����ָ����MainView�е��������Series/Object�ȣ�
    kAttached,

    //! ��MainView�У�����Yֵ����λ��
    kValued,
};

/*!
 * \enum PanelItemType
 *  �������
 */
enum /*class*/ CHARTY_API PanelItemType {
    //! value
    kPanelValueAxis = 0x1,

    //! ratio
    kPanelRatioAxis = 0x2,

    //! ������key��
    kPanelCalendar = 0x4,

    //! ����
    kPanelTitle = 0x8,

    //! all
    kPanelAll = 0xFFFF,
};

/*!
 * \struct SeriesStyle
 *
 * \brief ���еķ����ʾ
 *
 */
struct CHARTY_API SeriesStyle {
    SeriesStyle() = default;

    // line
    SeriesStyle(SkColor c)
        :color(c) {
    }

    // line
    SeriesStyle(SkScalar w_stroke, SkColor c)
        :color(c), stroke_width(w_stroke) {
    }

    // bar
    SeriesStyle(bool fill, SkColor c, SkScalar w_bar = SkIntToScalar(0))
        :color(c), fill_bar(fill), bar_width(w_bar) {
    }

    // candle
    SeriesStyle(SkColor up, SkColor down, bool fill = false)
        :up_color(up), down_color(down), fill_bar(fill) {
    }

    //! �����
    bool antialias = false;

    //! line style
    SkScalar stroke_width = SkIntToScalar(1);
//     SkPaint::Cap line_cap = SkPaint::kDefault_Cap;
//     SkPaint::Join line_join = SkPaint::kDefault_Join;

    //! bar/candle style
    bool fill_bar = false;

    //! bar�Ŀ�ȡ� 0 : ��ͼ�����ܾ�����
    SkScalar bar_width = SkIntToScalar(0);

    //! ��������ɫ
    SkColor color = SK_ColorCYAN;

    //! candle/updown/axis series color
    SkColor up_color = SK_ColorRED;
    SkColor down_color = SK_ColorGREEN;
};
extern CHARTY_API const SeriesStyle& siaSeriesStyle();

/*!
 * \class ChartFormater
 *
 * \brief �ṩ���ݵĸ�ʽ������
 *
 */
struct CHARTY_API ChartFormater {
public:
    /*! \brief ����ChartPanel���õľ��ȺͷŴ�������ʽ������
     *
     *    @param	val		��ʽ������
     *    @return	��ʽ������������ַ�����ʽ��
     */
    SkString formatVal(chart_val_t val) const;

    /*! \brief ��ʽ��ʱ��
     *
     *    @param	key		ʱ��ֵ
     *    @param	format	ʱ���ʽ����ѭstd::strftime��ʽ�����Ϊ�գ���ʹ��Ԥ��ĸ�ʽ
     *    @return
     */
    SkString formatKey(chart_key_t key, const SkString& format = SkString("")) const;

    /*! \brief �������ڵ�panel���
     *
     *    @param	panel	���
     *    @return
     */
    void setPanel(HPANEL panel);

private:
    HPANEL panel_ = nullptr;
};

/*!
 * \class SeriesTooltipData
 *
 * \brief ����tooltip�����ݡ�
 *	������ƶ��������ϵ�ʱ�򣬽��ᵯ��һ��tooltipչʾ���ָʾλ�ô���������ϸ��Ϣ��
 *
 */
struct CHARTY_API SeriesTooltipData : public ChartFormater {
    //! ��������
    SkString series_name;

    //! ��������
    SeriesType typ;

    //! �������λ�ö�Ӧ��key
    chart_key_t key;

    //! <data_name, data_val>
    std::map<SkString, chart_val_t> datas;

    // ----------------
    //! ���title
    SkString title;

    //! ��ʾ��Tooltip�ϵ���Ŀ
    struct Item {
        SkString cont;
        SkColor color;
    };
    SkTArray<Item> items;
};

/*! \brief ���е�tooltip��ʾ��
 *
 *    @param	[in|out] SeriesTooltipData
 *    @return	����false������ʾtooltip��
 */
typedef std::function<bool(SeriesTooltipData&)> SeriesTooltipCallback;

enum class CHARTY_API PanelTitleItemType {
    //! ʹ�����泣��
    kConstant,

    // ʹ��ͼ��symbol
    kChartSymbol,

    //! ʹ��������
    kSeriesName,
};

/*!
 * \class PanelTitleItem
 *
 * \brief ���Title��item������ʾ��
 *	Ĭ������£�title��ʾ��ǰ����ϵ��������������Ҳ���Ӧʮ���ߵ��ƶ��仯
 *
 */
struct CHARTY_API PanelTitleItem {
    //! ��ʾ�������庬����Ҫʹ��PanelTitleItemType����
    SkString item;

    //! ����
    PanelTitleItemType typ = PanelTitleItemType::kConstant;

    //! �����Sk_InvalidColor��ʹ��Ĭ�ϵ���ɫ������ʹ���ṩ����ɫ
    SkColor color = Sk_InvalidColor;
};

namespace ChartEventType {
//! ���ݱ仯
static const char* kDataChanged = u8"sia::charty::DataChanged";

// ��ʾ��Χ�仯
static const char* kDomainChanged = u8"sia::charty::DomainChanged";

// viewitemѡ��״̬�仯
// data��setString("item", (ViewItem*))
static const char* kViewItemSeletChanged = u8"sia::charty::ViewItemSelected";

// viewitemɾ��
// data��setString("item", (ViewItem*))
static const char* kViewItemDeleted = u8"sia::charty::ViewItemDeleted";

// ȫ�����з��仯
static const char* kSiaSeriesStyleChanged = u8"sia::charty::SiaSeriesStyleChanged";

// ͼ������仯
static const char* kSiaThemeChanged = u8"sia::charty::SiaThemeChanged";

// ���м���ɾ��
// data��setString("name", $series_name)
static const char* kSeriesBeforeDelete = u8"sia::charty::SeriesBeforeDelete";

// ������ɾ��
// data��setString("name", $series_name)
static const char* kSeriesDeleted = u8"sia::charty::SeriesDeleted";

};

}
}


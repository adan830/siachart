#pragma once
#include "visualitem.h"
#include "sensorwidget.h"
#include "viewitemtooltip.h"
#include "viewitem.h"

namespace sia {
namespace charty {

class MainView;

/*!
 * \class Series
 *
 * \brief ��������չʾͼ�����������
 *  ���о����������֣�widget_type_name �� series_name
 *  widget_type_name��ָ�����е����ͣ���lineSeries/CandleSeries�ȹ̶���
 *  series_name��ָ���û��������е����֣�����test��myline�ȣ�������
 */
class Series : public ViewItem {
    enum SeriesFlag {
        //! �Ƿ�ʹ�õĶ��Ƶ���ۣ�ͨ������setStyle���õ���ۣ������Զ������
        kUseCustomStyle = 1 << (ViewItemFlag::kMaxFlagShift + 1),

        //! �Ƿ���ʾmax/min����ʾ
        kShowMinMaxTip = 1 << (ViewItemFlag::kMaxFlagShift + 2),

        //! ��ɾ����ʱ���Ƿ�ͬ�������ʹ�õ��������У�
        kRemoveDatasourceOnDelete = 1 << (ViewItemFlag::kMaxFlagShift + 3),
    };

public:
    Series(const SkString& widget_name, const SkString& series_name, SeriesType typ);

    virtual ~Series();

    inline SkString seriesName()const {
        return series_name_;
    }

    /*! \brief �������ʹ���һ��ʵ��
     *
     *    @return   ʵ��ָ�룬�����߸����ͷ�
     */
    static my_sp<Series> create(const SkString& series_name, SeriesType typ);

    /*! \brief ÿ������������Ҫһ�����͵���������ʾ
     *
     *    @param datas ������������
     *    @return
     */
    bool setDataNames(const SkTArray<SkString>& datas);

    /*! \brief ����Ĭ�ϵ����������ڵ���valAt��ʱ��ȡ�������е�ֵ
     *
     *    @param	dataname ������
     *    @return
     */
    void setDefaultDataName(const SkString& dataname);

    /*! \brief ���������ٵ�ʱ��ͬʱɾ������������Դ����
     *		!!!WARNING: ��С��ʹ�ã�����ж������ͬʱ�������������������ᵼ��������������ʾ����
     *
     *    @param on �Ƿ�����־
     *    @return
     */
    void setRemoveDataOnDelete(bool on);

    /*! \brief �Ƿ���Ҫɾ������Դ��
     *
     *    @param
     *    @return
     */
    bool isRemoveDatasource()const;

    /*! \brief ���������е�ÿ���������ڵĵ�λ��
     *
     *    @param
     *    @return
     */
    bool calcPoints();

    /*! \brief ������x���ϣ�һ�����е����Ŀ��ӻ����
     *          ����bar/candle�Ŀ��ӿ�ȣ���ͼ�����۾������Ŀ�ȣ�
     *
     *    @param    desire_width    ������ȣ���������ݴ�ֵ�����һ������Ŀ��
     *                              �������ս��С��0��Ҳ���������������Ŀ��
     *    @return
     */
    SkScalar barWidth(SkScalar desire_width = SkIntToScalar(0));

    std::set<SkString> datanames() const;

    void setColor(SkColor c);

    void setStyle(const SeriesStyle& ss);

    inline SkColor color()const {
        return style().color;
    }

    inline bool isValidPoint(const SkPoint& pt)const {
        return (pt.x() != chartbase::kInvalidCoord
                && pt.y() != chartbase::kInvalidCoord);
    }

    const SeriesStyle& style()const;

    /*! \brief ����tooltip��ʾ���ݵĻص�
     *
     *    @param	cb	�ص����������Ϊnullptr���������һ��Ĭ�ϵ�
     *    @return
     */
    void setTooltipCallback(SeriesTooltipCallback cb = nullptr);

    /*! \brief ��ʾ�����Сֵ��ʾ
     *
     *    @param	show
     *    @return
     */
    void showMinMaxTip(bool show);

    /*! \brief ������key����ֵ��
     *
     *    @param
     *    @return
     */
    chart_val_t valAt(int visual_idx) const;

protected:
    virtual void onSensed(int idx, const SkPoint& pt) override;

    // ������Ҫ��������
    struct _DataPos {
        SkString dataname;

        SkTArray<chart_key_t> keys;
        SkTArray<chart_val_t> vals;
        SkTArray<SkPoint> points;

        inline bool isValid()const {
            return !dataname.isEmpty();
        }

        inline int itemCount()const {
            return keys.count();
        }
    };
    const _DataPos& dataPosItem(const SkString& dataname) const;

    virtual void onDraw(SkCanvas* canvas) override;

    virtual void onStyleChanged();

protected:
    // ����������widget name�ǲ�ͬ��
    SkString series_name_;

    //! Ĭ��ȡֵ��������(data_name)
    SkString default_dataname_;

    // ��������
    SeriesType typ_ = SeriesType::kBlank;

    // ��������
    SkTArray<_DataPos> datapos_;

    //! ��۶���
    SeriesStyle style_;

    // tooltip
    SeriesTooltipCallback tooltip_cb_ = nullptr;

    //! �����Сֵ��λ��
    struct _DataItem {
        chart_key_t key;
        chart_val_t val;
        SkPoint point;

        void set(chart_key_t k, chart_val_t v, const SkPoint& p) {
            key = k;
            val = v;
            point = p;
        }
    };
    _DataItem min_item_;
    _DataItem max_item_;
};

}
}

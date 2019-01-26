#pragma once

#include "stacklayout.h"
#include "panelitem.h"
#include "valueslice.h"
#include "viewitemtooltip.h"
#include "viewobject.h"
#include "precision.h"
#include "gridline.h"

namespace sia {
namespace charty {
class Series;
class ViewItem;

/*!
 * \class MainView
 *
 * \brief ����ͼ��Panel::Series�ı�����ʾ����Ҫ���ݼ���
 *
 */
class MainView : public PanelItem {
public:
    enum Flags {
        //! ���м�ֵ��ʱ��Ҫ�������Ϊ���µȷֵ�������
        kMiddleVal = 0x2,

        //! �Ƿ��Ǹ�ͼ
        kSubView = 0x4,
    };

    MainView(Panel* parent = nullptr);
    ~MainView();

    inline const SkTArray<SkScalar>& indexXPos()const {
        return xpos_;
    }

    /*! \brief �����Ƿ���Ϊ��ͼ
     *	    ��ͼû�������ߺͱ�����
     *    @param
     *    @return
     */
    void setAsSubview(bool on);

    my_sp<Series> addSeries(const SkString& name, SeriesType typ, const SkTArray<SkString>& datas);
    bool removeSeries(const SkString& name);

    bool addObject(const SkString& name, const ObjectParam&para, my_sp<IVisualObject> obj);
    bool removeObject(const SkString& name);

    my_sp<Series> series(const SkString& name)const;
    std::vector<my_sp<Series>> serieses()const;

    inline chart_val_t minY()const {
        return vslice_.minY();
    }

    inline chart_val_t maxY()const {
        return vslice_.maxY();
    }

    virtual void recalc(Bits flags) override;

    void setValueSlice(int count);

    /*! \brief �������з��
     *
     *    @param	styles
     *    @return
     */
    void setSeriesColor(const std::map<SkString, SkColor>& colors);
    void setSeriesStyle(const std::map<SkString, SeriesStyle>& styles);

    /*! \brief �������е���ʾ���ݻص�
     *
     *    @param	callbacks <series_name, callback> �ص�
     *    @return
     */
    void setSeriesTooltipCallback(std::map<SkString, SeriesTooltipCallback> callbacks);

    const ValueSlice& valSlice()const {
        return vslice_;
    }

    void setMiddleVal(chart_val_t val);

    // ��ʾ�����Сֵ����ʾ
    void showSeriesMinMaxTip(const SkString&series_name, bool show);

    /*! \brief ÿ�����ж�����Ҫ����ʾһ����ʾ��
     *
     *    @param
     *    @return
     */
    inline ViewItemTooltip* seriesTip() {
        return &tip_;
    }

    ViewItem* viewItem(const SkString& name)const;

    /*! \brief ������ͼ��Ч���(contentWidth)���ȷ֣��ȷ�ΪChartContext::curIndexCount��
     *	    ÿһ��ռ�õĿ��
     *    @param
     *    @return
     */
    SkScalar indexWidth()const;

    Precision& precision() {
        return precision_;
    }

    GridLine& grid() {
        return grid_;
    }

    //////////////////////////////////////////////////////////////////////////
    // ����ת��

    /*! \brief ����x����λ�ã���������Ӧ����������λ��
     *
     *    @param
     *    @return   �����������Ѽ���ChartContext::indexBeginƫ������
     */
    int dataIndexOfPos(SkScalar x)const;

    /*! \brief ����x����λ�ã���������Ӧ����������λ�ã�����ChartContext::indexBeginƫ��
     *
     *    @param
     *    @return ��������������ChartContext::indexBeginƫ������
     */
    int visualIndexOfPos(SkScalar x)const;

    /*! \brief ���key�Ƿ��ڵ�ǰ��view��ʾ����
     *	    ��Ϊ���ǵ�key��������ģ���С���󣩣����ԣ�ֻ��Ҫ���
     *      key�Ƿ����indexBegin��valueֵ����С��indexEnd��valueֵ����
     *
     *    @return   ���û������ʾ���ڣ�����-1
     */
    int visualIndexOfKey(chart_key_t key) const;

    /*! \brief ����y���꣬��������Ӧ��valueֵ
     *		������ֵ�Ǵ��ϵ��¼�С�����������Ǵ��ϵ�������ģ�
     */
    double valueOfYPos(SkScalar y) const;

    /*! \brief ֵ��panel�е�y����λ��
     *
     *    @param    v
     *    @return
     */
    SkScalar yPosOfValue(chart_val_t v) const;

    /*! \brief ����dataname��idx������������
     *
     */
    SkPoint posOfDataname(int idx, const SkString& dataname)const;

    /*! \brief ����idx�������Ӧ��x����
     *
     *    @param    visual_idx ������DataSource::indexBegin��ƫ�ƣ�
     *    @return   x���꣬
     *				< 0 :	����index���ڵ�ǰ����ʾ���أ�����С��0.
     *				>= 0:	��ȷ����
     */
    SkScalar xPosOfVisualIndex(int visual_idx) const;

    /*! \brief ����key���ڵ�x����
     *		���key���ڵ�ǰ��ʾ�ķ��أ�
     *    @param key
     *    @return �ο�xPosOfVisualIndex����ֵ
     */
    SkScalar xPosOfKey(chart_key_t key) const;

    /*! \brief �������ж�Ӧ��ʱ��ֵ
     *
     *    @param    idx �Ѽ���ChartContext::indexBegin��ƫ����
     *    @return
     */
    chart_key_t datetimeOfIndex(int idx) const;

    /*! \brief ��ͼ�ǲ���û���κο��ӻ�����Ŀ�ˣ�
     *
     *    @param
     *    @return
     */
    bool viewEmpty()const;

public:
    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    virtual void onSizeChange() override;

    virtual void preparePaints() override;

    void calcYRange();
    void calcXPoss();

    std::set<SkString> datanames() const;

private:
    StackLayout stacklay_;

    BitFlags flags_;

    //! ����index��x����
    SkTArray<SkScalar> xpos_;

    // ����
    std::unordered_map<SkString, my_sp<Series>> series_;

    // objects
    std::unordered_map<SkString, my_sp<ObjectItem>> objects_;

    //! ��value��Ϊ�ȷֵļ�����λ��ÿ����λ�϶�Ӧvalue/ratio
    // �����ݴ�ֵ��ʾ����
    ValueSlice vslice_;

    // paints
    SkPaint* bk_ = nullptr;
    SkPaint middle_grid_;

    //! ��ʾ��
    ViewItemTooltip tip_;

    // ��ʾ����
    Precision precision_;

    //! ������
    GridLine grid_;
};
}
}


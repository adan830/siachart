#pragma once

#include "graphy/scene.h"
#include "defines.h"
#include "viewobject.h"
#include "chartapi.h"

namespace sia {
namespace charty {

struct ChartPrivate;
class CrosshairDetail;
class IVisualObject;

/*!
 * \class Chart
 *
 * \brief ͨ��ͼ��
 *
 */
typedef sia::graphy::Scene ChartBase;
class CHARTY_API Chart : public ChartBase {
public:
    /*! \brief ����ͼ�������Ҫһ��ȫ��Ψһ��ͼ��ID
     *
     *    @param	id	������ȫ��ΨһID
     *    @return
     */
    explicit Chart(const SkString& id);

    ~Chart();

    /*! \brief ͼ��id
     *
     *    @param
     *    @return ͼ��ʵ����ID
     */
    SkString ID()const;

    /*! \brief ���ͼ�����
     *
     *    @param
     *    @return
     */
    HPANEL addPanel(/*bool with_calendar*/);

    /*! \brief ��ȡpanel����
     *
     *    @param
     *    @return
     */
    uint32_t panelCount()const;

    /*! \brief ��ȡindex��Ӧ��panel���
     *
     *    @param	idx		panel��˳������ [0, panelCount)
     *    @return	panel ���
     */
    HPANEL panel(uint32_t idx) const;

    /*! \brief �Ƴ����
     *
     *    @param
     *    @return
     */
    bool removePanel(HPANEL panel);

    /*! \brief ���һ������
     *
     *    @param  panel ����Ҫ��ӵ����
     *    @param  series_name ��������
     *    @param  type ��������
     *    @param  datanames ������Ҫ����������
     *    @return   false |
     *      1����������ͻ
     *      2��panel�Ҳ���
     */
    bool addSeries(HPANEL panel, const SkString& series_name, SeriesType typ, const SkTArray<SkString>& datanames);
    bool addSeries(HPANEL panel, const SkString& series_name, SeriesType typ, const SkString& dataname);

    /*! \brief ��ȡ��������б�
     *
     *    @param	panel	���
     *    @param	series_names [out]	������
     *    @return	true ��ȡ�ɹ�
     */
    bool getSeries(HPANEL panel, SkTArray<SkString>& series_names);

    /*! \brief ѡ������
     *
     *    @param	panel	��塣���Ϊnullptr������Ϊȡ����ǰѡ�����е�ѡ��״̬��
     *    @param	series_name	Ҫѡ�е�������
     *    @return	true ѡ�гɹ�
     */
    bool selectSeries(HPANEL panel, const SkString& series_name);

    /*! \brief ��ʾ���е������Сֵλ����ʾ
     *
     *    @param	panel	���
     *    @param	series_name	������
     *    @param	show	true����ʾ��
     *    @return
     */
    void showSeriesMinMaxTip(HPANEL panel, const SkString&series_name, bool show);

    /*! \brief �Ƴ��Ѵ��ڵ�����
     *
     *    @param	panel	�����������
     *    @param	series_name	������
     *    @return
     */
    void removeSeries(HPANEL panel, const SkString& series_name);

    /*! \brief ����panel�е�������ʾ��ɫ
     *		�˷������Ե�ɫ��������Ч������Bar/Line�ȡ�����Candle/Updown������ʹ��setSeriesStyle����
     *
     *    @param    panel   �������
     *    @param    colors <SeriesName, SkColor>   ������ɫ��Ŀǰ������У������Ӧ��series name�����ڣ���������Ч
     */
    void setSeriesColor(HPANEL panel, const std::map<SkString, SkColor>& colors);

    /*! \brief �������е���۷��
     *		��ʱ�����õ����б������Ѵ��ڵģ�����������Ч
     *    @param	panel	��Ҫ���÷������
     *    @param	styles <SeriesName, SeriesStyle>	���е���۷��
     *    @return
     */
    void setSeriesStyle(HPANEL panel, const std::map<SkString, SeriesStyle>& styles);

    /*! \brief ��panel����ʾ��һ���м�ֵΪ���֣���Ϊ���������ȷֵĲ��֡�
     *
     *    @param    val �м�ֵ��val == chartbase::kInvalidDataVal: ȡ�������м�ֵ
     */
    void setMiddleVal(HPANEL panel, chart_val_t val);

    /*! \brief ���/�༭���ݡ�
     *		��Ϊ���ݵ�key��Ψһ�ģ�����ӵ����ݻ����ԭ�е����ݣ����Դ˷���Ҳ���Ա༭���ݡ�
     *
     *    @param    data_name   ��������
     *    @param    datas   ����
     *    @return
     */
    void addData(const SkString& data_name, const std::map<chart_key_t, chart_val_t>& datas);

    /*! \brief �Ƴ����ݡ�
     *		�Ƴ���������ָ���������ݣ�ִ�к������������١�
     *
     *    @param	data_name	������
     *    @return
     */
    void removeData(const SkString& data_name);

    /*! \brief �Ƴ�����
     *		�Ƴ���������ָ���ݽṹ�еĲ������ݡ�ִ�к���������Ȼ���ڡ�
     *
     *    @param	data_name	������
     *    @param	keys	����key
     *    @return
     */
    void removeData(const SkString& data_name, const SkTArray<chart_key_t>& keys);

    /*! \brief ���õ�ǰ��ʾ������
     *
     *    @param    begin ��ʾ��ʼindex
     *    @param    end ��ʾ����index��-1����ʾ�����һ������
     *    @return
     */
    void setCurrentDomain(int32_t begin = 0, int32_t end = -1);

    /*! \brief ���ñ�����ʾ������
     *	    ���֣�symbol����һ��ͼ������ӵ�е�
     *    @param    symbol
     */
    void setSymbol(const SkString& symbol);

    /*! \brief ���ñ�����ʾ����Ϣ����
     *
     *    @param	items	������ʾ���ݡ���ο���PanelTitleItem
     *    @return	false�� ���panelû��title
     */
    bool setTitleItems(HPANEL panel, const SkTArray<PanelTitleItem>& items);

    /*! \brief ��ʾ/����panel��Title�ؼ�
     *
     *    @param	panel	���
     *    @param	show	�Ƿ���ʾ
     *    @return
     */
    void showPanelTitle(HPANEL panel, bool show);

    /*! \brief ��ʾ/��������ͼ��������panel��ĳ��ؼ�
     *
     *    @param    item    �ο�PanelItemType
     *    @param    show    true ��ʾ
     *    @return
     */
    void showPanelItem(int item, bool show);

    /*! \brief ����ֵ��ʾ�ĵ�λ��
     *	��showGrid���ʹ�ã�������ÿ��ֵ��λ����ʾһ�����߷ָ����
     *
     *    @param    panel ���
     *    @param    count ���ж��ٵ�λ
     *    @return
     */
    void setValueSlice(HPANEL panel, int count);

    /*! \brief �Ŵ���ʾ���ݷ�Χ
     *
     */
    void zoomIn();

    /*! \brief ��С��ʾ���ݷ�Χ
     *
     */
    void zoomOut();

    /*! \brief ʮ�����ƶ���ʱ�����ϵ��Ǹ�dataname��
     *	    ʮ���ߵĵ�����ֻ�����ڵ�һ��panel��(��panel)��
     *    @param dataname ��������
     */
    void setCrossHairDataname(const SkString& datanme);

    /*! \brief ����ʮ���ߵ���ϸ������ʾ
     *
     *    @param	detail	����
     *    @return
     */
    void setCrossHairDetail(CrosshairDetail* detail);

    /*! \brief Tooltip��ʾ���ݵĻص�����
     *		�����õ�ʱ�������������Ѿ����ڣ���������ʧ��
     *
     *    @param	panel	���
     *    @param	callbacks <series_name, callback>	�ص�����
     *				ÿ��series��������һ����ʾ
     *    @return
     */
    void setSeriesTooltipCallback(HPANEL panel, std::map<SkString, SeriesTooltipCallback> callbacks);

    /*! \brief Ӧ��һ����ʽ
     *
     *    @param    panel   ��ʽ��������ĸ�panel����ʾ
     *    @param    ta_name ��ʽ��
     *    @param    param   ��ʽ��Ҫ�Ĳ���
     *    @return	��ʽʵ����ID�� >0 Ϊ��ӳɹ�
     */
    int32_t showFormula(HPANEL panel, const SkString& ta_name, const formula::TaParam& param);

    /*! \brief ����һ����ʽ��ʾ
     *
     *    @param	panel	���
     *    @param	formula_id	��ʽ��id
     *    @return
     */
    void removeFormula(HPANEL panel, int32_t formula_id);

    /*! \brief ���һ���Զ���Ŀ��ӻ�����Object��
     *
     *    @param    panel   ��ӵ��ĸ�panel��
     *              name    ������
     *              para    �������
     *              obj     �����������
     *
     *    @return   ���panel�����ڣ����߶������Ѵ��ڣ����߲������󣬷���false
     */
    bool addObject(HPANEL panel, const SkString& name, const ObjectParam&para, my_sp<IVisualObject> obj);

    /*! \brief �Ƴ����ӻ�����
     *
     *    @param	panel	���
     *    @param	object_name	������
     *    @return
     */
    void removeObject(HPANEL panel, const SkString& object_name);

    /*! \brief ��������/ʱ��ĸ�ʽ������
     *
     *    @param	format ��calendar axis����ʾ�ĸ�ʽ���ο�c++ strftime
     *    @param	crosshair_detail_format ��ʮ������ʾ�ĸ�ʽ���ο�c++ strftime
     *    @return
     */
    void setCalendarFormat(const SkString& format, const SkString& crosshair_detail_format);

    /*! \brief �ƶ�ʮ����
     *
     *    @param	offset	>0 �����ƶ���<0 �����ƶ�. abs(offset) �ƶ���index������
     *    @return
     */
    void moveCrosshair(int offset);

    /*! \brief ƽ����ͼ��������ʾ��Χ��
     *
     *    @param	offset	>0 �����ƶ��� <0 �����ƶ�������ֵ���ƶ���index��
     *    @return	����false�������ƶ�ʧ�ܣ����磬�����Ѿ��ƶ�����0��index�����������Ѿ������һ��index��
     */
    bool moveView(int offset);

    /*! \brief ���þ��Ƚӿ�
     *			ÿ����嶼Ӧ�����Լ�����ʾ����/�����������ã�����۸����������ʾ���Ⱥͳ����ǲ�ͬ�ġ�
     *
     *    @param	panel
     *    @param	precision ��ͼ����չʾ��ʱ�򣬱�����С����λ��
     *    @param	divisor ����Ľ�ȥ�����ݿ���ȫ�������������������ʱ�򣬻����������ӣ�ת��Ϊ������С��
     *
     *    @return
     */
    void setPrecision(HPANEL panel, int16_t precision, int32_t divisor);

    /*!
     * \enum �ڽ�֧�ֵĹ��ܣ�ʹ��setFunction���û��߽���ĳЩ���ܡ�
     *
     */
    enum FunctionType : int32_t {
        //! ��������
        kFunctionKeyInput = 1 << 1,

        //! �������
        kFunctionMouseInput = 1 << 2,

        //! ����ѡ�Ŵ��ڿ�����kFunctionMouseInput��Ч
        kFunctionRubberSelect = 1 << 3,

        //! ʮ���߹���
        kFunctionCrossHair = 1 << 4,

        //! �������ţ��ڿ�����kFunctionKeyInput��Ч
        kFunctionKeyZoom = 1 << 5,

        //! �����ƶ���ͼ���ڿ�����kFunctionKeyInput��Ч
        kFunctionKeyMoveView = 1 << 6,

        //! ֧��Tooltip����
        kFunctionTooltip = 1 << 7,

        //! ֧��ѡ������
        kFunctionSelectSeries = 1 << 8,

        //! ������
        kFunctionMaxShift = 16,

        //! ���еĹ���
        kFunctionAll = ((1<<kFunctionMaxShift) -1),
    };

    /*! \brief ����/����ĳЩ�ڽ��Ѿ�֧�ֵĹ���
     *
     *    @param	functions	�ο�FunctionType
     *    @param	enable	true����
     *    @return
     */
    void setFunction(int32_t functions, bool enable);

    /*! \brief ���ĳЩ�����Ƿ��Ѿ�����
     *
     *    @param	functions	������Ŀ�����FunctionTypeֵ
     *    @return	true	�����Թ����Ѿ�����
     */
    bool functionEnabled(int32_t functions)const;

    /*! \brief Ϊ��������Ƿ���ʾˮƽ/��ֱ�ָ���
     *
     *    @param	panel	���
     *    @param	horizontal	ˮƽ�ߣ���ˮƽ�ָ�ĸ�����setValueSlice��ȷ��
     *    @param	vertical	��ֱ��
     *    @param	vertical_count	��ֱ�ߵĸ���
     *    @return
     */
    void showGrid(HPANEL panel, bool horizontal, bool vertical);

    /*! \brief �ȷ����������ʾ��ֱ������
     *
     *    @param	panel	���
     *    @param	line_count	��ֱ�߸���
     *    @return
     */
    void setGridVerticalSlice(HPANEL panel, int line_count);

    /*! \brief ���ض���index����ʾ��ֱ������
     *
     *    @param	panel	���
     *    @param	visual_indexes	��Ҫ��ʾ��ֱ�����ߵ�visual index ֵ
     *    @param	middle_index	��Ϊ�м��index����ʾ���߽��ȱ�Ĵ�һЩ��-1: ��Ч��
     *    @return
     */
    void setGridVerticalSlice(HPANEL panel, const SkTArray<int>& visual_indexes, int middle_index = -1);

    /*! \brief ���ض���key����ʾ��ֱ������
     *
     *    @param	panel
     *    @param	keys_slice	keyֵ
     *    @param	middle_key	��Ϊ�м��key����ʾ���߱ȱ�Ĵ֣�kInvalidDataKey����Ч
     *    @return
     */
    void setGridVerticalSlice(HPANEL panel, const SkTArray<chart_key_t>& keys_slice, chart_key_t middle_key = chartbase::kInvalidDataKey);

    /*! \brief �ϲ���塣��sub���ϲ���main����С�
     *  sub��彫�ᱻ���٣��޷��ָ�����sub����е�����ͼ���ݱ��ϲ���main����ϡ�
     *
     *    @param    main    �ϲ��������
     *    @param    sub    ���ϲ������
     *    @return
     */
    bool mergePanel(HPANEL main, HPANEL sub);

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    /*! \brief View���¼�����
     *
     *    @param
     *    @return
     */
    virtual bool onEvent(const SkEvent& evt) override;

    /*! \brief ȫ�����ͼ��ǣ�浽���ݵ����¼��㣬�����벼���ػ档
     *		���ֻ��Ϊ�˸��½��棬��ǣ�沼�ֺ����ݼ��㣬��ʹ��redraw����
     */
    void update(Bits flags);

    /*! \brief ��������»��ƣ���ǣ�����ݼ���
     *
     *    @param
     *    @return
     */
    void redraw();

    /*! \brief ����¼�����
     *
     *    @param
     *    @return
     */
    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

    virtual bool onKey(sia::KeyBoard key, sia::InputState state, uint32_t modifiers) override;

    virtual void onSizeChange() override;

    virtual void onSurfaceChanged() override;
private:
    my_sp<ChartPrivate> d = nullptr;
};

/*! \brief ����ȫ�ֵ��������չʾ
 *
 *    @param	ss	SeriesStyle�������
 *    @return
 */
CHARTY_API void setSiaSeriesStyle(const SeriesStyle& ss);

/*! \brief �������õ�ȫ���������
 *
 *    @param
 *    @return	const SeriesStyle&
 */
CHARTY_API const SeriesStyle& siaSeriesStyle();

}
}

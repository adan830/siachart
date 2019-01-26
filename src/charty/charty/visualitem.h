#pragma once

#include "siapi.h"
#include "graphy/widget.h"
#include "defines.h"
#include "chartctx.h"
#include "dict.h"

namespace sia {
namespace graphy {
class PaintGroup;
}

namespace charty {

/*!
 * \class VisualItem
 *
 * \brief ͼ�������пɼ�������Ԫ�ض�Ҫ�̳��ڴ���
 *
 */
class VisualItem : public graphy::Widget {
public:
    VisualItem(const SkString& name, graphy::Widget* parent);

    inline const chartbase::DataSource* datasource() const {
        if (ctx_) {
            return ctx_->datasource();
        }
        return nullptr;
    }

    /*! \brief �Ƿ�׼���ã��ܹ������Լ���������
     *		�����������棬һ������������׼�����ˣ�һ����paint����׼������
     *
     *    @param
     *    @return	true	���Ի����ˡ�
     */
    bool ready() const;

    void setChartContext(ChartContext* t);

    inline ChartContext* chartCtx()const {
        return ctx_;
    }

    Dict* dictionary()const;

    /*! \brief ���¼���������صĲ���
     *
     */
    virtual void recalc(Bits flags) {

    }

    static SkScalar textVerticalMiddle(const SkPaint& paint, SkScalar height = 0);

    /*! \brief ����ַ���ռ�õľ��δ�С
     *		ע�⣺�ַ����������
     *    @param	str
     *    @return
     */
    static SkRect textRect(const SkString& str, const SkPaint& paint,
                           const SkPoint& left_top = SkPoint::Make(SkIntToScalar(0), SkIntToScalar(0)));

    /*! \brief ����\n ���л���
     *      ��ǰ��֧��ˮƽ����
     *
     *    @param    text ���û�� \n ���з�������Ƶ�ͬһ����
     *    @param    rc �ı����Ƶ��˾�����    ��Ŀǰ��û��ʵ��һ���Զ�wrap�Ĺ��ܣ�
     *    @param    spacing �м�࣬ͬʱ���rc��padding����Ҫϸ�����á�
     *    @return
     */
    static void drawMultilineText(SkCanvas* canvas, const SkPaint& paint,
                                  const SkString& text, const SkRect& rc, SkScalar spacing = 5);

protected:
    void forVisualChildren(std::function<bool(VisualItem*)> cb);

    /*! \brief ��painters�в����û����õ�paint������Ҳ��������Լ�����һ��
     *
     *    @param    name paint��������
     *    @return
     */
    SkPaint* declarePaint(const SkString& name, SkColor def_color = SK_ColorWHITE, bool antialias = false);

    /*! \brief �����ļ���Ҫ���geometry
     *
     *    @param
     *    @return   false ����ⲿû������
     */
    bool getConfigGeometry(SkRect& rc);

    /*! \brief �����¼�
     *
     *    @param
     *    @return
     */
    virtual bool onEvent(const SkEvent&) override;

    /*! \brief ׼��paints
     *
     *    @param
     *    @return
     */
    virtual void preparePaints();

protected:
    my_sp<graphy::PaintGroup> painters_;

    void refreshPaints();

private:
    ChartContext* ctx_ = nullptr;

    //! ���������ı���
    std::unordered_map<SkString, my_sp<SkPaint>> paints_;

    //! ����paint�Ƿ��Ѿ�׼����
    bool paint_ready_ = false;
};

}
}

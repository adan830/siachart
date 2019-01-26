#pragma once

#include "panelayout.h"
#include "visualitem.h"
#include "valueaxis.h"
#include "ratioaxis.h"
#include "mainview.h"
#include "title.h"

namespace sia {

namespace chartbase {
class TaParam;
}

namespace formula {
class IFormula;
}

namespace charty {

// class ValueAxis;
// class RatioAxis;
// class Title;
// class MainView;
class CalendarAxis;
class Series;

/*!
 * \class VisualItem
 *
 * \brief ͼ�������пɼ�������Ԫ�ض�Ҫ�̳��ڴ���
 *
 */
class Panel : public VisualItem {

    struct _Formula {
        // ���ĸ�view����ʾ��
        my_sp<MainView> view = nullptr;

        // ��ʽ
        formula::IFormula* fml = nullptr;

        //! ָ����
        SkString formula_name;

        //! title����Brief Param��
        SkString brief_name;

        //! ��ʽʵ��id
        int32_t id = 0;

        //! �����ͼ�������������
        SkTArray<SkString> data_names;

        //! �����ͼ���������
        SkTArray<SkString> series_names;

        //! ��ʽ����
        formula::TaParam param;

        inline bool hasSeries(const SkString& series_name)const {
            for (auto& s : series_names) {
                if (series_name.equals(s)) {
                    return true;
                }
            }
            return false;
        }
    };

public:
    Panel(graphy::Widget* parent);
    ~Panel();

    virtual void recalc(Bits flags) override;

    inline MainView* mainView() {
        return mainview_.get();
    }

    /*! \brief ��ȡ���е���ͼ�����͸�ͼ��
     *
     *    @param
     *    @return
     */
    void views(SkTArray<my_sp<MainView>>& out)const;

    /*! \brief ���е�view�У����͸�ͼ�����е�����
     *
     *    @param
     *    @return
     */
    std::vector<my_sp<Series>> serieses()const;

    /*! \brief ��panel�У��������ֲ�������
     *
     *    @param    series_name ������
     *    @param    view [out] ����view
     *    @return   ����ָ��
     */
    my_sp<Series> series(const SkString& series_name, my_sp<MainView>& view)const;

    /*! \brief ����һ����ʽ���������ͼ��
     *
     *    @param	ta_name	��ʽ��
     *    @param	para	��ʽ����/�������
     *    @return	��ʽ��ͼ���ڵ�ID��>0��������ʽ�ɹ���
     */
    int32_t showFormula(const SkString& ta_name, const formula::TaParam& para);

    /*! \brief ����һ����ʽ
     *
     *    @param
     *    @return
     */
    void removeFormula(int32_t id);

    /*! \brief ��ʾ�������ؿؼ�
     *
     *    @param	show	true��ʾ
     *    @return
     */
    template <typename _Item>
    void showPanelItem(my_sp<_Item>& item, PanelItemType typ, bool show) {
        flags_.set(typ, show);
        if (show) {
            if (!item) {
                item = std::make_shared<_Item>(this);
                initPanelItem(item.get());
            }
        } else {
            if (item) {
                // �Ӹ��׿ؼ����Ƴ�
                removeChild(item.get());
                item.reset();
            }
        }
    }

    inline bool hasItem(PanelItemType typ)const {
        return flags_.test(typ);
    }

    inline GridLine& grid() {
        return mainview_->grid();
    }

    inline Title* title() {
        return &title_;
    }

    /*! \brief ��ʾ���ؿؼ�
     *
     *    @param    item    �μ�PanelItemType
     *    @return
     */
    void showItem(int item, bool show);

    void seriesRemoved(const SkString& series_name);

    inline const SkTArray<my_sp<MainView>>& subViews()const {
        return subviews_;
    }

    /*! \brief ��sub���ϲ�����
     *
     *    @param
     *    @return
     */
    bool mergePanel(Panel* sub);

    /*! \brief �Ƴ���ͼitem
     *
     *    @param    item    ������series����object��
     *    @return
     */
    void removeViewitem(ViewItem* item);

    inline const std::unordered_map<int32_t, _Formula>& formulas()const {
        return formulas_;
    }

protected:
    /*! \brief ��ʼ���ؼ�
     *
     *    @param
     *    @return
     */
    void initItems();

    void initPanelItem(VisualItem* w);

    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

    // ˢ�¹�ʽ
    void refreshFormulas();

    /*! \brief ������ʽ���
     *
     *    @param
     *    @return
     */
    bool createFormulaSeries(const SkString& formula_name, const int32_t id, _Formula& fml,
                             const SkTArray<formula::TaSeries>* p_series, const formula::TaParam& user_param);

    /*! \brief ����������������ָ��ʵ��
     *
     *    @param
     *    @return   -1 û�д�ָ��
     */
    int32_t findFormulaBySeriesName(const SkString& series_name)const;

private:
    BitFlags flags_;

    // �ؼ�
    PanelLayout layout_;

    //! ��ͼ
    my_sp<MainView> mainview_ = nullptr;

    //! ��ͼ
    SkTArray<my_sp<MainView>> subviews_;

    Title title_;// = nullptr;
    my_sp<ValueAxis> value_ = nullptr;
    my_sp<RatioAxis> ratio_ = nullptr;
    my_sp<CalendarAxis> calendar_ = nullptr;

    //! ͼ���еĹ�ʽ
    std::unordered_map<int32_t, _Formula> formulas_;
};

#define ToPanel(hPanel) reinterpret_cast<Panel*>(hPanel)
#define HPanel(panel) reinterpret_cast<HPANEL>(panel)

}
}

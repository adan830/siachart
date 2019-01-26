#pragma once

#include "panelitem.h"
#include "defines.h"

namespace sia {
namespace charty {

/*!
 * \class VisualItem
 *
 * \brief ͼ�������пɼ�������Ԫ�ض�Ҫ�̳��ڴ���
 *
 */
class Title : public PanelItem {
    enum Flags {
        // �Ƿ���ʾΪ��
        kBlankTitle = 0x1,

        // �Ƿ��ж�̬item
        kDynamicItem = 0x2,
    };

public:
    Title(Panel* parent = nullptr);

    void addTitleItem(const PanelTitleItem& item);
    void removeTitleItem(const SkString& name);

    void setItems(const SkTArray<PanelTitleItem>& items);

    void setBlank(bool blank);

    inline const SkTArray<PanelTitleItem>& titleItems()const {
        return items_;
    }

    inline bool hasCustomItem()const {
        return !flags_.test(Flags::kBlankTitle) && !items_.empty();
    }

    inline bool hasDynamicItem()const {
        return !flags_.test(Flags::kBlankTitle) && flags_.test(Flags::kDynamicItem);
    }

    /*! \brief ���б�ɾ����
     *
     *    @param    series_name ������
     *    @return
     */
    void seriesRemoved(const SkString& series_name);

    /*! \brief ��panel�У��������ֲ�������
     *
     *    @param    series_name ������
     *    @param    view ����view
     *    @return   ����ָ��
     */
    my_sp<Series> series(const SkString& series_name, my_sp<MainView>& view)const;

protected:
    virtual void preparePaints() override;

    virtual void onDraw(SkCanvas* canvas) override;

    void checkTitleItems();

private:
    //! title��ʾ����Ŀ
    SkTArray<PanelTitleItem> items_;

    //! �Ƿ���ʾΪ��
    BitFlags flags_;

    SkPaint* bk_ = nullptr;
    SkPaint* fg_ = nullptr;
    SkPaint* border_ = nullptr;
};
}
}

#pragma once

#include "defines.h"
#include "visualitem.h"

namespace sia {
namespace charty {

class Panel;
class MainView;
class Series;

/*!
 * \class PanelItem
 *
 * \brief ����Panel��ֱ�Ӻ��Ӷ�Ҫ�̳д���
 *
 *
 */
class PanelItem : public VisualItem {
public:
    PanelItem(const SkString& name, Panel* parent);

    inline void setParent(Panel* p) {
        VisualItem::setParent((Widget*)p);
    }

    Panel* parent()const;

    inline Panel* panel()const {
        return parent();
    }

    MainView* mainView()const;

    std::vector<my_sp<Series>> serieses()const;

    /*! \brief PanelItem �������Ƶ�ʱ�򣬶�����padding�ġ�
     *
     *    @param
     *    @return
     */
    void setContentArea(SkRect rc) {
        content_rect_ = rc;
    }

    const SkRect& contentRect()const {
        return content_rect_;
    }

    inline SkScalar contentHeight()const {
        return content_rect_.height();
    }

    inline SkScalar contentWidth()const {
        return content_rect_.width();
    }

    inline SkScalar contentX()const {
        return content_rect_.left();
    }

    inline SkScalar contentY()const {
        return content_rect_.right();
    }

private:
    SkRect content_rect_;
};

}
}

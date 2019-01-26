#pragma once

#include "resitem.h"

namespace sia {
namespace resy {

/*!
 * \class StrokeItem
 *
 * \brief ���ڹ���Paint��Stroke���
 *
 */
class RESY_API StrokeItem : public ResItem {
public:
    StrokeItem(const SkString& id = SkString(""))
        :ResItem(ResType::kResStroke, SkString("stroke"), id) {
    }

    /*! \brief ����stroke�ַ���
     *
     *    @param res �﷨����$width [$cap] [$join]��, eg:"3.2 round miter"
     *          $width = (0, 1000]
     *          $cap = [butt|round|square]
     *          $join = [miter|round|bevel]
     *    @return
     */
    virtual bool parse(const SkString& res) override;

    inline SkScalar width()const {
        return width_;
    }

    SkPaint::Join join()const {
        return join_;
    }

    SkPaint::Cap cap()const {
        return cap_;
    }

    /*! \brief ����Cap
     *
     *    @param cap = [butt|round|square]
     *    @return default: SkPaint::kDefault_Cap
     */
    static SkPaint::Cap parseCap(const char* cap);

    /*! \brief ����join
     *
     *    @param join = [miter|round|bevel]
     *    @return default: SkPaint::kDefault_Join
     */
    static SkPaint::Join parseJoin(const char* join);

private:
    SkScalar width_ = 0.f;
    SkPaint::Join join_ = SkPaint::kDefault_Join;
    SkPaint::Cap cap_ = SkPaint::kDefault_Cap;
};

}
}

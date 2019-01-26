#pragma once

#include "chartapi.h"
#include "defines.h"
#include "../base/base.h"

namespace sia {
namespace charty {

class ObjectItem;

/*!
 * \class RelPos
 *
 * \brief ������꣨relate position������
 *
 */
struct CHARTY_API RelPos {
    SkScalar l = chartbase::kInvalidCoord;
    SkScalar t = chartbase::kInvalidCoord;
    SkScalar r = chartbase::kInvalidCoord;
    SkScalar b = chartbase::kInvalidCoord;
};

/*!
 * \class ObjectParam
 *
 * \brief �Զ������Ĵ�С/λ�ò���
 *
 */
struct CHARTY_API ObjectParam {
    //! λ��
    PosType postyp;

    //! �������(Ŀǰֻ֧������ڸ�����border��λ��) ��PosType::kRelated ��Ч��
    RelPos relpos;

    //! �����С����ѡ����
    SkSize sz;

    //! ����x�����ϵĶ�λ
    chart_key_t x;

    //! attach������� ��PosType::kAttached ��Ч��
    SkString attach;

    //! ����value���㣬��PosType::kValued ��Ч��
    chart_val_t val;

    //! ��ʾ���ݣ�Ϊ������ʾ��Ϣ
    SkString tooltip;
};

/*!
 * \class IVisualObject
 *
 * \brief ͼ��Ŀ��ӻ�Object
 *
 */
class CHARTY_API IVisualObject {
    friend class ObjectItem;
    // friend class std::_Ref_count_obj<IVisualObject>;
public:
    static my_sp<IVisualObject> create(ObjectType typ);

    explicit IVisualObject(ObjectType typ)
        :typ_(typ) {
    }

    virtual ~IVisualObject() {

    }

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) = 0;
    virtual void buildPaint();

public:
    SkColor color;

protected:
    ObjectType typ_ = ObjectType::kNotSet;
    PosType pos_ = PosType::kNotSet;
    RelPos rel_;
    SkPaint paint_;
};

class CHARTY_API TextObject : public IVisualObject {
    // friend class std::_Ref_count_obj<TextObject>;
public:
    SkString text;
    bool wrap;
    SkString font_face;
    SkScalar text_size = SkIntToScalar(0);
    SkFontStyle font_style = SkFontStyle::Normal();

    TextObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;

    virtual void buildPaint() override;

private:
    sk_sp<SkTypeface> face_ = nullptr;
};

class CHARTY_API PictrueObject : public IVisualObject {
    // friend class std::_Ref_count_obj<PictrueObject>;

public:
    SkString path;
    PictrueObject();

protected:

    virtual void draw(SkCanvas*, const SkRect& rc) override;

    virtual void buildPaint() override;

private:
    sk_sp<SkImage> img_ = nullptr;
};

class CHARTY_API LineObject : public IVisualObject {
    // friend class std::_Ref_count_obj<LineObject>;
public:
    chart_val_t val;
    LineObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;
    virtual void buildPaint() override;

};

class CHARTY_API RichTextObject : public IVisualObject {
    // friend class std::_Ref_count_obj<RichTextObject>;

public:
    RichTextObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;
    virtual void buildPaint() override;
};

class CHARTY_API MovieObject : public IVisualObject {
    // friend class std::_Ref_count_obj<MovieObject>;

public:
    MovieObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;
    virtual void buildPaint() override;
};

class CHARTY_API CustomObject : public IVisualObject {
    // friend class std::_Ref_count_obj<CustomObject>;
public:
    typedef std::function<void(SkCanvas*, const SkRect&)> DrawCallback;

    void setDrawer(DrawCallback cb);
    CustomObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;

private:
    DrawCallback cb_;
};

}
}
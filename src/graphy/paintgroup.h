#pragma once

#include "graphyapi.h"
#include "resy/resitem.h"

// paintman ���ڴ������ļ���������paint����
// ������ʵ������ʽ�Ļ���
namespace sia {
namespace resy {
class ResItem;
}
namespace graphy {

class ComposedPaint;
class PaintMan;

enum PaintResType {
    kGeometry = resy::ResType::kFramework + 1,
};

class GRAPHY_API GeometryItem : public resy::ResItem {
public:
    GeometryItem(const SkString& res_id = SkString(""))
        :ResItem(PaintResType::kGeometry, SkString("geometry"), res_id) {
    }

    virtual bool parse(const SkString& res) override;

    const SkRect& rect()const {
        return rc_;
    }

private:
    SkRect rc_;
};

/*!
 * \class PaintGroup
 *
 * \brief һ��SkPaint��geometry�ļ��Ϲ���ÿ��Group����һ��@paint����Ķ��PaintItem ��һ��geometry��
 *
 *      ���磺graphy::Widget����ʹ��PaintMan/PaintGroup�����SkPaint���������Լ��������ʾ��
 *      ע�⣺PaintGroup��graphy::Widget��û�б�Ȼ����ϵ��graphy�ϲ��Ӧ�����������֮��Ĺ�����
 *
 *      ͨ������£����ǿ��Խ�һ��Widget��һ��PaintGroup��Ӧ������
 *      ʹ�ü̳еķ�ʽ����д��Ҫ���麯����������paint�����仯��ʱ��õ�֪ͨ��
 *      �������Ҫ��ʹ�������ڶ�̬�ı������ʾ����ô���ʹ�ø�����㡣
 *
 *      group_id:   @paint/group/item
 *
 */
class GRAPHY_API PaintGroup {

    friend class PaintMan;
public:
    PaintGroup(const SkString& group_id);
    ~PaintGroup();

    SkPaint* getPaint(const SkString& paint_item_name, SkPaint* default_p = nullptr);
    bool applyRes(const SkString& paint_item_name, my_sp<sia::resy::ResItem> res);

    bool getGeometry(SkRect& out, const SkString& paint_item_name = SkString("geometry"));

    bool adjustRawRes(SkString& res_typ, SkString& key, SkString& res_val);

    // Ԥ����
#ifdef DEBUG_CODE
    void dumpInfo();
#endif

private:
    my_sp<ComposedPaint> composedPaint(const SkString& paint_item_name, bool create_if_not_exist = false);

protected:
    // ��Դ�б�
    // key: paint_item_name
    std::unordered_map<SkString, my_sp<ComposedPaint>> paints_;
    SkString group_id_;
};
}
}

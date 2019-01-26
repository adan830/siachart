#pragma once

#include "siapi.h"
#include "paintgroup.h"
#include "resy/resman.h"

// paintman ���ڴ������ļ���������paint����
// ������ʵ������ʽ�Ļ���
namespace sia {
namespace graphy {

class Widget;
class PaintGroup;

/*!
 * \class PaintMan
 *
 * \brief SkPaint���õĽ����͹���
 *      ���磺graphy::Widget����ʹ��PaintMan/PaintGroup�����SkPaint���������Լ��������ʾ��
 *      ע�⣺PaintGroup��graphy::Widget��û�б�Ȼ����ϵ��graphy�ϲ��Ӧ�����������֮��Ĺ�����
 *
 *      ͨ������£����ǿ��Խ�һ��Widget��һ��PaintGroup��Ӧ������
 *      ʹ�ü̳еķ�ʽ����д��Ҫ���麯����������paint�����仯��ʱ��õ�֪ͨ��
 *      �������Ҫ��ʹ�������ڶ�̬�ı������ʾ����ô���ʹ�ø�����㡣
 */
class GRAPHY_API PaintMan :
    public sia::resy::ResMan,
    public std::enable_shared_from_this<PaintMan> {

public:
    PaintMan();
    ~PaintMan();

    // ���󴴽���ɺ�Ҫ������������������г�ʼ����
    void init();

    bool load(const SkString& file);

    my_sp<PaintGroup> paintGroup(const SkString& group_name, bool create_if_not_exist = false);

    static my_sp<PaintMan> instance();

#ifdef DEBUG_CODE
    void dumpInfo()const;
#endif

protected:
    virtual void onLuaOpen(sol::state& lua) override;

    // paint_item: @paint/group/item
    void buildPaint(const SkString& paint_item, const SkString& res_typ, const SkString& res_val);

private:
    // ��ǰϵͳ��ע���painters
    // key -- paint_group_name
    std::unordered_map<SkString, my_sp<PaintGroup>> paints_;

    // ʵ��ָ��
    static my_sp<PaintMan> inst_;
};

}
}

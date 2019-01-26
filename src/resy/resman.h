#pragma once

#include "siapi.h"
#include "resitem.h"
#include "deps/sol2/sol.hpp"
#include "resfactory.h"

#define GLOBAL_GROUP_NAME "@global"
#define PAINT_GROUP_NAME "@paint"

namespace sia {
namespace resy {

/*!
 * \class ResGroup
 *
 * \brief һ����Դ���Ϲ���
 *      Ϊ�˱�����Դ�Ĺ���������Ϊ�İ���Դ��Ϊһ�飨��ʹ���ߵ��÷���ע�Ⲣ������Դ�����ͷ��飩��
 *      ���磬һ������ӵ���Լ����ص���Դ�������������ã���ɫ���ã�ͼƬ�ȣ���ͬʱ��Ҳ������һЩȫ�ֵ���Դ��
 *      ��������£����ǿ������һ��ȫ����Դ��@global����һЩȫ�ֵ�����/ͼƬ�ȷŽ�ȥ��
 *      �����һ��@window�������Լ���ͼƬ/����ȡ������Ϳ��Խ���Դ��Ч/�����Ĺ���������
 *
 *      ��Դ������Ҫͳһ����࣡�໥���������ֳ�ͻ��
 */
class RESY_API ResGroup { /*: public util::enable_shared_from_this_virtual<ResGroup>*/
public:
    virtual ~ResGroup();

    // void add(const SkTArray<my_sp<ResItem>>& a);
    void add(my_sp<ResItem> a);

    /*! \brief ������е�Resource
     *
     *		ResItem �� ResGroup���໥���õĹ�ϵ�������ֶ������ڴ�
     */
    void clearRes();

    // ���÷����淶���벻Ҫ�������ص�ָ��ֵ��
    // TODO: ��ResItem �̳���enable_shared_from_this����ʹ��shared_from_this()
    template<typename ResType>
    ResType* get(const SkString& id) {
        if (auto p = rawRes(id)) {
            return reinterpret_cast<ResType*>(p.get());
        }
        return nullptr;
    }

    my_sp<ResItem> rawRes(const SkString& id);

#ifdef DEBUG_CODE
    void dumpInfo();
#endif

private:
    // key - resource id ��ȫ·��id������@global/font/h1��
    std::unordered_map<SkString, my_sp<ResItem>> res_;
};

/*!
 * \class ResManBase
 *
 * \brief ������Դ���������������/�̳д�����ʵ���Լ�����Դ����/��������
 *      ���磺һ��Ӧ�ÿ�����Ҫʹ��lua����һ�ֽű����������Լ��Ĵ�����ɫ/��С�ȣ�����ʵ��һ����CSS�Ľű���
 *      ����Լ̳д��࣬�ṩ�Լ�����Դ�����������Բο�sia::graphy::PaintMan��ʵ��
 *
 *      ������ʵ���˼��ֻ�������Դ����/��������
 *      ** font/image/stroke/color
 *
 *      �˹���������������lua���������Թ�lua����ʹ�ã�ʹ��ʵ�����Բο���doc/lua/paint_parser.lua����
 *      1��host_reg_res : ע����Դ
 *      2��host_raise_error: ��ʾlua��������
 *      ͨ��parseFile������ִ��/�����û��Զ������Դ�﷨���Դ�ʹ�ýű���չ�����ܵ�Ŀ��
 *
 */
class RESY_API ResMan {
public:
    ResMan();

    virtual ~ResMan();

    /*! \brief ��յ�ǰ���е���Դ����Դ�����Դ��
     *
     *    @param
     *    @return
     */
    void clearRes();

    /*! \brief �����û��Զ������Դ�ļ�
     *
     *    @param    lua_res_file �û��Լ���Ƶ���Դ�ļ�����ѭlua�﷨��
     *              res_parser_code �û�����Լ���Ƶ���Դ�﷨ʵ�ֵĽ������루lua������
     *
     *    @return   ������ִ��󣬿���ͨ�� onLuaError �������Ĵ�����Ϣ��
     */
    bool parseFile(const std::string& lua_res_file, const std::string& res_parser_code);

    /*! \brief ����resGroup
     *
     *    @param    name    ��������'@'��ͷ������ж༶����Ҫ�� '/'�ָ�·�����磺@paint/title
     *              Ŀǰ��������֪/������group����(�Զ����group�����ɺ��������ظ�)��
     *              1��@global  ����ȫ�ֵ���Դgroup��������������Դ������������
     *              2��@paint   sia::graphy�Զ���ģ���������SkPaint
     *
     *              create_if_not_exist true ���û�оʹ���һ��group
     *    @return
     */
    my_sp<ResGroup> findGroup(const SkString& name) {
        return getGroup(name, false);
    }

    /*! \brief ע��һ����Դ������
     *	        �������ͬ���͵���Դ�����������µĽ����ǰ��ģ�
     *
     *    @param    factory ��Դ������
     *    @return
     */
    void regResFactory(my_sp<IResFactory> factory);

#ifdef DEBUG_CODE
    void dumpInfo()const;
#endif

    inline SkString errorMsg()const {
        return error_msg_;
    }

protected:
    // ��lua�򿪵�ʱ�����ע��һЩ�Զ���ĺ���
    virtual void onLuaOpen(sol::state& lua) {}

    /*! \brief ע����Դ
     *
     *    @param	group_name	��Դ�����飬�ο�ResGroup
     *    @param	res_typ	��Դ���ͣ��ο�ResItem(color/font ...)
     *    @param	key	��Դ�����ο�ResItem::id
     *    @param	val	��Դֵ
     *    @return	��Դ����ָ�룬�����Դ�������󣬷���nullptr
     */
    virtual my_sp<ResItem> regResItem(const SkString& group_name, const SkString& res_typ, const SkString& key, const SkString& val);

    /*
     *	lua����
     */
    virtual void onLuaError(const SkString& err) {
        std::cout << err.c_str() << std::endl;
    }

    /*! \brief ����һ����Դ������
     *
     *    @param
     *    @return
     */
    my_sp<IResFactory> factory(const SkString& res_type)const;

protected:
    bool splitGroupPath(const SkString& prefix, SkString& name, SkString& item) const;
    bool extractResType(const SkString& resval, SkString& typ) const;

    /*! \brief ���ô�����Ϣ
     *
     *    @param	err	������Ϣ
     *    @return
     */
    void setError(const SkString& err);
private:
    /*! \brief ����resGroup
     *
     *    @param    name    ��������'@'��ͷ������ж༶����Ҫ�� '/'�ָ�·�����磺@paint/title
     *              Ŀǰ��������֪/������group����(�Զ����group�����ɺ��������ظ�)��
     *              1��@global  ����ȫ�ֵ���Դgroup��������������Դ������������
     *              2��@paint   sia::graphy�Զ���ģ���������SkPaint
     *
     *              create_if_not_exist true ���û�оʹ���һ��group
     *    @return
     */
    my_sp<ResGroup> getGroup(const SkString& name, bool create_if_not_exist = false);

protected:
    // key - ��Դ��id���ο�getGroup����
    std::unordered_map<SkString, my_sp<ResGroup>> all_;

    // ���ڽ���������Դ�������Զ����Լ�����Դ������
    std::unordered_map<SkString, my_sp<IResFactory>> factory_;

    // ������Ϣ
    SkString error_msg_;
};

//////////////////////////////////////////////////////////////////////////
template <typename Res>
class KnownResFactory : public IResFactory {
public:
    KnownResFactory(ResMan* man)
        :IResFactory(Res().resTypeStr()), man_(man) {
    }

    virtual my_sp<ResItem> parseRes(const SkString& group_name, const SkString& key, const SkString& val) override {
        my_sp<ResItem> p(new Res(key));
        p->setCommonResRef(man_->findGroup(group_name), man_->findGroup(SkString(GLOBAL_GROUP_NAME)));

        if (!p->parse(val)) {
            return nullptr;
        }
        return p;
    }

private:
    ResMan* man_ = nullptr;
};

}
}

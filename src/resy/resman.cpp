#include "resman.h"
#include "asset.h"
#include "fontitem.h"
#include "coloritem.h"
#include "imageitem.h"
#include "strokeitem.h"
#include "effectitem.h"

namespace sia {
namespace resy {

ResGroup::~ResGroup() {
    clearRes();
}

void ResGroup::add(my_sp<ResItem> p) {
    std::cout << "+ res:" << p->key().c_str() << std::endl;
    // ��Դkey�б����Ǵ�����Դ���͵ģ�
    assert(p->key().contains("/"));
    res_.insert({ p->key(), p });
}

void ResGroup::clearRes() {
    for (auto i : res_) {
        i.second->releaseRes();
    }
    res_.clear();
}

my_sp<ResItem> ResGroup::rawRes(const SkString& id) {
    auto i = res_.find(id);
    if (i != res_.end()) {
        return i->second;
    }
    return nullptr;
}

#ifdef DEBUG_CODE
void ResGroup::dumpInfo() {
    for (auto i : res_) {
        std::cout << i.first.c_str() << " --> " << i.second->respath().c_str() << " | ";
    }
}
#endif

//////////////////////////////////////////////////////////////////////////
ResMan::ResMan() {
    // ������ע��һЩ��֪��
    regResFactory(std::make_shared<KnownResFactory<FontItem>>(this));
    regResFactory(std::make_shared<KnownResFactory<ColorItem>>(this));
    regResFactory(std::make_shared<KnownResFactory<ImageItem>>(this));
    regResFactory(std::make_shared<KnownResFactory<StrokeItem>>(this));
    regResFactory(std::make_shared<KnownResFactory<EffectItem>>(this));
}

ResMan::~ResMan() {
    clearRes();
}

void ResMan::clearRes() {
    for (auto i : all_) {
        i.second->clearRes();
    }
    all_.clear();
}

bool ResMan::parseFile(const std::string& lua_file, const std::string& user_res_parser_file) {
    if (lua_file.empty() || user_res_parser_file.empty()) {
        assert(false);
        return false;
    }

    // ���������Լ���lua������
    std::string common_res_parser;
    if (!Asset::read("lua/res_parser.lua", common_res_parser)) {
        assert(false);
        setError(SkString(u8"load asset [lua/res_paresr.lua] failed!"));
        return false;
    }

    try {
        sol::state lua;
        lua.open_libraries(sol::lib::base);
        // ע���Լ�c++������ȥ����luaȥ����
        // ���������֪ͨ
        lua.set_function("host_raise_error", [=](const std::string& err) {
            setError(SkString(err.c_str()));
        });

        // ע����Դ
        lua.set_function("host_reg_res", [=](const std::string& name, const std::string& typ, const std::string& key, const std::string& val) {
            // std::cout << name << typ << key << val;
            this->regResItem(SkString(name.c_str()), SkString(typ.c_str()), SkString(key.c_str()), SkString(val.c_str()));
        });
        onLuaOpen(lua);

        // ����/���/ִ��/�����
        auto configer_code = lua.safe_script_file(lua_file);
        if (!configer_code.valid() /*|| !code().valid()*/) {
            assert(false);
            return false;
        }

        // �Ȱ����ǵ�ͨ�õ���Դ������ע���ȥ
        lua.safe_script(common_res_parser);

        // �����ִ���û��ṩ���Զ�����Դ����
        configer_code = lua.safe_script(user_res_parser_file);
        if (!configer_code.valid()/* || !code().valid()*/) {
            assert(false);
            return false;
        }
    } catch (sol::error& e) {
        setError(SkString(e.what()));
        return false;
    }

    // �ȴ��ص�
    return true;
}

my_sp<ResItem> ResMan::regResItem(const SkString& group, const SkString& typ, const SkString& key, const SkString& val) {
//     std::cout << "resman reg res: group:" << group.c_str() << ", type:" << typ.c_str()
//               << ", key:" << key.c_str() << ", val:" << val.c_str() << std::endl;

    SkString res_type = typ;
    if (res_type.isEmpty()) {
        // ���е���Դ����Ҫ����������Ҫ��֪��������Դ���͡�
        // ���ԭʼ����Դ�ַ�������û����Դ���͵Ļ�����������Ҫ��һ������
        // ���õĶ����а�����Դ���͡�����������֪����Դ���ͣ�
        if (!extractResType(val, res_type)) {
            return nullptr;
        }
    }

    SkString group_name, paint_item;
    if (!splitGroupPath(group, group_name, paint_item)) {
        return nullptr;
    }

    // ʹ�ù������ơ������Ͳ����������������͵�ʱ���ٸ�����Ĵ��롣
    if (auto fac = factory(res_type)) {
        auto item = fac->parseRes(group_name, key, val);
        if (item && !key.isEmpty()) {
            // ֻ����Դid��Ϊ�յ�ʱ�����Ҫ��¼����
            // ������Դ��������Դ������������ʹ�ã�Ȼ��ͻ�����
            if (auto p = getGroup(group_name, true)) {
                p->add(item);
            }
        }
        return item;
    }
    std::cout << "parse res:" << res_type.c_str() << " failed!\n";
    return nullptr;
}

void ResMan::regResFactory(my_sp<IResFactory> factory) {
    factory_.insert({ factory->resType(), factory });
}

my_sp<sia::resy::IResFactory> ResMan::factory(const SkString& res_type) const {
    auto i = factory_.find(res_type);
    if (i != factory_.end()) {
        return i->second;
    }
    return nullptr;
}

my_sp<ResGroup> ResMan::getGroup(const SkString& name, bool create_if_not_exist /*= false*/) {
    auto i = all_.find(name);
    if (i != all_.end()) {
        return i->second;
    }

    if (create_if_not_exist) {
        auto p = std::make_shared<ResGroup>();
        all_.insert({ name, p });
        return p;
    }
    return nullptr;
}

bool ResMan::splitGroupPath(const SkString& prefix, SkString& name, SkString& item) const {
    if (prefix.isEmpty()) {
        return false;
    }

    // ���� @paint/mywin/common/color/red   --> @paint/mywin
    //      @global/color/red   --> @global

    SkTArray<SkString> out;
    SkStrSplit(prefix.c_str(), "/", SkStrSplitMode::kCoalesce_SkStrSplitMode, &out);
    if (out.count() <= 1) {
        name = prefix;
        item = prefix;
        return true;
    }

    int idx = -1;
    if (out[0].equals(GLOBAL_GROUP_NAME)) {
        name = SkString(GLOBAL_GROUP_NAME);
        idx = 1;
    } else if (out[0].equals(PAINT_GROUP_NAME)) {
        name.append(PAINT_GROUP_NAME);
        name.append("/");
        name.append(out[1]);

        idx = 2;
    }

    if (idx > 0) {
        for (; idx < out.count(); ++idx) {
            item.append(out[idx]);
            item.append("/");
        }
        if (!item.isEmpty()) {
            // ȥ������ ��/��
            item.remove(item.size() - 1, 1);
        }
    } else {
        name = prefix;
        item = prefix;
    }
    return true;
}

bool ResMan::extractResType(const SkString& resval, SkString& typ) const {
    // ֻ�����òſ�����ȡtype
    if (!resval.startsWith("@")) {
        return false;
    }

    // ��val�н���
    SkTArray<SkString> out;
    SkStrSplit(resval.c_str(), "/", SkStrSplitMode::kCoalesce_SkStrSplitMode, &out);
    if (out.count() < 2) {
        return false;
    }
    typ = out[1];
    return true;
}

void ResMan::setError(const SkString& err) {
    error_msg_ = err;
    onLuaError(err);
}

#ifdef DEBUG_CODE
void ResMan::dumpInfo()const {
    for (auto i : all_) {
        std::cout << "ResManBase: " << i.first.c_str() << " : ";
        i.second->dumpInfo();
        std::cout << std::endl;
    }
}
#endif

}
}

#pragma once

#include "formulapi.h"

namespace sia {

namespace chartbase {
class ColumnData;
class DataSource;
class TaParam;
}

namespace formula {

class IFormula;

class GroupMethod {
public:
    GroupMethod(const SkString& name);

    inline bool add(const SkString& method) {
        if (exist(method)) {
            return false;
        }
        methods_.push_back(method);
        return true;
    }

    inline const SkTArray<SkString>& methods()const {
        return methods_;
    }

    inline const SkString & groupName()const {
        return group_;
    }

    inline int count()const {
        return methods_.count();
    }

    void dumpInfo()const;

    inline bool exist(const SkString& method)const {
        for (auto i : methods_) {
            if (method.equals(i)) {
                return true;
            }
        }
        return false;
    }

private:
    SkTArray<SkString> methods_;
    SkString group_;
};

class TAFuncGroup : public GroupMethod {
public:
    TAFuncGroup(const SkString& group);
};

/*!
 * \class TaLib
 *
 * \brief �ṩTalib�ĵ��ýӿ�
 *
 */
class TaLib {
public:
    TaLib(const chartbase::DataSource* ds = nullptr);

    virtual ~TaLib();

    const SkTArray<my_sp<TAFuncGroup>>& groups()const {
        return methods_;
    }

    void dumpInfo()const;

protected:
    void initMethods();

private:
    //! talib �ĺ���
    SkTArray<my_sp<TAFuncGroup>> methods_;

    //! ����Դ
    const chartbase::DataSource* ds_ = nullptr;
};
}
}

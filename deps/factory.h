#pragma once

/*!
 * \brief ͨ�õĹ���ģʽ��factory pattern��ʵ�֡������������ڶ�̬����C++����
 *
 *	�ο��� https://github.com/google/infact
 *	����һ���򻯵�ʵ�֣����þͺ� :D
 *	������µ�ģʽʹ�ã�
 *		class Base {}
 *		class D1 : public Base {}
 *		class D2 : public Base {}
 *		...
 *		����ָ�����ֶ�̬���ɶ���
 *	----------------
 *		FFactory<Base> ff;
 *		ff.add<D1>("D1");
 *		ff.add<D2>("D2");
 *		...
 *	-----------------
 *		������
 *		Base* d1 = ff.create("D1");
 */

#include <algorithm>
#include "SkString.h"

template<typename _Typ>
class _TrivalC {
public:
    virtual _Typ* create() = 0;
    virtual void destroy(_Typ*) = 0;
    virtual const SkString& typeName()const = 0;
};

/*!
 * \class TypeFactory
 *
 * \brief ���͹���
 *		_Trival_Class���ṩcreate /destroy ��ӿڣ�
 */
template<typename _Typ, typename _BaseTyp>
class TypeFactory : public _TrivalC<_BaseTyp> {
    static_assert(std::is_base_of<_BaseTyp, _Typ>::value, "_Typ MUST be derived from _BaseTyp");

public:
    TypeFactory(const SkString& typ_name)
        :name_(typ_name) {
    }

    virtual ~TypeFactory() {
        for (auto w : objs_) {
            delete w;
        }
    }

    virtual _BaseTyp* create() override {
        _Typ *w = new _Typ(typeName());
        objs_.push_back(w);
        return w;
    }

    virtual void destroy(_BaseTyp* p) override {
        auto itr_end = objs_.end();
        auto i = std::remove(objs_.begin(), itr_end, p);
        if (i != itr_end) {
            delete p;
            objs_.pop_back_n(int(itr_end - i));
        }
    }

    virtual const SkString& typeName()const {
        return name_;
    }

protected:
    //! ��ʵ�����Ķ���
    SkTArray<_BaseTyp*> objs_;

    //! �ܹ�������������
    SkString name_;
};

/*!
 * \class FFactory
 *
 * \brief ���͹����Ĺ�����
 *
 */
template<typename _BaseTyp>
class FFactory {
public:
    ~FFactory() {
        clear();
    }

    void clear() {
        for (auto f : factorys_) {
            delete f;
        }
        factorys_.reset();
    }

    /*! \brief ��ע������͹���
     *
     *    @param	names [out]	������
     *    @return
     */
    void factoryNames(SkTArray<SkString>& names)const {
        for (auto t : factorys_) {
            names.push_back(t->typeName());
        }
    }

    template<class _FactoryItem>
    TypeFactory<_FactoryItem, _BaseTyp>* add(const SkString& name) {
        auto factory = new TypeFactory<_FactoryItem, _BaseTyp>(name);
        factorys_.push_back(factory);
        return factory;
    }

    _BaseTyp* create(const SkString& name) {
        if (auto f = findFactory(name)) {
            return f->create();
        }
        return nullptr;
    }

    void destory(const SkString& name, _BaseTyp* obj) {
        if (auto f = findFactory(name)) {
            return f->destroy(obj);
        }
    }

protected:
    /*! \brief ����ע������͹���
     *
     *    @param	name	������
     *    @return	���͹���ʵ��
     */
    _TrivalC<_BaseTyp>* findFactory(const SkString& name) {
        for (auto t : factorys_) {
            if (t->typeName() == name) {
                return t;
            }
        }
        return nullptr;
    }

private:
    //! ��ע������͹���ʵ��
    SkTArray<_TrivalC<_BaseTyp>*> factorys_;
};

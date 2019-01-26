#pragma once

#include "deps/factory.h"
#include "formulapi.h"
#include "tadata.h"
#include "taparam.h"
#include "iformula.h"

namespace sia {
namespace formula {

struct FormulaManPrivate;

/*!
 * \class FormulaMan
 *
 * \brief ����ϵͳ����ע��Ĺ�ʽ
 *
 */
class FORMULA_API FormulaMan {
public:
    FormulaMan();
    ~FormulaMan();

    /*! \brief ע�ṫʽ
     *
     *    @param	group	��ʽ����
     *    @param	name	��ʽ������ʽ����ȫ��Ψһ����ͬ�ķ���������Ҳ������ͬ��
     *    @return
     */
    template<typename _Typ>
    void regFormula(const SkString& group, const SkString& name) {
        ff_.add<_Typ>(name);
        _regFormula(group, name);
    }

    /*! \brief ʹ�ù�ʽ
     *
     *    @param
     *    @return
     */
    IFormula* create(const SkString& name);

    /*! \brief �����Ѵ�����ʵ��
     *
     *    @param	name	������
     *    @param	indi	ʵ��
     *    @return
     */
    void destroy(const SkString& name, IFormula* indi);

    /*! \brief ��ȡ��ʽ����
     *
     *    @param	group	[out]
     *    @return
     */
    void groups(SkTArray<SkString>& group)const;

    /*! \brief ���鹫ʽ
     *
     *    @param	group
     *    @param	method [out]
     *    @return
     */
    void methods(const SkString& group, SkTArray<SkString>& method)const;

protected:
    void _regFormula(const SkString& group, const SkString& name);

private:
    FFactory<IFormula> ff_;

    //! ˽������
    FormulaManPrivate* d = nullptr;
};
}
}

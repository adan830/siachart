#pragma once

#include "formulapi.h"
#include "tadata.h"
#include "taparam.h"

namespace sia {
namespace chartbase {
class DataSource;
}
namespace formula {

/*!
 * \class IFormula
 *
 * \brief ��װ��ʽ�Ļ����ӿ��࣡
 *	�÷���
 *		1������IFormula���� m
 *		2����ѯ����������Ҫ����ʲô���ݣ�queryParam
 *		3������2��ѯ�������ݣ�m.data() = mydata;
 *			3.1 ����ʹ������Դ�ı�ݷ�ʽ��m.useDataSource
 *		4��������������㣺m.calc(myparam);
 *		5���ӹ�ʽ���ó����ݲ�ʹ�ã�m.data().out1();
 */
class FORMULA_API IFormula {
public:
    IFormula(const SkString& name);
    virtual ~IFormula();

    const TaData& data()const {
        return data_;
    }

    /*! \brief ��ȡ���ݽṹ
     *		����ͨ���˷��������ڲ��������ݡ�
     *
     *    @param
     *    @return
     */
    inline TaData& data() {
        return data_;
    }

    /*! \brief ʹ�ô˷�����data()�й����Ҫ�Ĳ�����
     *		�˷������Զ����ṩ������Դ��Ѱ���������Ƶ��������У�
     *			open/high/low/clow/oi/volume
     *
     *    @param	ds	����Դ
     *    @param	idx_begin	����Դ���������Կ�ʼ
     *    @param	idx_end	����Դ��������������
     *    @return
     */
    virtual bool useDataSource(const chartbase::DataSource *ds, int idx_begin, int idx_end);

    /*! \brief ��д�˷���ʵ�ֹ�ʽ�ļ���
     *
     *    @param	para	��ʽ�����������
     *    @return
     */
    virtual bool calc(const TaParam& para) = 0;

    /*! \brief ��ѯ����
     *
     *    @param	para [out] �������
     *    @return
     */
    virtual bool queryParam(SkTArray<TaParamItem>& para);

    /*! \brief ��ȡĬ�ϵĲ���
     *
     *    @param	para	[out]	�����Ĭ�ϲ���
     *    @return	true	��ѯ�ɹ�
     */
    virtual bool defaultParam(TaParam& para);

    /*! \brief ��ѯ�����ͼ�������
     *
     *    @param	series [out]
     *    @return
     */
    virtual bool suggestOutSeries(SkTArray<TaSeries>& series);

    inline TaParam& param() {
        return para_;
    }

    const SkString& name()const {
        return name_;
    }

protected:
    //! in
    TaParam para_;

    //! ��ʽ��
    SkString name_;

    //! ��ʽ���ݣ�������������
    TaData data_;
};
}
}

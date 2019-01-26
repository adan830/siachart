#pragma once

#include "deps/singleton.h"
#include "chart.h"
#include "dict.h"

namespace sia {
namespace formula {
class IFormula;
class FormulaMan;
}

namespace charty {

struct ChartManPrivate;
class Chart;

/*!
 * \class ChartMan
 *
 * \brief ͼ��ʵ��������
 *
 */
class CHARTY_API ChartMan final: public Singleton<ChartMan> {
    friend class Singleton<ChartMan>;
    friend class Chart;
public:
    ChartMan();
    ~ChartMan();

    /*! \brief ����ID��ѯͼ��Ķ���
     *
     *    @param	chart_id	ͼ��ID
     *    @return	ͼ�����
     */
    Chart* chart(const SkString& chart_id) const;

    /*! \brief �����е�ͼ��ʵ���㲥�¼�
     *
     *    @param	evt	�¼�
     *    @return
     */
    void broadcast(const SkEvent& evt) const;

    /*! \brief Ӧ����������
     *
     *    @param	config_file	���������ļ�
     *    @param	error_msg [out]	������Ϣ
     *    @return	false�� ����ʧ�ܣ�������Ϣ�μ�����error
     */
    bool loadThemeConfig(const SkString& config_file, SkString& error_msg);

    /*! \brief �ֵ����
     *
     *    @param
     *    @return
     */
    Dict* dict();

    /*! \brief ������ʽʵ��
     *
     *    @param	name	������ʽ
     *    @return
     */
    formula::IFormula* createFormula(const SkString& name);

    /*! \brief ���ٹ�ʽʵ��
     *
     *    @param	inst	��ʽʵ��ָ�룬��createFormula����
     *    @return
     */
    void destroyFormula(formula::IFormula* inst);

    /*! \brief ��ʽ������ʵ��
     *
     *    @param
     *    @return
     */
    formula::FormulaMan* formulaMan();

protected:
    /*! \brief ע���µ�ͼ��ʵ��
     *
     *    @param	c	ͼ�����ʵ��
     *    @return
     */
    void regInst(Chart* c);

    /*! \brief ȡ��ע��ͼ��ʵ��
     *
     *    @param	c	ͼ�����ʵ��
     *    @return
     */
    void unregInst(Chart* c);

private:
    ChartManPrivate* d = nullptr;
};
}
}

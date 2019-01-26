#pragma once

#include "tadata.h"
#include "taparam.h"

namespace sia {
namespace formula {

/*!
 * \class TaItem
 *
 * \brief Talib �Ĺ�ʽ��
 *	1��init
 *	2��queryParam
 *	3��calc
 */
class TaItem {
public:
    TaItem(const SkString& name = SkString(""));

    ~TaItem();

    void setName(const SkString& name);

    bool init();

    /*! \brief ��ѯ����
     *
     *    @param	para [out]
     *    @return
     */
    bool queryParam(SkTArray<TaParamItem>& para);

    /*! \brief ��ȡĬ�ϵĲ���
     *
     *    @param	para	[out]	�����Ĭ�ϲ���
     *    @return	true	��ѯ�ɹ�
     */
    bool defaultParam(TaParam& para);

    /*! \brief �����ͼ���������
     *
     *    @param	series [out]
     *    @return
     */
    bool suggestOutSeries(SkTArray<TaSeries>& series);

    bool calc(const TaParam& para);

    void setData(TaData* d);

    bool valid() const;

protected:
    bool initFunc();

    bool initInputParam();

    bool initOutParam();

    bool evalCall();

private:
    //! ��ѡ������� optional input
    TaParam opt_in_;

    //! ����Դ
    TaData* d_ = nullptr;

    //! TA ������
    SkString ta_name_;

    //! ����
    TA_ParamHolder *param_holder_ = nullptr;

    //! TA ������Ϣ
    const TA_FuncInfo *func_info_ = nullptr;
};
}
}

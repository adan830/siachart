#pragma once

#include <unordered_map>
#include "chartapi.h"

namespace sia {
namespace charty {
/*!
 * \class Dict
 *
 * \brief ͼ���ڲ��������������ֺ����ֵ�
 *      �ֵ���ȫ�ֹ���ģ��������ڣ�
 *          1�������������ֽ�����human readable��
 *          2��������
 */
class CHARTY_API Dict final {
    friend struct ChartManPrivate;

private:
    ~Dict();

public:
    /*! \brief ���һ���ֵ�ӳ��
     *
     *    @param    k  english key
     *    @param    v  human readable text
     */
    void add(const SkString& k, const SkString& v);
    void add(const std::map<SkString, SkString>& dict);

    /*! \brief �Ƴ��ֵ���Ŀ
     *
     *    @param	k	�ֵ�key
     *    @return
     */
    void remove(const SkString& k);

    /*! \brief ��ȡ�ɶ����ı�
     *
     *    @param k  key
     *    @return human readable text
     */
    SkString get(const SkString& k)const;

private:
    // key: english name
    // value: human readable text
    std::unordered_map<SkString, SkString> dict_;
};
}
}

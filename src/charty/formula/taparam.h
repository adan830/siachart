#pragma once

#include "formulapi.h"
#include "../base/base.h"

namespace sia {
namespace formula {

/*!
 * \class TaVariable
 *
 * \brief ��ʽ���������ֵ
 *
 */
struct FORMULA_API TaVariable {
    SkString name;
    SkTArray</*TA_Real*/double> vd;

    template<typename T>
    T asT()const {
        if (vd.empty()) {
            return T();
        }
        return (T)(vd.front());
    }

    template<typename T>
    std::vector<T> asList()const {
        std::vector<T> ret;
        for (auto t : vd) {
            ret.push_back(reinterpret_cast<T>(t));
        }
        return ret;
    }

    void set(double val);

    void push(double val);
};

/*!
 * \class TaSeries
 *
 * \brief ��ʽ�������
 *
 */
struct FORMULA_API TaSeries {
    // ������
    SkString series_name;

    //! ������Ҫ������������ʽ����ģ�����ԭ�е����ݾ��ɣ�ֻҪ���ڣ�
    SkTArray<SkString> data_name;

    //! ��������
    chartbase::SeriesType typ;

    //! ���е���ɫ����
    SkColor color = SK_ColorCYAN;
};

/*!
 * \class TaParam
 *
 * \brief ��ʽ����/�������
 *
 */
class FORMULA_API TaParam {
public:
    TaParam() {
    }

    template<typename T>
    T asT(const SkString& name)const {
        for (const TaVariable& t : in_) {
            if (t.name.equals(name)) {
                return t.asT<T>();
            }
        }
        return T();
    }

    template<typename T>
    std::vector<T> asList(const SkString& name)const {
        for (const TaVariable& t : in_) {
            if (t.name.equals(name)) {
                return t.asList<T>();
            }
        }
        return std::vector<T>();
    }

    void setParam(const SkString& name, double val);

    void pushParam(const SkString& name, double val);

    void addOutSeries(const TaSeries& series);

    const SkTArray<TaSeries>& outSeries()const;

    /*! \brief �����б��д��ʽ��
     *
     *    @param	with_name	true:���ϲ�������
     *    @return	���磺(1,2,3) / with_name: (signal:12, fast:26)
     */
    SkString paramBrief(bool with_name)const;

protected:
    void insert(const SkString& name, double val);

private:
    //����ʽ��Ҫ����������������ж��
    SkTArray<TaVariable> in_;

    // ��ʽ������ͼ����ʾ���˲�����ѡ��
    // 1. һ����ʽ�����ж�����͵��������������MACD���������ͣ�1*updown + 2*line���������
    // 2. ��ʽ������ɺ�Ҳ������ȫ��������ݣ����������ͼ�����У�
    SkTArray<TaSeries> out_series_;
};

}
}

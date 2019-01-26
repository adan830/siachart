#pragma once

#include "dict.h"

namespace sia {
namespace charty {

/*!
 * \class SeriesName
 *
 * \brief ������/�������ȵ�����
 *
 */
class SeriesName {
public:
    /*! \brief
     *		����ָ�깫ʽ�������������datanameҪ���û��ֶ���ӵ����������ֿ���
     *		���Ƕ���ָ�깫ʽ���ɵ����ݣ���ǰ׺: fml:$formula_name:$id:$dataname
     *    @param
     *    @return
     */
    static SkString formulaDataname(const SkString& formula_name, int32_t id, const SkString& dataname) {
        SkString ret;
        ret.appendf("fml:%s:%d:%s", formula_name.c_str(), id, dataname.c_str());
        return ret;
    }

    static SkString formulaSeriesName(const SkString& formula_name, const SkString& series_name, int32_t id) {
        SkString ret;
        ret.appendf("fml:%s:%s:%d", formula_name.c_str(), series_name.c_str(), id);
        return ret;
    }

    // �������е�����
    static SkString translateSeriesName(const SkString& name, Dict* dict) {
        SkString real_name = name;
        if (name.startsWith("fml:")) {
            // ����Ҫ��ָ�����е����ָ���ȡ����
            SkTArray<SkString> out;
            SkStrSplit(name.c_str(), ":", &out);
            if (out.count() >= 4) {
                real_name = out[2];
            }
        }

        if (dict) {
            dict->get(real_name);
        }
        return real_name;
    };

    // �������ݵ�����
    static SkString translateDataName(const SkString& name, Dict* dict) {
        SkString real_name = name;
        if (name.startsWith("fml:")) {
            // ����Ҫ��ָ�����е����ָ���ȡ����
            SkTArray<SkString> out;
            SkStrSplit(name.c_str(), ":", &out);
            if (out.count() >= 4) {
                real_name = out[3]; // ��������
            }
        }

        if (dict) {
            dict->get(real_name);
        }
        return real_name;
    };

};

}
}

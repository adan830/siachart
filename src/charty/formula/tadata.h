#pragma once

#include "formulapi.h"
#include "deps/talib/include/ta_libc.h"
#include "../base/datasource.h"

namespace sia {
namespace formula {


enum FORMULA_API TaInputDataIndex {
    kOpen = 0,
    kHigh = 1,
    kLow = 2,
    kClose = 3,
    kVolume = 4,
    kOi = 5,	// open interest
    // kStamp = 6,	// ��ʱ��֧��
    kMaxInputIndex = kOi + 1,
};

enum FORMULA_API TaOutputDataIndex {
    kOut1 = 0,
    kOut2 = 1,
    kOut3 = 2,
    kOutInt = 3,
    kMaxOutputIndex = kOutInt,
};

enum class TaParamType {
    //
    kReal,
    kInt,

    //-------------------------
    // ������
    // open / high / low / close
    kPriceList,

    kIntList,

    // ĳһ��list���� open �� volume
    kRealList,
};

struct TaParamItem {
    //! ������
    SkString name;

    //! ��ʾ��
    SkString display_name;

    //! ��������
    TaParamType typ;

    //! ��ѡ����
    bool opt = true;

    // TA_OptInputFlags         flags;

    // const void *dataSet;
    //! Ĭ��ֵ
    double     default_val;

    //! ��ʾ
    SkString hint;

    //! ����
    SkString help_file;
};

class FORMULA_API TaData {
public:
    template<typename T>
    struct _NamedData {
        //! ������
        SkString name;

        //! ����
        SkTArray<T> data;

        //! ��ʽִ����ɺ���������ݴ��ĸ�key��ʼ��Ч
        // �������Ч������data���Ǵ�0��ʼ�ġ���Ч���ݸ���Ϊ[$count]
        // ���ݺ�TaData::keys�Ķ�Ӧ��ϵ�� TaData::keys[i+key_offset] = _NamedData[i]
        int key_offset = -1;

        //! ��Ч���ݸ���
        int count = 0;

        inline bool valid()const {
            return (-1 < key_offset) && !name.isEmpty() && (count > 0);
        }
    };

    const TA_Real* open()const {
        return open_.begin();
    }

    const TA_Real* high()const {
        return high_.begin();
    }

    const TA_Real* close()const {
        return close_.begin();
    }

    const TA_Real* low()const {
        return low_.begin();
    }

    // open interest �� δƽ����
    const TA_Real* oi()const {
        return oi_.begin();
    }

    const TA_Real* volume() const {
        return volume_.begin();
    }

    /*! \brief �������ݣ�open/high/low/close/oi/volume
     *
     *    @param	idx	��ο�TaInputDataIndexֵ
     *    @return	����
     */
    const TA_Real* indexData(uint16_t idx) const;

    int dataLength()const {
        return (int)open_.count();
    }

    TA_Integer* outInt() {
        return out_int_.data.begin();
    }

    TA_Real* outReal(TaOutputDataIndex idx);

    TA_Real* out1() {
        return out1_.data.begin();
    }

    TA_Real* out2() {
        return out2_.data.begin();
    }

    TA_Real* out3() {
        return out3_.data.begin();
    }

    /*! \brief ���ù�ʽ������ݵ����ֺ���Ч����������
     *	        ����ʽ������ɺ󣬻��������/��Ϣ��
     *          �ο���TaItem::evalCall
     *          �ο���TA_GetOutputParameterInfo/TA_OutputParameterInfo
     *
     *    @param    out_idx = TaOutputDataIndex, ָʾout1/out2/out3/int
     *    @param    name    ��������
     *    @param    valid_idx_begin ��Ч��key����λ��
     *    @param    valid_count ��Ч���ݵĸ���
     *    @return
     */
    void setOut(TaOutputDataIndex out_idx, const SkString& name, int valid_idx_begin, int valid_count);

    /*! \brief �������Ĳ�������֮ǰ�����ô˺�����ʼ���ڴ�ռ䣬����¼key��ʱ�䣩ֵ
     *	        key ����Ҫ��open/high... ������Ҫ��ȫ��һһ��Ӧ����
     *
     *    @param    total   �ܵ����ݴ�С
     *              keys    ʱ����������
     *    @return
     */
    void beginInsert(int total, const SkTArray<chartbase::data_key_t>& keys);
    void setItem(TaInputDataIndex idx, int i, TA_Real val);
    void endInsert();

    /*! \brief ������Ч���������
     *
     *    @param    std::function<bool(const SkString& data_name, const std::map<data_key_t, data_val_t>& data)
     *    @return
     */
    void forValidOutData(std::function<bool(
                             const SkString&, const std::map<chartbase::data_key_t, chartbase::data_val_t>&)> )const;

protected:
    SkTArray<TA_Real>& indexData(TaInputDataIndex idx);

private:
    // input
    SkTArray<TA_Real> open_;
    SkTArray<TA_Real> high_;
    SkTArray<TA_Real> low_;
    SkTArray<TA_Real> close_;
    SkTArray<TA_Real> volume_;
    SkTArray<TA_Real> oi_;

    // output
    // Ŀǰ������֧��3��������飬����ֻ��һ��int�������
    _NamedData<TA_Real> out1_;
    _NamedData<TA_Real> out2_;
    _NamedData<TA_Real> out3_;
    _NamedData<TA_Integer> out_int_;

    //! ʱ��������
    SkTArray<chartbase::data_key_t> keys_;
};

}
}

#pragma once

#include "baseapi.h"
#include "bitflag.h"
#include "base.h"

/*! \brief ����Դ�е����ݰ���ʱ��ֵ˳������������ÿһ��ʱ��ֵ�Ϲ����˶��value��
 *	    ��ͼ��ʹ�õ�ʱ��ͨ���ǰ���ʱ��ȡ�������������ֵ�����ʱ����Ҫ������
 */
namespace sia {
namespace chartbase {

class DataSource;
/*!
 * \class ColumnData
 *
 * \brief ÿһ��ʱ��ֵ����һ������
 *
 */
class BASE_API ColumnData { /* : public std::enable_shared_from_this<ColumnData>*/
    friend class DataSource;
public:
    ColumnData(data_key_t k)
        :k_(k) {
    }

    inline bool operator()(const data_key_t& k, const ColumnData& t)const {
        return k == t.key();
    }

    void set(const SkString& data_name, data_val_t v);

    data_val_t get(const SkString& data_name)const;

    inline data_key_t key()const {
        return k_;
    }

    /*! \brief �Ƴ�������ָ������
     *
     *    @param	data_name	������
     *    @return
     */
    void remove(const SkString& data_name);

    std::vector<SkString> datanames() const;

    inline bool empty()const {
        return d_.empty();
    }

private:
    // ͨ����ʱ��ֵ
    data_key_t k_;

    // ��������
    // key: data name
    std::unordered_map<SkString, data_val_t> d_;
};

/*!
 * \class IDataSource
 *
 * \brief ����Դ�ӿ�
 *
 */
class BASE_API DataSource {
    using Elem = my_sp<ColumnData>;
    using Containter = SkTArray<Elem>;

    enum Flags {
        kDataReady = 0x1,
    };

public:
    /*! \brief ������� / �༭����
     *		���key����������ӣ��Ѵ��ڣ����޸Ķ�Ӧ��value
     *
     *    @param	data_name	��������ȷ��Ψһ��
     *    @param	datas	����
     *    @return
     */
    void addData(const SkString&data_name, const std::map<data_key_t, data_val_t>& datas);

    /*! \brief �Ƴ�����
     *		�����Ӧ�����������ѿգ���column����ɾ��
     *
     *    @param	data_name	������
     *    @return
     */
    void removeData(const SkString&data_name);
    void removeData(const SkString&data_name, const SkTArray<data_key_t>& keys);

    /*! \brief ����ָ������������: [idx_begin, idx_end]
     *
     *    @param idx_begin >= 0 ���俪ʼ
     *    @param idx_end >= -1 �������: -1: ���������һ��
     *    @param cb ���ݻص�
     *    @return
     */
    bool forIndexData(int32_t idx_begin, int32_t idx_end, std::function<bool(int32_t, const DataSource::Elem)> cb) const;

    bool keys(int32_t idx_begin, int32_t idx_end, SkTArray<data_key_t>& out) const;
    data_key_t key(int32_t idx)const;

    /*! \brief ����ĳ��key��index
     *	    ����������begin��end��������������С���ҷ�Χ
     *      ���������index��Χ�����key�ڴ����index��Χ֮�⣬���޷����ҵ���
     *
     *    @param
     *    @return   ���û���ҵ�������-1������ >= 0
     */
    int keyIndex(data_key_t k, int32_t idx_begin = 0, int32_t idx_end = -1)const;

    inline bool ready()const {
        return flags_.test(Flags::kDataReady);
    }

    /*! \brief ��ѯdataname�������� idx��������ֵ
     *
     *    @param    idx [-1, x] ���Ϊ-1������ȡ���һ��ֵ�����x����total()��������Чֵ
     *    @return   ����Ϊ��Чֵ����������Ҫ�ж�
     */
    data_val_t value(int32_t idx, const SkString& dataname)const;

    void beginTransaction();
    void endTransaction();

    inline int32_t total()const {
        return int32_t(all_.count());
    }

    /*! \brief ���±������ڵ�names��Ӧ��data���������Сֵ
     *
     *    @param    names   ��������
     *    @param    idx_begin��idx_end   ������������
     *    @param    min,max [out]   ������������������Сֵ
     *    @return
     */
    bool getIndexMinMaxVal(const std::set<SkString>& names,
                           int32_t idx_begin, int32_t idx_end, data_val_t& min, data_val_t& max) const;

    const Elem get(int idx)const;

protected:
    Elem _get(data_key_t k, bool create);

    /*! \brief ɾ��ColumnData::empty() == true��Ԫ��
     *
     *    @param
     *    @return
     */
    void clearEmptyData();

    // ����
    void sortData();

    /*! \brief �±��������
     *
     *    @param idx_begin,idx_end [in|out]
     *    @return   false ���޷������Ĵ���ֵ
     */
    bool validateIndex(int32_t &idx_begin, int32_t &idx_end)const;

private:
    // ��key˳������
    Containter all_;
    BitFlags flags_;
};

}
}

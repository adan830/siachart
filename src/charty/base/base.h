#pragma once

#include "baseapi.h"

namespace sia {
namespace chartbase {

using data_key_t = int64_t;

// #define USE_INT64_DATA_VAL

#ifdef USE_INT64_DATA_VAL
using data_val_t = int64_t;
#else
using data_val_t = double;
#endif

#ifdef DONT_USE_THIS
// ��Ҫ����������롣�˴���ֻ��Ϊ���ñ༭����ɫ���ÿ��㡣�ٺ�
enum ConstDefs : int64_t {
    //! ��Чchart_val_t
    kInvalidDataValue = std::numeric_limits<data_val_t>::max(),
    //! ��Ч chart_key_t
    kInvalidDataKey = std::numeric_limits<data_val_t>::max(),
    //! ��Ч����
    kInvalidCoord,
};
#endif

#define MAX_AS_INVALID(T, DEF) \
    static const T DEF = std::numeric_limits<T>::max()

//! ��Чchart_val_t
MAX_AS_INVALID(data_val_t, kInvalidDataValue);

//! ��Ч chart_key_t
MAX_AS_INVALID(data_key_t, kInvalidDataKey);

//! ��Ч����
MAX_AS_INVALID(SkScalar, kInvalidCoord);
#undef MAX_AS_INVALID

inline bool validCoord(SkScalar x) {
    return kInvalidCoord != x;
}

inline bool validCoord(const SkPoint& p) {
    return validCoord(p.x()) && validCoord(p.y());
}

inline bool validKey(data_key_t k) {
    return kInvalidDataKey != k;
}

inline bool validVal(data_val_t v) {
    return kInvalidDataValue != v;
}

/*!
 * \enum sia::chartbase::SeriesType
 *
 *   ͼ����������
 */
enum class BASE_API SeriesType {
    kBlank = 0,
    kLine = 1,
    kBar = 2,
    kUpdownBar = 3,
    kCandle = 4,
};

}
}
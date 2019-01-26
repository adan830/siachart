#pragma once

namespace sia {
namespace charty {
class ValidArea {
public:
    inline void reset(int n) {
        areas_.reset(n);
    }

    inline const SkRect& get(int idx)const {
        if (areas_.count() <= idx) {
            static const SkRect RC_EMPTY = SkRect::MakeEmpty();
            return RC_EMPTY;
        }
        return areas_[idx];
    }

    inline void setOne(const SkRect& rc) {
        big_piece_ = rc;
    }

    inline void set(int idx, const SkRect& rc) {
        if (areas_.count() <= idx) {
            return;
        }
        areas_[idx] = rc;
    }

    inline bool contains(int idx, const SkPoint& pt) const {
        if (big_piece_.contains(pt.x(), pt.y())) {
            return true;
        }
        return get(idx).contains(pt.x(), pt.y());
    }

    inline int count()const {
        return areas_.count();
    }

protected:
    // ���õ�����Ŀ�index�ĸ�Ӧ��
    SkRect big_piece_;

    // ��index��sensor��
    SkTArray<SkRect> areas_;
};
}
}

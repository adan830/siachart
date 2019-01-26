#pragma once

#include <vector>
#include <unordered_map>
#include "SkString.h"
#include "SkRect.h"
#include "layout.h"

namespace sia {
namespace graphy {

class GRAPHY_API TemplateLayout : public Layout {

    class LayoutItem :
        public std::enable_shared_from_this<LayoutItem> {
    public:
        // bool parse(const char item, rapidjson::Value &v);
        void resetGeometry();

        void dumpGeom(char c)const;

        void dumpMeasure(char c) const;

        // У��ͬһ��/���ϵ�Ԫ�ؿռ�
        // ͬһ���ϣ���С�߶�Ӧ��ȡ�����Ǹ����������δ���õĻ�����
        // ͬһ���ϣ���С���Ӧ��ȡ�����Ǹ����������δ���õĻ�����
        void adjustMaxMinWidth(SkScalar min, SkScalar max);
        void adjustMaxMinHeight(SkScalar min, SkScalar max);

        SkScalar minWidth(SkScalar all) {
            if (width.isFixed()) {
                return width.val();
            }

            if (min_width > ZERO_SCALAR) {
                return min_width;
            }

            if (width.isPercent()) {
                return width.calc(all, max_width, min_width);
            }
            return ZERO_SCALAR;
        }

        SkScalar minHeight(SkScalar all) {
            if (height.isFixed()) {
                return height.val();
            }

            if (min_height > ZERO_SCALAR) {
                return min_width;
            }

            if (height.isPercent()) {
                return height.calc(all, max_height, min_height);
            }
            return ZERO_SCALAR;
        }

    public:
        SkString widget;

        // ���Ͻ�λ��
        SkPoint topleft;
        // ��С
        SkSize size;

        // ��ļ���
        SkScalar max_width = MAX_SCALAR;
        SkScalar min_width = ZERO_SCALAR;
        Measure width;

        // �ߵļ���
        SkScalar max_height = MAX_SCALAR;
        SkScalar min_height = ZERO_SCALAR;
        Measure height;
    };
    typedef my_sp<LayoutItem> LayoutItemPtr;

    // 2άģ�����
    class TmplMatrix {
        static const char kNullElement = '\0';
    public:
        bool init(const SkString& d2);

        char element(int r, int c)const;

        int row()const;
        int col()const;

        void forEach(std::function<bool(int, int, char, LayoutItemPtr)> onItem);
        void forEachRow(std::function<bool(int, std::vector<LayoutItemPtr>)> onItem);
        void forEachCol(std::function<bool(int, std::vector<LayoutItemPtr>)> onItem);

        bool addLayoutItem(char item, LayoutItemPtr l);

        bool calc(const SkRect& rc);

    protected:
        LayoutItemPtr layoutItem(char c)const;

    private:
        // ����ģ��
        SkTArray<SkString> tmpl_;

        // ��Ҫ�����ֵ�Ԫ��
        std::unordered_map<char, LayoutItemPtr> items_;
    };

public:
    virtual ~TemplateLayout();
    virtual bool parse(const SkString& layout) override;

protected:
    virtual void doLayout() override;

private:
    TmplMatrix matrix_;
};
}
}

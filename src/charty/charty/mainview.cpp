#include "mainview.h"
#include <cassert>
#include "series.h"
#include "objectitem.h"

namespace sia {
namespace charty {

MainView::MainView(Panel* parent)
    :PanelItem(SkString("mainview"), parent) {
    setZIndex(VisualItemZValue::kBackground);
    setLayout(&stacklay_);
}

MainView::~MainView() {
    // ɾ��series
}

void MainView::setAsSubview(bool on) {
    // subview �ڴ�����ʱ��ָ�������Ҳ��ܱ�����Ϊ��View
    // ��ͼû�б���ɫ
    flags_.set(Flags::kSubView, on);
}

my_sp<Series> MainView::addSeries(const SkString& name, SeriesType typ, const SkTArray<SkString>& datas) {
    if (series(name)) {
        assert(false);
        return nullptr;
    }

    if (auto series = Series::create(name, typ)) {
        if (series->setDataNames(datas)) {
            series->setChartContext(chartCtx());
            series_.insert({ name, series });
            addChild(series.get());
            return series;
        }
    }
    return nullptr;
}

bool MainView::removeSeries(const SkString& name) {
    if (auto ss = series(name)) {
        // ֪ͨ��ͼ������ɾ������
        SkEvent evt(ChartEventType::kSeriesBeforeDelete);
        evt.setString("name", name.c_str());
        chartCtx()->emitEvent(evt);

        // �������Щobject�����������series�ģ�ҲҪһ��ɾ��
        bool has_related = false;
        do {
            has_related = false;
            for (auto i : objects_) {
                if (i.second->isAttachedTo(ss.get())) {
                    removeObject(i.first);
                    has_related = true;
                    break;
                }
            }
        } while (has_related);

        // ss->setParent(nullptr);
        removeChild(ss.get());
        series_.erase(name);

        // ���¼���ͼ�����ʾ
        recalc(0);

        // ������ɾ��!
        // ������ע�⣺����Ϣִ�к󣬿��ܵ���thisָ��ʵ����ɾ����
        // ���������Դ���Ϣ֮�󲻿�����ִ���κδ��룡
        SkEvent evt_del(ChartEventType::kSeriesDeleted);
        evt_del.setString("name", name.c_str());
        chartCtx()->emitEvent(evt_del);

        return true;
    }
    return false;
}

bool MainView::addObject(const SkString& name, const ObjectParam&para, my_sp<IVisualObject> obj) {
    auto item = std::make_shared<ObjectItem>(name, this);
    objects_[name] = item;

    item->setChartContext(chartCtx());
    item->setParam(para, obj);
    addChild(item.get());

    return true;
}

bool MainView::removeObject(const SkString& name) {
    auto i = objects_.find(name);
    if (i == objects_.end()) {
        return false;
    }

    auto item = i->second;
    removeChild(item.get());
    objects_.erase(i);

    return true;
}

ViewItem* MainView::viewItem(const SkString& name) const {
    // ��Ϊseriesͬʱʹ����seriesname �� widget_name,���Բ������������ַ�ʽ������
    // ���� Candle ��name() == "candle_series"�� ����series_name ����������������Ȥ��name
    // ViewItem* ret = nullptr;
    //     forEachChild([&](Widget* w)->bool {
//         if (w->name().equals(name)) {
//             ret = dynamic_cast<ViewItem*>(w);
//             return false;
//         }
//         return true;
//     });

    // ����series
    for (auto i : series_) {
        if (name.equals(i.second->seriesName())) {
            return i.second.get();
        }
    }

    // ����object
    for (auto i : objects_) {
        if (name.equals(i.second->name())) {
            return i.second.get();
        }
    }
    return nullptr;
}

my_sp<Series> MainView::series(const SkString& name)const {
    auto i = series_.find(name);
    if (i != series_.end()) {
        return i->second;
    }
    return nullptr;
}

std::vector<my_sp<Series>> MainView::serieses() const {
    std::vector<my_sp<Series>> ret;
    for (auto i : series_) {
        ret.push_back(i.second);
    }
    return ret;
}

void MainView::recalc(Bits flags) {
    calcXPoss();
    calcYRange();
}

void MainView::setValueSlice(int count) {
    vslice_.setSlice(count);
}

void MainView::setSeriesColor(const std::map<SkString, SkColor>& colors) {
    for (auto i : colors) {
        if (auto s = series(i.first)) {
            s->setColor(i.second);
        }
    }
}

void MainView::setSeriesStyle(const std::map<SkString, SeriesStyle>& styles) {
    for (auto i : styles) {
        if (auto s = series(i.first)) {
            s->setStyle(i.second);
        }
    }
}

void MainView::setSeriesTooltipCallback(std::map<SkString, SeriesTooltipCallback> callbacks) {
    for (auto i : callbacks) {
        if (auto s = series(i.first)) {
            s->setTooltipCallback(i.second);
        }
    }
}

void MainView::setMiddleVal(chart_val_t val) {
    vslice_.setMiddleVal(val);
    flags_.set(Flags::kMiddleVal, chartbase::validVal(val));
}

void MainView::showSeriesMinMaxTip(const SkString&series_name, bool show) {
    if (auto s = series(series_name)) {
        s->showMinMaxTip(show);
    }
}

int MainView::dataIndexOfPos(SkScalar x) const {
    int ret = visualIndexOfPos(x);
    if (ret >= 0) {
        if (auto ctx = chartCtx()) {
            ret += ctx->indexBegin();
            if (ret < ctx->totalIndexCount()) {
                return ret;
            }
        }
    }
    return -1;
}

int MainView::visualIndexOfPos(SkScalar x) const {
    if (xpos_.empty()) {
        return -1;
    }

    if (x < 0) {
        return 0;
    }

    int ret = (int)(x * xpos_.count() / contentWidth());
    if (ret >= xpos_.count()) {
        return xpos_.count() - 1;
    }
    return ret;
}

int MainView::visualIndexOfKey(chart_key_t key) const {
    auto ctx = chartCtx();
    auto ds = datasource();

    chart_key_t k1 = ds->key(ctx->indexBegin());
    if (key < k1) {
        return -1;
    }

    chart_key_t k2 = ds->key(ctx->indexEnd());
    if (key > k2) {
        return -1;
    }

    // ��ͨ������Դ�ҵ���index
    return ds->keyIndex(key, ctx->indexBegin(), ctx->indexEnd()) - ctx->indexBegin();
}

double MainView::valueOfYPos(SkScalar y) const {
    return vslice_.valueOfYPos(y, contentHeight());
//     const double v_per_dpi = util::divide((double)(maxY() - minY()), (double)contentHeight());
//     return maxY() - y * v_per_dpi;
}

SkScalar MainView::yPosOfValue(chart_val_t v) const {
    return vslice_.yPosOfValue(v, contentHeight());
//     const double h = contentHeight();
//     // ÿ1ֵռ�õĸ߶�
//     const double dy = util::divide(h, (double)(maxY() - minY()));
//     return (SkScalar)(h - (v - minY()) * dy);
}

SkPoint MainView::posOfDataname(int idx, const SkString& dataname) const {
    chart_val_t v = datasource()->value(idx, dataname);
    if (chartbase::kInvalidDataValue == v) {
        return SkPoint::Make(0, 0);
    }

    idx -= (chartCtx()->indexBegin());
    if (idx < 0 || idx >= xpos_.count()) {
        return SkPoint::Make(0, 0);
    }
    return SkPoint::Make(xpos_[idx], yPosOfValue(v));
}

SkScalar MainView::xPosOfVisualIndex(int visual_idx) const {
    if (visual_idx < 0 || visual_idx >= xpos_.count()) {
        return SkIntToScalar(-1);
    }
    return xpos_[visual_idx];
}

SkScalar MainView::xPosOfKey(chart_key_t key) const {
    ChartContext* ctx = chartCtx();
    auto idx_begin = ctx->indexBegin();
    auto idx = datasource()->keyIndex(key, idx_begin, ctx->indexEnd()) - idx_begin;
    return xPosOfVisualIndex(idx);
}

chart_key_t MainView::datetimeOfIndex(int idx) const {
    return datasource()->key(idx);
}

bool MainView::viewEmpty() const {
    return series_.empty() && objects_.empty();
}

SkScalar MainView::indexWidth() const {
    if (auto ctx = chartCtx()) {
        auto count = ctx->curIndexCount();
        if (count > 0) {
            // һ��index�Ŀ��
            return contentWidth() / count;
        }
    }
    return SkIntToScalar(0);
}

void MainView::onDraw(SkCanvas* canvas) {
    // ������
    if (!flags_.test(Flags::kSubView)) {
        // ��ͼ��û�б���ɫ��
        const auto rc = rect();
        canvas->drawRect(rect(), *bk_);
        grid_.draw(canvas, this, width(), contentRect().height(), vslice_.valPos());
    }
    VisualItem::onDraw(canvas);
}

bool MainView::onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
    // ��Ϊ��������ĺ��ӽڵ㣬������ȫ�໥���ǵġ�
    // ���������Ĵ������̣�ֻ�����ϲ��һ�������ܹ��õ������Ϣ
    // ��������ֱ�ӽӹ���Ϣ��Ȼ�����˴��ݵ����ӽڵ���
    bool handled = false;
    // object �����ϲ㣬�ȴ���
    for (auto i : objects_) {
        handled = i.second->onMouse(x, y, state, modifiers);
        if (handled) {
            break;
        }
    }

    if (!handled) {
        for (auto i : series_) {
            handled = i.second->onMouse(x, y, state, modifiers);
            if (handled) {
                break;
            }
        }
    }
    return handled;
}

void MainView::onSizeChange() {
    calcXPoss();
    calcYRange();
    vslice_.calc(precision(), contentHeight());

    VisualItem::onSizeChange();
}

void MainView::preparePaints() {
    tip_.preparePaints();

    bk_ = declarePaint(SkString("background"));
    grid_.setPaints(declarePaint(SkString("grid")));
}

void MainView::calcXPoss() {
    xpos_.reset();

    if (auto ds = datasource()) {
        auto count = chartCtx()->curIndexCount();
        if (count > 0) {
            // һ��index�Ŀ��
            const double w_one = indexWidth()/*contentWidth() / count*/;
            // ����ȡ�����ĵ�
            double x = w_one / 2 + contentX();
            for (int i = 0; i < count; ++i) {
                xpos_.push_back((SkScalar)x);
                x += w_one;
            }
        }
    }
}

void MainView::calcYRange() {
    if (!ready()) {
        return;
    }

    if (series_.empty()) {
        // �㵵λ��Ϣ
        vslice_.setMaxMin(chartbase::kInvalidDataValue, chartbase::kInvalidDataValue);
        vslice_.calc(precision(), contentHeight());
    } else {
        // ��ȡ��ǰ��ʾ������������Сֵ
        auto ctx = chartCtx();
        chart_val_t min_y, max_y;
        if (datasource()->getIndexMinMaxVal(datanames(), ctx->indexBegin(), ctx->indexEnd(), min_y, max_y)) {
            // �㵵λ��Ϣ
            vslice_.setMaxMin(min_y, max_y);
            vslice_.calc(precision(), contentHeight());
        }
    }

}

std::set<SkString> MainView::datanames() const {
    std::set<SkString> ret;
    for (auto i : series_) {
        for (auto& name : i.second->datanames()) {
            ret.insert(name);
        }
    }
    return ret;
}

}

}


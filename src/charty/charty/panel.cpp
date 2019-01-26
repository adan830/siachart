#include "panel.h"
#include <cassert>

#include "defines.h"
#include "title.h"
#include "valueaxis.h"
#include "ratioaxis.h"
#include "mainview.h"
#include "calendaraxis.h"
#include "series.h"
#include "../formula/talib.h"
#include "../formula/tadata.h"
#include "../formula/iformula.h"
#include "chartman.h"
#include "seriesname.h"
#include "viewitem.h"
#include "private.h"

namespace sia {

namespace charty {

Panel::Panel(graphy::Widget* parent)
    :VisualItem(SkString("panel"), parent) {
    setZIndex(VisualItemZValue::kPanel);

    flags_.set(PanelItemType::kPanelAll);

    initItems();
}

Panel::~Panel() {
}

void Panel::recalc(Bits flags) {
    for (auto w : children()) {
        if (auto *v = dynamic_cast<VisualItem*>(w)) {
            v->recalc(flags);
        }
    }

    if (flags & ChartPrivate::kDataDirty) {
        refreshFormulas();
    }
}

void Panel::views(SkTArray<my_sp<MainView>>& out) const {
    out.push_back(mainview_);
    for (auto p : subviews_) {
        out.push_back(p);
    }
}

std::vector<my_sp<Series>> Panel::serieses() const {
    std::vector<my_sp<Series>> ret;
    auto viewSeries = [](std::vector<my_sp<Series>>& v, my_sp<MainView> view) {
        for (auto item : view->serieses()) {
            v.push_back(item);
        }
    };
    viewSeries(ret, mainview_);

    for (auto v : subviews_) {
        viewSeries(ret, v);
    }
    return ret;
}

my_sp<Series> Panel::series(const SkString& series_name, my_sp<MainView>& view) const {
    my_sp<Series> ret = nullptr;
    auto viewSeries = [](my_sp<MainView> view, const SkString& sname)->my_sp<Series> {
        for (auto item : view->serieses()) {
            if (sname.equals(item->seriesName())) {
                return item;
            }
        }
        return nullptr;
    };
    if (ret = viewSeries(mainview_, series_name)) {
        view = mainview_;
        return ret;
    }

    for (auto v : subviews_) {
        if (ret = viewSeries(v, series_name)) {
            view = v;
            return ret;
        }
    }
    return nullptr;
}

bool Panel::createFormulaSeries(const SkString& formula_name, const int32_t id, Panel::_Formula& fml,
                                const SkTArray<formula::TaSeries>* p_series, const formula::TaParam& user_param) {

    // ָ��brief��
    fml.brief_name = SkStringPrintf("%s%s", formula_name.c_str(), user_param.paramBrief(false).c_str());

    // ����һ�����еĴ�����
    std::map<SkString, SkColor> series_colors;
    auto createMySeries = [&](const SkString& series_name, const formula::TaSeries& ts) {
        fml.series_names.push_back(series_name);
        SkTArray<SkString> data_names;
        for (auto& ss : ts.data_name) {
            data_names.push_back(SeriesName::formulaDataname(formula_name, id, ss));
        }

        if (auto ss = fml.view->addSeries(series_name, ts.typ, data_names)) {
            // Ҫ�������ɾ����ʱ��ͬʱɾ���������������
            ss->setRemoveDataOnDelete(true);

            // ��title����ʾ
            PanelTitleItem title;
            title.item = series_name;
            title.typ = PanelTitleItemType::kSeriesName;
            title_.addTitleItem(title);
        }
        series_colors[series_name] = ts.color;
    };

    // ������һ��������е�ʱ�򣬾Ͳ������PanelTitleItemType::kConstant���͵ı����ˡ�
    // ���ǰ�ָ���� item($para)������������
    bool only_1_out = (p_series->count() == 1);
    bool use_user_config = !(user_param.outSeries().empty());
    if (only_1_out) {
        if (!use_user_config) { // �û�δ�ṩ����
            SkString series_name = fml.brief_name;
            createMySeries(series_name, p_series->back());
            return true;
        }
    } else {
        // ������ǰ�����һ��ָ���brief title
        if (!use_user_config) {
            // ��title����ʾ
            PanelTitleItem title;
            title.item = fml.brief_name;
            title.typ = PanelTitleItemType::kConstant;
            title_.addTitleItem(title);
        }
    }

    // �����������
    for (const formula::TaSeries& ts : *p_series) {
        SkString series_name = SeriesName::formulaSeriesName(formula_name, ts.series_name, id);
        createMySeries(series_name, ts);
    }
    fml.view->setSeriesColor(series_colors);

    return true;
}

int32_t Panel::findFormulaBySeriesName(const SkString& series_name) const {
    for (auto i : formulas_) {
        if (i.second.hasSeries(series_name)) {
            return i.first;
        }
    }
    return -1;
}

int32_t Panel::showFormula(const SkString& formula_name, const formula::TaParam& param) {
    using namespace sia::formula;

    //! ������ӵ�ͼ���е�ÿ����ʽ��������һ��Ψһ��id
    static int32_t _id = 1;
    if (auto ctx = chartCtx()) {
        // �ȴ�����ʽʵ��
        auto indi = ChartMan::instance()->createFormula(formula_name);
        // ��ʽ�����������
        // TODO��fix �������⣿���ڹ�ʽ���������Ǽ������е����ݣ�
        if (indi
                && indi->useDataSource(datasource(), 0, ctx->totalIndexCount() - 1)	// ��������Դ������
                && indi->calc(param)) {	// ����

            // �����ȿ��£��Ƿ����ṩ����������á�
            auto& user_series = param.outSeries();
            auto *p_series = &user_series;
            bool use_user_config = true;
            SkTArray<formula::TaSeries> suggest_series;
            if (user_series.empty()) {
                // ����û�û����ȷ�趨���еĻ�����ô�Ͱ��չ�ʽ������������趨����
                if (indi->suggestOutSeries(suggest_series)) {
                    p_series = &suggest_series;
                    use_user_config = true;
                }
            }

            // ���û��������ã���ô�����ʽҲû��
            if (!p_series || p_series->empty()) {
                ChartMan::instance()->destroyFormula(indi);
                return -1;
            }

            // ÿ����ʽһ��Ψһid
            _id++;

            // ��¼��ʽ�����������
            _Formula fml;
            fml.view = mainview_;
            fml.fml = indi;
            fml.id = _id;
            fml.formula_name = formula_name;
            fml.param = param;

            // ��������ӵ�����Դ���棡
            const formula::TaData& dat = indi->data();
            dat.forValidOutData([&](const SkString& data_name, const std::map<chartbase::data_key_t, chartbase::data_val_t>& d)->bool {
                SkString dname = SeriesName::formulaDataname(formula_name, _id, data_name);
                fml.data_names.push_back(dname);
                ctx->addData(dname, d);
                return true;
            });

            // �����������
            if (createFormulaSeries(formula_name, _id, fml,p_series, param)) {
                // ��¼�������е�ָ��
                formulas_.insert({ _id, fml });

                return _id;
            }
        }

        if (indi) {
            // ����
            ChartMan::instance()->destroyFormula(indi);
        }
    }
    return -1;
}

void Panel::removeFormula(int32_t id) {
    auto i = formulas_.find(id);
    if (i == formulas_.end()) {
        return;
    }

    // ɾ������
    const _Formula& fml = i->second;
    // ��Title���Ƴ�Item
    title_.removeTitleItem(fml.brief_name);

    // ɾ�����С�
    // �ڴ�����ʱ�������ã�Series::setRemoveDataOnDelete(true)
    for (const SkString& s : fml.series_names) {
        fml.view->removeSeries(s);
    }
    formulas_.erase(i);
}

void Panel::showItem(int item, bool show) {
    if (item & PanelItemType::kPanelCalendar) {
        showPanelItem<CalendarAxis>(calendar_, PanelItemType::kPanelCalendar, show);
    }

    if (item & PanelItemType::kPanelTitle) {
        title_.setBlank(!show);
        flags_.set(PanelItemType::kPanelTitle, show);
        // showPanelItem<Title>(title_, PanelItemType::kPanelTitle, show);
    }

    if (item & PanelItemType::kPanelValueAxis) {
        showPanelItem<ValueAxis>(value_, PanelItemType::kPanelValueAxis, show);
    }

    if (item & PanelItemType::kPanelRatioAxis) {
        showPanelItem<RatioAxis>(ratio_, PanelItemType::kPanelRatioAxis, show);
    }

    // ���¼��㲼��
    relayout();
}

void Panel::seriesRemoved(const SkString& series_name) {
    title_.removeTitleItem(series_name);

    // ��鸱ͼ�������ͼ������Ϊ�յĻ�����Ҫ�Ѹ�ͼɾ��
    auto i = std::remove_if(subviews_.begin(), subviews_.end(), [](my_sp<MainView> view)->bool {
        return view->viewEmpty();
    });
    // �Ƴ������ͼ
    subviews_.pop_back_n(int(subviews_.end() - i));
}

bool Panel::mergePanel(Panel* sub) {
    if (!sub) {
        return false;
    }

    // �ϲ�title��ʾ
    if (Title* sub_title = sub->title()) {
        for (auto& itm : sub_title->titleItems()) {
            title_.addTitleItem(itm);
        }
    }

    // �ϲ�ָ��
    for (auto i : sub->formulas()) {
        formulas_.insert(i);
    }

    SkTArray<my_sp<MainView>> sub_views;
    sub->views(sub_views);
    // �ϲ�����ͼ
    for (auto p : sub_views) {
        p->setAsSubview(true);
        p->setParent(this);
        subviews_.push_back(p);
    }

    return true;
}

void Panel::removeViewitem(ViewItem* item) {
    if (MainView* view = item->mainView()) {
        switch (item->viewType()) {
        case ViewItemType::kViewObject: {
            view->removeObject(item->name());

            break;
        }

        case ViewItemType::kViewSeries: {
            if (auto p = dynamic_cast<Series*>(item)) {
                SkString series_name = p->seriesName();
                view->removeSeries(series_name);

                // �����ָ����������У���ôҪɾ�������ʽ
                int32_t id = findFormulaBySeriesName(series_name);
                if (-1 != id) {
                    removeFormula(id);
                }
            }
        }
        break;
        }
    }
}

void Panel::initItems() {
    showItem(PanelItemType::kPanelCalendar, flags_.test(PanelItemType::kPanelCalendar));
    // showItem(PanelItemType::kPanelTitle, flags_.test(PanelItemType::kPanelTitle));
    showItem(PanelItemType::kPanelRatioAxis, flags_.test(PanelItemType::kPanelRatioAxis));
    showItem(PanelItemType::kPanelValueAxis, flags_.test(PanelItemType::kPanelValueAxis));

    // title ������ɾ��
    initPanelItem(&title_);

    // ����ͼ
    if (!mainview_) {
        mainview_ = std::make_shared<MainView>();
        initPanelItem(mainview_.get());
    }

    layout_.setPanel(this);
    setLayout(&layout_);
}

void Panel::initPanelItem(VisualItem* w) {
    this->addChild(w);
    w->setChartContext(chartCtx());
}

bool Panel::onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
    if (subviews_.empty()) {
        return false;
    }

    // ԭʼ����
    SkPoint pt = mainview_->windowToClient(SkPoint::Make(SkIntToScalar(x), SkIntToScalar(y)));
    // ����Ƕ����ͼ���������ģ���Ϊ����໥���ǣ������Ϣֻ������������ĸ���ͼ�д���
    // ��������Ҫ�������Ϣ������ȥ
    if (mainview_->onMouse(pt.x(), pt.y(), state, modifiers)) {  // ������ͼ����
        return true;
    }

    // Ȼ������ͼ
    for (auto p : subviews_) {
        if (p->onMouse(pt.x(), pt.y(), state, modifiers)) {
            return true;
        }
    }
    return false;
}

void Panel::refreshFormulas() {
    return;

    std::unordered_map<int32_t, _Formula> tmp = formulas_;
    for (auto i : tmp) {
        const _Formula& fml = i.second;

        // ɾ��ָ��
        removeFormula(i.first);

        // �ؽ�ָ��
        showFormula(fml.formula_name, fml.param);
    }
}

}
}


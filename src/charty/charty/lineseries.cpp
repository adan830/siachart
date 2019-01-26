#include "lineseries.h"
#include "SkPath.h"
#include "graphy/scene.h"
#include "mainview.h"

namespace sia {

namespace charty {

LineSeries::LineSeries(const SkString& series_name)
    :Series(SkString("line_series"), series_name, SeriesType::kLine) {
    setZIndex(VisualItemZValue::kLineSeries);
}

void LineSeries::onDraw(SkCanvas* canvas) {
    if (!mainView() || !ready() || !calcPoints()) {
        return;
    }

    pat_.setColor(color());
    SkPath path;
    for (auto& dp : datapos_) {
        const auto& pts = dp.points;
        if (pts.empty()) {
            continue;
        }

        // ���� Ҫ����Ч��λ�ö�����¼����
        for (int i = 0; i < pts.count(); ++i) {
            // �ҵ���Ч�ĵ�
            if (!chartbase::validCoord(pts[i])) {
                continue;
            }

            // ��¼��Ӧ��
            // ��ptΪ���ĵ㣬��x�������ҵȷ���չ��indexWidth��С��
            // �� y ���ϵȷ���չ�� 2 ������
            setSensorArea(i, pts[i], indexWidth(), 4);

            if (i > 0 && chartbase::validCoord(pts[i - 1])) {
                path.lineTo(pts[i]);
            } else {
                path.moveTo(pts[i]);
            }
        }
    }
    canvas->drawPath(path, pat_);

    Series::onDraw(canvas);
}

void LineSeries::preparePaints() {
    auto& ss = style();

    pat_.setStyle(SkPaint::kStroke_Style);
    pat_.setColor(color());
    pat_.setStrokeWidth(ss.stroke_width);
    pat_.setStrokeCap(SkPaint::kRound_Cap);
    pat_.setStrokeJoin(SkPaint::kDefault_Join);
    pat_.setAntiAlias(ss.antialias);
}

}

}


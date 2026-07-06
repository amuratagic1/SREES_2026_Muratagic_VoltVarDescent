#pragma once
#include <gui/View.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/plot/View.h>
#include <dense/Matrix.h>
#include <string>
#include <vector>

class PlotPanelView : public gui::View
{
    gui::plot::View _plot;
    gui::GridLayout _gl;
    std::vector<std::string> _pvNames;
    dense::DblMatrix         _qHistory; // rows = iterations, cols = pvNames

    static const td::ColorID* seriesColors()
    {
        static const td::ColorID c[] = {
            td::ColorID::SkyBlue, td::ColorID::Orange, td::ColorID::Green,
            td::ColorID::Red,     td::ColorID::Purple, td::ColorID::Brown,
            td::ColorID::Pink,    td::ColorID::Gray
        };
        return c;
    }

    void rebuildPlot()
    {
        _plot.reset();
        const td::ColorID* colors = seriesColors();
        td::UINT4 nIter = _qHistory.getNoOfRows();
        td::UINT4 nPV   = _qHistory.getNoOfCols();

        for (td::UINT4 j = 0; j < nPV; j++)
        {
            if (nIter < 2) continue;

            auto col = _qHistory.getColumnManipulator(j);
            std::vector<gui::CoordType> x(nIter), y(nIter);
            for (td::UINT4 i = 0; i < nIter; i++)
            {
                x[i] = static_cast<gui::CoordType>(i);
                y[i] = static_cast<gui::CoordType>(col(i));
            }
            _plot.addFunction(x.data(), y.data(), x.size(),
                              colors[j % 8], 2.0f, {},
                              td::String(_pvNames[j].c_str()));
        }
        _plot.showLegend(true);
        _plot.drawAgain();
    }

public:
    PlotPanelView()
    : _plot(&gui::plot::Renderer::font, &gui::plot::Renderer::font, td::ColorScheme::Auto)
    , _gl(1, 1)
    {
        _plot.setxAxisName(tr("plotIterAxis").c_str(), false);
        _plot.setyAxisName(tr("plotQAxis").c_str(), false);
        _plot.showGrid(true);

        gui::GridComposer gc(_gl);
        gc.appendRow(_plot);
        setLayout(&_gl);
    }

    void setData(const std::vector<std::string>& pvNames, const dense::DblMatrix& qHistory)
    {
        _pvNames = pvNames;
        _qHistory = qHistory;
        rebuildPlot();
    }
};

#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/ViewSwitcher.h>
#include <dense/Matrix.h>
#include <string>
#include <vector>
#include "TextOutputView.h"
#include "PlotPanelView.h"

class SolverOutputPanel : public gui::View
{
    gui::Button       _btnToggle;
    TextOutputView    _textView;
    PlotPanelView     _plotPanel;
    gui::ViewSwitcher _switcher;
    gui::GridLayout   _gl;
    bool              _showingPlot = false;

public:
    SolverOutputPanel()
    : _btnToggle(tr("btnShowPlot"))
    , _switcher(2)
    , _gl(2, 1)
    {
        _switcher.addView(&_textView,  true);
        _switcher.addView(&_plotPanel, false);

        _btnToggle.onClick([this]()
        {
            _showingPlot = !_showingPlot;
            _switcher.showView(_showingPlot ? 1 : 0);
            _btnToggle.setTitle(_showingPlot ? tr("btnShowText") : tr("btnShowPlot"));
        });

        gui::GridComposer gc(_gl);
        gc.appendRow(_btnToggle, td::HAlignment::Right);
        gc.appendRow(_switcher, td::HAlignment::Left, td::VAlignment::Center);
        setLayout(&_gl);
    }

    void setText(const td::String& txt) { _textView.setText(txt); }
    void setPlotData(const std::vector<std::string>& pvNames, const dense::DblMatrix& qHistory)
    {
        _plotPanel.setData(pvNames, qHistory);
    }
};

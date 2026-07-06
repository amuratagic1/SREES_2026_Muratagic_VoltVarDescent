#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/ViewSwitcher.h>
#include <dense/Matrix.h>
#include <functional>
#include <string>
#include <vector>
#include "SolverControlPanel.h"
#include "SolverOutputPanel.h"
#include "DSSEngine.h"

class SolverView : public gui::View
{
    struct WarningPanel : public gui::View
    {
        gui::Label      _lbl;
        gui::GridLayout _gl;

        WarningPanel()
        : _lbl(tr("solverNotValidated"))
        , _gl(1, 1)
        {
            gui::GridComposer gc(_gl);
            gc.appendRow(_lbl, td::HAlignment::Center, td::VAlignment::Center);
            setLayout(&_gl);
        }
    };

    struct ContentPanel : public gui::View
    {
        SolverControlPanel  _controls;
        SolverOutputPanel   _outputPanel;
        gui::GridLayout     _gl;

        ContentPanel() : _gl(2, 2)
        {
            _gl.setSpaceBetweenCells(8, 0);

            gui::GridComposer gc(_gl);
            gc.appendRow(_controls);
            gc.appendCol(_outputPanel);

            // zero-height spacers enforce 1:4 column width ratio
            _gl.insertSpacer(1, 0, 0, 1, 0, 0);
            _gl.insertSpacer(1, 1, 0, 4, 0, 0);

            setLayout(&_gl);
        }

        CardButton&       btnRun()         { return _controls.btnRun(); }
        gui::NumericEdit& neMaxIter()      { return _controls.neMaxIter(); }
        gui::NumericEdit& neLearningRate() { return _controls.neLearningRate(); }
        gui::NumericEdit& neMaxStep()      { return _controls.neMaxStep(); }
        gui::NumericEdit& neEpsilon()      { return _controls.neEpsilon(); }
        gui::NumericEdit& neQMax()         { return _controls.neQMax(); }
        gui::NumericEdit& neTol()          { return _controls.neTol(); }
        void setText(const td::String& s) { _outputPanel.setText(s); }
        void setPlotData(const std::vector<std::string>& pvNames, const dense::DblMatrix& qHistory)
        {
            _outputPanel.setPlotData(pvNames, qHistory);
        }
    };

    WarningPanel              _warningPanel;
    ContentPanel              _content;
    gui::ViewSwitcher         _switcher;
    gui::GridLayout           _gl;
    std::function<td::String()> _getScript;
    DSSEngine*                _dss = nullptr;

public:
    SolverView()
    : _switcher(2)
    , _gl(1, 1)
    {
        _switcher.addView(&_warningPanel, true);
        _switcher.addView(&_content,      false);

        _content.btnRun().onClick([this]()
        {
            if (!_dss) return;
            std::string script = _getScript ? std::string(_getScript().c_str()) : "";
            if (script.empty())
            {
                _content.setText(tr("noDssScript"));
                return;
            }

            _dss->commandBlock(script);
            std::string loadErr = _dss->lastError();
            if (!loadErr.empty())
            {
                td::String msg = tr("circuitLoadError");
                msg += " ";
                msg += loadErr.c_str();
                _content.setText(msg);
                return;
            }

            auto pvNames = _dss->getPVNames();
            if (pvNames.empty())
            {
                _content.setText(tr("noPvSystems"));
                return;
            }

            int32_t maxIter = 50;
            double  maxStep = 150.0;
            double  learningRate = 5e6;
            double  epsilon = 1.0;
            double  qMax = 1800.0;
            double  tol = 1e-3;
            _content.neMaxIter().getValue(maxIter);
            _content.neLearningRate().getValue(learningRate);
            _content.neMaxStep().getValue(maxStep);
            _content.neEpsilon().getValue(epsilon);
            _content.neQMax().getValue(qMax);
            _content.neTol().getValue(tol);

            auto result = _dss->runVanillaGD(
                pvNames,
                static_cast<int>(maxIter),
                maxStep,
                learningRate,
                epsilon,
                qMax,
                tol
            );

            _content.setText(td::String(result.log.c_str()));
            _content.setPlotData(result.pvNames, result.qHistory);
        });

        gui::GridComposer gc(_gl);
        gc.appendRow(_switcher);
        setLayout(&_gl);
    }

    void setEngine(DSSEngine* dss) { _dss = dss; }

    void setScriptGetter(std::function<td::String()> fn) { _getScript = fn; }

    void setNetworkValidated(bool ok)
    {
        _switcher.showView(ok ? 1 : 0);
    }
};

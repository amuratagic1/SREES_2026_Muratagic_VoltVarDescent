#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/ComboBox.h>
#include <gui/NumericEdit.h>
#include <gui/VerticalLayout.h>
#include <td/LinePattern.h>
#include "../network/CardButton.h"

class SolverControlPanel : public gui::View
{
    gui::Label          _lblAlgorithm;
    gui::ComboBox       _cbAlgorithm;
    gui::Label          _lblCostFn;
    gui::ComboBox       _cbCostFn;
    gui::Label          _lblMaxIter;
    gui::NumericEdit    _neMaxIter;
    gui::Label          _lblLearningRate;
    gui::NumericEdit    _neLearningRate;
    gui::Label          _lblMaxStep;
    gui::NumericEdit    _neMaxStep;
    gui::Label          _lblEpsilon;
    gui::NumericEdit    _neEpsilon;
    gui::Label          _lblQMax;
    gui::NumericEdit    _neQMax;
    gui::Label          _lblTol;
    gui::NumericEdit    _neTol;
    CardButton          _btnRun;
    gui::VerticalLayout _vl;

public:
    SolverControlPanel()
    : _lblAlgorithm(tr("lblAlgorithm"))
    , _lblCostFn(tr("lblCostFn"))
    , _lblMaxIter(tr("lblMaxIter"))
    , _lblLearningRate(tr("lblLearningRate"))
    , _lblMaxStep(tr("lblMaxStep"))
    , _lblEpsilon(tr("lblEpsilon"))
    , _lblQMax(tr("lblQMax"))
    , _lblTol(tr("lblTol"))
    , _neMaxIter(td::int4)
    , _neLearningRate(td::real8)
    , _neMaxStep(td::real8)
    , _neEpsilon(td::real8)
    , _neQMax(td::real8)
    , _neTol(td::real8)
    , _btnRun(trCStr("btnRunSolver"), ":solver", 0.7f, true, 70, true, nullptr,
              td::LinePattern::Solid, true)
    , _vl(25)
    {
        _cbAlgorithm.addItem(tr("algVanillaGD"));
        _cbAlgorithm.addItem(tr("algAdaptiveGD"));
        _cbAlgorithm.addItem(tr("algMomentum"));
        _cbAlgorithm.selectIndex(0);

        _cbCostFn.addItem(tr("costVoltageDev"));
        _cbCostFn.addItem(tr("costPowerLoss"));
        _cbCostFn.addItem(tr("costVoltLimit"));
        _cbCostFn.addItem(tr("costReactivePenalty"));
        _cbCostFn.selectIndex(0);

        _neMaxIter.setText("50");
        _neMaxIter.setMinValue(1);
        _neMaxIter.setMaxValue(10000);

        _neLearningRate.setText("5000000");
        _neLearningRate.setMinValue(1.0);
        _neLearningRate.setMaxValue(1e12);

        _neMaxStep.setText("150.0");
        _neMaxStep.setMinValue(0.001);
        _neMaxStep.setMaxValue(10000.0);

        _neEpsilon.setText("1.0");
        _neEpsilon.setMinValue(0.001);
        _neEpsilon.setMaxValue(1000.0);

        _neQMax.setText("1800.0");
        _neQMax.setMinValue(0.0);
        _neQMax.setMaxValue(10000.0);

        _neTol.setText("0.001");
        _neTol.setMinValue(1e-6);
        _neTol.setMaxValue(100.0);
        _neTol.setNumberOfDigitsAfterDecimalPoint(6);

        _vl << _lblAlgorithm;
        _vl << _cbAlgorithm;
        _vl.appendSpace(6);
        _vl << _lblCostFn;
        _vl << _cbCostFn;
        _vl.appendSpace(6);
        _vl << _lblMaxIter;
        _vl << _neMaxIter;
        _vl.appendSpace(6);
        _vl << _lblLearningRate;
        _vl << _neLearningRate;
        _vl.appendSpace(6);
        _vl << _lblMaxStep;
        _vl << _neMaxStep;
        _vl.appendSpace(6);
        _vl << _lblEpsilon;
        _vl << _neEpsilon;
        _vl.appendSpace(6);
        _vl << _lblQMax;
        _vl << _neQMax;
        _vl.appendSpace(6);
        _vl << _lblTol;
        _vl << _neTol;
        _vl.appendSpacer();
        _vl << _btnRun;
        setLayout(&_vl);
    }

    CardButton&       btnRun()         { return _btnRun; }
    gui::NumericEdit& neMaxIter()      { return _neMaxIter; }
    gui::NumericEdit& neLearningRate() { return _neLearningRate; }
    gui::NumericEdit& neMaxStep()      { return _neMaxStep; }
    gui::NumericEdit& neEpsilon()      { return _neEpsilon; }
    gui::NumericEdit& neQMax()         { return _neQMax; }
    gui::NumericEdit& neTol()          { return _neTol; }
};

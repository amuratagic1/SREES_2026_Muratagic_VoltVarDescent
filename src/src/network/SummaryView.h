#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <string>
#include "DSSEngine.h"
#include "CardButton.h"

class SummaryView : public gui::View
{
    gui::Label      _lblTitle;
    gui::Label      _lblBuses;
    gui::Label      _lblNodes;
    gui::Label      _lblLines;
    gui::Label      _lblLoads;
    gui::Label      _lblTransformers;
    gui::Label      _lblCapacitors;
    gui::Label      _lblGenerators;
    gui::Label      _lblPVSystems;
    gui::Label      _lblTotal;
    CardButton      _btnValidate;
    gui::Label      _lblStatus;
    gui::GridLayout _gl;

    td::String formatCount(const char* labelKey, int32_t n) const
    {
        td::String s = tr(labelKey);
        s += ": ";
        s += std::to_string(n).c_str();
        return s;
    }

    td::String formatEmpty(const char* labelKey) const
    {
        td::String s = tr(labelKey);
        s += ": ";
        s += tr("summaryNA");
        return s;
    }

public:
    SummaryView()
    : _lblTitle(tr("networkSummary"))
    , _btnValidate(trCStr("btnValidate"), ":validate", 0.7f, true, 70, true, ":validated")
    , _gl(14, 1)
    {
        _lblTitle.setFont(gui::Font::ID::SystemLargerBold);
        _lblStatus.setTextColor(td::Accent::Error);
        _btnValidate.setValidatedTitle(trCStr("btnValidated"));

        gui::GridComposer gc(_gl);
        gc.appendRow(_lblTitle,        td::HAlignment::Left);
        gc.startNewRowWithSpace(0, 16);
        gc.appendRow(_lblBuses,        td::HAlignment::Left);
        gc.appendRow(_lblNodes,        td::HAlignment::Left);
        gc.appendRow(_lblLines,        td::HAlignment::Left);
        gc.appendRow(_lblLoads,        td::HAlignment::Left);
        gc.appendRow(_lblTransformers, td::HAlignment::Left);
        gc.appendRow(_lblCapacitors,   td::HAlignment::Left);
        gc.appendRow(_lblGenerators,   td::HAlignment::Left);
        gc.appendRow(_lblPVSystems,    td::HAlignment::Left);
        gc.appendRow(_lblTotal,        td::HAlignment::Left);
        gc.startNewRowWithSpace(0, 16);
        gc.appendRow(_btnValidate,     td::HAlignment::Center);
        gc.appendRow(_lblStatus,       td::HAlignment::Left);
        setLayout(&_gl);
        reset();
    }

    CardButton& btnValidate() { return _btnValidate; }

    void reset()
    {
        _lblBuses.setTitle(formatEmpty("summaryBuses"));
        _lblNodes.setTitle(formatEmpty("summaryNodes"));
        _lblLines.setTitle(formatEmpty("summaryLines"));
        _lblLoads.setTitle(formatEmpty("summaryLoads"));
        _lblTransformers.setTitle(formatEmpty("summaryTransformers"));
        _lblCapacitors.setTitle(formatEmpty("summaryCapacitors"));
        _lblGenerators.setTitle(formatEmpty("summaryGenerators"));
        _lblPVSystems.setTitle(formatEmpty("summaryPVSystems"));
        _lblTotal.setTitle(formatEmpty("summaryElements"));
        _btnValidate.setValidated(false);
        _lblStatus.setTitle("");
        reDraw();
    }

    void setValidated(bool v) { _btnValidate.setValidated(v); reDraw(); }
    void setStatus(const td::String& msg) { _lblStatus.setTitle(msg); }
    void clearStatus() { _lblStatus.setTitle(""); }

    void update(const DSSEngine::Summary& s)
    {
        _lblBuses.setTitle(formatCount("summaryBuses", s.buses));
        _lblNodes.setTitle(formatCount("summaryNodes", s.nodes));
        _lblLines.setTitle(formatCount("summaryLines", s.lines));
        _lblLoads.setTitle(formatCount("summaryLoads", s.loads));
        _lblTransformers.setTitle(formatCount("summaryTransformers", s.transformers));
        _lblCapacitors.setTitle(formatCount("summaryCapacitors", s.capacitors));
        _lblGenerators.setTitle(formatCount("summaryGenerators", s.generators));
        _lblPVSystems.setTitle(formatCount("summaryPVSystems", s.pvSystems));
        _lblTotal.setTitle(formatCount("summaryElements", s.totalElements));
    }
};

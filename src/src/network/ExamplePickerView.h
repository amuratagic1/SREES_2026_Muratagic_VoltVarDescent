#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/Font.h>
#include <gui/VerticalLayout.h>
#include <gui/Timer.h>
#include <functional>
#include "CardButton.h"
#include "ExampleScripts.h"

class ExamplePickerView : public gui::View
{
    gui::Label          _lblHint;
    CardButton          _btnSimple;
    CardButton          _btnMedium;
    CardButton          _btnComplex;
    gui::VerticalLayout _vl;
    gui::Timer          _closeTimer;
    std::function<void(int)> _onPick;
    int _pickedIndex = -1;

    // Closing the containing window must not happen synchronously inside a CardButton's
    // own click handler: that button is still mid-dispatch on the call stack, and
    // destroying it here crashes on the next paint. Deferring via a one-shot timer lets
    // the click event finish unwinding first.
    void pick(int index)
    {
        _pickedIndex = index;
        _closeTimer.start();
    }

protected:
    void measure(gui::CellInfo& ci) override
    {
        gui::View::measure(ci);
        ci.minHor = 420;
        ci.minVer = 300;
    }

    void reMeasure(gui::CellInfo& ci) override
    {
        gui::View::reMeasure(ci);
        ci.minHor = 420;
        ci.minVer = 300;
    }

public:
    ExamplePickerView(std::function<void(int)> onPick)
    : _lblHint(tr("exPickerHint"))
    , _btnSimple(trCStr(exampleScript(0).titleKey), ":sample", 0.6f, true, 58, true)
    , _btnMedium(trCStr(exampleScript(1).titleKey), ":sample", 0.6f, true, 58, true)
    , _btnComplex(trCStr(exampleScript(2).titleKey), ":sample", 0.6f, true, 58, true)
    , _vl(9)
    , _closeTimer(this, 0.05f, false)
    , _onPick(std::move(onPick))
    {
        _lblHint.setFont(gui::Font::ID::SystemNormal);

        _btnSimple.onClick([this]()  { pick(0); });
        _btnMedium.onClick([this]()  { pick(1); });
        _btnComplex.onClick([this]() { pick(2); });

        _closeTimer.onTimer([this]()
        {
            _closeTimer.stop();
            if (_onPick) _onPick(_pickedIndex);
            if (auto* wnd = getContainingWindow())
                wnd->close();
        });

        _vl << _lblHint;
        _vl.appendSpace(10);
        _vl << _btnSimple;
        _vl.appendSpace(8);
        _vl << _btnMedium;
        _vl.appendSpace(8);
        _vl << _btnComplex;
        setLayout(&_vl);
    }
};

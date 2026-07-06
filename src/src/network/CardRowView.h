#pragma once
#include <gui/View.h>
#include <gui/HorizontalLayout.h>
#include "CardButton.h"

class CardRowView : public gui::View
{
    CardButton            _btnStart;
    CardButton            _btnOpen;
    CardButton            _btnSample;
    gui::HorizontalLayout _hl;

public:
    CardRowView()
    : _btnStart(trCStr("btnStartScratch"), ":start")
    , _btnOpen(trCStr("btnOpenDss"), ":open")
    , _btnSample(trCStr("btnChooseExample"), ":sample")
    , _hl(7)
    {
        _hl.appendSpacer();
        _hl << _btnStart;
        _hl.appendSpace(10);
        _hl << _btnOpen;
        _hl.appendSpace(10);
        _hl << _btnSample;
        _hl.appendSpacer();
        setLayout(&_hl);
    }

    CardButton& btnStart()  { return _btnStart; }
    CardButton& btnOpen()   { return _btnOpen; }
    CardButton& btnSample() { return _btnSample; }
};

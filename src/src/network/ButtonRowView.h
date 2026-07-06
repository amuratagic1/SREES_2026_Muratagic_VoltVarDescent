#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include <gui/HorizontalLayout.h>

class ButtonRowView : public gui::View
{
    gui::Button _btnLoad;
    gui::Button _btnSave;
    gui::HorizontalLayout _hl;

public:
    ButtonRowView()
    : _btnLoad(tr("btnLoad"))
    , _btnSave(tr("btnSave"))
    , _hl(4)
    {
        _hl.appendSpace(16);
        _hl << _btnLoad;
        _hl.appendSpace(4);
        _hl << _btnSave;
        setLayout(&_hl);
    }

    gui::Button& btnLoad() { return _btnLoad; }
    gui::Button& btnSave() { return _btnSave; }
};

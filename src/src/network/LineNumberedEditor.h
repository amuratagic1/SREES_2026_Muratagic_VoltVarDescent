#pragma once
#include <gui/View.h>
#include <gui/TextEdit.h>
#include <gui/HorizontalLayout.h>
#include <gui/_aux/LineNumbers.h>
#include <functional>

class LineNumberedEditor : public gui::View
{
    gui::TextEdit         _editor;
    gui::LineNumbers      _lineNumbers;
    gui::HorizontalLayout _hl;

public:
    LineNumberedEditor()
    : _lineNumbers(&_editor)
    , _hl(2)
    {
        _editor.setLineNumbers(&_lineNumbers);
        _hl << _lineNumbers << _editor;
        setLayout(&_hl);
    }

    void       setText(const char* text) { _editor.setText(text); }
    td::String getText()           const { return _editor.getText(); }

    void setOnModified(std::function<void()> fn)
    {
        _editor.onModified(fn);
    }
};

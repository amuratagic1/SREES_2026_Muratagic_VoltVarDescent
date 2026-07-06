#pragma once
#include <gui/View.h>
#include <gui/TextEdit.h>
#include <gui/Font.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>

class TextOutputView : public gui::View
{
    gui::TextEdit   _edit;
    gui::GridLayout _gl;

public:
    TextOutputView()
    : _gl(1, 1)
    {
        const auto& monoFonts = gui::Font::getSystemMonospacedFamilyNames();
        if (monoFonts.size() > 0)
            _edit.setFontName(monoFonts[0]);

        gui::GridComposer gc(_gl);
        gc.appendRow(_edit, td::HAlignment::Left, td::VAlignment::Center);
        setLayout(&_gl);
    }

    void setText(const td::String& txt) { _edit.setText(txt); }
};

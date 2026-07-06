#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include "CardRowView.h"

class WelcomePanel : public gui::View
{
    gui::Image      _imgIcon;
    gui::ImageView  _iconView;
    gui::Label      _title;
    gui::Label      _subtitle;
    CardRowView     _cardRow;
    gui::GridLayout _gl;

public:
    WelcomePanel()
    : _imgIcon(":network")
    , _iconView(true)
    , _title(tr("welcomeTitle"))
    , _subtitle(tr("welcomeSubtitle"))
    , _gl(6, 1)
    {
        _gl.setSpaceBetweenCells(0, 0);
        _iconView.setImage(&_imgIcon);
        _iconView.setFixedWidth(96);
        _iconView.setFixedHeight(96);

        _title.setFont(gui::Font::ID::SystemControlTitle);
        _subtitle.setFont(gui::Font::ID::SystemNormal);
        _subtitle.setAlignment(td::HAlignment::Center);

        gui::GridComposer gc(_gl);
        gc.appendRow(_iconView,  td::HAlignment::Center);
        gc.startNewRowWithSpace(0, 8);
        gc.appendRow(_title,    td::HAlignment::Center);
        gc.appendRow(_subtitle, td::HAlignment::Center);
        gc.startNewRowWithSpace(0, 28);
        gc.appendRow(_cardRow);
        setLayout(&_gl);
    }

    CardRowView& cardRow() { return _cardRow; }
};

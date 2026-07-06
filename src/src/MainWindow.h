#pragma once
#include <gui/Window.h>
#include "MainView.h"

class MainWindow : public gui::Window
{
    MainView _mainView;

public:
    MainWindow()
    : gui::Window(gui::Size(1200, 600))
    {
        setTitle(tr("appTitle"));
        setCentralView(&_mainView);
    }
};

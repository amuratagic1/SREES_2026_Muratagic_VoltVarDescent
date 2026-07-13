#pragma once
#include <gui/View.h>
#include <gui/FileDialog.h>
#include <gui/Panel.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/ViewSwitcher.h>
#include <fstream>
#include <sstream>
#include <functional>
#include "ExampleScripts.h"
#include "ExamplePickerView.h"
#include "WelcomePanel.h"
#include "EditorPanel.h"

class NetworkView : public gui::View
{
    static constexpr td::UINT4 ExamplePickerDlgID = 301;

    WelcomePanel      _welcome;
    EditorPanel       _editor;
    gui::ViewSwitcher _switcher;
    gui::GridLayout   _gl;
    gui::Timer        _closePickerTimer;
    gui::Window*      _pPickerWndToClose = nullptr;

    void showExamplePicker()
    {
        auto onPick = [this](int index, gui::Window* pickerWnd)
        {
            _editor.loadText(exampleScript(index).script);
            _switcher.showView(1);

            // Close the picker window from a timer owned by this (long-lived) view,
            // not one owned by the picker itself -- see the comment in
            // ExamplePickerView::pick() for why closing it from its own timer crashes.
            _pPickerWndToClose = pickerWnd;
            _closePickerTimer.start();
        };

        gui::Panel::showModal<ExamplePickerView>(
            this,
            tr("dlgChooseExample"),
            ExamplePickerDlgID,
            {{gui::Dialog::Button::ID::Cancel, tr("btnCancel")}},
            [](gui::Dialog::Button::ID, ExamplePickerView*) {},
            onPick);
    }

public:
    NetworkView()
    : _switcher(2)
    , _gl(1, 1)
    , _closePickerTimer(this, 0.05f, false)
    {
        _closePickerTimer.onTimer([this]()
        {
            _closePickerTimer.stop();
            if (_pPickerWndToClose)
            {
                _pPickerWndToClose->close();
                _pPickerWndToClose = nullptr;
            }
        });

        _switcher.addView(&_welcome, true);
        _switcher.addView(&_editor,  false);

        _welcome.cardRow().btnStart().onClick([this]()
        {
            _editor.loadText("");
            _switcher.showView(1);
        });

        _welcome.cardRow().btnOpen().onClick([this]()
        {
            gui::OpenFileDialog::show(this, tr("openDssScript"), "dss", 202,
                [this](gui::FileDialog* pDlg)
                {
                    std::ifstream file(pDlg->getFileName().c_str());
                    if (file.is_open())
                    {
                        std::ostringstream ss;
                        ss << file.rdbuf();
                        _editor.loadText(ss.str().c_str());
                        _switcher.showView(1);
                    }
                });
        });

        _welcome.cardRow().btnSample().onClick([this]()
        {
            showExamplePicker();
        });

        gui::GridComposer gc(_gl);
        gc.appendRow(_switcher);
        setLayout(&_gl);
    }

    td::String getText() const { return _editor.getText(); }
    void setEngine(DSSEngine* dss) { _editor.setEngine(dss); }
    void setOnValidated(std::function<void(bool)> fn) { _editor.setOnValidated(fn); }
};

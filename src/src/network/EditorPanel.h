#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include <gui/FileDialog.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <fstream>
#include <sstream>
#include <functional>
#include "ButtonRowView.h"
#include "LineNumberedEditor.h"
#include "SummaryView.h"
#include "DSSEngine.h"

class EditorPanel : public gui::View
{
    LineNumberedEditor        _editor;
    SummaryView               _summary;
    ButtonRowView             _btnRow;
    gui::GridLayout           _gl;
    std::function<void(bool)> _onValidated;
    DSSEngine*                _dss = nullptr;
    bool                      _isValidated = false;

public:
    EditorPanel()
    : _gl(3, 2)
    {
        _gl.setSpaceBetweenCells(4, 8);

        gui::GridComposer gc(_gl);
        gc.appendRow(_editor);
        gc.appendCol(_summary);
        gc.appendRow(_btnRow);
        gc.appendEmptyCols(1);

        // zero-height spacers enforce 3:1 column width ratio
        _gl.insertSpacer(2, 0, 0, 3, 0, 0);
        _gl.insertSpacer(2, 1, 0, 1, 0, 0);

        setLayout(&_gl);

        _editor.setOnModified([this]()
        {
            if (!_isValidated) return;
            _isValidated = false;
            _summary.setValidated(false);
            _summary.clearStatus();
            if (_onValidated) _onValidated(false);
        });

        _summary.btnValidate().onClick([this]()
        {
            if (!_dss) return;
            std::string script = std::string(_editor.getText().c_str());
            if (script.empty())
            {
                _summary.reset();
                if (_onValidated) _onValidated(false);
                return;
            }
            _dss->commandBlock(script);
            std::string err = _dss->lastError();
            bool ok = err.empty();
            if (ok)
            {
                _summary.update(_dss->getSummary());
                _isValidated = true;
                _summary.setValidated(true);
                _summary.clearStatus();
            }
            else
            {
                _summary.reset();
                td::String status = tr("errPrefix");
                status += " ";
                status += err.c_str();
                _summary.setStatus(status);
            }
            if (_onValidated) _onValidated(ok);
        });

        _btnRow.btnLoad().onClick([this]()
        {
            gui::OpenFileDialog::show(this, tr("openDssScript"), "dss", 200,
                [this](gui::FileDialog* pDlg)
                {
                    std::ifstream file(pDlg->getFileName().c_str());
                    if (file.is_open())
                    {
                        std::ostringstream ss;
                        ss << file.rdbuf();
                        _editor.setText(ss.str().c_str());
                        _isValidated = false;
                        _summary.reset();
                        if (_onValidated) _onValidated(false);
                    }
                });
        });

        _btnRow.btnSave().onClick([this]()
        {
            gui::SaveFileDialog::show(this, tr("saveDssScript"), "dss", 201,
                [this](gui::FileDialog* pDlg)
                {
                    std::ofstream file(pDlg->getFileName().c_str());
                    if (file.is_open())
                    {
                        td::String content = _editor.getText();
                        file << content.c_str();
                    }
                });
        });
    }

    void setEngine(DSSEngine* dss) { _dss = dss; }

    void setOnValidated(std::function<void(bool)> fn) { _onValidated = fn; }

    void loadText(const char* text)
    {
        _editor.setText(text);
        _isValidated = false;
        _summary.reset();
        if (_onValidated) _onValidated(false);
    }

    td::String getText() const { return _editor.getText(); }
};

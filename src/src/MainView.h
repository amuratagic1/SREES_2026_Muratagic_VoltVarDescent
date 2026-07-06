#pragma once
#include <gui/TabView.h>
#include <gui/Image.h>
#include "network/NetworkView.h"
#include "solver/SolverView.h"
#include "DSSEngine.h"

class MainView : public gui::TabView
{
    gui::Image _imgNetwork;
    gui::Image _imgSolver;
    DSSEngine  _dss;

public:
    MainView()
    : gui::TabView(gui::TabHeader::Type::FitToText, 8, 64)
    , _imgNetwork(":network")
    , _imgSolver(":solver")
    {
        auto* pNetworkView = new NetworkView();
        auto* pSolverView  = new SolverView();

        pNetworkView->setEngine(&_dss);
        pSolverView->setEngine(&_dss);

        int posNetwork = addView(pNetworkView, tr("tabNetwork"), &_imgNetwork);
        int posSolver  = addView(pSolverView,  tr("tabSolver"),  &_imgSolver);

        setNonRemovable(posNetwork, true);
        setNonRemovable(posSolver,  true);

        pSolverView->setScriptGetter([pNetworkView]() { return pNetworkView->getText(); });
        pNetworkView->setOnValidated([pSolverView](bool ok) { pSolverView->setNetworkValidated(ok); });
    }

    void onInitialAppearance() override
    {
        gui::TabView::onInitialAppearance();
        if (getNumberOfViews() >= 2)
        {
            showView(0);
            getScroller().scrollVisibleOriginTo(gui::Point(0, 0));
        }
    }
};

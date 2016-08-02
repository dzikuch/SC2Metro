//  SC2 Metronome, a tool for improving mechanics in StarCraft 2(TM)
//  Copyright (C) 2016 Martial Demolins AKA Folco

//  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
//  as published by the Free Software foundation, either version 3 of the License, or (at your option) any later version.

//  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
//  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License along with this program.
//  If not, see <http://www.gnu.org/licenses/>.

#include "NativeEventFilter.hpp"
#include "DlgNewTimer.hpp"
#include "MainWindow.hpp"
#include "TimerList.hpp"
#include "TableItem.hpp"
#include <QSize>
#include <QIcon>
#include <QList>
#include <QChar>
#include <QPointer>
#include <QToolBar>
#include <QFileInfo>
#include <QHeaderView>
#include <QStringList>
#include <QAbstractItemView>

MainWindow* MainWindow::mainWindow = nullptr;

MainWindow::MainWindow()
{
    // Install a native event filter to handle the hotkeys
    this->nativeEventFilter = new NativeEventFilter;

    // Create the actions. they will be available in the main toolbar
    this->actionNewList = new QAction(QIcon(":/icon64/NewList.png"), "", this);
    this->actionNewList->setToolTip(tr("Create a new empty list"));

    this->actionOpenList = new QAction(QIcon(":/icon64/OpenList.png"), "", this);
    this->actionOpenList->setToolTip(tr("Open an existing list"));

    this->actionSaveList = new QAction(QIcon(":/icon64/SaveList.png"), "", this);
    this->actionSaveList->setToolTip(tr("Save the current list"));

    this->actionNewTimer = new QAction(QIcon(":/icon64/NewTimer.png"), "", this);
    this->actionNewTimer->setToolTip(tr("Create a new Timer for the current list"));

    this->actionEditTimer = new QAction(QIcon(":/icon64/EditTimer.png"), "", this);
    this->actionEditTimer->setToolTip(tr("Modify the currently selected Timer"));

    this->actionRemovTimer = new QAction(QIcon(":/icon64/RemoveTimer.png"), "", this);
    this->actionRemovTimer->setToolTip(tr("Remove the currently selected timer"));

    this->actionMisc = new QAction(QIcon(":/icon64/Misc.png"), "", this);
    this->actionMisc->setToolTip(tr("About, Help and Licenses"));

    QAction* actionSeparator1 = new QAction(this);
    QAction* actionSeparator2 = new QAction(this);
    actionSeparator1->setSeparator(true);
    actionSeparator2->setSeparator(true);

    // Create the main toolbar and insert the actions
    QList<QAction*> actionList;
    actionList << actionNewList << actionOpenList << actionSaveList << actionSeparator1 << actionNewTimer << actionEditTimer << actionRemovTimer
               << actionSeparator2 << actionMisc;

    QSize iconSize(MAIN_TOOLBAR_ICON_WIDTH, MAIN_TOOLBAR_ICON_HEIGHT);

    QToolBar* toolBar = new QToolBar(this);
    toolBar->setIconSize(iconSize);
    toolBar->addActions(actionList);
    toolBar->setMovable(false);
    this->addToolBar(toolBar);

    // Create the main widget, a table which display the timers
    // The table display three columns: sound name, period and hotkey
    this->timerTable = new QTableWidget(0, 3, this);

    // Table properties
    this->timerTable->setShowGrid(true);
    this->timerTable->setSortingEnabled(false);
    this->timerTable->setAlternatingRowColors(true);
    this->timerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->timerTable->setSelectionMode(QAbstractItemView::SingleSelection);
    this->timerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->timerTable->horizontalHeader()->setStretchLastSection(true);
    this->timerTable->verticalHeader()->setVisible(false);

    // Set the columns header and size
    QStringList labels;
    labels << tr("Sound name") << tr("Period") << tr("Hotkey");
    this->timerTable->setHorizontalHeaderLabels(labels);

    this->timerTable->setColumnWidth(COLUMN_NAME, 200);
    this->timerTable->setColumnWidth(COLUMN_PERIOD, 100);
    this->timerTable->setColumnWidth(COLUMN_HOTKEY, 200);

    // Finally, install the table in the main window
    setCentralWidget(this->timerTable);

    // Establish internal connections
    connect(this->actionNewTimer, &QAction::triggered, this, &MainWindow::newTimerTriggerred);
}

void MainWindow::establishExternalConnections()
{
}

MainWindow::~MainWindow()
{
    delete this->nativeEventFilter;
    this->mainWindow = nullptr;
}

MainWindow* MainWindow::instance()
{
    if (mainWindow == nullptr) {
        mainWindow = new MainWindow;
    }
    return mainWindow;
}

QMenu* MainWindow::createPopupMenu()
{
    return nullptr;
}

//
//  Methods called when the toolbar actions are triggered
//

void MainWindow::newTimerTriggerred()
{
    QPointer<DlgNewTimer> dlg = new DlgNewTimer;
    if (dlg->exec() == QDialog::Accepted) {
        TimerList::instance()->newTimer(dlg->getFilename(), dlg->getPeriod(), dlg->getKeySquence(), dlg->getNativeVirtualKey(), dlg->getNativeModifiers());
    }
    delete dlg;
}

//
//  Methods called by the TimerList instance
//

void MainWindow::newTimer(QString filename, int period, QKeySequence keySequence)
{
    QString displayedName = QFileInfo(filename).completeBaseName();
    TableItem* itemName = new TableItem(displayedName);

    QString displayedPeriod = QString("%1:%2").arg(period / 60, 2, 10, QChar('0')).arg(period % 60, 2, 10, QChar('0'));
    TableItem* itemPeriod = new TableItem(displayedPeriod);

    QString displayedHotkey = keySequence.toString();
    TableItem* itemHotkey = new TableItem(displayedHotkey);

    int row = this->timerTable->rowCount();
    this->timerTable->insertRow(row);
    this->timerTable->setItem(row, COLUMN_NAME, itemName);
    this->timerTable->setItem(row, COLUMN_PERIOD, itemPeriod);
    this->timerTable->setItem(row, COLUMN_HOTKEY, itemHotkey);
}

void MainWindow::setTimerPlaying(int index)
{
    TableItem* item = new TableItem("playing !");
    this->timerTable->setItem(index, 0, item);
}

void MainWindow::setTimerStopped(int index)
{
    TableItem* item = new TableItem("stopped !");
    this->timerTable->setItem(index, 0, item);
}

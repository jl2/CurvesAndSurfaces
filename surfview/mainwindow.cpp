/*
  mainwindow.cpp

  Copyright (c) 2012, Jeremiah LaRocco jeremiah.larocco@gmail.com

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <QtGui>
#include <QSettings>

#include <cstdlib>

#include "mainwindow.h"

#include "surfaceviewer.h"

/*!
  Performs initialization
*/
MainWindow::MainWindow() : QMainWindow(), promptExit(true), showingFacets(true), showingPolygons(true) {
  
    // Create SurfaceViewer widget
    sview = new SurfaceViewer(this);
  
    qset = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                         "SurfaceViewer", "SurfaceViewer");

    readSettings();
  
    // Make the SurfaceViewer the central widget 
    setCentralWidget(sview);

    // Initialize GUI stuff
    setWindowTitle(tr("Surface Viewer"));
    setWindowIcon(QIcon(":/images/icon.png"));
    createActions();
    createMenus();
    createToolbar();
    createStatusBar();
}

MainWindow::~MainWindow() {
    // Delete everything
    delete aboutAction;
    delete aboutQtAction;
    delete quitAction;
    delete resetViewAction;

    delete theToolbar;
  
    delete fileMenu;
    delete optionsMenu;
    delete helpMenu;
    delete statusLabel;
    delete tbIcon;

    delete sview;
}

/*!
  Initializes all of the QActions used by the game
*/
void MainWindow::createActions() {
    // About
    aboutAction = new QAction(tr("About"), this);
    aboutAction->setIcon(QIcon(":/images/about.png"));
    aboutAction->setShortcut(tr("Ctrl+A"));
    aboutAction->setStatusTip(tr("About SurfaceViewer"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    // About QT
    aboutQtAction = new QAction(tr("About Qt"), this);
    aboutQtAction->setIcon(QIcon(":/images/about.png"));
    aboutQtAction->setStatusTip(tr("About Qt"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // Exit
    quitAction = new QAction(tr("Exit"), this);
    quitAction->setIcon(QIcon(":/images/quit.png"));
    quitAction->setShortcut(tr("Ctrl+Q"));
    quitAction->setStatusTip(tr("Quit"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));//quit()));

    // Reset view
    resetViewAction = new QAction(tr("Reset View"), this);
    resetViewAction->setIcon(QIcon(":/images/view.png"));
    resetViewAction->setShortcut(tr("Ctrl+V"));
    resetViewAction->setStatusTip(tr("Reset the view"));
    connect(resetViewAction, SIGNAL(triggered()), this, SLOT(resetView()));
    
    showFacetsAction = new QAction(tr("Show Facets"), this);
    showFacetsAction->setStatusTip(tr("Show facet outlines."));
    showFacetsAction->setCheckable(true);
    showFacetsAction->setChecked(showingFacets);
    connect(showFacetsAction, SIGNAL(triggered()), this, SLOT(toggleFacets()));
    
    showPolygonsAction = new QAction(tr("Show Polygons"), this);
    showPolygonsAction->setStatusTip(tr("Fill facets."));
    showPolygonsAction->setCheckable(true);
    showPolygonsAction->setChecked(showingPolygons);
    connect(showPolygonsAction, SIGNAL(triggered()), this, SLOT(togglePolygons()));
}

/*!
  Initialize menus
*/
void MainWindow::createMenus() {
    // Game menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(quitAction);

    // Options menu
    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showPolygonsAction);
    optionsMenu->addAction(showFacetsAction);

    // Help menu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutQtAction);
}

/*!
  Initialize toolbar
*/
void MainWindow::createToolbar() {
    theToolbar = addToolBar(tr("View"));
    theToolbar->addAction(resetViewAction);
}

/*!
  Initialize status bar
*/
void MainWindow::createStatusBar() {
    // Make the label as large as it will ever be
    statusLabel = new QLabel("No surface");
    statusLabel->setAlignment(Qt::AlignHCenter);

    // Then set its minimum size and add to the status bar
    statusLabel->setMinimumSize(statusLabel->sizeHint());
    statusBar()->addWidget(statusLabel);
}

/*!
  Updates the status bar to display number of bombs left.
*/
void MainWindow::updateStatusBar(QString txt) {
    statusLabel->setText(txt);
}

/*!
  Display the about box
*/
void MainWindow::about() {
    QMessageBox::about(this, tr("About Surface Viewer"),
                       tr("<h2>Surface Viewer 0.1</h2>"
                          "<p>Copyright &copy; 2012 Jeremiah LaRocco</p>"
                          "<p>A surface  file viewer</p>"
                          "<p>Program to experiment with the curves and surfaces from <b>\"Curves and Surfaces for Computer Graphics\"</b></p>"));
}

/*!
  Prompt before closing
*/
void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}

/*!
  Reset the view
*/
void MainWindow::resetView() {
    if (sview) {
        sview->resetView();
    }
}

void MainWindow::readSettings() {
    qset->sync();
    // For future reference:
    // qset->value("whatever", default_int_value).toInt();
    // qset->value("whatever", default_string_value).toString();
    // qset->value("whatever", QDateTime(2012, 4,2)).toDateTime();
}

void MainWindow::toggleFacets() {
    showingFacets = !showingFacets;
    if (sview) {
        sview->setShowFacets(showingFacets);
    }
}
void MainWindow::togglePolygons() {
    showingPolygons = !showingPolygons;
    if (sview) {
        sview->setShowPolygons(showingPolygons);
    }
}

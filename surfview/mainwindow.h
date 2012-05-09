/*
  mainwindow.h

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <ctime>

class QAction;
class QLabel;
class QIcon;
class QMenu;
class QToolBar;
class QCloseEvent;
class QSettings;
class QTimer;

class SurfaceViewer;

class MainWindow : public QMainWindow {
    Q_OBJECT;

public:
    MainWindow();
    ~MainWindow();

private slots:
    void about();
    void resetView();
    void updateStatusBar(QString fileName);
    void toggleFacets();
    void togglePolygons();

protected:
    // Initialization functions
    void createActions();
    void createMenus();
    void createToolbar();
    void createStatusBar();
    void closeEvent(QCloseEvent *event);

    void readSettings();
private:
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *quitAction;
    QAction *resetViewAction;

    QAction *showFacetsAction;
    QAction *showPolygonsAction;

    QToolBar *theToolbar;
  
    QMenu *fileMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QLabel *statusLabel;
    QIcon *tbIcon;

    QSettings *qset;

    SurfaceViewer *sview;

    bool promptExit;

    bool showingFacets;
    bool showingPolygons;
};

#endif

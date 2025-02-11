/*
 * docdisplaywindow.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

//
#ifndef DOCDISPLAYWINDOW_H
#define DOCDISPLAYWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>

namespace Ui {
  class DocDisplayWindow;
}

class DocDisplayWindow : public QMainWindow {
  Q_OBJECT
public:
  DocDisplayWindow(QWidget *parent = 0);
  ~DocDisplayWindow();

protected:
  void changeEvent(QEvent *e);
  void closeEvent(QCloseEvent *e);

private:
  Ui::DocDisplayWindow *ui;

private slots:
  void printTriggered();

signals:
  void closed();
};

#endif// DOCDISPLAYWINDOW_H

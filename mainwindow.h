#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "lightbutton.h"

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_NewGameButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_SolveButton_clicked();

    vector<int> gauss(vector<vector<int>> &mat, vector<int> &b);

private:
    Ui::MainWindow *ui;
    vector<vector<LightButton*>> m_board;
    int cnt;
};
#endif // MAINWINDOW_H

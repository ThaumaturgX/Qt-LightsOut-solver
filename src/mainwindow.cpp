#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>
using std::vector;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Lights out");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_NewGameButton_clicked()
{
    // clear grid
    auto &grid = ui->gridLayout;
    if (grid->layout())
    {
        QLayoutItem* item;
        while ((item = grid->layout()->takeAt(0)))
        {
            delete item->widget();
            delete item;
        }
    }
    m_board.clear();
    // make a new board
    int n = ui->SizeInput->text().toInt();
    cnt = n*n;
    m_board.resize(n, vector<LightButton*>(n));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
        {
            LightButton *button = new LightButton(i, j);
            button->setFixedSize(QSize(200/n, 200/n));
            ui->gridLayout->addWidget(button, i, j);
            m_board[i][j] = button;
            // On click logic
            connect(button, &LightButton::clicked, this, [&](){
                LightButton *thisButton = dynamic_cast<LightButton*>(QObject::sender());
                if(ui->EditModeCheckBox->isChecked()) {
                    cnt += (m_board[thisButton->geti()][thisButton->getj()]->switchState() ? 1 : -1);
                    ui->WinLabel->setText(cnt ? QString() : "You won");
                    return;
                }
                int di[5] = {0, 1, 0, -1, 0};
                int dj[5] = {-1, 0, 1, 0, 0};
                for(int t = 0; t < 5; t++)
                {
                    int ii = thisButton->geti() + di[t];
                    int jj = thisButton->getj() + dj[t];
                    if(ii >= 0 && ii < int(m_board.size()) && jj >= 0 && jj < int(m_board.size()))
                        cnt += (m_board[ii][jj]->switchState() ? 1 : -1);
                }
                ui->WinLabel->setText(cnt ? QString() : "You won");
            });
        }
    // Make random moves
    if(ui->EditModeCheckBox->isChecked()) return;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
        {
            int x = rand()%10 + 1;
            if(x>=4)
                emit m_board[i][j]->clicked();
        }
    ui->WinLabel->setText(cnt ? QString() : "You won");
}


void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    ui->SizeInput->setText(QString::number(position));
}

void MainWindow::on_SolveButton_clicked()
{
    int n = m_board.size();
    vector<vector<int>> mat;
    int di[5] = {0, 1, 0, -1, 0};
    int dj[5] = {-1, 0, 1, 0, 0};
    // construct the matrix
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
        {
            vector<int> v(n*n, 0);
            for(int t = 0; t < 5; t++)
            {
                int ii = i + di[t];
                int jj = j + dj[t];
                if(ii >= 0 && ii < n && jj >= 0 && jj < n)
                    v[n*ii + jj] = 1;
            }
            mat.push_back(v);
        }
    // transpose it
    for(int i = 0; i < n; i++)
        for(int j = i + 1; j < n; j++)
            std::swap(mat[i][j], mat[j][i]);
    // create a vector representing the state of the board
    vector<int> b;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            b.push_back(m_board[i][j]->getState());
    auto solution = gauss(mat, b);
    for(int i = 0; i < n*n; i++)
        m_board[i/n][i%n]->setText((solution[i]%2 == 1) ? "X" : "");
}

vector<int> MainWindow::gauss(std::vector<std::vector<int>> &mat, std::vector<int> &b)
{
    int n = mat.size();
    for(int i = 0; i < n; i++)
    {
        // i is the current row

        // if i-th element isn't 1, find a row where it is and swap
        if(mat[i][i] != 1) {
            for(int j = i + 1; j < n; j++) {
                if(mat[j][i] == 1) {
                   std::swap(mat[i], mat[j]);
                   std::swap(b[i], b[j]);
                }
            }
        }

        // iterate over the remaining rows and make all elements
        // in the jth column equal to 0
        for(int j = i + 1; j < n; j++)
        {
            if(mat[j][i] == 1)
            {
                for(int k = i; k < n; k++)
                    mat[j][k] = (mat[j][k] + mat[i][k])%2;
                b[j] = (b[j] + b[i])%2;
            }
        }
    }

    // fill the answer
    vector<int> ans(n);
    for(int i = n - 1; i >= 0; i--)
    {
        ans[i] = b[i];
        for(int j = i + 1; j < n; j++)
            ans[i] = (ans[i] + mat[i][j]*ans[j])%2;
    }
    return ans;
}


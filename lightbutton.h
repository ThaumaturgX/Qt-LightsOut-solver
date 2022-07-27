#ifndef LIGHTBUTTON_H
#define LIGHTBUTTON_H

#include <QPushButton>

class LightButton : public QPushButton
{
    Q_OBJECT
public:
    LightButton(int i, int j);
    int switchState();
    int getState(){return state;}
    int geti(){return i;}
    int getj(){return j;}
    //virtual void mousePressEvent(QMouseEvent *e) override;
private:
    bool state;
    int i;
    int j;
};
#endif // LIGHTBUTTON_H

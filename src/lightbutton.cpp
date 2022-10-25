#include "lightbutton.h"

LightButton::LightButton(int i, int j)
{
    this->i = i;
    this->j = j;
    state = 1;
    setStyleSheet("background-color: yellow");
}

int LightButton::switchState()
{
    state = !state;
    setStyleSheet(state ? "background-color: yellow" : "background-color: gray");
    return state;
}


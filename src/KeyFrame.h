#pragma once

enum Action {
    TRANSLATE, ROTATE
};

class KeyFrame {
public:
    float value;
    Action action;
    double time;

    KeyFrame(float val, Action action1, double time);

};

KeyFrame::KeyFrame(float val, Action action1, double time) {
    value = val;
    action = action1;
    this->time = time;
}

#pragma once

class KeyFrame {
public:
    float translate;
    float rotate;
    double time;

    KeyFrame(float translation, float rotation, double time);

};

KeyFrame::KeyFrame(float translation, float rotation, double time) {
    translate = translation;
    rotate = rotation;
    this->time = time;
}

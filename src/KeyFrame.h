#pragma once

class KeyFrame {
public:
    float translatex;
    float translatey;
    float translatez;
    float rotate;
    double time;

    KeyFrame(float translationx, float translationy, float translationz, float rotation, double time);

};

KeyFrame::KeyFrame(float translationx,float translationy,float translationz, float rotation, double time) {
    translatex = translationx;
    translatey = translationy;
    translatez = translationz;
    rotate = rotation;
    this->time = time;
}

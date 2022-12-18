#pragma once
#include "Products.h"
#include "ModelData.h"

class InfoField {
public:
    InfoField(int x0, int y0);
    void ChangeMode(IClickable* new_object);
    virtual void draw(sf::RenderWindow& window, sf::Font& font);
private:
    int x0_, y0_;
    int visualization_type_ = -1;
    const int width_ = 1640 - 1170, height_ = 400;
    IClickable* object_ = nullptr;
    sf::RectangleShape texture_;
};
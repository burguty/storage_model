#pragma once
#include "Products.h"
#include "ModelData.h"

class InfoField : public IDrawable {
public:
    InfoField(int x0, int y0);
    void ChangeMode(IClickable* new_object);
    virtual void draw(sf::RenderWindow& window);
    virtual void Move(int x, int y);
private:
    int visualization_type_ = -1;
    const int width_ = 220, height_ = 400;
    IClickable* object_ = nullptr;
    sf::RectangleShape texture_;
};
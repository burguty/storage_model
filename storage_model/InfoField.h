#pragma once
#include "Products.h"
#include "ModelData.h"

class InfoField {
public:
    InfoField(int x0, int y0, sf::Font& font);
    ~InfoField();
    void ChangeMode(IClickable* new_object);
    virtual void draw(sf::RenderWindow& window);
    bool Click(int x, int y);
    void TypeSymbol(char c);
    void DeleteSymbol();
private:
    int x0_, y0_;
    int visualization_type_ = -1;
    const int width_ = 1640 - 1170, height_ = 400;
    IClickable* object_ = nullptr;
    TextLine* line_ = nullptr;
    InputLine* input_line_ = nullptr;
    Button* button_ = nullptr;
    sf::Font& font_;
    sf::RectangleShape texture_;
};
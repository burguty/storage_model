#pragma once
#include "Products.h"
#include "ModelData.h"
#include "Shop.h"

class InfoField {
public:
    InfoField(Storage* storage, int x0, int y0, sf::Font& font);
    ~InfoField();
    void ChangeMode(IClickable* new_object);
    virtual void draw(sf::RenderWindow& window);
    bool Click(int x, int y);
    void TypeSymbol(char c = 0);
private:
    Storage* storage_;
    void ClearFront();
    int x0_, y0_;
    int visualization_type_ = -1;
    const int width_ = 1640 - 1170, height_ = 400;
    IClickable* object_ = nullptr;
    std::vector<TextLine*>text_lines_;
    std::vector<InputLine*>input_lines_; 
    std::vector<Button*>buttons_;
    sf::Font& font_;
    sf::RectangleShape texture_;
};
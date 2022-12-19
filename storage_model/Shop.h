#pragma once
#include "Products.h"
#include <random>

class Request;

class Shop : public IClickable {
public:
    Shop(int x0, int y0, sf::Font& font);
    ~Shop() = default;

    virtual void draw(sf::RenderWindow& window);
    virtual void DrawInformation(sf::RenderWindow& window, int x0, int y0);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
    Request* MakeRequest(Storage* storage);
private:
    const int width_ = 200, height_ = 200;
    sf::RectangleShape texture_;
};

class Request : public IClickable {
public:
    Request(int product_type, int product_count, Shop* customer);
    virtual ~Request() = default;
    virtual void draw(sf::RenderWindow& window);
    virtual void DrawInformation(sf::RenderWindow& window, int x0, int y0);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
private:
    int product_type_;
    int product_count_;
    Shop* customer_;
};

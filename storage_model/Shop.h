#pragma once
#include "Products.h"
#include <random>

class Request;

class Shop : public IClickable {
public:
    Shop(int index, sf::Font& font, std::mt19937& gen);
    ~Shop() = default;

    virtual void draw(sf::RenderWindow& window);
    virtual void DrawInformation(sf::RenderWindow& window,
        int x0, int y0, sf::Font& font);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
    Request* MakeRequest(Storage* storage);
private:
    int CountX0(int index) {
        return (1150 + 30) / 2 - 100 / 2 + index * ((1150 - 30 - 7 * 100) / 4);
    }
    int CountY0(int index) {
        return 20 + abs(index) * 50;
    }
    std::mt19937& gen_;
    const int width_ = 100, height_ = 100;
    sf::RectangleShape texture_;
};

class Request : public IClickable {
public:
    Request(int product_type, int product_count, Shop* customer);
    virtual ~Request() = default;
    virtual void draw(sf::RenderWindow& window);
    virtual void DrawInformation(sf::RenderWindow& window,
        int x0, int y0, sf::Font& font);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
private:
    int product_type_;
    int product_count_;
    Shop* customer_;
};

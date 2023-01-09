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
    int GetX0();
    int GetY0();
    void AddProductBatch(ProductBatch* batch);
private:
    int CalculateX0(int index);
    int CalculateY0(int index);
    std::mt19937& gen_;
    const int width_ = 100, height_ = 100;
    std::wstring shop_name_ = L"Имя магазина";
    int requests_sum_ = 0;
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
    int approved_count_ = 0;
    int GetProductType();
    void RecalculateProfitAndApprovedCount(Storage* storage, int approved_count);
    void RecalculateProductCount(Storage* storage);
    int GetTargetX();
    int GetTargetY();
    void AddProductBatch(ProductBatch* batch);
private:
    int profit = 0;
    int product_type_;
    int product_count_;
    std::wstring name_;
    Shop* customer_;
};

#pragma once
#include "Frontend.h"
#include "ModelData.h"
#include <cmath>
#include <deque>
#include <vector>

class IProduct {
public:
    IProduct(int product_type, int price, int production_day);
    virtual ~IProduct();
    int GetProductType();
protected:
    int GetPrice();
    int GetRemains(int day);
    void ChangePrice(int new_price);
private:
    int product_type_, price_, production_day_, shelf_life_;
};

class IClickable : public IDrawable {
public:
    IClickable(int x0, int y0);
    virtual void DrawInformation(sf::RenderWindow& window, int x0, int y0) = 0;
    virtual int GetVisualizationType() = 0;
    virtual IClickable* Click(int x, int y) = 0;
};

class ProductBatch : public IProduct, public IClickable {
public:
    ProductBatch(int product_type, int price, int production_day, 
        int count_at_box, int box_count, int x0, int y0);
    void Reduction(int new_cost);
    int ProductCount();
    int CalculateRequiredBox(int products_count);
    int CalculateSellingProducts(int products_count);
    int RequestPrice(int products_count);
    bool Sell(int products_count);
    virtual void draw(sf::RenderWindow& window);
    virtual void Move(int x, int y);
    virtual void DrawInformation(sf::RenderWindow& window, int x0, int y0);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
private:
    int count_at_box_, box_count_;
    const int width = 15, height = 15;
    sf::RectangleShape texture_;
};

class StorageRoom : public IClickable {
public:
    StorageRoom(int product_type, int x0, int y0, sf::Font& font);
    virtual ~StorageRoom();

    void AddDelivery(ProductBatch* batch);
    std::vector<ProductBatch*> Clearing(int day);
    int RequestPrice(int products_count);
    void ProductShipments(int products_count);

    virtual void draw(sf::RenderWindow& window);
    virtual void Move(int x, int y);
    virtual void DrawInformation(sf::RenderWindow& window, int x0, int y0);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
private:
    int CalculateXForBatch(int ind = -1);
    int CalculateYForBatch(int ind = -1);
    int product_type_;
    std::deque<ProductBatch*>batches_;
    const int width_ = 150, height_ = 90, step_ = 10, in_line_ = 5;
    sf::RectangleShape texture_;
    sf::Text back_text_;
};

class Storage : public IClickable {
public:
    Storage(ModelData* data, int x0, int y0, sf::Font& font);
    virtual ~Storage();

    void AddDelivery(ProductBatch* batch);
    std::vector<ProductBatch*> Clearing(int day);
    int RequestPrice(int product_type, int products_count);
    void ProductShipments(int product_type, int products_count);

    virtual void draw(sf::RenderWindow& window);
    virtual void Move(int x, int y);
    virtual void DrawInformation(sf::RenderWindow& window, int x0, int y0);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
private:
    StorageRoom* rooms_[17];
    const int width_ = 1150 - 300, height_ = 350, step_ = 15, in_line_ = 3;
    sf::RectangleShape texture_;
};

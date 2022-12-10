#pragma once
#include "Frontend.h"
#include "ModelData.h"
#include <cmath>
#include <deque>

class IProduct {
protected:
    IProduct(int product_type, int price, int production_day);
    virtual ~IProduct();
    int GetPrice();
    int GetRemains(int day);
    void ChangePrice(int new_price);
private:
    int product_type_, price_, production_day_, shelf_life_;
};

class ProductBatch : public IProduct, public IDrawable {
public:
    ProductBatch(int product_type, int price, int production_day, 
        int count_at_box, int box_count, int x0, int y0);
    int Reduction(int new_cost);
    int CalculateRequiredCount(int products_count);
    int RequestPrice(int box_count);
    bool Sell(int box_count);
    virtual void draw(sf::RenderWindow& window);
    virtual void Move(int x, int y);
    virtual bool Click(int x, int y);
private:
    int count_at_box_, box_count_;
};

class StorageRoom : public IDrawable {
public:
    StorageRoom(int x0, int y0, int product_type);
    virtual ~StorageRoom();

private:
    int product_type_;
    std::deque<ProductBatch*>batches;
};
#pragma once
#include "Frontend.h"
#include "ModelData.h"
#include <cmath>
#include <deque>
#include <vector>

class IProduct {
public:
    IProduct(int product_type, int price);
    IProduct(int product_type, int price, int days);
    virtual ~IProduct();
    int ProductType();
    int Price();
    void GoToTheNextDay();
    bool IsOverdue();
    void SetColor(sf::Color color);
    void CalculateColor();
    void SetRemains();
protected:
    int Remains();
    void ChangePrice(int new_price);
    int product_type_;
    sf::Color back_color_;
    int object_type_; // 0 - Purchase
private:
    int price_, remains_;
};

class IClickable : virtual public IDrawable {
public:
    IClickable() = default;
    virtual void DrawInformation(sf::RenderWindow& window, 
        int x0, int y0, sf::Font& font) = 0;
    virtual int GetVisualizationType() = 0;
    virtual IClickable* Click(int x, int y) = 0;
};

class ProductBatch : public IProduct, public IClickable, public IMovable {
public:
    ProductBatch(int product_type, int price, int purchase_price, 
        int box_count, int x0, int y0);
    ProductBatch(int product_type, int price, int purchase_price, 
        int box_count, int days, int x0, int y0);
    int PurchasePrice();
    void Reduction(int new_cost);
    int ProductsCount();
    int CalculateRequiredBox(int products_count);
    int CalculateSellingProducts(int products_count);
    int RequestPrice(int products_count);
    bool Sell(int products_count);
    int BoxCount();

    void Move(int x, int y);
    virtual void draw(sf::RenderWindow& window);
    virtual void DrawInformation(sf::RenderWindow& window, 
        int x0, int y0, sf::Font& font);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
private:
    int count_at_box_, box_count_, purchase_price_;
    const int width = 15, height = 15;
    sf::RectangleShape texture_;
};

class StorageRoom : public IClickable {
public:
    StorageRoom(int product_type, int x0, int y0, sf::Font& font);
    virtual ~StorageRoom();

    void AddDelivery(ProductBatch* batch);
    std::vector<ProductBatch*> Clearing();
    int RequestPrice(int products_count);
    std::vector<ProductBatch*> ProductShipments(int box_count);
    int ProductType();
    int ProductsCount();
    int Income();
    int ProductsPrice();
    int ProductsPurchasePrice();
    int SpentOnPurchase();
    int Profit();
    int BoxCount();
    void StartPurchasePhase(int purchase_price, int time_of_purchase);
    void StopPurchasePhase();
    std::deque<IMovable*> GoToTheNextDayCars();
    void GoToTheNextDay();
    void SetOrderCount(int count);
    bool IsOrderCountCorrect();
    bool IsOrderCountCorrect(int order_count);
    int GetOrderCount();

    virtual void draw(sf::RenderWindow& window);
    virtual void DrawInformation(sf::RenderWindow& window, int x0, int y0, sf::Font& font);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
    int CalculateXForPurchase(int ind = -1);
    int CalculateYForPurchase(int ind = -1);
    int CalculateXForBatch(int ind = -1);
    int CalculateYForBatch(int ind = -1);
private:
    int mode_ = 0;
    int order_count_ = 0, purchase_price_, time_of_purchase_;
    int max_box_count_;

    int product_type_, count_at_box_, income_ = 0, expenses_ = 0;
    std::deque<ProductBatch*>batches_;
    std::vector<ProductBatch*>purchases_;

    const int width_ = 150, height_ = 90, step_ = 10, in_line_ = 5;
    sf::RectangleShape texture_;
    sf::Text back_text_;
};

class Storage : public IClickable {
public:
    Storage(ModelData* data, int x0, int y0, sf::Font& font);
    virtual ~Storage();

    void AddDelivery(ProductBatch* batch);
    std::vector<ProductBatch*> Clearing();
    int RequestPrice(int product_type, int products_count);
    std::vector<ProductBatch*> ProductShipments(int product_type, int box_count);
    int SpentOnPurchase();
    int Profit();
    int Price();
    int Income();
    void GoToTheNextDay();
    std::deque<IMovable*> GoToTheNextDayCars();
    bool IsProductUsing(int product_type);
    int ProductsCount(int product_type);
    void StartPurchasePhase(int time_of_purchase);
    void StopPurchasePhase();
    bool IsOrderCountsCorrect();

    virtual void draw(sf::RenderWindow& window);
    virtual void DrawInformation(sf::RenderWindow& window, 
        int x0, int y0, sf::Font& font);
    virtual int GetVisualizationType();
    virtual IClickable* Click(int x, int y);
private:
    int mode_ = 0;
    StorageRoom* rooms_[15];
    const int width_ = 1150 - 300, height_ = 350, step_ = 15, in_line_ = 5;
    sf::RectangleShape texture_;
};

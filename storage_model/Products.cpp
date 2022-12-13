#include "Products.h"

//IClickable
IClickable::IClickable(int x0, int y0) :IDrawable(x0, y0) {}

// IProduct
IProduct::IProduct(int product_type, int price, int production_day) :
    product_type_(product_type), price_(price), production_day_(production_day) {
    shelf_life_ = GetShelfLife(product_type);
}
IProduct::~IProduct() {}
int IProduct::GetPrice() {
    return price_;
}
int IProduct::GetRemains(int day) {
    return production_day_ + shelf_life_ - day;
}
void IProduct::ChangePrice(int new_price) {
    price_ = new_price;
}
int IProduct::GetProductType() {
    return product_type_;
}

// ProductBatch
ProductBatch::ProductBatch(int product_type, int price, int production_day, 
    int count_at_box, int box_count, int x0, int y0):
    IProduct(product_type, price, production_day), IClickable(x0, y0), 
    count_at_box_(count_at_box), box_count_(box_count){
    texture_.setPosition(x0, y0);
    texture_.setFillColor(sf::Color::Black);
    texture_.setOutlineThickness(3);
    texture_.setOutlineColor(sf::Color::Black);
    texture_.setSize(sf::Vector2f(width, height));
}
void ProductBatch::Reduction(int new_cost) {
    ChangePrice(new_cost);
}
int ProductBatch::CalculateRequiredBox(int products_count) {
    return std::min(box_count_, 
        static_cast<int>(std::ceil(static_cast<double>(products_count) / count_at_box_)));
}
int ProductBatch::RequestPrice(int products_count) {
    return CalculateRequiredBox(products_count) * count_at_box_ * GetPrice();
}
int ProductBatch::CalculateSellingProducts(int products_count) {
    return count_at_box_ * CalculateRequiredBox(products_count);
}
bool ProductBatch::Sell(int products_count) {
    int box_count = CalculateRequiredBox(products_count);
    box_count_ -= box_count;
    return box_count_ == 0;
}
int ProductBatch::ProductCount() {
    return box_count_ * count_at_box_;
}

void ProductBatch::Move(int x, int y) {
    x0_ = x;
    y0_ = y;
    texture_.setPosition(sf::Vector2f(x0_, y0_));
}
int ProductBatch::GetVisualizationType() {
    return 1;
}
IClickable* ProductBatch::Click(int x, int y) {
    if (x0_ <= x && x <= x0_ + width && y0_ <= y && y <= y0_ + height)
        return static_cast<IClickable*>(this);
    return nullptr;
}
void ProductBatch::draw(sf::RenderWindow& window) {
    window.draw(texture_);
}
void ProductBatch::DrawInformation(sf::RenderWindow& window, int x0, int y0) {
    sf::CircleShape circle(10, 50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(x0, y0);
    window.draw(circle);
}

// StorageRoom
StorageRoom::StorageRoom(int product_type, int x0, int y0, sf::Font& font) :
    IClickable(x0, y0), product_type_(product_type) {
    texture_.setPosition(x0, y0);
    texture_.setFillColor(sf::Color::White);
    texture_.setSize(sf::Vector2f(width_, height_));
    texture_.setOutlineThickness(5);
    texture_.setOutlineColor(sf::Color::Black);

    back_text_.setFont(font);
    back_text_.setString(L"0");
    back_text_.setCharacterSize(24);
    back_text_.setPosition(x0 + 10, y0 + 10);
    back_text_.setFillColor(sf::Color(0, 255, 0, 180));
}
StorageRoom::~StorageRoom() {
    while (!batches_.empty()) {
        delete batches_.back();
        batches_.pop_back();
    }
}

void StorageRoom::AddDelivery(ProductBatch* batch) {
    batch->Move(CalculateXForBatch(), CalculateYForBatch());
    batches_.push_back(batch);
}
void StorageRoom::ProductShipments(int products_count) {
    while (!batches_.empty() && products_count > 0) {
        int count = batches_[0]->CalculateSellingProducts(products_count);
        if (batches_[0]->Sell(count)) {
            delete batches_[0];
            batches_.pop_front();
        }
        products_count -= count;
    }
    for (int i = 0; i < batches_.size(); i++) {
        batches_[i]->Move(CalculateXForBatch(i), CalculateYForBatch(i));
    }
}
int StorageRoom::RequestPrice(int products_count) {
    int price = 0;
    for (int i = 0; products_count > 0 && i < batches_.size(); i++) {
        price += batches_[i]->RequestPrice(products_count);
        products_count -= batches_[i]->CalculateSellingProducts(products_count);
    }
    return price;
}
int StorageRoom::CalculateXForBatch(int ind) {
    return x0_ + step_ + (15 + step_) * 
        ((ind == -1 ? batches_.size() : ind) % in_line_);
}
int StorageRoom::CalculateYForBatch(int ind) {
    return y0_ + step_ + (15 + step_) * 
        ((ind == -1 ? batches_.size() : ind) / in_line_);
}

void StorageRoom::Move(int x, int y) {
    x0_ = x;
    y0_ = y;
    texture_.setPosition(x0_, y0_);
}
int StorageRoom::GetVisualizationType() {
    return 0;
}
IClickable* StorageRoom::Click(int x, int y) {
    for (auto batch : batches_) {
        IClickable* result = batch->Click(x, y);
        if (result != nullptr)
            return result;
    }
    if (x0_ <= x && x <= x0_ + width_ && y0_ <= y && y <= y0_ + height_)
        return static_cast<IClickable*>(this);
    return nullptr;
}
void StorageRoom::draw(sf::RenderWindow& window) {
    window.draw(texture_);
    window.draw(back_text_);
    for (auto product_batch : batches_)
        product_batch->draw(window);
}
void StorageRoom::DrawInformation(sf::RenderWindow& window, int x0, int y0) {
    sf::CircleShape circle(10, 50);
    circle.setFillColor(sf::Color::Black);
    circle.setPosition(x0, y0);
    window.draw(circle);
}

// Storage
Storage::Storage(ModelData* data, int x0, int y0, sf::Font& font) :
    IClickable(x0, y0) {
    int nomb = 0;
    for (int product_type = 0; product_type < 17; product_type++) {
        if (data->IsBeingProductUsed(product_type)) {
            rooms_[product_type] = new StorageRoom(product_type,
                x0 + step_ + (step_ + 150) * (nomb % in_line_),
                y0_ + step_ + (step_ + 90) * (nomb / in_line_), font);
            if (data->GetCountProduct(product_type) != 0) {
                rooms_[product_type]->AddDelivery(new ProductBatch(product_type,
                    GetProductPrice(product_type), 0,
                    GetCountAtBox(product_type), data->GetCountProduct(product_type),
                    0, 0));
            }
            nomb++;
        } else {
            rooms_[product_type] = nullptr;
        }
    }
    texture_.setPosition(x0, y0);
    texture_.setFillColor(sf::Color::White);
    texture_.setSize(sf::Vector2f(width_, height_));
    texture_.setOutlineThickness(3);
    texture_.setOutlineColor(sf::Color::Black);
}
Storage::~Storage() {
    for (int i = 0; i < 17; i++) {
        delete rooms_[i];
    }
}

void Storage::AddDelivery(ProductBatch* batch) {
    rooms_[batch->GetProductType()]->AddDelivery(batch);
}
int Storage::RequestPrice(int product_type, int products_count) {
    return rooms_[product_type]->RequestPrice(products_count);
}
void Storage::ProductShipments(int product_type, int products_count) {
    rooms_[product_type]->ProductShipments(products_count);
}

void Storage::draw(sf::RenderWindow& window) {
    window.draw(texture_);
    for (int i = 0; i < 17; i++)
        if (rooms_[i] != nullptr)
            rooms_[i]->draw(window);
}
void Storage::Move(int x, int y) {
    x0_ = x;
    y0_ = y;
    texture_.setPosition(x, y);
}
int Storage::GetVisualizationType() {
    return 0;
}
IClickable* Storage::Click(int x, int y) {
    for (int i = 0; i < 17; i++) {
        if (rooms_[i] == nullptr)
            continue;
        IClickable* result = rooms_[i]->Click(x, y);
        if (result != nullptr)
            return result;
    }
    if (x0_ <= x && x <= x0_ + width_ && y0_ <= y && y <= y0_ + height_)
        return static_cast<IClickable*>(this);
    return nullptr;
}
void Storage::DrawInformation(sf::RenderWindow& window, int x0, int y0) {
    sf::CircleShape circle(10, 50);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(x0, y0);
    window.draw(circle);
}

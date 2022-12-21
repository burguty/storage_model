#include "Products.h"

// IProduct
IProduct::IProduct(int product_type, int price) :
    product_type_(product_type), price_(price) {
    remains_ = GetShelfLife(product_type);
    if (remains_ <= 2) {
        if (remains_ <= 0) {
            back_color_ = sf::Color(0, 0, 0, 120);
        } else {
            back_color_ = sf::Color(255, 0, 0, 120);
        }
    } else {
        back_color_ = sf::Color(0, 255, 0, 120);
    }
}
IProduct::~IProduct() {}
int IProduct::Price() {
    return price_;
}
int IProduct::Remains() {
    return remains_;
}
void IProduct::ChangePrice(int new_price) {
    price_ = new_price;
}
int IProduct::ProductType() {
    return product_type_;
}
bool IProduct::IsOverdue() {
    return remains_ <= 0;
}
void IProduct::GoToTheNextDay() {
    remains_--;
    if (remains_ <= 2) {
        if (remains_ <= 0) {
            back_color_ = sf::Color(0, 0, 0, 120);
        } else {
            back_color_ = sf::Color(255, 0, 0, 120);
        }
    } else {
        back_color_ = sf::Color(0, 255, 0, 120);
    }
}
void IProduct::SetColor(sf::Color color) {
    back_color_ = color;
}

// ProductBatch
ProductBatch::ProductBatch(int product_type, int price, int purchase_price,
    int count_at_box, int box_count, int x0, int y0) :
    IProduct(product_type, price), IDrawable(x0, y0),
    count_at_box_(count_at_box), box_count_(box_count), purchase_price_(purchase_price) {
    texture_.setOutlineThickness(3);
    texture_.setOutlineColor(sf::Color(0, 0, 0, 150));
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
    return CalculateRequiredBox(products_count) * count_at_box_ * Price();
}
int ProductBatch::CalculateSellingProducts(int products_count) {
    return count_at_box_ * CalculateRequiredBox(products_count);
}
bool ProductBatch::Sell(int products_count) {
    int box_count = CalculateRequiredBox(products_count);
    box_count_ -= box_count;
    return box_count_ == 0;
}
int ProductBatch::ProductsCount() {
    return box_count_ * count_at_box_;
}
int ProductBatch::PurchasePrice() {
    return purchase_price_;
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
    texture_.setPosition(x0_, y0_);
    texture_.setFillColor(back_color_);
    window.draw(texture_);
}
void ProductBatch::DrawInformation(sf::RenderWindow& window, int x0, int y0, sf::Font& font) {
    sf::Text name(L"Партия товара \"" + GetProductName(product_type_) + L"\"" +
        L"\nКоличество в партии - " + IntToString(count_at_box_ * box_count_) + L" шт." +
        L"\nСрок годности - " + IntToString(Remains()) + L" дней" +
        L"\nЦена за шт. - " + IntToString(Price()) + L" руб." +
        L"\nЗакупочная цена за шт. - " + IntToString(purchase_price_) + L" руб." +
        L"\nСтоимость партии - " + IntToString(Price() * count_at_box_ * box_count_) + L" руб.",
        font, 24);
    name.setPosition(x0 + 5, y0 + 5);
    name.setFillColor(sf::Color::Black);
    window.draw(name);
}
void ProductBatch::Move(int x, int y) {
    x0_ = x, y0_ = y;
}

// StorageRoom
StorageRoom::StorageRoom(int product_type, int x0, int y0, sf::Font& font) :
    IDrawable(x0, y0), product_type_(product_type) {
    texture_.setPosition(x0, y0);
    texture_.setFillColor(sf::Color::White);
    texture_.setSize(sf::Vector2f(width_, height_));
    texture_.setOutlineThickness(5);
    texture_.setOutlineColor(sf::Color::Black);

    back_text_.setFont(font);
    back_text_.setString(GetProductName(product_type));
    back_text_.setStyle(sf::Text::Bold);
    back_text_.setCharacterSize(26);
    back_text_.setPosition(x0 + 10, y0 + 10);
    back_text_.setFillColor(sf::Color(0, 191, 255, 255));
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
std::vector<ProductBatch*> StorageRoom::ProductShipments(int box_count) {
    int products_count = GetCountAtBox(product_type_) * box_count;
    std::vector<ProductBatch*>result;
    while (!batches_.empty() && products_count > 0) {
        int count = batches_[0]->CalculateSellingProducts(products_count);
        if (batches_[0]->Sell(count)) {
            result.push_back(batches_[0]);
            batches_.pop_front();
        } else {
            result.push_back(new ProductBatch(product_type_,
                batches_[0]->Price(), batches_[0]->PurchasePrice(),
                GetCountAtBox(product_type_), 
                count / GetCountAtBox(product_type_), x0_, y0_));
        }
        products_count -= count;
    }
    for (int i = 0; i < batches_.size(); i++) {
        batches_[i]->Move(CalculateXForBatch(i), CalculateYForBatch(i));
    }
    return result;
}
std::vector<ProductBatch*> StorageRoom::Clearing() {
    std::vector<ProductBatch*> trash;
    while (!batches_.empty() && batches_[0]->IsOverdue()) {
        profit_ -= batches_[0]->PurchasePrice() * batches_[0]->ProductsCount();
        trash.push_back(batches_[0]);
        batches_.pop_back();
    }
    return trash;
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
int StorageRoom::ProductsCount() {
    int result = 0;
    for (ProductBatch* batch : batches_) {
        result += batch->ProductsCount();
    }
    return result;
}
int StorageRoom::ProductsPrice() {
    int result = 0;
    for (ProductBatch* batch : batches_) {
        result += batch->Price() * batch->ProductsCount();
    }
    return result;
}
int StorageRoom::ProductsPurchasePrice() {
    int result = 0;
    for (ProductBatch* batch : batches_) {
        result += batch->PurchasePrice();
    }
    return result;
}
int StorageRoom::SpentOnPurchase() {
    int result = 0;
    for (ProductBatch* batch : batches_) {
        result += batch->PurchasePrice() * batch->ProductsCount();
    }
    return result;
}
int StorageRoom::Profit() {
    return profit_;
}
void StorageRoom::GoToTheNextDay() {
    for (ProductBatch* batch : batches_)
        batch->GoToTheNextDay();
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
void StorageRoom::DrawInformation(sf::RenderWindow& window, int x0, int y0, sf::Font& font) {
    sf::Text name(L"Склад товара \"" + GetProductName(product_type_) + L"\"" +
        L"\nКоличество партий - " + IntToString(batches_.size()) + L" шт." +
        L"\nКоличество товаров - " + IntToString(ProductsCount()) + L" шт." +
        L"\nОбщая цена товаров - " + IntToString(ProductsPrice()) + L" руб." +
        L"\nПрибыль от проданных товаров - " + IntToString(profit_) + L" руб." +
        L"\nЧистая прибыль - " + IntToString(profit_ - SpentOnPurchase()) + L" руб.",
        font, 24);
    name.setPosition(x0 + 5, y0 + 5);
    name.setFillColor(sf::Color::Black);
    window.draw(name);
}

// Storage
Storage::Storage(ModelData* data, int x0, int y0, sf::Font& font) :
    IDrawable(x0, y0) {
    int nomb = 0;
    for (int product_type = 0; product_type < 17; product_type++) {
        if (data->IsBeingProductUsed(product_type)) {
            rooms_[product_type] = new StorageRoom(product_type,
                x0 + step_ + (step_ + 150) * (nomb % in_line_),
                y0_ + step_ + (step_ + 90) * (nomb / in_line_), font);
            if (data->GetCountProduct(product_type) != 0) {
                rooms_[product_type]->AddDelivery(new ProductBatch(product_type,
                    GetProductPrice(product_type), GetProductPrice(product_type) / 2,
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

std::vector<ProductBatch*> Storage::Clearing() {
    std::vector<ProductBatch*> trash;
    for (int i = 0; i < 17; i++) {
        if (rooms_[i] != nullptr) {
            std::vector<ProductBatch*> trash_i = rooms_[i]->Clearing();
            trash.insert(trash.end(), trash_i.begin(), trash_i.end());
        }
    }
    return trash;
}
void Storage::AddDelivery(ProductBatch* batch) {
    rooms_[batch->ProductType()]->AddDelivery(batch);
}
int Storage::RequestPrice(int product_type, int products_count) {
    return rooms_[product_type]->RequestPrice(products_count);
}
std::vector<ProductBatch*> Storage::ProductShipments(int product_type, int box_count) {
    return rooms_[product_type]->ProductShipments(box_count);
}
int Storage::Profit() {
    int profit = 0;
    for (int i = 0; i < 17; i++)
        if (rooms_[i] != nullptr)
            profit += rooms_[i]->Profit();
    return profit;
}
int Storage::Price() {
    int price = 0;
    for (int i = 0; i < 17; i++)
        if (rooms_[i] != nullptr)
            price += rooms_[i]->ProductsPrice();
    return price;
}
int Storage::SpentOnPurchase() {
    int purchase = 0;
    for (int i = 0; i < 17; i++)
        if (rooms_[i] != nullptr)
            purchase += rooms_[i]->SpentOnPurchase();
    return purchase;
}
void Storage::GoToTheNextDay() {
    for (int i = 0; i < 17; i++)
        if (rooms_[i] != nullptr)
            rooms_[i]->GoToTheNextDay();
}

void Storage::draw(sf::RenderWindow& window) {
    window.draw(texture_);
    for (int i = 0; i < 17; i++)
        if (rooms_[i] != nullptr)
            rooms_[i]->draw(window);
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
void Storage::DrawInformation(sf::RenderWindow& window, int x0, int y0, sf::Font& font) {
    sf::Text name(L"Склад\nОбщая цена товаров - " + IntToString(Price()) + L" руб." +
        L"\nПрибыль от проданных товаров - " + IntToString(Profit()) + L" руб." +
        L"\nЧистая прибыль - " + IntToString(Profit() - SpentOnPurchase()) + L" руб.",
        font, 24);
    name.setPosition(x0 + 5, y0 + 5);
    name.setFillColor(sf::Color::Black);
    window.draw(name);
}
bool Storage::IsProductUsing(int product_type) {
    return rooms_[product_type] != nullptr;
}
int Storage::ProductsCount(int product_type) {
    return rooms_[product_type]->ProductsCount();
}
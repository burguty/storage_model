#include "Products.h"

// IProduct
IProduct::IProduct(int product_type, int price) :
    product_type_(product_type), price_(price) {
    remains_ = GetShelfLife(product_type);
    CalculateColor();
}
IProduct::IProduct(int product_type, int price, int days) :
    product_type_(product_type), price_(price), remains_(days) {}
IProduct::~IProduct() {}
void IProduct::SetRemains() {
    remains_ = GetShelfLife(product_type_);
    object_type_ = 1;
    CalculateColor();
}
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
    CalculateColor();
}
void IProduct::SetColor(sf::Color color) {
    back_color_ = color;
}
void IProduct::CalculateColor() {
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
int IProduct::GetRemains() {
    return remains_;
}

// ProductBatch
ProductBatch::ProductBatch(int product_type, int price, int purchase_price, int box_count, int x0, int y0, sf::Font& font) :
    IProduct(product_type, price), IDrawable(x0, y0),
    count_at_box_(GetCountAtBox(product_type)), box_count_(box_count), 
    purchase_price_(purchase_price), font_(font) {
    texture_.setOutlineThickness(3);
    texture_.setOutlineColor(sf::Color(0, 0, 0, 150));
    texture_.setSize(sf::Vector2f(width, height));
    object_type_ = 1;
}
ProductBatch::ProductBatch(int product_type, int price, int purchase_price, int box_count, int days, int x0, int y0, sf::Font& font) :
    IProduct(product_type, price, days), IDrawable(x0, y0),
    count_at_box_(GetCountAtBox(product_type)), box_count_(box_count),
    purchase_price_(purchase_price), font_(font) {
    texture_.setOutlineThickness(3);
    texture_.setOutlineColor(sf::Color(0, 0, 0, 150));
    texture_.setSize(sf::Vector2f(width, height));
    object_type_ = 0;
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
bool ProductBatch::IsItPossibleToSell(int products_count) {
    int box_count = CalculateRequiredBox(products_count);
    return box_count_ == box_count;
}
void ProductBatch::Sell(int products_count) {
    int box_count = CalculateRequiredBox(products_count);
    box_count_ -= box_count;
}
int ProductBatch::ProductsCount() {
    return box_count_ * count_at_box_;
}
int ProductBatch::BoxCount() {
    return box_count_;
}
int ProductBatch::PurchasePrice() {
    return purchase_price_;
}
int ProductBatch::GetVisualizationType() {
    return object_type_;
}
IClickable* ProductBatch::Click(int x, int y) {
    if (x0_ <= x && x <= x0_ + width && y0_ <= y && y <= y0_ + height)
        return static_cast<IClickable*>(this);
    return nullptr;
}
void ProductBatch::draw(sf::RenderWindow& window) {
    if (object_type_ == 1) {
        texture_.setPosition(x0_, y0_);
        texture_.setFillColor(back_color_);
        window.draw(texture_);
        
    } else {
        texture_.setPosition(x0_, y0_);
        texture_.setFillColor(sf::Color(30, 30, 30, 120));
        window.draw(texture_);
        sf::Text date(IntToString(GetRemains()), font_, 16);
        date.setPosition(x0_ + 3, y0_ - 4);
        date.setFillColor(sf::Color(0, 191, 255, 255));
        date.setStyle(sf::Text::Bold);
        window.draw(date);
    }
}
void ProductBatch::DrawInformation(sf::RenderWindow& window, int x0, int y0, sf::Font& font) {
    if (object_type_ == 1) {
        sf::Text information(L"Партия товара \"" + GetProductName(product_type_) + L"\"" +
            L"\nКоличество в оптовых упаковок - " + IntToString(box_count_) + L" шт." +
            L"\nСрок годности - " + IntToString(Remains()) + L" дней" +
            L"\nЦена - " + IntToString(Price()) + L" руб/шт" +
            L"\nЗакупочная цена - " + IntToString(purchase_price_) + L" руб/шт" +
            L"\nСтоимость партии - " + IntToString(Price() * count_at_box_ * box_count_) + L" руб.",
            font, 24);
        information.setPosition(x0 + 5, y0 + 5);
        information.setFillColor(sf::Color::Black);
        window.draw(information);
    } else {
        sf::Text information(L"Поставка товара \"" + GetProductName(product_type_) + L"\"" +
            L"\nКоличество в партии - " + IntToString(box_count_) + L" шт." +
            L"\n Дней до доставки - " + IntToString(Remains()) +
            L"\nЗакупочная цена - " + IntToString(purchase_price_) + L" руб/шт" +
            L"\nСтоимость партии - " + IntToString(Price() * count_at_box_ * box_count_) + L" руб.",
            font, 24);
        information.setPosition(x0 + 5, y0 + 5);
        information.setFillColor(sf::Color::Black);
        window.draw(information);
    }
}
void ProductBatch::Move(int x, int y) {
    x0_ = x, y0_ = y;
}

// StorageRoom
StorageRoom::StorageRoom(int product_type, int x0, int y0, sf::Font& font) :
    IDrawable(x0, y0), product_type_(product_type),
    count_at_box_(GetCountAtBox(product_type)),
    max_box_count_(GetMaxBoxCount(product_type)),
    font_(font) {
    texture_.setPosition(x0, y0);
    texture_.setFillColor(sf::Color::White);
    texture_.setSize(sf::Vector2f(width_, height_));
    texture_.setOutlineThickness(5);
    texture_.setOutlineColor(sf::Color::Black);

    back_text_.setFont(font);
    back_text_.setString(GetProductName(product_type));
    back_text_.setStyle(sf::Text::Bold);
    back_text_.setCharacterSize(26);
    back_text_.setPosition(x0 + 10, y0 + 5);
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
    expenses_ += batch->PurchasePrice() * batch->ProductsCount();
    batches_.push_back(batch);
}
std::vector<ProductBatch*> StorageRoom::ProductShipments(int box_count) {
    int products_count = GetCountAtBox(product_type_) * box_count;
    std::vector<ProductBatch*>result;
    while (!batches_.empty() && products_count > 0) {
        int count = batches_[0]->CalculateSellingProducts(products_count);
        if (batches_[0]->IsItPossibleToSell(count)) {
            result.push_back(batches_[0]);
            batches_.pop_front();
        } else {
            result.push_back(new ProductBatch(product_type_, batches_[0]->Price(),
                batches_[0]->PurchasePrice(),
                count, x0_, y0_, font_));
            batches_[0]->Sell(count);
        }
        income_ += result.back()->Price() * count;
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
int StorageRoom::CalculateXForPurchase(int ind) {
    return x0_ + step_ + (15 + step_) *
        ((ind == -1 ? purchases_.size() : ind) % in_line_);
}
int StorageRoom::CalculateYForPurchase(int ind) {
    return y0_ + 90 - step_ - 15 + (15 + step_) *
        ((ind == -1 ? purchases_.size() : ind) / in_line_);
}
int StorageRoom::ProductType() {
    return product_type_;
}
int StorageRoom::ProductsCount() {
    int result = 0;
    for (ProductBatch* batch : batches_) {
        result += batch->ProductsCount();
    }
    for (ProductBatch* batch : purchases_) {
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
    return income_ - expenses_;
}
int StorageRoom::Income() {
    return income_;
}
int StorageRoom::BoxCount() {
    int result = 0;
    for (ProductBatch* batch : batches_)
        result += batch->BoxCount();
    for (ProductBatch* batch : purchases_)
        result += batch->BoxCount();
    return result;
}
void StorageRoom::GoToTheNextDay() {
    for (ProductBatch* batch : batches_)
        batch->GoToTheNextDay();
}
void StorageRoom::StartPurchasePhase(int purchase_price, int time_of_purchase) {
    mode_ = 1;
    order_count_ = 0;
    purchase_price_ = purchase_price;
    time_of_purchase_ = time_of_purchase;
}
void StorageRoom::StopPurchasePhase() {
    if (order_count_ > 0) {
        purchases_.push_back(new ProductBatch(product_type_, GetProductPrice(product_type_), purchase_price_, 
            order_count_, time_of_purchase_, CalculateXForPurchase(), CalculateYForPurchase(), font_));
    }
    mode_ = 0;
}
bool StorageRoom::IsOrderCountCorrect() {
    return max_box_count_ >= BoxCount() + order_count_;
}
bool StorageRoom::IsOrderCountCorrect(int order_count) {
    return max_box_count_ >= BoxCount() + order_count;
}
int StorageRoom::GetOrderCount() {
    return order_count_;
}
void StorageRoom::SetOrderCount(int count) {
    order_count_ = count;
}
std::deque<IMovable*> StorageRoom::GoToTheNextDayCars(){
    auto it = purchases_.begin();
    std::deque<IMovable*> result;
    while(it != purchases_.end()){
        (*it)->GoToTheNextDay();
        if ((*it)->IsOverdue()) {
            (*it)->StartMoving(CalculateXForBatch(), CalculateYForBatch(), 1);
            batches_.push_back(*it);
            batches_.back()->SetRemains();
            result.push_back(static_cast<IMovable*>(batches_.back()));
            it = purchases_.erase(it);
        } else {
            it++;
        }
    }
    for (int i = 0; i < purchases_.size(); i++) {
        purchases_[i]->Move(CalculateXForPurchase(i), CalculateYForPurchase(i));
    }
    return result;
}

int StorageRoom::GetVisualizationType() {
    if (mode_ == 0) {
        return 0;
    }
    return 3;
}
IClickable* StorageRoom::Click(int x, int y) {
    if (mode_ == 0) {
        for (auto batch : batches_) {
            IClickable* result = batch->Click(x, y);
            if (result != nullptr)
                return result;
        }
    }
    if (x0_ <= x && x <= x0_ + width_ && y0_ <= y && y <= y0_ + height_)
        return static_cast<IClickable*>(this);
    return nullptr;
}
void StorageRoom::draw(sf::RenderWindow& window) {
    window.draw(texture_);
    window.draw(back_text_);
    if (mode_ == 0) {
        for (auto product_batch : batches_)
            product_batch->draw(window);
        for (auto purchase : purchases_)
            purchase->draw(window);
    } else {
        sf::Text box_count(L"Заказано " + IntToString(order_count_), *back_text_.getFont(), 26);
        box_count.setStyle(sf::Text::Bold);
        box_count.setPosition(x0_ + 10, y0_ + 5 + 26 + 5);
        box_count.setFillColor(IsOrderCountCorrect() ?
            sf::Color(0, 141, 205, 255) : sf::Color(185, 41, 105));
        box_count.setCharacterSize(20);
        window.draw(box_count);
    }
}
void StorageRoom::DrawInformation(sf::RenderWindow& window, int x0, int y0, sf::Font& font) {
    sf::Text information;
    if (mode_ == 0) {
        information.setString(L"Склад товара \"" + GetProductName(product_type_) + L"\"" +
            L"\nКоличество партий - " + IntToString(batches_.size()) + L" шт." +
            L"\nКоличество товаров - " + IntToString(ProductsCount()) + L" шт." +
            L"\nОбщая цена товаров - " + IntToString(ProductsPrice()) + L" руб." +
            L"\nДоход - " + IntToString(income_) + L" руб." +
            L"\nЧистая прибыль - " + IntToString(income_ - expenses_) + L" руб."
            );
        information.setCharacterSize(24);
        information.setFillColor(sf::Color::Black);
    } else {
        int box_count_in_room = BoxCount();
        if (max_box_count_ - box_count_in_room < order_count_) {
            information.setString(GetProductName(product_type_) +
                L"\nДоступно для заказа - " + IntToString(max_box_count_ - box_count_in_room) + L" шт."
            );
            information.setFillColor(sf::Color::Red);
            information.setCharacterSize(24);
        } else {
            information.setString(GetProductName(product_type_) +
                L"\nОптовых упаковок на складе - " + IntToString(box_count_in_room) + L" шт." + 
                L"\nДоступно для заказа - " + IntToString(max_box_count_ - box_count_in_room) + L" шт." +
                L"\nЗаполнено с учетом заказа на " + IntToString((box_count_in_room + order_count_) * 100 / max_box_count_) + L"%" +
                L"\nСтоимость заказа - " + IntToString(order_count_ * count_at_box_ * purchase_price_) + L" руб."
            );
            information.setFillColor(sf::Color::Black);
            information.setCharacterSize(20);
        }
    }
    information.setFont(font);
    information.setPosition(x0 + 5, y0 + 5);
    window.draw(information);
}

// Storage
Storage::Storage(ModelData* data, int x0, int y0, sf::Font& font) :
    IDrawable(x0, y0) {
    int nomb = 0;
    for (int product_type = 0; product_type < 15; product_type++) {
        if (data->IsBeingProductUsed(product_type)) {
            rooms_[product_type] = new StorageRoom(product_type,
                x0 + step_ + (step_ + 150) * (nomb % in_line_),
                y0_ + step_ + (step_ + 90) * (nomb / in_line_), font);
            if (data->GetCountProduct(product_type) != 0) {
                rooms_[product_type]->AddDelivery(new ProductBatch(product_type,
                    GetProductPrice(product_type),
                    GetProductPrice(product_type) / 2,
                    data->GetCountProduct(product_type),
                    0, 0, font));
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
    for (int i = 0; i < 15; i++) {
        delete rooms_[i];
    }
}

std::vector<ProductBatch*> Storage::Clearing() {
    std::vector<ProductBatch*> trash;
    for (int i = 0; i < 15; i++) {
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
    for (int i = 0; i < 15; i++)
        if (rooms_[i] != nullptr)
            profit += rooms_[i]->Profit();
    return profit;
}
int Storage::Income() {
    int income = 0;
    for (int i = 0; i < 15; i++)
        if (rooms_[i] != nullptr)
            income += rooms_[i]->Income();
    return income;
}
int Storage::Price() {
    int price = 0;
    for (int i = 0; i < 15; i++)
        if (rooms_[i] != nullptr)
            price += rooms_[i]->ProductsPrice();
    return price;
}
int Storage::SpentOnPurchase() {
    int purchase = 0;
    for (int i = 0; i < 15; i++)
        if (rooms_[i] != nullptr)
            purchase += rooms_[i]->SpentOnPurchase();
    return purchase;
}
void Storage::GoToTheNextDay() {
    for (int i = 0; i < 15; i++)
        if (rooms_[i] != nullptr)
            rooms_[i]->GoToTheNextDay();
}
void Storage::draw(sf::RenderWindow& window) {
    window.draw(texture_);
    for (int i = 0; i < 15; i++)
        if (rooms_[i] != nullptr)
            rooms_[i]->draw(window);
}
int Storage::GetVisualizationType() {
    return 0;
}
IClickable* Storage::Click(int x, int y) {
    for (int i = 0; i < 15; i++) {
        if (rooms_[i] == nullptr)
            continue;
        IClickable* result = rooms_[i]->Click(x, y);
        if (result != nullptr)
            return result;
    }
    if (mode_ == 0 && x0_ <= x && x <= x0_ + width_ && y0_ <= y && y <= y0_ + height_)
        return static_cast<IClickable*>(this);
    return nullptr;
}
void Storage::DrawInformation(sf::RenderWindow& window, int x0, int y0, sf::Font& font) {
    sf::Text information(L"Склад\nОбщая цена товаров - " + IntToString(Price()) + L" руб." +
        L"\nДоход - " + IntToString(Income()) + L" руб." +
        L"\nЧистая прибыль - " + IntToString(Profit()) + L" руб.",
        font, 24);
    information.setPosition(x0 + 5, y0 + 5);
    information.setFillColor(sf::Color::Black);
    window.draw(information);
}
bool Storage::IsProductUsing(int product_type) {
    return rooms_[product_type] != nullptr;
}
int Storage::ProductsCount(int product_type) {
    return rooms_[product_type]->ProductsCount();
}
void Storage::StartPurchasePhase(int time_of_purchase) {
    mode_ = 1;
    for (int i = 0; i < 15; i++) {
        if (rooms_[i] != nullptr)
            rooms_[i]->StartPurchasePhase(GetPurchasePrice(i), time_of_purchase);
    }
}
void Storage::StopPurchasePhase() {
    mode_ = 0;
    for (int i = 0; i < 15; i++) {
        if (rooms_[i] != nullptr)
            rooms_[i]->StopPurchasePhase();
    }
}
bool Storage::IsOrderCountsCorrect() {
    for (int i = 0; i < 15; i++) {
        if (rooms_[i] != nullptr)
            if (!rooms_[i]->IsOrderCountCorrect())
                return false;
    }
    return true;
}
std::deque<IMovable*> Storage::GoToTheNextDayCars() {
    std::deque<IMovable*> result;
    for (int i = 0; i < 15; i++) {
        if (rooms_[i] != nullptr) {
            std::deque<IMovable*> result_i = rooms_[i]->GoToTheNextDayCars();
            result.insert(result.end(), result_i.begin(), result_i.end());
        }
    }
    return result;
}

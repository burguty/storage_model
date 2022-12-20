#include "Shop.h"

sf::Vector2i const gaps[17] = {
    {500, 1000}, {2000, 5000}, {10000, 15000},
    {1000, 30000}, {5000, 90000}, {2000, 12000},
    {2000, 3000}, {4000, 100000}, {3000, 7000},
    {4000, 5000}, {1000, 20000}, {1000, 30000},
    {8000, 20000}, {10000, 100000}, {5000, 10000},
    {30000, 50000}, {30000, 50000}
};

Request::Request(int product_type, int product_count, Shop* customer) : 
    IClickable(), IDrawable(-1, -1), 
    product_type_(product_type), product_count_(product_count), 
    customer_(customer), name_(L"Пятёрочка") {}
void Request::draw(sf::RenderWindow& window) {
    return;
}
void Request::DrawInformation(sf::RenderWindow& window, int x0, int y0, sf::Font& font) {
    sf::Text text(L"Заказа от магазина " + name_ +
        L"\nЗаказываемый товар - " + GetProductName(product_type_) +
        L"\nНеобходимое количество - " + IntToString(product_count_ / GetCountAtBox(product_type_)) + L"\nопт. упаковок"
        L"\n---" +
        L"\nПрибыль от продажи - " + IntToString(profit), font, 24);
    text.setPosition(x0 + 5, y0 + 5);
    text.setFillColor(sf::Color::Black);
    window.draw(text);
}
int Request::GetVisualizationType() {
    return 2;
}
IClickable* Request::Click(int x, int y) { 
    return nullptr;
}
int Request::GetProductType() {
    return product_type_;
}
void Request::RecalculateProfitAndApprovedCount(Storage* storage, int approved_count) {
    approved_count_ = std::max(0,
        std::min(storage->ProductsCount(product_type_) / GetCountAtBox(product_type_),
            approved_count));
    profit = storage->RequestPrice(product_type_, 
        approved_count_ * GetCountAtBox(product_type_));
}
void Request::RecalculateProductCount(Storage* storage) {
    product_count_ = std::min(product_count_, storage->ProductsCount(product_type_));
}

Shop::Shop(int index, sf::Font& font, std::mt19937& gen) :
    IClickable(), IDrawable(CountX0(index), CountY0(index)), gen_(gen) {
    texture_.setPosition(x0_, y0_);
    texture_.setFillColor(sf::Color::White);
    texture_.setSize(sf::Vector2f(width_, height_));
    texture_.setOutlineThickness(3);
    texture_.setOutlineColor(sf::Color::Black);
}
int Shop::CountX0(int index) {
    return (1150 + 30) / 2 - 100 / 2 + index * ((1150 - 30 - 7 * 100) / 8) + index * 100;
}
int Shop::CountY0(int index) {
    return 30 + 5 + abs(index) * 40;
}
void Shop::draw(sf::RenderWindow& window) {
    window.draw(texture_);
}
void Shop::DrawInformation(sf::RenderWindow& window,
    int x0, int y0, sf::Font& font) {
    sf::CircleShape circle(10, 50);
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition(x0, y0);
    window.draw(circle);
}
int Shop::GetVisualizationType() {
    return 0;
}
IClickable* Shop::Click(int x, int y) {
    if (x0_ <= x && x <= x0_ + width_ && y0_ <= y && y <= y0_ + height_)
        return static_cast<IClickable*>(this);
    return nullptr;
}
Request* Shop::MakeRequest(Storage* storage) {
    if (gen_() % 100 < 30) return nullptr;
    std::vector<int> x(17);
    for (int i = 0; i < 17; ++i) {
        x[i] = gen_() % (gaps[i].y - gaps[i].x) + gaps[i].x;
    }
    std::vector<std::pair<int, int>> f(17);
    for (int i = 0; i < 17; ++i) {
        if (storage->IsProductUsing(i)) {
            f[i].first = GetProductPrice(i) * std::min(x[i], storage->ProductsCount(i)); - storage->RequestPrice(i, x[i]);
        } else {
            f[i].first = -1;
        }
        f[i].second = i;
    }
    std::sort(f.begin(), f.end());
    int size_k = 0;
    for (int i = 17-1; i >= 17-5; --i) {
        if (f[i].first > 0) ++size_k;
    }
    size_k--;
    if (size_k == -1) {
        return nullptr;
    }
    if (size_k == 0) {
        return new Request(f[16].second, x[f[16].second], this);
    }

    std::vector<long double> k(size_k);
    for (int i = 0; i < size_k; ++i) {
        k[i] = static_cast<long double>(f[17 - i - 1].first) / f[17 - i - 2].first;
    }
    long double denom = 1;
    for (int i = 0; i < size_k; ++i) {
        denom *= k[i];
        denom += 1;
    }
    std::vector<long double> v(size_k + 1);
    v[size_k] = static_cast<long double>(1) / denom;
    for (int i = size_k - 1; i >= 0; --i) {
        v[i] = v[i + 1] * k[i];
    }
    long double rand = static_cast<long double>(gen_()) / ((1ll << 32) - 1);
    int index = v.size()-1;
    long double sum = 0;
    while (index >= 0) {
        if (rand < sum) {
            break;
        }
        sum += v[index];
        index--;
    }
    int type = f[17 - index - 1 - 1].second;
    return new Request(type, x[type], this);
}

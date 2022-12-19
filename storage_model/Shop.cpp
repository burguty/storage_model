#include "Shop.h"

sf::Vector2i const gaps[17] = {
    {500, 1000}, {2000, 5000}, {10000, 15000},
    {1000, 30000}, {5000, 90000}, {2000, 12000},
    {2000, 3000}, {4000, 100000}, {3000, 7000},
    {4000, 5000}, {1000, 20000}, {1000, 30000},
    {8000, 20000}, {10000, 100000}, {5000, 10000},
    {30000, 50000}
};

Request::Request(int product_type, int product_count, Shop* customer) : 
    IClickable(), IDrawable(-1, -1), 
    product_type_(product_type), product_count_(product_count), 
    customer_(customer) {}
void Request::draw(sf::RenderWindow& window) {
    return;
}
void Request::DrawInformation(sf::RenderWindow& window,
    int x0, int y0, sf::Font& font) {
    window.draw(sf::CircleShape(10));
    return;
}
int Request::GetVisualizationType() {
    return 2;
}
IClickable* Request::Click(int x, int y) { 
    return nullptr;
}

int CountX0(int index) {
    return (1150 + 30) / 2 - 100 / 2 + index * ((1150 - 30 - 7 * 100) / 8);
}

int CountY0(int index) {
    return 20 + abs(index) * 40;
}

Shop::Shop(int index, sf::Font& font) :
    IClickable(), IDrawable(CountX0(index), CountY0(index)) {
    texture_.setPosition(CountX0(index), CountY0(index));
    texture_.setFillColor(sf::Color::White);
    texture_.setSize(sf::Vector2f(width_, height_));
    texture_.setOutlineThickness(3);
    texture_.setOutlineColor(sf::Color::Black);
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
    std::mt19937 gen(4321);
    if (gen() % 100 < 30) return nullptr;
    std::vector<int> x(17);
    for (int i = 0; i < 17; ++i) {
        x[i] = gen() % (gaps[i].y - gaps[i].x) + gaps[i].x;
    }
    std::vector<std::pair<int, int>> f(17);
    for (int i = 0; i < 17; ++i) {
        f[i].first = GetProductPrice(i) * x[i] - storage->RequestPrice(i, x[i]);
        f[i].second = i;
    }
    std::sort(f.rbegin(), f.rend());
    int size_k = 0;
    for (int i = 17 - 1; i >= 17 - 5; --i) {
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
    for (int i = 0; i < size_k - 1; ++i) {
        denom *= k[i];
        denom += 1;
    }
    std::vector<long double> v(size_k + 1);
    v[size_k] = static_cast<long double>(1) / denom;
    for (int i = size_k - 1; i >= 0; --i) {
        v[i] = v[i + 1] * k[i];
    }
    long double rand = static_cast<long double>(gen()) / ((1ll << 32) - 1);
    int index = 0;
    long double sum = 0;
    while (index < size_k) {
        if (sum <= rand && rand <= sum + v[index]) {
            break;
        }
        sum += v[index];
        index++;
    }
    int type = f[17 - size_k + index - 1].second;
    return new Request(type, x[type], this);
}

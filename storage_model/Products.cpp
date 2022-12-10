#include "Products.h"

int shelf_lifes[3] = {2, 5, 4};
int GetShelfLife(int type) {
    return shelf_lifes[type];
}


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

ProductBatch::ProductBatch(int product_type, int price, int production_day, 
    int count_at_box, int box_count, int x0, int y0):
    IProduct(product_type, price, production_day), IDrawable(x0, y0), count_at_box_(count_at_box), box_count_(box_count){}
int ProductBatch::Reduction(int new_cost) {
    ChangePrice(new_cost);
}
int ProductBatch::CalculateRequiredCount(int products_count) {
    return std::min(box_count_, 
        static_cast<int>(std::round(static_cast<double>(products_count) / count_at_box_)));
}
int ProductBatch::RequestPrice(int box_count) {
    return box_count * count_at_box_ * GetPrice();
}
bool ProductBatch::Sell(int box_count) {
    box_count_ -= box_count;
    return box_count_ == 0;
}
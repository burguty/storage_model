# include "InfoField.h"

InfoField::InfoField(int x0, int y0) :x0_(x0), y0_(y0) {
    texture_.setSize(sf::Vector2f(width_, height_));
    texture_.setPosition(x0, y0);
    texture_.setOutlineThickness(5);
    texture_.setOutlineColor(sf::Color::Black);
    texture_.setFillColor(sf::Color::White);
}
void InfoField::ChangeMode(IClickable* new_object) {
    object_ = new_object;
    if (new_object != nullptr) {
        visualization_type_ = new_object->GetVisualizationType();
    }
}

void InfoField::draw(sf::RenderWindow& window, sf::Font& font) {
    window.draw(texture_);
    if (object_ != nullptr) {
        object_->DrawInformation(window, x0_, y0_, font);
        if (visualization_type_ == 1) {}
    }
}
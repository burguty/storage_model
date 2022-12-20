# include "InfoField.h"

InfoField::InfoField(Storage* storage, int x0, int y0, sf::Font& font) :
    storage_(storage), x0_(x0), y0_(y0), font_(font) {
    texture_.setSize(sf::Vector2f(width_, height_));
    texture_.setPosition(x0, y0);
    texture_.setOutlineThickness(5);
    texture_.setOutlineColor(sf::Color::Black);
    texture_.setFillColor(sf::Color::White);
    line_ = new TextLine(x0_, y0_ + height_ + 20, 
        180, 40, L"", font_, 20);
    input_line_ = new InputLine(x0_ + 200 + 20, y0_ + height_ + 20,
        80, 40, font_);
    button_ = new Button(x0_, y0_ + height_ + 20 + 40 + 20,
        120, 40, L"", font_);
}

InfoField::~InfoField() {
    delete line_;
    delete button_;
    delete input_line_;
}

bool InfoField::Click(int x, int y) {
    if (object_ != nullptr) {
        if (visualization_type_ == 1) {
            if (input_line_->Click(x, y))
                return true;
            if (button_->Click(x, y)) {
                dynamic_cast<ProductBatch*>(object_)->Reduction(
                    std::stoi(input_line_->GetText())
                );
                return true;
            }
        }
        if (visualization_type_ == 2) {
            if (input_line_->Click(x, y)) {
                return false;
            }
            if (button_->Click(x, y)) {
                return true;
            }
        }
    }
    return false;
}

void InfoField::DeleteSymbol() {
    if (object_ != nullptr) {
        if (visualization_type_ > 0) {
            input_line_->DeleteSymbol();
            if (visualization_type_ == 2) {
                dynamic_cast<Request*>(object_)->RecalculateProfitAndApprovedCount(storage_, 
                    (input_line_->GetText().empty() ? 0 : std::stoi(input_line_->GetText())));
            }
        }
    }
}

void InfoField::TypeSymbol(char c) {
    if (object_ != nullptr) {
        if (visualization_type_ > 0) {
            input_line_->TypeSymbol(c);
            if (visualization_type_ == 2) {
                dynamic_cast<Request*>(object_)->RecalculateProfitAndApprovedCount(storage_,
                    std::stoi(input_line_->GetText()));
            }
        }
    }
}

void InfoField::ChangeMode(IClickable* new_object) {
    object_ = new_object;
    if (new_object != nullptr) {
        visualization_type_ = new_object->GetVisualizationType();
        if (visualization_type_ == 1) {
            line_->SetText(L"Введите новую цену");
            input_line_->Clear();
            input_line_->SetStatus(false);
            button_->SetText(L"Готово");
        }
        if (visualization_type_ == 2) {
            line_->SetText(L"Введите кол-во опт. упаковок:");
            input_line_->Clear();
            input_line_->SetStatus(false);
            button_->SetText(L"Готово");
            dynamic_cast<Request*>(object_)->RecalculateProductCount(storage_);
        }
    }
}

void InfoField::draw(sf::RenderWindow& window) {
    window.draw(texture_);
    if (object_ != nullptr) {
        object_->DrawInformation(window, x0_, y0_, font_);
        if (visualization_type_ == 1) {
            line_->draw(window);
            input_line_->draw(window);
            button_->draw(window);
        }
        if (visualization_type_ == 2) {
            line_->draw(window);
            input_line_->draw(window);
            button_->draw(window);
        }
    }
}
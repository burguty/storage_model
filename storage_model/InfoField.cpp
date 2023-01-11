#include "InfoField.h"

InfoField::InfoField(Storage* storage, int x0, int y0, sf::Font& font) :
    storage_(storage), x0_(x0), y0_(y0), font_(font) {
    texture_.setSize(sf::Vector2f(width_, height_));
    texture_.setPosition(x0, y0);
    texture_.setOutlineThickness(5);
    texture_.setOutlineColor(sf::Color::Black);
    texture_.setFillColor(sf::Color::White);
}

InfoField::~InfoField() {
    ClearFront();
}

void InfoField::ClearFront() {
    while (!text_lines_.empty()) {
        delete text_lines_.back();
        text_lines_.pop_back();
    }
    while (!buttons_.empty()) {
        delete buttons_.back();
        buttons_.pop_back();
    }
    while (!input_lines_.empty()) {
        delete input_lines_.back();
        input_lines_.pop_back();
    }
}

bool InfoField::Click(int x, int y) {
    if (object_ != nullptr) {
        if (visualization_type_ == 1) {
            if (input_lines_[0]->Click(x, y))
                return true;
            if (buttons_[0]->Click(x, y)) {
                dynamic_cast<ProductBatch*>(object_)->Reduction(
                    std::stoi(input_lines_[0]->GetText())
                );
                return true;
            }
        }
        if (visualization_type_ == 2) {
            if (input_lines_[0]->Click(x, y)) {
                return false;
            }
            if (buttons_[0]->Click(x, y)) {
                return true;
            }
        }
        if (visualization_type_ == 3) {
            if (buttons_[0]->Click(x, y)) {
                dynamic_cast<StorageRoom*>(object_)->SetOrderCount(0);
                ChangeMode(nullptr);
                return true;
            }
            if (buttons_[1]->Click(x, y)) {
                int type = dynamic_cast<StorageRoom*>(object_)->ProductType();
                if (dynamic_cast<StorageRoom*>(object_)->BoxCount() >= GetMaxBoxCount(type) / 2) {
                    dynamic_cast<StorageRoom*>(object_)->SetOrderCount(0);
                    input_lines_[0]->SetText(L"0");
                } else {
                    int box_count = GetMaxBoxCount(type) / 2 - dynamic_cast<StorageRoom*>(object_)->BoxCount();
                    dynamic_cast<StorageRoom*>(object_)->SetOrderCount(box_count);
                    input_lines_[0]->SetText(IntToString(box_count));
                }
                return true;
            }
            if (input_lines_[0]->Click(x, y)) {
                return true;
            }
        }
    } 
    return false;
}

void InfoField::TypeSymbol(char c) {
    if (object_ != nullptr) {
        if (visualization_type_ > 0) {
            if (c == 0)
                input_lines_[0]->DeleteSymbol();
            else if (input_lines_[0]->GetText().size() <= 4)
                input_lines_[0]->TypeSymbol(c);
            if (visualization_type_ == 2) {
                std::wstring count = input_lines_[0]->GetText();
                dynamic_cast<Request*>(object_)->RecalculateProfitAndApprovedCount(
                    storage_,
                    (count.empty() ? 0 : std::stoi(count))
                );
            }
            if (visualization_type_ == 3) {
                std::wstring box_count = input_lines_[0]->GetText();
                dynamic_cast<StorageRoom*>(object_)->SetOrderCount(
                    box_count.empty() ? 0 : std::stoi(box_count)
                );
            }
        }
    }
}

void InfoField::ChangeMode(IClickable* new_object) {
    object_ = new_object;
    if (new_object != nullptr) {
        visualization_type_ = new_object->GetVisualizationType();
        ClearFront();
        if (visualization_type_ == 1) {
            text_lines_.push_back(new TextLine(x0_, y0_ + height_ + 20,
                180, 40, L"Новая цена:", font_, 20));
            input_lines_.push_back(new InputLine(x0_ + 130 + 20, y0_ + height_ + 20,
                80, 40, font_));
            buttons_.push_back(new Button(x0_, y0_ + height_ + 20 + 40 + 20,
                120, 40, L"Готово", font_));
            input_lines_[0]->Clear();
            input_lines_[0]->SetStatus(false);
        }
        if (visualization_type_ == 2) {
            text_lines_.push_back(new TextLine(x0_, y0_ + height_ + 20,
                180, 40, L"Количество оптовых упаковок:", font_, 20));
            input_lines_.push_back(new InputLine(x0_ + 300 + 20, y0_ + height_ + 20,
                80, 40, font_));
            buttons_.push_back(new Button(x0_, y0_ + height_ + 20 + 40 + 20,
                120, 40, L"Готово", font_));
            input_lines_[0]->Clear();
            input_lines_[0]->SetStatus(false);
            dynamic_cast<Request*>(object_)->RecalculateProductCount(storage_);
        }
        if (visualization_type_ == 3) {
            text_lines_.push_back(new TextLine(x0_, y0_ + height_ + 20,
                180, 40, L"Заказать:", font_, 20));

            input_lines_.push_back(new InputLine(x0_ + 130 + 20, y0_ + height_ + 20,
                80, 40, font_));
            input_lines_[0]->SetText(
                IntToString(dynamic_cast<StorageRoom*>(object_)->GetOrderCount())
            );

            buttons_.push_back(new Button(x0_, y0_ + height_ + 20 + 40 + 20,
                190, 40, L"Не заказывать", font_));
            buttons_.push_back(new Button(x0_, y0_ + height_ + 20 + 40 + 20 + 40 + 20,
                290, 40, L"Оптимальное значение", font_));
        }
    }
}

void InfoField::draw(sf::RenderWindow& window) {
    window.draw(texture_);
    if (object_ != nullptr) {
        object_->DrawInformation(window, x0_, y0_, font_);
        for (TextLine* line : text_lines_)
            line->draw(window);
        for (InputLine* input_line : input_lines_)
            input_line->draw(window);
        for (Button* button : buttons_)
            button->draw(window);
    }
}
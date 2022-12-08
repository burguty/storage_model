#pragma once
#include <iostream>
#include <string>

class ModelData {
public:
    // �����������
    // ����������
    // ������ ����������
    bool IsBeingProductUsed(int product_type);
private:
    // ��������� ������
    // ������ ������������ �������
    bool empty_;
};

class InputWindow {
public:
    InputWindow();
    ~InputWindow();
    ModelData* MainLoop();
private:
    int GetNumber(std::string s); // ������ ����������
};
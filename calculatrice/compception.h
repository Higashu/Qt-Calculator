#pragma once

#include <string> //A retier sur Qt
//#include<QString>

class CompCeption
{
private:
    std::string message;

public:
    CompCeption(std::string m) : message(m) {}
    ~CompCeption() = default;
    std::string getMessage() const { return message; }
};
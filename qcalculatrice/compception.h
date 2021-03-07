#pragma once

#include <QString>

class CompCeption
{

private:
    QString message;

public:
    CompCeption(QString m) : message(m) {}
    ~CompCeption() = default;
    QString getMessage() const { return message; }
};

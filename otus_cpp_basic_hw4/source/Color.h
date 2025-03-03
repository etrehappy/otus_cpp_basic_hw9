#pragma once
#include <iostream>

class Color {
  public:
    Color();
    Color(double red, double green, double blue);
    double red() const;
    double green() const;
    double blue() const;
    
    friend inline std::istream& operator>>(std::istream& stream, Color& variable);

  private:
    double r{};
    double g{};
    double b{};
};

inline std::istream& operator>>(std::istream& stream, Color& variable) {
    stream >> variable.r >> variable.g >> variable.b;
    return stream;
}

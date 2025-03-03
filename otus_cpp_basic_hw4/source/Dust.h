#pragma once
#include "Point.h"
#include "Painter.h"
#include "Velocity.h"

class Dust {
  public:
    Dust(const Point& vector, const Velocity& ball_velocity, const double radius);
    ~Dust();
    Velocity getVelocity() const;
    Point getCenter() const;
    void setCenter(const Point& center);
    void draw(Painter& painter) const;
    void reduceDurationDisplay(double step);
    double getDurationDisplay() const;

  private:
    Point m_center;
    Velocity m_velocity;
    Color m_color;
    double m_durationDisplay;
    double m_radius; 

    double random_color();

};


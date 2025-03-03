#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"

class Ball {
public:
    Ball(Point& center, Point& vector, Color& color, double radius, bool isCollidable);

    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    bool isCollidable() const;


private:
    Point m_center;
    Velocity m_velocity;    
    Color m_color;
    double m_radius;
    double m_mass;
    bool m_isCollidable;
    
};

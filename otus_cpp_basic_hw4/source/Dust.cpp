#include "Dust.h"
#include <random>
#include <chrono>

Dust::Dust(const Point& vector, const Velocity& ball_velocity, const double radius) 
    : m_center(vector), m_velocity(ball_velocity),
      m_color(random_color(), random_color(), random_color()),
      m_durationDisplay(1.), m_radius(radius) {}

Dust::~Dust() {}

Velocity Dust::getVelocity() const {    
    return m_velocity;
}


Point Dust::getCenter() const {
    return m_center;
}

void Dust::setCenter(const Point& center) {
    m_center = center;
}

/** 
@brief Рисует частицы
 */
void Dust::draw(Painter& painter) const {    
    painter.draw(m_center, m_radius, m_color);
}


///@brief уменьшаем время отображения
void Dust::reduceDurationDisplay(double step) {
    m_durationDisplay -= step;
}

double Dust::getDurationDisplay() const {
    
    return m_durationDisplay;
}



double Dust::random_color() {
    std::random_device rd;
    std::mt19937 mersenne(rd()); 

    std::uniform_int_distribution die{10, 100};

       int a_random_double = die(mersenne);

    return (double) a_random_double / 100.0;
}
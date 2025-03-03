#pragma once
#include "Ball.hpp"
#include "Dust.h"
#include <vector>

class Physics {
  public:
    Physics(double timePerTick = 0.001);
    void setWorldBox(const Point& topLeft, const Point& bottomRight);
    void update(std::vector<Ball>& balls, std::vector<Dust>& dusts,
                size_t ticks, double totalTime) const;

  private:
    void collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dusts) const;
    void collideWithBox(std::vector<Ball>& balls, std::vector<Dust>& dusts) const;
    void move(std::vector<Ball>& balls) const;
    void processCollision(Ball& a, Ball& b, 
                            double distanceBetweenCenters2, std::vector<Dust>& dusts) const;
    // Доп. задание 3
    void move(std::vector<Dust>& dusts) const;    
    void reduceDustsDuration(std::vector<Dust>& dusts) const;
    void cleanDusts(std::vector<Dust>& dusts, double totalTime) const;

  private:
    Point topLeft;
    Point bottomRight;
    double timePerTick;
};

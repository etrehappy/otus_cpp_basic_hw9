#pragma once
#include "Ball.hpp"
#include "Physics.h"
#include "Dust.h"
#include <vector>
#include <iostream>

class Painter;

class World {
  public:
    World(const std::string& worldFilePath);
    void show(Painter& painter) const;
    void update(double time, double totalTime);

  private:
    // Границы мира заданы углами прямоугольника
    Point topLeft;
    Point bottomRight;
    // Объект физического движка
    Physics physics;
    // Контейнер с шарами
    std::vector<Ball> balls;

    //Доп. задание 3
    std::vector<Dust> dusts;

    // Длина отрезка времени, который не был
    // учтен при прошлой симуляции. См. реализацию update
    double restTime = 0.;
};
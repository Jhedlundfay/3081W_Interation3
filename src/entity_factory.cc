/**
 * @file entity_factory.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include <ctime>
#include <cstdlib>
#include <iostream>

#include "src/common.h"
#include "src/entity_factory.h"
#include "src/entity_type.h"
#include "src/params.h"
#include "src/pose.h"
#include "src/rgb_color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
EntityFactory::EntityFactory() {
  srand(time(nullptr));
}

ArenaEntity* EntityFactory::CreateEntity(EntityType etype) {
  switch (etype) {
    case (kRobot):
      return CreateRobot();
      break;
    case (kLight):
      return CreateLight();
      break;
    case (kFood):
      return CreateFood();
      break;
    default:
      std::cout << "FATAL: Bad entity type on creation\n";
      assert(false);
  }
  return nullptr;
}

Robot* EntityFactory::CreateRobot() {
  thread_local unsigned int seed = time(NULL);
  auto* robot = new Robot;
  robot->set_type(kRobot);
  robot->set_color(ROBOT_COLOR);
  robot->set_pose(SetPoseRandomly());
  robot->set_radius(ROBOT_RADIUS+(rand_r(&seed) % ROBOT_RADIUS));
  robot->get_sensors().push_back(new Sensor(LEFT, kLight));
  robot->get_sensors().push_back(new Sensor(RIGHT, kLight));
  robot->get_sensors().push_back(new Sensor(LEFT, kFood));
  robot->get_sensors().push_back(new Sensor(RIGHT, kFood));
  for (auto &sensor : robot->get_sensors()) {
    sensor->set_pose(sensor->CalcPose(ROBOT_INIT_POS, ROBOT_RADIUS));
  }
  sensor_count_ += 4;
  ++entity_count_;
  ++robot_count_;
  robot->set_id(robot_count_);
  return robot;
}

Light* EntityFactory::CreateLight() {
  thread_local unsigned int seed = time(NULL);
  auto* light = new Light;
  light->set_type(kLight);
  light->set_color(LIGHT_COLOR);
  light->set_pose(SetPoseRandomly());
  light->set_radius((rand_r(&seed) % LIGHT_RADIUS)+LIGHT_RADIUS);
  ++entity_count_;
  ++light_count_;
  light->set_id(light_count_);
  return light;
}

Food* EntityFactory::CreateFood() {
  auto* food = new Food;
  food->set_type(kFood);
  food->set_color(FOOD_COLOR);
  food->set_pose(SetPoseRandomly());
  food->set_radius(FOOD_RADIUS);
  ++entity_count_;
  ++food_count_;
  food->set_id(food_count_);
  return food;
}

Pose EntityFactory::SetPoseRandomly() {
  // Dividing arena into 19x14 grid. Each grid square is 50x50
  return {static_cast<double>((30 + (random() % 19) * 50)),
        static_cast<double>((30 + (random() % 14) * 50))};
}

NAMESPACE_END(csci3081);

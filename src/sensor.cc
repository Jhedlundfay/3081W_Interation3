/**
 * @file sensor.cc
 *
 * @copyright 2018 3081 Staff, All right reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <cmath>

#include "src/sensor.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Sensor::Sensor(int side, EntityType type):
  receiver_type_(), pose_(DEFAULT_POSE), color_(), speed_(0) {
  set_receiver_type(type);
  set_side(side);
  set_type(kSensor);
  set_color(SENSOR_COLOR);
  set_radius(SENSOR_RADIUS);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Sensor::ReceiveInfo(std::vector<ArenaEntity*> entities) {
  double impulse = 0.0;
  for (auto &ent : entities) {
    if (ent->get_type() == get_receiver_type()) {
    impulse += ((ent)->get_intensity()
    / (std::pow(1.08,
          Distance(get_pose().x,
             get_pose().y,
             (ent)->get_pose().x,
             (ent)->get_pose().y))));
  }
  }
  set_impulse(impulse);
}

double Sensor::Distance(double x1, double y1, double x2, double y2) {
  return std::sqrt(std::pow(x2 - x1, 2.0) + std::pow(y2 - y1, 2.0));
}

Pose Sensor::CalcPose(Pose pose, int radius) {
  if (which_side() == LEFT) {
    return {
      static_cast<double>(
        pose.x + (radius *
            (std::cos(
                (std::fmod((pose.theta - 40.0), 360.0)) * M_PI / 180.0)))),
  static_cast<double>(
          pose.y + (radius *
              (std::sin(
            (std::fmod((pose.theta - 40.0), 360.0)) * M_PI / 180.0)))),
  static_cast<double>(
          pose.theta)};
  } else {
    return {
      static_cast<double>(
        pose.x + (radius *
            (std::cos(
                (std::fmod((pose.theta + 40.0), 360.0)) * M_PI / 180.0)))),
  static_cast<double>(
          pose.y + (radius *
              (std::sin(
            (std::fmod((pose.theta + 40.0), 360.0)) * M_PI / 180.0)))),
  static_cast<double>(
          pose.theta)};
  }
}

NAMESPACE_END(csci3081);

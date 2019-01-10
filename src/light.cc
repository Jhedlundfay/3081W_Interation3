/**
 * @file light.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <ctime>

#include "src/light.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Light::Light() :
    motion_handler_(this),
    motion_behavior_(this) {
  set_color(LIGHT_COLOR);
  set_pose(LIGHT_POSITION);
  set_radius(LIGHT_RADIUS);
  set_type(kLight);
  motion_handler_.Advance();
}

void Light::TimestepUpdate(unsigned int dt) {
  motion_handler_.UpdateVelocity();
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());
  sensor_touch_->Reset();
  if (get_march_direction() == true) {
    motion_handler_.Retreat();
    std::clock_t end = std::clock();
    double elapsed_time = static_cast<double>
      (end - get_start_time()) / CLOCKS_PER_SEC;
    if (elapsed_time >= 0.2) {
      set_march_direction(false);
      motion_handler_.Advance();
    }
  }
}

void Light::Reset() {
  motion_handler_.Advance();
  thread_local unsigned int seed = time(NULL);
  set_pose(SetPoseRandomly());
  set_radius((rand_r(&seed) % LIGHT_RADIUS)+LIGHT_RADIUS);
} /* Reset */

Pose Light::SetPoseRandomly() {
  return {static_cast<double>((30 + (random() % 19) * 50)),
      static_cast<double>((30 + (random() % 14) * 50))};
}

void Light::HandleCollision(EntityType object_type, ArenaEntity * object) {
  sensor_touch_->HandleCollision(object_type, object);
  set_march_direction(true);
  set_start_time(std::clock());
}


NAMESPACE_END(csci3081);

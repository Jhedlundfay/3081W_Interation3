/**
 * @file robot.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <ctime>
#include <cmath>

#include "src/robot.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Robot::Robot() :
    sensors_(),
    motion_handler_(this),
    motion_behavior_(this) {
  sensors_.push_back(new Sensor(LEFT, kLight));
  sensors_.push_back(new Sensor(RIGHT, kLight));
  sensors_.push_back(new Sensor(LEFT, kFood));
  sensors_.push_back(new Sensor(RIGHT, kFood));
  for (auto &sensor : sensors_) {
    sensor->set_pose(sensor->CalcPose(ROBOT_INIT_POS, ROBOT_RADIUS));
  }
  set_collision_time(std::clock());
  set_food_time(std::clock());
  set_type(kRobot);
  set_color(ROBOT_COLOR);
  set_pose(ROBOT_INIT_POS);
  set_radius(ROBOT_RADIUS);
  motion_handler_.Advance();
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Robot::TimestepUpdate(unsigned int dt) {
  // Update heading as indicated by touch sensor
  motion_handler_.UpdateVelocity();

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());

  // Reset Sensor for next cycle
  sensor_touch_->Reset();

  // Update the positions of the sensors
  for (auto &sensor : sensors_) {
    sensor->set_pose(sensor->CalcPose(get_pose(), get_radius()));
  }

  if (get_march_direction() == true) {
    motion_handler_.Retreat();
    std::clock_t collision_end = std::clock();
    double collision_elapsed_time = static_cast<double>
      (collision_end - get_collision_time()) / CLOCKS_PER_SEC;
    if (collision_elapsed_time >= 2) {
      set_march_direction(false);
      motion_handler_.Advance();
    }
  }

  if (food_exists_) {
  std::clock_t food_end = std::clock();
  double food_elapsed_time = static_cast<double>
    (food_end - get_food_time()) / CLOCKS_PER_SEC;
  if (food_elapsed_time >= 30 && food_elapsed_time < 120) {
    set_hunger(1);
  } else if (food_elapsed_time >= 120 && food_elapsed_time < 150) {
    set_hunger(2);
  } else if (food_elapsed_time >= 150) {
    has_starved(true);
  }
  int max_impulse_l = 0;
  EntityType max_type_l = kLight;
  int max_impulse_r = 0;
  EntityType max_type_r = kLight;
    for (auto &sensor : sensors_) {
      if (sensor->get_receiver_type() == kLight && is_hungry() == 0) {
  motion_handler_.HandleImpulse(sensor->get_impulse(),
              get_l_behavior(),
              sensor->which_side());
      }
      if (sensor->get_receiver_type() == kFood && is_hungry() == 2) {
  motion_handler_.HandleImpulse(sensor->get_impulse(),
              get_f_behavior(),
              sensor->which_side());
      }
      if (is_hungry() == 1) {
  if (sensor->which_side() == LEFT) {
    if (sensor->get_impulse() > max_impulse_l) {
      max_impulse_l = sensor->get_impulse();
      max_type_l = sensor->get_receiver_type();
    }
  } else if (sensor->which_side() == RIGHT) {
    if (sensor->get_impulse() > max_impulse_r) {
      max_impulse_r = sensor->get_impulse();
      max_type_r = sensor->get_receiver_type();
    }
  }
      }
    }
    if (is_hungry() == 1) {
      if (max_type_l == kLight) {
  motion_handler_.HandleImpulse(max_impulse_l,
              get_l_behavior(),
              LEFT);
      } else {
  motion_handler_.HandleImpulse(max_impulse_l,
              get_f_behavior(),
              LEFT);
      }
      if (max_type_r == kLight) {
  motion_handler_.HandleImpulse(max_impulse_r,
              get_l_behavior(),
              RIGHT);
      } else {
  motion_handler_.HandleImpulse(max_impulse_r,
              get_f_behavior(),
              RIGHT);
      }
    }
  } else {
    for (auto &sensor : sensors_) {
      if (sensor->get_receiver_type() == kLight) {
  motion_handler_.HandleImpulse(sensor->get_impulse(),
              get_l_behavior(),
              sensor->which_side());
      }
    }
  }
} /* TimestepUpdate() */

void Robot::Reset() {
  set_collision_time(std::clock());
  set_food_time(std::clock());
  set_color(ROBOT_COLOR);
  set_pose(SetPoseRandomly());
  thread_local unsigned int seed = time(NULL);
  set_radius(ROBOT_RADIUS+(rand_r(&seed) % ROBOT_RADIUS));
  motion_handler_.set_max_speed(ROBOT_MAX_SPEED);
  motion_handler_.set_max_angle(ROBOT_MAX_ANGLE);
  sensor_touch_->Reset();
  motion_handler_.Advance();
  has_starved(false);
  for (auto &sensor : sensors_) {
    sensor->set_pose(sensor->CalcPose(get_pose(), get_radius()));
  }
} /* Reset() */

void Robot::HandleCollision(EntityType object_type, ArenaEntity * object) {
  if (object_type == kFood) {
    set_hunger(0);
    set_food_time(std::clock());
  } else {
    sensor_touch_->HandleCollision(object_type, object);
    set_march_direction(true);
    set_collision_time(std::clock());
  }
}

Pose Robot::SetPoseRandomly() {
  return {static_cast<double>((30 + (random() % 19) * 50)),
      static_cast<double>((30 + (random() % 14) * 50))};
}

NAMESPACE_END(csci3081);

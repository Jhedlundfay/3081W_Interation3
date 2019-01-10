/**
 * @file motion_handler.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler.h"
#include <iostream>
#include <cmath>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void MotionHandler::Advance() {
  set_velocity(SPEED, SPEED);
}

void MotionHandler::Retreat() {
  set_velocity(SPEED, SPEED-1);
}

void MotionHandler::KillSpeed() {
  set_velocity(0, 0);
}

void MotionHandler::HandleImpulse(double impulse, int behavior, int side) {
  switch (behavior) {
  case LOVE : if (side == LEFT) {
      set_velocity(clamp_vel(SPEED - impulse), get_velocity().right);
    } else {
      set_velocity(get_velocity().left, clamp_vel(SPEED - impulse));
    }
    break;
  case AGGRESSION : if (side == RIGHT) {
      set_velocity(clamp_vel(SPEED + impulse), get_velocity().right);
    } else {
      set_velocity(get_velocity().left, clamp_vel(SPEED + impulse));
    }
    break;
  case EXPLORATION : if (side == RIGHT) {
      set_velocity(clamp_vel(SPEED - impulse), get_velocity().right);
    } else {
      set_velocity(get_velocity().left, clamp_vel(SPEED - impulse));
    }
    break;
  default : if (side == LEFT) {
      set_velocity(clamp_vel(SPEED + impulse), get_velocity().right);
    } else {
      set_velocity(get_velocity().left, clamp_vel(SPEED + impulse));
    }
    break;
  }
}

void MotionHandler::UpdateVelocity() {
  if (entity_->get_touch_sensor()->get_output()) {
    entity_->RelativeChangeHeading(+180);
  }
}

double MotionHandler::clamp_vel(double vel) {
  double clamped = vel;
  if (vel > 10.0) {
    clamped = (vel > 10.0) ?
      10.0:
      vel;
  }
  if (vel < 0.0) {
    clamped = (vel < 0.0) ?
      0.0:
      vel;
  }
  return clamped;
} /* clamp_vel() */

NAMESPACE_END(csci3081);

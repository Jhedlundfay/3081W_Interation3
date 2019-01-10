/**
 * @file light.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_LIGHT_H_
#define SRC_LIGHT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include <ctime>

#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"
#include "src/pose.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing an immobile light within the Arena.
 *
 * Lights are simple mobile entities so they function like simplified robots.
 */
class Light : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor.
   */
  Light();

  /**
   * @brief Resets the light to a random position/size
   */
  void Reset() override;

  /**
   * @brief Update the Lights' positions at each timestep
   *
   * @params dt is the change in time of one timestep
   */
  void TimestepUpdate(unsigned int dt) override;

  /**
   * @brief Handes lights' collisions by activating the sensor
   */
  void HandleCollision(EntityType object_type, ArenaEntity * object = NULL);
  /**
   * @brief Makes a new random pose from a pair of random doubles.
   */
  Pose SetPoseRandomly();

  /**
   * @brief Get the name of the Light for visualization purposes, and to
   * aid in debugging.
   */
  std::string get_name() const override {
    return "Light" + std::to_string(get_id());
  }

  /**
   * @brief sets a clock using the <ctime> package for the sake of having
   * the light only go into avoidance behavior for fixed amounts of time
   */
  std::clock_t get_start_time() { return start_; }

  void set_start_time(std::clock_t time) { start_ = time; }

  /**
   * @brief getter for the direction the light is moving; whether
   * advancing or retreating.
   *
   * @return since the light can only be on the advance or on
   * the retreat, the direction it is moving in is represented as
   * a boolean.
   */
  bool get_march_direction() { return retreating_; }

  /**
   * @brief setter for the direction the light is moving.
   * 
   * @return if the light is set to false it is _not_ retrearting;
   * therefor it is advancing. If it is set to true it is retreating.
   * I made false the advancing behavior because it is the behavior
   * performed by moving entities and 0/false should be the default.
   */
  void set_march_direction(bool direction) { retreating_ = direction; }

  MotionHandler get_motion_handler() { return motion_handler_; }

  MotionBehaviorDifferential get_motion_behavior() { return motion_behavior_; }

 private:
  MotionHandler motion_handler_;
  MotionBehaviorDifferential motion_behavior_;
  bool retreating_{false};
  std::clock_t start_ = std::clock();
};

NAMESPACE_END(csci3081);

#endif  // SRC_LIGHT_H_

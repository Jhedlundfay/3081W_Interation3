/**
 * @file robot.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include <ctime>
#include <vector>

#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"
#include "src/sensor.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class MotionBehaviorDifferential;
class LightSensor;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a robot within the arena.
 *
 * Robots are composed of a motion handler, motion behavior, their touch sensor,
 * and the four stimuli sensors that they use to process information about the
 * arena around them indirectly.
 * These classes interact to maintain the pose (position and heading) of the
 * robot. At each time step, the wheel velocities are used to calculate the
 * next pose of the robot. The handler manages the pose and user requests. The
 * behavior calculates the new pose based on wheel velocities.
 *
 * Robots are affected based on their behavior with respect to the stimuli in
 * the arena. That information is used by the motion_handler to update
 * the wheel velocities.
 *
 * The touch sensor is activated when the robot collides with an object.
 * The heading is modified after a collision to move the robot away from the
 * other object.
 */
class Robot : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor using initialization values from params.h.
   */

  Robot();

  Robot(const Robot& other) = delete;
  Robot& operator=(const Robot& other) = delete;

  /**
   * @brief Reset the Robot to a newly constructed state (needed for reset
   * button to work in GUI).
   */
  void Reset() override;

  /**
   * @brief Update the Robot's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;


  /**
   * @brief Handles the collision by setting the sensor to activated.
   */
  void HandleCollision(EntityType object_type, ArenaEntity * object = NULL);

  /**
   * @brief Get the name of the Robot for visualization and for debugging.
   */
  std::string get_name() const override { return "Robot"; }

  /**
   * @brief Makes a new random pose from a pair of random doubles.
   */
  Pose SetPoseRandomly();

  int get_l_behavior() const { return l_behavior_; }

  void set_l_behavior(int behavior) { l_behavior_ = behavior; }

  int get_f_behavior() const {return f_behavior_; }

  void set_f_behavior(int behavior) { f_behavior_ = behavior; }

  std::clock_t get_collision_time() { return collision_start_; }

  std::clock_t get_food_time() { return food_start_; }

  void set_collision_time(std::clock_t time) { collision_start_ = time; }

  void set_food_time(std::clock_t time) { food_start_ = time; }

  bool get_march_direction() { return retreating_; }

  void set_march_direction(bool direction) { retreating_ = direction; }

  MotionHandler get_motion_handler() { return motion_handler_; }

  MotionBehaviorDifferential get_motion_behavior() { return motion_behavior_; }

  void set_hunger(int hunger) { hunger_ = hunger; }

  int is_hungry() const  { return hunger_; }

  void has_starved(bool starved) { starved_ = starved; }

  bool is_starved() const { return starved_; }

  bool food_exists_{true};

  std::vector<Sensor *> get_sensors() { return sensors_; }

 private:
  int hunger_{0};
  std::vector<Sensor *> sensors_;
  // Manages pose and wheel velocities that change with time and collisions.
  MotionHandler motion_handler_;
  // Calculates changes in pose foodd on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;
  // Start time (for retreating)
  std::clock_t collision_start_ = std::clock();
  std::clock_t food_start_ = std::clock();
  int f_behavior_{AGGRESSION};
  int l_behavior_{FEAR};
  bool retreating_{false};
  bool starved_{false};
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_H_

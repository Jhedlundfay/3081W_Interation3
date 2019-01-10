/**
 * @file food.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_FOOD_H_
#define SRC_FOOD_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_immobile_entity.h"
#include "src/common.h"
#include "src/entity_type.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing an immobile food source within the Arena.
 *
 * Food can feed the robots. If the robot passes near or over the food its
 * hunger is set to zero.
 *
 * The food sources' positions can be updated when asked, and they can also
 * track their own velocity and heading. They have a touch sensor for
 * responding to collision events which is activated/deactivated on collision
 * events.
 *
 */
class Food : public ArenaImmobileEntity {
 public:
  /**
   * @brief Constructor.
   *
   * @param params A food_params passed down from main.cc for the
   * initialization of the Food.
   */
  Food();

  /**
   * @brief Reset the Food using the initialization parameters received
   * by the constructor.
   */
  void Reset() override;

  /**
   *
   * @brief Turns a pair of random doubles into the pose for the Reset function
   */
  Pose SetPoseRandomly();


  /**
   * @brief Get the name of the Food for visualization purposes, and to
   * aid in debugging.
   *
   * @return Name of the Food.
   */
  std::string get_name() const override { return "Food"; }
};

NAMESPACE_END(csci3081);

#endif  // SRC_FOOD_H_

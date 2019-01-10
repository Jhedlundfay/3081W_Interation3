/**
 * @file food.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/food.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Food::Food() : ArenaImmobileEntity() {
  set_type(kFood);
  set_color(FOOD_COLOR);
  set_pose(FOOD_INIT_POS);
  set_radius(FOOD_RADIUS);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Food::Reset() {
  set_pose(SetPoseRandomly());
} /* Reset */

Pose Food::SetPoseRandomly() {
  return {static_cast<double>((30 + (random() % 19) * 50)),
      static_cast<double>((30 + (random() % 14) * 50))};
}


NAMESPACE_END(csci3081);

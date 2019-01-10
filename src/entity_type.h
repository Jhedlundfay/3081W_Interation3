/**
 * @file entity_type.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_ENTITY_TYPE_H_
#define SRC_ENTITY_TYPE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

enum EntityType {
  kRobot, kLight, kFood, kEntity,
  kRightWall, kLeftWall, kTopWall, kBottomWall,
  kUndefined, kSensor, kLightSensor, kFoodSensor
};

NAMESPACE_END(csci3081);

#endif  // SRC_ENTITY_TYPE_H_

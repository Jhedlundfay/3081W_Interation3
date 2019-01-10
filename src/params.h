/**
 * @file params.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_PARAMS_H_
#define SRC_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
/*
 * @file. Constructors for classes should use reasonable default values as
 * defined here. An exception might be if the value is 0.
 */

// graphics
#define X_DIM 1024
#define Y_DIM 768
#define TEXT_BOX_WIDTH 50
#define GUI_MENU_WIDTH 180
#define GUI_MENU_GAP 10

// arena
#define N_LIGHTS 5
#define MAX_NUM_LIGHTS 8
#define ARENA_X_DIM X_DIM
#define ARENA_Y_DIM Y_DIM

// game status
#define WON 0
#define LOST 1
#define PLAYING 2

// side
#define LEFT 0
#define RIGHT 1

// behavior
#define FEAR 0
#define LOVE 1
#define AGGRESSION 2
#define EXPLORATION 3

// advance_speed
#define SPEED 2

// entity
#define DEFAULT_POSE \
  { 200, 200, 0}
#define DEFAULT_COLOR \
  { 255, 255, 255 }
#define DEFAULT_RADIUS 20

// mobile entity
#define STARTING_VELOCITY 0.0

// robot
#define N_ROBOTS 10
#define ROBOT_ANGLE_DELTA 1
#define ROBOT_RADIUS 20
#define ROBOT_SPEED_DELTA 1
#define ROBOT_COLLISION_DELTA 1
#define ROBOT_INIT_POS \
  { 500, 500 , 0}
#define ROBOT_COLOR \
  { 0, 0, 255 }

#define ROBOT_HEADING 270
#define ROBOT_INIT_SPEED 0
#define ROBOT_MAX_SPEED 10
#define ROBOT_MAX_ANGLE 360

// food
#define N_FOODS 5
#define FOOD_RADIUS 20
#define FOOD_COLLISION_DELTA 1
#define FOOD_INIT_POS \
  { 400, 400 }
#define FOOD_COLOR \
  { 255, 0, 0 }

// light
#define LIGHT_POSITION \
  { 200, 200 }
#define LIGHT_RADIUS 20
#define LIGHT_MIN_RADIUS 10
#define LIGHT_MAX_RADIUS 50
#define LIGHT_COLOR \
  { 255, 255, 255 }


// sensor
#define SENSOR_RADIUS 5
#define SENSOR_COLOR \
  { 255, 255, 0 }

#endif  // SRC_PARAMS_H_

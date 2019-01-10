/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "src/arena.h"
#include "src/arena_params.h"
#include "src/entity_type.h"
#include "src/robot.h"
#include "src/params.h"
#include "src/sensor.h"
#include "src/motion_handler.h"
#include "src/communication.h"

#ifdef MOTION_HANDLER_TESTS


class MotionHandlerTest : public ::testing::Test {

protected:

  virtual void SetUp() {
    robot = new csci3081::Robot();
    motion_handler = new csci3081::MotionHandler(robot);
  }

  csci3081::Robot * robot;
  csci3081::MotionHandler * motion_handler;

};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(MotionHandlerTest, KillSpeedTest) {
  motion_handler->KillSpeed();
  EXPECT_EQ(motion_handler->get_velocity().left, 0)
    << "FAIL: Kill speed is not affecting the left wheel velocity";
  EXPECT_EQ(motion_handler->get_velocity().right, 0)
    << "FAIL: Kill speed is not affecting the right wheel velocity";
}

TEST_F(MotionHandlerTest, AdvanceTest) {
  motion_handler->Advance();
  ASSERT_EQ(motion_handler->get_velocity().left, SPEED)
    << "FAIL: Left wheel is not normally moving at its default speed";
  ASSERT_EQ(motion_handler->get_velocity().right, SPEED)
    << "FAIL: Right wheel is not normally moving at its defualt speed";
}

TEST_F(MotionHandlerTest, RetreatTest) {
  motion_handler->Retreat();
  EXPECT_GT(motion_handler->get_velocity().left,
	    motion_handler->get_velocity().right)
    << "FAIL: Right wheel is not slower than left while retreating";
}

TEST_F(MotionHandlerTest, ZeroImpulseTest) {
  motion_handler->Advance();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; i < 2; i++) {
      motion_handler->HandleImpulse(0.0, i, j);
      EXPECT_EQ(motion_handler->get_velocity().left, SPEED)
	<< "FAIL: Zero impulse resulted in non-default wheel velocity";
      EXPECT_EQ(motion_handler->get_velocity().right, SPEED)
	<< "FAIL: Zero impulse resutled in non-default wheel velocity";
    }
  }
}

TEST_F(MotionHandlerTest, ClampNegative) {
  double negative = -1.0;
  EXPECT_EQ(motion_handler->clamp_vel(negative), 0.0)
    << "FAIL: Negative velocities are not clamped to zero";
}

TEST_F(MotionHandlerTest, ClampLarge) {
  double large = 200.0;
  EXPECT_EQ(motion_handler->clamp_vel(large), 10.0)
    << "FAIL: Large velocities are not clamped to ten";
}

TEST_F(MotionHandlerTest, ClampNormal) {
  double normal = 5.0;
  EXPECT_EQ(motion_handler->clamp_vel(normal), normal)
    << "FAIL: Velocities within the threshold are clamped unnecessarily";
}

TEST_F(MotionHandlerTest, LargeImpulseLoveTest) {
  motion_handler->Advance();
  motion_handler->HandleImpulse(1200.0, LOVE, LEFT);
  EXPECT_EQ(motion_handler->get_velocity().left, 0)
    << "FAIL: Love, left side is not stopping the left wheel";
  EXPECT_EQ(motion_handler->get_velocity().right, SPEED)
    << "FAIL: Love, left side is affecting the right wheel";
  motion_handler->Advance();
  motion_handler->HandleImpulse(1200.0, LOVE, RIGHT);
  EXPECT_EQ(motion_handler->get_velocity().left, SPEED)
    << "FAIL: Love, right side is affecting the left wheel";
  EXPECT_EQ(motion_handler->get_velocity().right, 0)
    << "FAIL: Love, right side is not stopping the right wheel";
}

TEST_F(MotionHandlerTest, LargeImpulseAggressionTest) {
  motion_handler->Advance();
  motion_handler->HandleImpulse(1200.0, AGGRESSION, RIGHT);
  EXPECT_EQ(motion_handler->get_velocity().left, 10.0)
    << "FAIL: Aggression, right side is not hastening the left wheel";
  EXPECT_EQ(motion_handler->get_velocity().right, SPEED)
    << "FAIL: Aggression, right side is affecting the right wheel";
  motion_handler->Advance();
  motion_handler->HandleImpulse(1200.0, AGGRESSION, LEFT);
  EXPECT_EQ(motion_handler->get_velocity().left, SPEED)
    << "FAIL: Aggression, left side is affecting the left wheel";
  EXPECT_EQ(motion_handler->get_velocity().right, 10.0)
    << "FAIL: Aggression, left side is not hastening the right wheel";
}

TEST_F(MotionHandlerTest, LargeImpulseExplorationTest) {
  motion_handler->Advance();
  motion_handler->HandleImpulse(1200.0, EXPLORATION, RIGHT);
  EXPECT_EQ(motion_handler->get_velocity().left, 0)
    << "FAIL: Exploration, right side is not stopping the left wheel";
  EXPECT_EQ(motion_handler->get_velocity().right, SPEED)
    << "FAIL: Exploration, right side is affecting the right wheel";
  motion_handler->Advance();
  motion_handler->HandleImpulse(1200.0, EXPLORATION, LEFT);
  EXPECT_EQ(motion_handler->get_velocity().left, SPEED)
    << "FAIL: Exploration, left side is affecting the left wheel";
  EXPECT_EQ(motion_handler->get_velocity().right, 0)
    << "FAIL: Exploration, side is not stopping the right wheel";
}

TEST_F(MotionHandlerTest, LargeImpulseFearTest) {
  motion_handler->Advance();
  motion_handler->HandleImpulse(1200.0, FEAR, LEFT);
  EXPECT_EQ(motion_handler->get_velocity().left, 10.0)
    << "FAIL: Fear, left side is not hastening the left wheel";
  EXPECT_EQ(motion_handler->get_velocity().right, SPEED)
    << "FAIL: Fear, left side is affecting the right wheel";
  motion_handler->Advance();
  motion_handler->HandleImpulse(1200.0, FEAR, RIGHT);
  EXPECT_EQ(motion_handler->get_velocity().left, SPEED)
    << "FAIL: Fear, right side is affecting the left wheel";
  EXPECT_EQ(motion_handler->get_velocity().right, 10.0)
    << "FAIL: Fear, right side is not hastening the right wheel";
}

#endif /* MOTION_HANDLER_TESTS */

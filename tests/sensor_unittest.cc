/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include <stdio.h>
#include <vector>
#include <limits>
#include <math.h>
#include "src/arena.h"
#include "src/arena_params.h"
#include "src/entity_type.h"
#include "src/robot.h"
#include "src/params.h"
#include "src/sensor.h"
#include "src/communication.h"

#ifdef SENSOR_TESTS


class SensorTest : public ::testing::Test {

  protected:

  virtual void SetUp() {
    robot = new csci3081::Robot();
    sensors = robot->get_sensors();
  }
  csci3081::Robot * robot;
  std::vector<csci3081::Sensor*> sensors;
  std::vector<csci3081::ArenaEntity*> entities;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

/* Test that the robot is actually pushing the right number of each type of
 * sensors onto its vector.
 * These should be assert statements because later tests that iterate over all
 * sensors will be meaningless if the sensors aren't there.
 */

TEST_F(SensorTest, ConstructorSensorNumbers) {
  int right_count = 0;
  int left_count = 0;
  int light_count = 0;
  int food_count = 0;
  for (auto &sensor : sensors) {
    if (sensor->which_side() == LEFT) {
      left_count++;
    }
    if (sensor->which_side() == RIGHT) {
	right_count++;
    }
    if (sensor->get_receiver_type() == csci3081::kLight) {
      light_count++;
    }
    if (sensor->get_receiver_type() == csci3081::kFood) {
      food_count++;
    }
  }
  ASSERT_EQ(right_count, 2)
    << "FAIL: There should be exactly two righthand sensors on each robot.";
  ASSERT_EQ(left_count, 2)
    << "FAIL: There should be exactly two lefthand sensors on each robot.";
  ASSERT_EQ(food_count, 2)
    << "FAIL: There should be exactly two food sensors on each robot.";
  ASSERT_EQ(light_count, 2)
    << "FAIL: There should be exactly two light sensors on each robot.";
}

/* Test that the sensor contructor is correctly applying the color/size 
 * parameters
 */

TEST_F(SensorTest, ConstructorColourAndShape) {
  csci3081::RgbColor sensor_color = SENSOR_COLOR;
  for (auto &sensor : sensors) {
    EXPECT_EQ(sensor->get_color(), sensor_color)
      << "FAIL: Sensors are not created in the right color";
    EXPECT_EQ(sensor->get_radius(), SENSOR_RADIUS)
      << "FAIL: Sensors are not the right size";
  }
}

TEST_F(SensorTest, DefaultImpulseZero) {
  for (auto &sensor : sensors) {
    ASSERT_EQ(sensor->get_impulse(), 0.0)
      << "FAIL: Sensors must have a default impulse of zero.";
  }
}

TEST_F(SensorTest, EmptyVectorImpulseZero) {
  for (auto &sensor : sensors) {
    sensor->ReceiveInfo(entities);
    EXPECT_EQ(sensor->get_impulse(), 0.0)
      << "FAIL: Sensors have non-zero impulse when passed an empty vector.";
  }
}

TEST_F(SensorTest, ZeroIntensityLightImpulse) {
  csci3081::Light * light0;
  light0 = new csci3081::Light();
  light0->set_intensity(0);
  entities.push_back(light0);
  for (auto &sensor : sensors) {
    sensor->ReceiveInfo(entities);
    EXPECT_EQ(sensor->get_impulse(), 0.0)
      << "FAIL: Non-zero sensor impulse from zero intensity light.";
  }
}

TEST_F(SensorTest, NonReceivedTypesImpulseZero) {
  csci3081::Robot * robot0;
  robot0 = new csci3081::Robot();
  entities.push_back(robot0);
  for (auto &sensor : sensors) {
    sensor->ReceiveInfo(entities);
    EXPECT_EQ(sensor->get_impulse(), 0.0)
      << "FAIL: Irrelevant entity types are affecting the impulse.";
  }
  csci3081::Food * food0;
  food0 = new csci3081::Food();
  entities.push_back(food0);
  for (auto &sensor : sensors) {
    if (sensor->get_receiver_type() == csci3081::kLight)
      EXPECT_EQ(sensor->get_impulse(), 0.0)
	<< "FAIL: Irrelevant entity types are affecting the impulse.";
  }
}

TEST_F(SensorTest, ZeroDistanceCalculation) {
  double x1 = random() % 10;
  double y1 = random() % 10;
  double x2 = x1;
  double y2 = y1;
  for (auto &sensor : sensors) {
    ASSERT_LT(sensor->Distance(x1,y1,x2,y2) - 0.0,
	      std::numeric_limits<double>::epsilon())
      << "FAIL: Distance calculator is not accurate to within epsilon";
    ASSERT_LT(0.0 - sensor->Distance(x1,y1,x2,y2),
	      std::numeric_limits<double>::epsilon())
      << "Fail: Distance calculator is not accurate to within epsilon";
  }
}

TEST_F(SensorTest, NonZeroDistanceCalculation) {
  double x1 = random() % 10;
  double y1 = random() % 10;
  double x2 = random() % 10;
  double y2 = random() % 10;
  double expected_distance = sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
  for (auto &sensor : sensors) {
    EXPECT_LT(sensor->Distance(x1,y1,x2,y2) - expected_distance,
	      std::numeric_limits<double>::epsilon())
      << "FAIL: Distance calculator is not returning the correct distance";
    EXPECT_LT(expected_distance - sensor->Distance(x1,y1,x2,y2),
	      std::numeric_limits<double>::epsilon())
      << "FAIL: Distance caluclator is not returning the correct distance";
  }
}

TEST_F(SensorTest, PoseCalculator) {
  robot->set_pose({200.0,200.0,0.0});
  robot->set_radius(20);
  csci3081::Pose expected_left = {
    static_cast<double>(
      200.0 + (20.0 * (std::cos(
      (std::fmod((0.0 - 40.0), 360.0)) * M_PI / 180.0)))),
    static_cast<double>(
      200.0 + (20.0 * (std::sin(
      (std::fmod((0.0 - 40.0), 360.0)) * M_PI / 180.0)))),
    static_cast<double>(0.0)};
  csci3081::Pose expected_right = {
    static_cast<double>(
      200.0 + (20.0 * (std::cos(
      (std::fmod((0.0 + 40.0), 360.0)) * M_PI / 180.0)))),
    static_cast<double>(
      200.0 + (20.0 * (std::sin(
      (std::fmod((0.0 + 40.0), 360.0)) * M_PI / 180.0)))),
    static_cast<double>(0.0)};

  for (auto &sensor : sensors) {
    sensor->set_pose(sensor->CalcPose(robot->get_pose(), robot->get_radius()));
    if (sensor->which_side() == LEFT) {
      EXPECT_EQ(sensor->get_pose(), expected_left);
    } else {
      EXPECT_EQ(sensor->get_pose(), expected_right);
    }
  }
}

TEST_F(SensorTest, LongDistanceLightImpulse) {
  entities.clear();
  csci3081::Light * light1;
  light1 = new csci3081::Light();
  entities.push_back(light1);
  light1->set_pose({1200.0, 1200.0, 0.0});
  for (auto &sensor : sensors) {
    sensor->ReceiveInfo(entities);
    EXPECT_LT(sensor->get_impulse(),
	      std::numeric_limits<double>::epsilon())
      << "FAIL: Lights do not approach zero impulse fast enough";
  }
}
  
TEST_F(SensorTest, ZeroDistanceLightImpulse) {
  entities.clear();
  csci3081::Light * light2;
  light2 = new csci3081::Light();
  entities.push_back(light2);
  for (auto &sensor : sensors) {
    if (sensor->get_receiver_type() == csci3081::kLight) {
    light2->set_pose(sensor->get_pose());
    sensor->ReceiveInfo(entities);
    EXPECT_EQ(sensor->get_impulse(), 1200.0)
      << "FAIL: Sensor impulse is not 1200 when directly under a light";
  }
  }
}

TEST_F(SensorTest, HalfDistanceLightImpulse) {
  entities.clear();
  csci3081::Light * light3;
  light3 = new csci3081::Light();
  entities.push_back(light3);
  csci3081::Pose halfway_pose;
  for (auto &sensor : sensors) {
    halfway_pose = {sensor->get_pose().x + 10,
		    sensor->get_pose().y,
		    sensor->get_pose().theta};
    if (sensor->get_receiver_type() == csci3081::kLight) {
      light3->set_pose(halfway_pose);
      sensor->ReceiveInfo(entities);
      EXPECT_LT(sensor->get_impulse(), 600)
	<< "Fail: Sensor impulses are not decreasing rapidly enough";
    }
  }
}

TEST_F(SensorTest, TwoLightImpulse) {
  entities.clear();
  csci3081::Light * light4;
  csci3081::Light * light5;
  light4 = new csci3081::Light();
  light5 = new csci3081::Light();
  entities.push_back(light4);
  entities.push_back(light5);
  for (auto &sensor : sensors) {
    if (sensor->get_receiver_type() == csci3081::kLight) {
      light4->set_pose(sensor->get_pose());
      light5->set_pose(sensor->get_pose());
      sensor->ReceiveInfo(entities);
      EXPECT_EQ(sensor->get_impulse(), 2400.0)
	<< "FAIL: Impulse is not correctly adding intensities";
    }
  }
} 

#endif /* SENSOR_TESTS */

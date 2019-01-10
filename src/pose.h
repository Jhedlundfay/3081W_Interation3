/**
 * @file pose.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_POSE_H_
#define SRC_POSE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <limits>
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A simple representation of the position/orientation of an entity
 * within the Arena.
 *
 * NOTE: Origin (0,0) is at the upper left corner of the Arena.
 */
struct Pose {
 public:
  /**
   * @brief Default constructor. Initialize the pose to (0,0,0)
   */
  Pose() {}

  /**
   * @brief Constructor
   *
   * @param in_x The X component of the Pose.
   * @param in_y The Y component of the Pose.
   */
  Pose(double in_x, double in_y) : x(in_x), y(in_y) {}

  Pose(double in_x, double in_y, double in_theta)
      : x(in_x),
        y(in_y),
        theta(in_theta) {}

  /**
   * @brief Default assignment operator. Simply copies the (x,y) values of
   * another Pose.
   *
   * @param other The Pose object to copy from.
   *
   * @return The left-hand-side Pose object that is now identical (in value)
   * to `other`.
   */
  Pose &operator=(const Pose &other) = default;

  /**
   * @brief Overloaded comparison operator. Since the values being compared
   * from pose to pose are doubles which cannot be compared using > and <
   * directly, this operator uses the mathematical limits package to compare
   * the differences between the x, y, and theta, values of two poses against
   * the epsilon value.
   */
  bool operator==(const Pose other) const {
    return ((x - other.x) < std::numeric_limits<double>::epsilon() &&
            (other.x - x) < std::numeric_limits<double>::epsilon() &&
            (y - other.y) < std::numeric_limits<double>::epsilon() &&
            (other.y - y) < std::numeric_limits<double>::epsilon() &&
            (theta - other.theta) < std::numeric_limits<double>::epsilon() &&
            (other.theta - theta) < std::numeric_limits<double>::epsilon());
  }

  double x{0};
  double y{0};
  double theta{0.0};
};

/*******************************************************************************
 * Forward Decls
 ******************************************************************************/
constexpr double deg2rad(double deg) { return deg * M_PI / 180.0; }
constexpr double rad2deg(double rad) { return rad * 180.0 / M_PI; }

NAMESPACE_END(csci3081);

#endif /* SRC_POSE_H_ */

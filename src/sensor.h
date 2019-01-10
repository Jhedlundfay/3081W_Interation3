/**
 * @file sensor.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <vector>

#include "src/entity_type.h"
#include "src/common.h"
#include "src/params.h"
#include "src/pose.h"
#include "src/rgb_color.h"
#include "src/arena_entity.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Sensors are essentially mobile entities but without touch sensors.
 *
 */
class Sensor {
 public:
  /**
   * @brief Sensor's constructor.
   */

  Sensor(int side, EntityType receiver_type_);

  virtual ~Sensor() = default;

  virtual void Reset() {}

  void ReceiveInfo(std::vector<ArenaEntity*> entities);

  Pose CalcPose(Pose pose, int radius);

  double Distance(double x1, double x2, double y1, double y2);

  const Pose &get_pose() const { return pose_; }
  void set_pose(const Pose &pose) { pose_ = pose; }

  void set_position(const double inx, const double iny) {
    pose_.x = inx;
    pose_.y = iny;
  }

  void set_heading(const double t) { pose_.theta = t; }

  void RelativeChangeHeading(const double delta) {
    pose_.theta += delta;
  }

  const RgbColor &get_color() const { return color_; }

  void set_color(const RgbColor &color) { color_ = color; }

  double get_radius() const { return radius_; }

  void set_radius(double radius) { radius_ = radius; }

  EntityType get_type() const { return type_; }
  void set_type(EntityType et) { type_ = et; }

  int get_id() const { return id_; }
  void set_id(int id) { id_ = id; }

  bool is_mobile(void) { return is_mobile_; }

  void set_mobility(bool value) { is_mobile_ = value; }

  bool is_active() {return is_active_; }

  void set_activity(bool value) { is_active_ = value; }

  virtual double get_speed() { return speed_; }
  virtual void set_speed(double sp) { speed_ = sp; }

  int which_side() { return side_; }

  void set_side(int side) { side_ = side; }

  void set_impulse(double impulse) { impulse_ = impulse; }

  double get_impulse() { return impulse_; }

  void set_receiver_type(EntityType type) { receiver_type_ = type; }

  EntityType get_receiver_type() { return receiver_type_; }

 private:
  int side_{LEFT};
  EntityType receiver_type_;
  double impulse_{0.0};
  double radius_{DEFAULT_RADIUS};
  Pose pose_;
  RgbColor color_;
  EntityType type_{kSensor};
  int id_{-1};
  bool is_mobile_{true};
  bool is_active_{true};
  double speed_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_H_

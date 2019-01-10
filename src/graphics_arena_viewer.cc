/**
 * @file graphics_arena_viewer.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
#include <iostream>
#include <string>

#include "src/graphics_arena_viewer.h"
#include "src/arena_params.h"
#include "src/params.h"
#include "src/rgb_color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
GraphicsArenaViewer::GraphicsArenaViewer(
    const struct arena_params *const params,
    Arena * arena, Controller * controller) :
    GraphicsApp(
        params->x_dim + GUI_MENU_WIDTH + GUI_MENU_GAP * 2,
        params->y_dim,
        "Robot Simulation"),
    controller_(controller),
    arena_(arena) {
  auto *gui = new nanogui::FormHelper(screen());
  nanogui::ref<nanogui::Window> window =
      gui->addWindow(
          Eigen::Vector2i(10 + GUI_MENU_GAP, 10),
          "Menu");

  window->setLayout(new nanogui::GroupLayout());

  gui->addGroup("Simulation Control");
  new_game_button_ =
    gui->addButton(
      "New Game",
      std::bind(&GraphicsArenaViewer::OnNewGameBtnPressed, this));
  new_game_button_->setFixedWidth(100);
  playing_button_ =
    gui->addButton(
      "Play",
      std::bind(&GraphicsArenaViewer::OnPlayingBtnPressed, this));
  playing_button_->setFixedWidth(100);

  gui->addGroup("Arena Configuration");
  food_button_ =
    gui->addButton(
                   "Disable Food",
                   std::bind(&GraphicsArenaViewer::OnFoodBtnPressed, this));
  food_button_->setFixedWidth(100);
  nanogui::Widget *panel = new nanogui::Widget(window);
  // *************** SLIDER 1 ************************//
  new nanogui::Label(panel, "Number of Robots", "sans-bold");
  nanogui::Slider *slider = new nanogui::Slider(panel);
  slider->setValue(1.0f);
  slider->setFixedWidth(100);

  nanogui::TextBox *textBox = new nanogui::TextBox(panel);
  textBox->setFixedSize(nanogui::Vector2i(60, 25));
  textBox->setFontSize(20);
  textBox->setValue("10");

  slider->setCallback(
    [textBox](float value) {
      textBox->setValue(std::to_string(static_cast<int>(value*10)));
    });

  slider->setFinalCallback(
    [&](float value) {
      if (paused_ || stopped_) {
      n_robots_ = static_cast<int>(value*10);
      controller_->ChangeArena();
      std::cout << "Final slider value: " << value;
      std::cout << " robot " << n_robots_ << std::endl;
      } else {
  std::cout << "Simulation is running." << std::endl;
  std::cout << "The arena must be paused to change the number of robots."
      << std::endl;
      }
    });
  // *************** SLIDER 2 ************************//
  new nanogui::Label(panel, "Number of Lights", "sans-bold");
  nanogui::Slider *slider2 = new nanogui::Slider(panel);
  slider2->setValue(1.0f);
  slider2->setFixedWidth(100);

  nanogui::TextBox *textBox2 = new nanogui::TextBox(panel);
  textBox2->setFixedSize(nanogui::Vector2i(60, 25));
  textBox2->setFontSize(20);
  textBox2->setValue("5");

  slider2->setCallback(
    [textBox2](float value) {
      textBox2->setValue(std::to_string(static_cast<int>(value*5)));
    });

  slider2->setFinalCallback(
    [&](float value) {
      if (paused_ || stopped_) {
      n_lights_ = static_cast<int>(value*5);
      controller_->ChangeArena();
      std::cout << "Final slider2 value: " << value;
      std::cout << " light " << n_lights_ << std::endl;
      } else {
  std::cout << "Simulation is running." << std::endl;
  std::cout << "The arena must be paused to change the number of lights."
      << std::endl;
      }
     });
  // *************** SLIDER 3 ************************//
  new nanogui::Label(panel, "Fear-Exploratory Ratio", "sans-bold");
  nanogui::Slider *slider3 = new nanogui::Slider(panel);
  slider3->setValue(0.0f);
  slider3->setFixedWidth(100);

  nanogui::TextBox *textBox3 = new nanogui::TextBox(panel);
  textBox3->setFixedSize(nanogui::Vector2i(60, 25));
  textBox3->setFontSize(20);
  textBox3->setValue("0");
  textBox3->setUnits("%");

  slider3->setCallback(
    [textBox3](float value) {
      textBox3->setValue(std::to_string(static_cast<int>(value*100)));
    });

  slider3->setFinalCallback(
    [&](float value) {
      controller_->SetArenaFERatio(value);
      std::cout << "Final slider3 value: " << value;
      std::cout << " robot " << static_cast<int>(value*10) << std::endl;
    });
  // *************** SLIDER 4 ************************//
  new nanogui::Label(panel, "Light Intensity", "sans-bold");
  nanogui::Slider *slider4 = new nanogui::Slider(panel);
  slider4->setValue(0.0f);
  slider4->setFixedWidth(100);

  nanogui::TextBox *textBox4 = new nanogui::TextBox(panel);
  textBox4->setFixedSize(nanogui::Vector2i(60, 25));
  textBox4->setFontSize(20);
  textBox4->setValue("0");

  slider4->setCallback(
    [textBox4](float value) {
      textBox4->setValue(std::to_string(static_cast<int>(value*1200)));
    });

  slider4->setFinalCallback(
    [&](float value) {
      controller_->UpdateLightIntensity(value);
      std::cout << "Final slider4 value: " << value;
      std::cout << " intensity " << static_cast<int>(value*1200) << std::endl;
    });
  // *************** SLIDER 5 ************************//
  new nanogui::Label(panel, "Number of Foods", "sans-bold");
  nanogui::Slider *slider5 = new nanogui::Slider(panel);
  slider5->setValue(1.0f);
  slider5->setFixedWidth(100);

  nanogui::TextBox *textBox5 = new nanogui::TextBox(panel);
  textBox5->setFixedSize(nanogui::Vector2i(60, 25));
  textBox5->setFontSize(20);
  textBox5->setValue("5");

  slider5->setCallback(
    [textBox5](float value) {
      textBox5->setValue(std::to_string(static_cast<int>(value*5)));
    });

  slider5->setFinalCallback(
    [&](float value) {
      if (food_) {
      if (paused_ || stopped_) {
      n_foods_ = static_cast<int>(value*5);
      controller_->ChangeArena();
      std::cout << "Final slider5 value: " << value;
      std::cout << " food " << n_foods_ << std::endl;
      } else {
  std::cout << "Simulation is running." << std::endl;
  std::cout << "The arena must be paused to change the number of foods."
      << std::endl;
      }
      } else {
  std::cout << "Food is currently disabled." <<std::endl;
  std::cout << "Food must be enabled to change the number of foods."
      << std::endl;
      }
     });

  panel->setLayout(new nanogui::BoxLayout
    (nanogui::Orientation::Vertical, nanogui::Alignment::Middle, 0, 15));

  screen()->performLayout();
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

// This is the primary driver for state change in the arena.
// It will be called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::UpdateSimulation(double dt) {
  if (paused_) {
    controller_->AdvanceTime(0);
  } else if (arena_->get_game_status() == WON) {
    stopped_ = true;
    playing_button_->setCaption("You Win");
    controller_->AdvanceTime(0);
  } else if (arena_->get_game_status() == LOST) {
    stopped_ = true;
    playing_button_->setCaption("You Lose");
    controller_->AdvanceTime(0);
  } else {
    controller_->AdvanceTime(dt);
  }
}

/*******************************************************************************
 * Handlers for User Keyboard and Mouse Events
 ******************************************************************************/
void GraphicsArenaViewer::OnPlayingBtnPressed() {
  if (!stopped_) {
    if (!paused_) {
      paused_ = true;
      playing_button_->setCaption("Play");
      controller_->AcceptCommunication(kPause);
    } else {
      paused_ = false;
      playing_button_->setCaption("Pause");
      controller_->AcceptCommunication(kPlay);
    }
  }
}

void GraphicsArenaViewer::OnFoodBtnPressed() {
  if (paused_ || stopped_) {
  if (!food_) {
    food_ = true;
    food_button_->setCaption("Disable Food");
    controller_->AcceptCommunication(kYesFood);
  } else {
    food_ = false;
    n_foods_ = 0;
    controller_->ChangeArena();
    food_button_->setCaption("Enable Food");
    controller_->AcceptCommunication(kNoFood);
  }
  } else {
    std::cout << "Simulation is running." << std::endl;
    std::cout << "The arena must be paused to disable or enable food."
        << std::endl;
  }
}

void GraphicsArenaViewer::OnNewGameBtnPressed() {
  paused_ = true;
  stopped_ = false;
  playing_button_->setCaption("Play");
  controller_->AcceptCommunication(kPause);
  controller_->AcceptCommunication(kNewGame);
}

/*******************************************************************************
 * Drawing of Entities in Arena
 ******************************************************************************/
void GraphicsArenaViewer::DrawRobot(NVGcontext *ctx,
                                     const Robot *const robot) {
  // translate and rotate all graphics calls that follow so that they are
  // centered, at the position and heading of this robot
  nvgSave(ctx);
  nvgTranslate(ctx,
               static_cast<float>(robot->get_pose().x),
               static_cast<float>(robot->get_pose().y));
  nvgRotate(ctx,
            static_cast<float>(robot->get_pose().theta * M_PI / 180.0));

  // robot's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, static_cast<float>(robot->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(robot->get_color().r, robot->get_color().g,
                       robot->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // robot id text label
  nvgSave(ctx);
  nvgRotate(ctx, static_cast<float>(M_PI / 2.0));
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, 0.0, -10.0, robot->get_name().c_str(), nullptr);
  std::string info = std::to_string(robot->get_l_behavior()) + ", " +
                     std::to_string(robot->get_f_behavior()) + ", " +
                     std::to_string(robot->is_hungry());
  nvgText(ctx, 0.0, 0.0, info.c_str(), nullptr);
  nvgRestore(ctx);
  nvgRestore(ctx);
}

void GraphicsArenaViewer::DrawSensor(NVGcontext *ctx,
                                       const Sensor *const sensor) {
  nvgBeginPath(ctx);
  nvgCircle(ctx,
            static_cast<float>(sensor->get_pose().x),
            static_cast<float>(sensor->get_pose().y),
            static_cast<float>(sensor->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(sensor->get_color().r, sensor->get_color().g,
                       sensor->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx,
          static_cast<float>(sensor->get_pose().x),
          static_cast<float>(sensor->get_pose().y),
          "X", nullptr);
}

void GraphicsArenaViewer::DrawArena(NVGcontext *ctx) {
  nvgBeginPath(ctx);
  // Creates new rectangle shaped sub-path.
  nvgRect(ctx, 0, 0, arena_->get_x_dim(), arena_->get_y_dim());
  nvgStrokeColor(ctx, nvgRGBA(255, 255, 255, 255));
  nvgStroke(ctx);
}

void GraphicsArenaViewer::DrawEntity(NVGcontext *ctx,
                                       const ArenaEntity *const entity) {
  // light's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx,
            static_cast<float>(entity->get_pose().x),
            static_cast<float>(entity->get_pose().y),
            static_cast<float>(entity->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(entity->get_color().r, entity->get_color().g,
                       entity->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // light id text label
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx,
          static_cast<float>(entity->get_pose().x),
          static_cast<float>(entity->get_pose().y),
          entity->get_name().c_str(), nullptr);
}

void GraphicsArenaViewer::DrawUsingNanoVG(NVGcontext *ctx) {
  // initialize text rendering settings
  if (!paused_ && !stopped_) {
  nvgFontSize(ctx, 18.0f);
  nvgFontFace(ctx, "sans-bold");
  nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  DrawArena(ctx);
  std::vector<ArenaEntity *> entities = arena_->get_entities();
  for (auto &entity : entities) {
    DrawEntity(ctx, entity);
  } /* for(i..) */
  std::vector<Robot *> robots = arena_->get_robots();
  for (auto &robot : robots) {
    DrawRobot(ctx, robot);
  }
  std::vector<Sensor *> sensors = arena_->get_sensors();
  for (auto &sensor : sensors) {
    DrawSensor(ctx, sensor);
  }
  }
}

NAMESPACE_END(csci3081);

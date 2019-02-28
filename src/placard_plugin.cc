/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gazebo/rendering/Scene.hh>
#include <ignition/math/Rand.hh>
#include "vrx_gazebo/placard_plugin.hh"

// Static initialization.
std::map<std::string, std_msgs::ColorRGBA> PlacardPlugin::kColors =
  {
    {"red",   CreateColor(1.0, 0.0, 0.0, 1.0)},
    {"green", CreateColor(0.0, 1.0, 0.0, 1.0)},
    {"blue",  CreateColor(0.0, 0.0, 1.0, 1.0)},
  };

std::vector<std::string> PlacardPlugin::kShapes =
  {"circle", "cross", "triangle"};

//////////////////////////////////////////////////
std_msgs::ColorRGBA PlacardPlugin::CreateColor(const double _r,
  const double _g, const double _b, const double _a)
{
  static std_msgs::ColorRGBA color;
  color.r = _r;
  color.g = _g;
  color.b = _b;
  color.a = _a;
  return color;
}

//////////////////////////////////////////////////
void PlacardPlugin::Load(gazebo::rendering::VisualPtr _parent,
  sdf::ElementPtr _sdf)
{
  GZ_ASSERT(_parent != nullptr, "Received NULL model pointer");

  this->scene = _parent->GetScene();
  GZ_ASSERT(this->scene != nullptr, "NULL scene");

  // This is important to disable the visual plugin running inside the GUI.
  if (!this->scene->EnableVisualizations())
    return;

  if (!this->ParseSDF(_sdf))
    return;

  // Quit if ros plugin was not loaded
  if (!ros::isInitialized())
  {
    ROS_ERROR("ROS was not initialized.");
    return;
  }

  if (this->shuffleEnabled)
  {
    this->nh = ros::NodeHandle(this->ns);
    this->changeSymbolServer = this->nh.advertiseService(
      this->topic, &PlacardPlugin::ChangeSymbol, this);
  }

  this->timer.Start();

  this->updateConnection = gazebo::event::Events::ConnectPreRender(
    std::bind(&PlacardPlugin::Update, this));
}

//////////////////////////////////////////////////
bool PlacardPlugin::ParseSDF(sdf::ElementPtr _sdf)
{
  // Parse the shape. We initialize it with a random shape.
  this->ShuffleShape();
  if (_sdf->HasElement("shape"))
  {
    std::string aShape = _sdf->GetElement("shape")->Get<std::string>();
    std::transform(aShape.begin(), aShape.end(), aShape.begin(), ::tolower);
    // Sanity check: Make sure the shape is allowed.
    if (std::find(this->kShapes.begin(), this->kShapes.end(), aShape) !=
          this->kShapes.end())
    {
      this->shape = aShape;
    }
    else
    {
      ROS_INFO_NAMED("PlacardPlugin",
                  "incorrect [%s] <shape>, using random shape", aShape.c_str());
    }
  }

  // Parse the color. We initialize it with a random color.
  this->ShuffleColor();
  if (_sdf->HasElement("color"))
  {
    std::string aColor = _sdf->GetElement("color")->Get<std::string>();
    std::transform(aColor.begin(), aColor.end(), aColor.begin(), ::tolower);
    // Sanity check: Make sure the color is allowed.
    if (this->kColors.find(aColor) != this->kColors.end())
    {
      this->color = aColor;
    }
    else
    {
      ROS_INFO_NAMED("PlacardPlugin",
                  "incorrect [%s] <color>, using random color", aColor.c_str());
    }
  }

  // Required: visuals.
  if (!_sdf->HasElement("visuals"))
  {
    ROS_ERROR("<visuals> missing");
    return false;
  }

  auto visualsElem = _sdf->GetElement("visuals");
  if (!visualsElem->HasElement("visual"))
  {
    ROS_ERROR("<visual> missing");
    return false;
  }

  auto visualElem = visualsElem->GetElement("visual");
  while (visualElem)
  {
    std::string visualName = visualElem->Get<std::string>();
    this->visualNames.push_back(visualName);
    visualElem = visualElem->GetNextElement();
  }

  // Optional: Is shuffle enabled?
  if (_sdf->HasElement("shuffle"))
  {
    this->shuffleEnabled = _sdf->GetElement("shuffle")->Get<bool>();

    // Required if shuffle enabled: ROS topic.
    if (!_sdf->HasElement("topic"))
    {
      ROS_ERROR("<topic> missing");
    }
    this->topic = _sdf->GetElement("topic")->Get<std::string>();
  }

  // Optional: ROS namespace.
  if (_sdf->HasElement("robot_namespace"))
    this->ns = _sdf->GetElement("robot_namespace")->Get<std::string>();

  return true;
}

//////////////////////////////////////////////////
void PlacardPlugin::Update()
{
  // Get the visuals if needed.
  if (this->visuals.empty())
  {
    for (auto visualName : this->visualNames)
    {
      auto visualPtr = this->scene->GetVisual(visualName);
      if (visualPtr)
        this->visuals.push_back(visualPtr);
      else
        ROS_ERROR("Unable to find [%s] visual", visualName.c_str());
    }
  }

  if (this->timer.GetElapsed() < gazebo::common::Time(1.0))
    return;

  // Restart the timer.
  this->timer.Reset();
  this->timer.Start();

  std::lock_guard<std::mutex> lock(this->mutex);

  // Update the visuals.
  for (auto visual : this->visuals)
  {
    std_msgs::ColorRGBA color;
    color.a = 0.0;

    auto name = visual->Name();
    auto delim = name.rfind("/");
    auto shortName = name.substr(delim + 1);

    if (shortName.find(this->shape) != std::string::npos)
      color = this->kColors[this->color];

    ignition::math::Color gazeboColor(color.r, color.g, color.b, color.a);

    visual->SetAmbient(gazeboColor);
    visual->SetDiffuse(gazeboColor);
  }
}

//////////////////////////////////////////////////
bool PlacardPlugin::ChangeSymbol(std_srvs::Trigger::Request &_req,
  std_srvs::Trigger::Response &_res)
{
  {
    std::lock_guard<std::mutex> lock(this->mutex);
    this->ShuffleShape();
    this->ShuffleColor();
  }

  _res.message = "New symbol: " + this->color + " " + this->shape;
  _res.success = true;
  return _res.success;
}

//////////////////////////////////////////////////
void PlacardPlugin::ShuffleColor()
{
  std::string newColor;
  do
  {
    auto iterColor = this->kColors.begin();
    std::advance(iterColor,
               ignition::math::Rand::IntUniform(0, this->kColors.size() - 1));
    newColor = (*iterColor).first;
  }
  while (newColor == this->color);
  this->color = newColor;
}

//////////////////////////////////////////////////
void PlacardPlugin::ShuffleShape()
{
  std::string newShape;
  do
  {
    newShape =
      this->kShapes[ignition::math::Rand::IntUniform(0,
        this->kShapes.size() - 1)];
  }
  while (newShape == this->shape);
  this->shape = newShape;
}

// Register plugin with gazebo
GZ_REGISTER_VISUAL_PLUGIN(PlacardPlugin)

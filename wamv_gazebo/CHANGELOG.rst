^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package wamv_gazebo
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1.0.0 (2019-02-28)
------------------
* Merge from default.
* Merge from symbols_dock_part2
* Merge from default.
* Merged in vrx (pull request #68)
  Rename vmrc to vrx
  Approved-by: Brian Bingham <briansbingham@gmail.com>
* Custom tweaks
* More leftovers.
* Rename vmrc to vrx.
* assembling pieces for stationkeeping
* Merged in urdf_easy (pull request #62)
  Simplify urdf
  Approved-by: Brian Bingham <briansbingham@gmail.com>
* Simplify urdf files.
* Locking the WAM-V conditionally.
* Playing with locking and releasing.
* Changed from buoyancy calculation method
* Decrease sensor noise to more clearly allow debugging of the simulation.
* Add the pinger plugin to the wamv_gazebo package.
  The wamv_gazebo_sensors.urdf file has been modified to add support for the pinger plugin.
* add missing dependencies
* Create perception.launch and lock the WAM-V.
* removing static tags so vessel is freee to move
* Contributors: Brian Bingham <briansbingham@gmail.com>, Carlos Aguero, Carlos Aguero <caguero@osrfoundation.org>, Carlos Agüero <cen.aguero@gmail.com>, Jonathan Wheare <jonathan.wheare@flinders.edu.au>, chapulina <burajiru.no.chapulina@gmail.com>

0.3.2 (2018-10-08)
------------------
* Include jrivero as maintainer of the ROS packages
* Contributors: Jose Luis Rivero <jrivero@osrfoundation.org>

0.3.1 (2018-10-05)
------------------

0.3.0 (2018-09-28)
------------------
* Tweak
* vrx metapackage and spring cleaning.
* Static model and fog.
* trying to get wamv to be static using a fixed joint
* Merge from default.
* reverting example rviz config back to original to be consistent with existing tutorial
* adding launch/config files for running the example
* adding examples to the sensors tutorial for the T and X propulsion configuration
* Create a standard sensor configuration for VRX.
* Merged in 3dlaser (pull request #41)
  Add 3D laser xacro
  Approved-by: Carlos Agüero <cen.aguero@gmail.com>
* Merge from default.
* Merged in holonomic-example-refactored (pull request #40)
  Holonomic example refactored
  Approved-by: Carlos Agüero <cen.aguero@gmail.com>
* Add 3D laser xacro
* Refactor thruster layout customization
* Enable on/off arguments for sensors xacro
* Fix multibeam laser xacro
* adding examples for T and X thruster configurations - accessible as args to sandisland.launch. Prototype - too much redundancy in the various urdf.xacro file hierarchy, but functional.
* Tabs -> spaces
* Initial style pass
* props now spinning, removed old method of thrust implementation, removed custome UsvDrive message
* working prototype - next remove old method
* increment - builds, but need to go home
* Add changelog.
* Merge from default
* Removing superfluous SDF for thrust
* More tweaks.
* Merge from default.
* Merged in sensor-examples (pull request #12)
  Add sensor macros and example
  Approved-by: Carlos Agüero <cen.aguero@gmail.com>
* Add multibeam to example sensor urdf
* Add simple visuals for sensors
* Move multibream -> multibeam
* Remove unneeded robot_description param from localization_example.launch
* Add optical frame for proper camera visualization
* Install config/launch files
* Merge default into sensor-examples
* Simplify wamv_gazebo macros
* Simplify xacro macros
* Refactor wind plugin.
* Split the wamv xacro file.
* More modular model with spinning propellers.
* Add example rviz config/launch
* Tweak
* Tweak
* Add sensor macros and example localization config
* Fix issues after wamv_gazebo migration
* Boostrap wamv_gazebo
* Contributors: Brian Bingham <briansbingham@gmail.com>, Carlos Aguero, Carlos Agüero <caguero@osrfoundation.org>, Carlos Agüero <cen.aguero@gmail.com>, Kevin Allen <kallen@osrfoundation.org>

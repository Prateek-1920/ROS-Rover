# ROVER URDF



## DESCRIPTON OF TASK
1. A rover named "robot" has been made using URDF.
2. urdf folder contains gazebo and xacro files which have plugins for different sensors and physical appearance of robot respectively.
3. Sensors used are -
```
Camera
Lidar
GPS
IMU
Z2 Stereo Camera

```
4. Skid-steer plugin is used to move the robot using teleop-key command.
5. Empty world file is made to provide a template world in gazebo.
6. Launch files are made to launch robot, gazebo and rviz.

## LIST OF ROSTOPICS PUBLISHED
```
/camera/parameter_descriptions
/camera/parameter_updates
/camera/rgb/camera_info
/camera/rgb/image_raw
/camera/rgb/image_raw/compressed
/camera/rgb/image_raw/compressed/parameter_descriptions
/camera/rgb/image_raw/compressed/parameter_updates
/camera/rgb/image_raw/compressedDepth
/camera/rgb/image_raw/compressedDepth/parameter_descriptions
/camera/rgb/image_raw/compressedDepth/parameter_updates
/camera/rgb/image_raw/theora
/camera/rgb/image_raw/theora/parameter_descriptions
/camera/rgb/image_raw/theora/parameter_updates
/clicked_point
/clock
/cmd_vel
/gazebo/link_states
/gazebo/model_states
/gazebo/parameter_descriptions
/gazebo/parameter_updates
/gazebo/performance_metrics
/gazebo/set_link_state
/gazebo/set_model_state
/gps/fix
/gps/fix/position/parameter_descriptions
/gps/fix/position/parameter_updates
/gps/fix/status/parameter_descriptions
/gps/fix/status/parameter_updates
/gps/fix/velocity/parameter_descriptions
/gps/fix/velocity/parameter_updates
/gps/fix_velocity
/imu
/initialpose
/joint_states
/move_base_simple/goal
/multisense_sl/camera/left/camera_info
/multisense_sl/camera/left/image_raw
/multisense_sl/camera/left/image_raw/compressed
/multisense_sl/camera/left/image_raw/compressed/parameter_descriptions
/multisense_sl/camera/left/image_raw/compressed/parameter_updates
/multisense_sl/camera/left/image_raw/compressedDepth
/multisense_sl/camera/left/image_raw/compressedDepth/parameter_descriptions
/multisense_sl/camera/left/image_raw/compressedDepth/parameter_updates
/multisense_sl/camera/left/image_raw/theora
/multisense_sl/camera/left/image_raw/theora/parameter_descriptions
/multisense_sl/camera/left/image_raw/theora/parameter_updates
/multisense_sl/camera/left/parameter_descriptions
/multisense_sl/camera/left/parameter_updates
/multisense_sl/camera/right/camera_info
/multisense_sl/camera/right/image_raw
/multisense_sl/camera/right/image_raw/compressed
/multisense_sl/camera/right/image_raw/compressed/parameter_descriptions
/multisense_sl/camera/right/image_raw/compressed/parameter_updates
/multisense_sl/camera/right/image_raw/compressedDepth
/multisense_sl/camera/right/image_raw/compressedDepth/parameter_descriptions
/multisense_sl/camera/right/image_raw/compressedDepth/parameter_updates
/multisense_sl/camera/right/image_raw/theora
/multisense_sl/camera/right/image_raw/theora/parameter_descriptions
/multisense_sl/camera/right/image_raw/theora/parameter_updates
/multisense_sl/camera/right/parameter_descriptions
/multisense_sl/camera/right/parameter_updates
/odom
/rosout
/rosout_agg
/scan
/tf
/tf_static
```
## IMAGES OF TASK
![CAMERA](https://user-images.githubusercontent.com/117933472/217905740-20bda450-52ab-4b44-901a-4bd8182d693e.png)
![ROVER](https://user-images.githubusercontent.com/117933472/217905775-102c0c01-1800-4941-ae0b-e69718205885.png)
![TELEOP](https://user-images.githubusercontent.com/117933472/217905789-59464396-c36d-4b51-b90d-ab467a27ef29.png)

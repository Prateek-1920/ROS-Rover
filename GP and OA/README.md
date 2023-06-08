## DESCRIPTION OF TASK
The rover has to move from one GPS co-ordinate to another.
This has to be done using CPP and Object oriented programming.

## APPROACH UNDERTAKEN
1. Initial GPS co-ordinates are received from GPS. Target co-ordinates are taken from the user.
2. Bearing angle is calculated.
3. A fucntion is used to get orientation of rover from /imu topic. Quaternion data is converted to euler using transform library and the value of YAW is stored in a variable.
4. A loop is made to publish angular velocity to /cmd_vel. Difference between bearing angle and yaw is calculated and if the difference is less than 0.01, the loop breaks and the rover stops rotating. It is now inline with the co-ordinates.
5. Another loop is made to make the rover move linearly by publishing linear velocity to /cmd_vel. Continuous distance is calculated between current position and destination and if it is less than 1.0m, the rover stops.

## TOPICS PUBLISHED
```
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
/joint_states
/odom
/rosout
/rosout_agg
/scan
/tf
/tf_static
```

## NODES USED
```
/move
/gazebo
/gazebo_gui
/joint_state_publisher
/robot_state_publisher
/rosout
```

## RQT GRAPH
![ATT task rqt](https://user-images.githubusercontent.com/117933472/221683925-e13c2555-b620-4815-aacd-c20b4518d36c.png)


## YOUTUBE VIEDO LINK
https://youtu.be/9hdL1kPecL8

https://youtu.be/jnuriYRAy8A



#ifndef _ILQR_PLANNER_H_
#define _ILQR_PLANNER_H_

#include <vector>

#include <ilqr_loco/TrajExecAction.h>
#include <tf/transform_datatypes.h>
#include "iLQG_mpc.c"

// #include "iLQR_mpc.c" //TODO integrate generated c-code
//iLQR_mpc(double x_cur[10], double x_des[6], double obs[2], int T);

// Note that the inputs to this function can be whatever is convenient for client
ilqr_loco::TrajExecGoal iLQR_gen_traj(nav_msgs::Odometry x_cur, std::vector<double> x_des,
                                        std_msgs::Float32MultiArray obstacle_pos, int T)
{
  // TODO pre-process inputs as necessary, put into C-style arrays

  // TODO convert orientation from quaternion

  double theta = tf::getYaw(x_cur.pose.pose.orientation);

  double x0[10] = {x_cur.pose.pose.position.x, x_cur.pose.pose.position.y, theta,
                          x_cur.twist.twist.linear.x, x_cur.twist.twist.linear.y,
                          x_cur.twist.twist.angular.z,
                          x_cur.twist.twist.linear.x, 0, 0, 0}

  double* xDes = &x_des[0];
  double* Obs = {(double)obstacle_pos.data[0],(double)obstacle_pos.data[1]};

  // TODO what is the output type?
  int N = T+1;
  int n = 10;
  int m = 2;

  struct trajectory Traj;
  Traj.x = malloc(n*N*sizeof(double));
  Traj.u = malloc(m*(N-1)*sizeof(double));
  // traj[0]: states, traj[1]: controls
  plan_trajectory(x0,xDes,Obs,T,&Traj);

  ilqr_loco::TrajExecGoal goal;
  // TODO put output from c-code into action message

  return goal;
}

#endif

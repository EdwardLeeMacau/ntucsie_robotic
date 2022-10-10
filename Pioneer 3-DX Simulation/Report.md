# Pioneer 3-DX Simulation 1 (R11922001)

## Description

### Part C: Keyboard Control

In this part, I simply read key press down events from `stdin` by `ArKeyHandler`, and do one of the action at a single time.

- UpArrow: Translate robot by setting velocity as (350, 350)
- DownArrow: Translate robot by setting velocity as (-350, -350)
- LeftArrow: Rotate robot by setting velocity as (-150, 150)
- RightArrow: Rotate robot by setting velocity as (150, -150)
- Release Key: Slow down the robot by setting velocity as (0, 0)

For the failed cases, I tried bind actions to key handler by `Functor` (function pointer), but `ArKeyHandler` cannot register the key `-1`, which means no available key in `stdin`. Thus, I choose to read key repeatedly in program main loop.

---

### Part D: Collision Avoidance

In this part, I detect obstacle and slowing down speed if need by `ArSonarDevice`. The result of detection affects the translational velocity. Using the velocity configured in Part-C as initial setting, once the sonar detected an obstacle in front of (or behind) the robot, the velocity is clipped down.

---

### Part E: Robot Pose and Odometry

I treat this problem as a control system issue, which requires to minimize the value of $||\vec{p}_t - \vec{p}_r||_2$, $\vec{p}_t$ represents target pose and $\vec{p}_r$ represents robot's current pose.

Q: Is the odometric pose the same as the true pose? Why?
> The odometric pose is not the same as the true pose, because the odometric pose is the related pose to initial point, and it prones to accumulated error. It needs extra calibrations by more observations, or suppress the accumulated error by a higher resolution odometric.

I have implemented P-control with deadband and limiter, grids route planner to solve the problem. The steps to achieve the goal as briefly described below:

1. Adjust the odometry, such that we can learn the related position between robot and target
2. Rotate the robot, such that it faces to X-axis by minimizing the error $||p_{\theta,r} - 0||_2$
3. Go ahead to minimize the error $||p_{x,t} - p_{x,r}||_2$
4. Rotate the robot, such that it faces to Y-axis by minimizing the error $||p_{\theta,r} - 90 \degree||_2$
5. Go ahead to minimize the error $||p_{y,t} - p_{y,r}||_2$
6. Rotate the robot, such that it faces to target position by minimizing the error $||p_{\theta,t} - p_{\theta,r}||_2$

In practise, there are always some tolerance between target and robot's pose due to control issues. I print the tolerance after robot stopped moving. Tolerance might differ for each experiments.

```plain
Type target pose (x, y, th): 7 5 1.57

Initial pose: { .x: 5090, .y: 3580, .th: 30.9397 (deg) }
Target pose: { .x: 7000, .y: 5000, .th: 90.0000 (deg) }

Achieve pose: { .x: 6956.83, .y: 4974.19, .th: 88.7093 (deg) }
Tolerance: { .x: 43.1669, .y: 25.8142, .th: 1.29068 (deg) }
```

---

## Demo Video

- [Part-C](https://drive.google.com/file/d/1bm51SoW73ieSob_6-JM0rcFFrWIuFm-O/view?usp=sharing)
- [Part-D](https://drive.google.com/file/d/19IAufmQN-TTqiO4HKC4qFU1tYHsrLBUO/view?usp=sharing)
- [Part-E](https://drive.google.com/file/d/1Ei65CbHHp4wj69WaJnWB7cGCCFDWB2sI/view?usp=sharing)

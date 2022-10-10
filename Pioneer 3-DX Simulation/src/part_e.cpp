#include <cstdlib>
#include <iostream>

#include "Aria.h"

#define min(a, b) ((a) < (b)) ? (a) : (b)

#define ROTATION_DEADBAND 3.0f
#define ROTATION_SPEED 30.0f
#define TRANSLATION_DEADBAND 30.0f
#define TRANSLATION_MAX_SPEED 160.0f

int main(int argc, char **argv)
{
	Aria::init();

	// Start of controlling
	ArRobot robot;
	ArSonarDevice sonar;
	ArSimpleConnector connector(&argc, argv);
	ArPose target;
	double x, y, th, diff;
	double v, r;

	// Read target pose from stdin, and store it to 'ArPose'
	std::cout << "Type target pose (x, y, th): ";
	std::cin >> x >> y >> th;

	x *= 1000;
	y *= 1000;
	th = th * 180 / 3.14;

	target.setPose(x, y, th);

	robot.addRangeDevice(&sonar);
	if (!connector.connectRobot(&robot)){
		printf("Could not connect to robot... exiting\n");
		Aria::shutdown();
		Aria::exit(1);
	}

	robot.comInt(ArCommands::ENABLE, 1);
	robot.runAsync(false);

	// Set robot odometry
	robot.moveTo(ArPose(5090, 3580, 30.9397));

	// Show obstacle
	r = sonar.currentReadingPolar(-180, 180, &th);
	// std::cout << "obstacle: { .r: " << r << ", th: " << th << " }" << std::endl;
	// std::cout << "obstacle: { .x: " << robot.getX() + r * ArMath::cos(th) << ", y: " << robot.getX() + r * ArMath::sin(th) << " }" << std::endl;

	// x = target.getX() - robot.getX();
	// y = target.getY() - robot.getY();
	// th = ArMath::radToDeg(atan2(y, x));	// Range: (-180, 180]

	// std::cout << "x: " << x << std::endl
	//           << "y: " << y << std::endl
	//           << "atan2(): " << th << " (deg) " << std::endl;

	// std::cout << "robot.pose = (" << robot.getX() << ", " << robot.getY() << ", " << robot.getTh() << ")" << std::endl;

	// Rotate to have pose.theta = 0
	// P Controller with deadband (1.0f) and limiter (30)
	while (ArMath::fabs(diff = robot.getTh()) >= ROTATION_DEADBAND) {
		v = ROTATION_SPEED;
		robot.lock();
		if (diff > 0) {
			robot.setVel2(v, -v);
		} else {
			robot.setVel2(-v, v);
		}
		robot.unlock();
		ArUtil::sleep(100);
	}

	robot.setVel2(0, 0);
	ArUtil::sleep(100);
	// std::cout << "robot.pose = (" << robot.getX() << ", " << robot.getY() << ", " << robot.getTh() << ")" << std::endl;

	// Rotate to have diff.x = epsilon
	// P Controller with deadband (50.0f) and limiter (160)
	while (ArMath::fabs(diff = target.getX() - robot.getX()) >= TRANSLATION_DEADBAND) {
		v = min(ArMath::fabs(diff), 160.0f);
		robot.lock();
		if (diff > 0) {
			robot.setVel2(v, v);
		} else {
			robot.setVel2(-v, -v);
		}
		robot.unlock();
		ArUtil::sleep(100);
	}

	robot.setVel2(0, 0);
	ArUtil::sleep(100);
	// std::cout << "robot.pose = (" << robot.getX() << ", " << robot.getY() << ", " << robot.getTh() << ")" << std::endl;

	// Rotate to have pose.theta = 90
	// P Controller with deadband (2.0f) and limiter (30)
	while (ArMath::fabs(diff = 90 - robot.getTh()) >= ROTATION_DEADBAND) {
		v = ROTATION_SPEED;
		robot.lock();
		if (diff < 0) {
			robot.setVel2(v, -v);
		} else {
			robot.setVel2(-v, v);
		}
		robot.unlock();
		ArUtil::sleep(100);
	}

	robot.setVel2(0, 0);
	ArUtil::sleep(100);
	// std::cout << "robot.pose = (" << robot.getX() << ", " << robot.getY() << ", " << robot.getTh() << ")" << std::endl;

	// Rotate to have diff.y = epsilon
	// P Controller with deadband (50.0f) and limiter (160)
	while (ArMath::fabs(diff = target.getY() - robot.getY()) >= TRANSLATION_DEADBAND) {
		v = min(ArMath::fabs(diff), 160.0f);
		robot.lock();
		if (diff > 0) {
			robot.setVel2(v, v);
		} else {
			robot.setVel2(-v, -v);
		}
		robot.unlock();
		ArUtil::sleep(100);
	}

	robot.setVel2(0, 0);
	ArUtil::sleep(100);
	// std::cout << "robot.pose = (" << robot.getX() << ", " << robot.getY() << ", " << robot.getTh() << ")" << std::endl;

	// Rotate to have pose.theta = 90
	// P Controller with deadband (2.0f) and limiter (30)
	while (ArMath::fabs(diff = target.getTh() - robot.getTh()) >= ROTATION_DEADBAND) {
		v = ROTATION_SPEED;
		robot.lock();
		if (diff < 0) {
			robot.setVel2(v, -v);
		} else {
			robot.setVel2(-v, v);
		}
		robot.unlock();
		ArUtil::sleep(100);
	}

	robot.setVel2(0, 0);
	ArUtil::sleep(100);

	// Update x, y after moved
	x = target.getX() - robot.getX();
	y = target.getY() - robot.getY();
	th = target.getTh() - robot.getTh();

	std::cout << "robot.pose = { .x: " << robot.getX() << ", .y: " << robot.getY() << ", .th: " << robot.getTh() << "}" << std::endl;
	std::cout << "diff = { .x: " << x << ", .y: " << y << ", .th: " << th << " }" << std::endl;

	// End of controling
	Aria::shutdown();

	Aria::exit(0);

	return EXIT_SUCCESS;
}

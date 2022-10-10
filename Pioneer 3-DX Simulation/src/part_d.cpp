#include <cstdlib>

#include "Aria.h"

int main(int argc, char **argv)
{
	Aria::init();

	// Start of controlling
	ArRobot robot;
	ArSonarDevice sonar;
	ArSimpleConnector connector(&argc, argv);
	ArKeyHandler keyHandler;
	double angle;
	double range;
	double lv, rv;

	// robot.radius = 250.0
	// printf("robot.radius = %.2f\n", robot.getRobotRadius());

	robot.addRangeDevice(&sonar);
	if (!connector.connectRobot(&robot)){
		printf("Could not connect to robot... exiting\n");
		Aria::shutdown();
		Aria::exit(1);
	}

	robot.comInt(ArCommands::ENABLE, 1);
	robot.runAsync(false);

	Aria::setKeyHandler(&keyHandler);
	// ArRobot contains an exit action for the Escape key. It also
	// stores a pointer to the keyhandler so that other parts of the program can
	// use the same keyhandler.
	robot.attachKeyHandler(&keyHandler);

	// Sleep a while and let the robot move
	bool run = true;
	while (run) {
		robot.lock();
		switch (keyHandler.getKey()) {
			case -1:
				lv = rv = 0;
				break;

			case ArKeyHandler::UP:
				range = sonar.currentReadingPolar(-45, 45, &angle) - robot.getRobotRadius() * 3;
				lv = rv = (range > 50) ? 350 : std::max(0.0, range);
				break;

			case ArKeyHandler::DOWN:
				range = sonar.currentReadingPolar(135, 215, &angle) - robot.getRobotRadius() * 3;
				lv = rv = (range > 50) ? -350 : std::min(0.0, -range);
				break;

			case ArKeyHandler::LEFT:
				lv = -150; rv = 150;
				break;

			case ArKeyHandler::RIGHT:
				lv = 150, rv = -150;
				break;

			case ArKeyHandler::ESCAPE:
				run = false;
				break;
		}
		robot.setVel2(lv, rv);
		robot.unlock();
		// printf("%f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
		// printf("%f %f\n", robot.sonar.currentReadingPolar(-45, 45), robot.sonar.currentReadingPolar(135, 225));
		ArUtil::sleep(100);
	}

	// End of controling
	Aria::shutdown();

	Aria::exit(0);

	return EXIT_SUCCESS;
}

#include <cstdlib>

#include "Aria.h"

int main(int argc, char **argv)
{
	ArRobot robot;
	ArSonarDevice sonar;

	robot.addRangeDevice(&sonar);

	Aria::init();

	ArSimpleConnector connector(&argc,argv);

	if (!connector.connectRobot(&robot)){
		printf("Could not connect to robot... exiting\n");
		Aria::shutdown();
		Aria::exit(1);
	}

	robot.comInt(ArCommands::ENABLE, 1);

	robot.runAsync(false);

	// Used to perform actions when keyboard keys are pressed
	ArKeyHandler keyHandler;

	Aria::setKeyHandler(&keyHandler);

	// ArRobot contains an exit action for the Escape key. It also
	// stores a pointer to the keyhandler so that other parts of the program can
	// use the same keyhandler.
	robot.attachKeyHandler(&keyHandler);
	printf("You may press escape to exit\n");

	// Start of controlling

	// Sleep a while and let the robot move
	bool run = true;
	while (run) {
		robot.lock();
		switch (keyHandler.getKey()) {
			case -1:
				robot.setVel2(0, 0);
				break;

			case ArKeyHandler::UP:
				robot.setVel2(350, 350);
				break;

			case ArKeyHandler::DOWN:
				robot.setVel2(-350, -350);
				break;

			case ArKeyHandler::LEFT:
				robot.setVel2(-150, 150);
				break;

			case ArKeyHandler::RIGHT:
				robot.setVel2(150, -150);
				break;

			case ArKeyHandler::ESCAPE:
				run = false;
				break;
		}
		robot.unlock();
		printf("%f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
		ArUtil::sleep(100);
	}

	// End of controling
	Aria::shutdown();

	Aria::exit(0);

	return EXIT_SUCCESS;
}

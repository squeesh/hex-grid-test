#include "includes.h"

#include <time.h>
#include <GL/freeglut.h>


static Controller* curr_ctrl = NULL;
const int RESOLUTION[] = {1024, 768};

const int BOARD_WIDTH  = 20;
const int BOARD_HEIGHT = 13;

bool SHOW_FPS = true;

time_t seconds = time(NULL);
int fps = 0;
void display() {
    time_t curr_seconds = time(NULL);
	if(SHOW_FPS) {
		if(curr_seconds > seconds) {
			//std::cout << "FPS: " << fps << std::endl;

			RenderController::print_string.clear();
			RenderController::print_string += std::string("FPS: ");
			std::stringstream temp;
			temp << fps;
			RenderController::print_string += temp.str();

			seconds = curr_seconds;
			fps = 0;
		}

		fps++;
	}

	curr_ctrl->render();
	glutSwapBuffers();
	
}

void reshape(int width, int height) {
	curr_ctrl->resize(width, height);
}

void key_pressed(unsigned char key, int x, int y) {
    curr_ctrl->keyboard_event(GlobalConsts::KEY_DOWN, key, x, y);
}

void key_released(unsigned char key, int x, int y) {
    curr_ctrl->keyboard_event(GlobalConsts::KEY_UP, key, x, y);
}

void mouse_click(int button, int state, int x, int y) {
    switch(state) {
        case GLUT_DOWN:
            curr_ctrl->mouse_event(GlobalConsts::MOUSE_DOWN, button, x, curr_ctrl->height - y);
            break;
	case GLUT_UP:
	    curr_ctrl->mouse_event(GlobalConsts::MOUSE_UP, button, x, curr_ctrl->height - y);
		break;

    }
}

void mouse_drag(int x, int y) {
    curr_ctrl->mouse_event(GlobalConsts::MOUSE_DRAG, -1, x, curr_ctrl->height - y);
}

void timer(int value) {
    curr_ctrl->tick();
    glutTimerFunc(GlobalConsts::SLEEP_TIME, timer, value);
}


int main(int argc, char** argv) {
    Py_Initialize();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(RESOLUTION[0], RESOLUTION[1]);
	glutCreateWindow("Hex Grid Thing...");

	curr_ctrl = Controller::py_get_controller();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutTimerFunc(GlobalConsts::SLEEP_TIME, timer, 0);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_pressed);
	glutKeyboardUpFunc(key_released);
	glutMouseFunc(mouse_click);
	glutMotionFunc(mouse_drag);

	curr_ctrl->init_gl(RESOLUTION[0], RESOLUTION[1]);
	curr_ctrl->init_board();
	//curr_ctrl->init_board(BOARD_WIDTH, BOARD_HEIGHT);

	glutMainLoop();

	Py_Finalize();

	return 0;
}


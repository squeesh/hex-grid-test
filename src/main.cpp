#include "includes.h"

#include <time.h>
#include <GL/freeglut.h>


static Controller* curr_ctrl = NULL;
const int RESOLUTION[] = {1024, 768};

const int BOARD_WIDTH  = 20;
const int BOARD_HEIGHT = 13;

int SLEEP_TIME = 10;

bool SHOW_FPS = true;

time_t seconds = time(NULL);
int fps = 0;
void display() {
    time_t curr_seconds = time(NULL);
	if(SHOW_FPS) {
		if(curr_seconds > seconds) {
			std::cout << "FPS: " << fps << std::endl;

			curr_ctrl->print_string.clear();
			curr_ctrl->print_string += std::string("FPS: ");
			std::stringstream temp;
			temp << fps;
			curr_ctrl->print_string += temp.str();

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
    switch(key) {
        case 27:
            exit(0);
            break;
	case ' ':
	    curr_ctrl->print_flag = true;
        default:
	    curr_ctrl->key_down(key, x, y);
	    break;
    }
}

void key_released(unsigned char key, int x, int y) {
    curr_ctrl->key_up(key, x, y);
}

void mouse_click(int button, int state, int x, int y) {
    switch(state) {
        case GLUT_DOWN:
		if(button == GlobalConsts::MOUSE_LEFT) {
			curr_ctrl->mouse_left_click(x, y);
		} else if(button == GlobalConsts::MOUSE_RIGHT) {
			curr_ctrl->mouse_right_click(x, y);
		} else if(button == GlobalConsts::MOUSE_SCROLL_UP) {
			curr_ctrl->mouse_scroll_up(x, y);
		} else if(button == GlobalConsts::MOUSE_SCROLL_DOWN) {
			curr_ctrl->mouse_scroll_down(x, y);
		}
		break;
	case GLUT_UP:
		if(button == GlobalConsts::MOUSE_LEFT) {
			curr_ctrl->mouse_left_release(x, y);
		} else if(button == GlobalConsts::MOUSE_RIGHT) {
			curr_ctrl->mouse_right_release(x, y);
		}
		break;
	
    }
}

void mouse_drag(int x, int y) {
	//std::cout << "drag: " << x << " | " << y << std::endl;

	if(curr_ctrl->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["down"]) {
		curr_ctrl->mouse_left_drag(x, y);
	} else if(curr_ctrl->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["down"]) {
		curr_ctrl->mouse_right_drag(x, y);
	}
}

void timer(int value) {
    curr_ctrl->tick();
    glutTimerFunc(SLEEP_TIME, timer, value);
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(RESOLUTION[0], RESOLUTION[1]);
	glutCreateWindow("Hex Grid Thing...");

	Py_Initialize();

	curr_ctrl = Controller::get_controller();
	
	
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutTimerFunc(SLEEP_TIME, timer, 0);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_pressed);
	glutKeyboardUpFunc(key_released);
	glutMouseFunc(mouse_click);
	glutMotionFunc(mouse_drag);

	curr_ctrl->init_gl(RESOLUTION[0], RESOLUTION[1]);
	curr_ctrl->py_init_board();
	//curr_ctrl->init_board(BOARD_WIDTH, BOARD_HEIGHT);

	glutMainLoop();

	Py_Finalize();

	return 0;
}


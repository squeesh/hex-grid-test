#include "includes.h"

#include <time.h>
#include <GL/freeglut.h>


static Controller* curr_ctrl = NULL;
static RenderController* curr_rend_ctrl = NULL;
//const int RESOLUTION[] = {1024, 768};

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

	curr_rend_ctrl->render();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	curr_rend_ctrl->resize(width, height);
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
            curr_ctrl->mouse_event(GlobalConsts::MOUSE_DOWN, button, x, y);
            break;
	case GLUT_UP:
	    curr_ctrl->mouse_event(GlobalConsts::MOUSE_UP, button, x, y);
		break;

    }
}

void mouse_drag(int x, int y) {
    curr_ctrl->mouse_event(GlobalConsts::MOUSE_DRAG, -1, x, y);
}

void timer(int value) {
    curr_ctrl->tick();
    glutTimerFunc(GlobalConsts::SLEEP_TIME, timer, value);
}

/*void timer() {
    std::chrono::milliseconds dura(GlobalConsts::SLEEP_TIME);

    while(!curr_ctrl->kill_threads) {
        //std::cout << "Tick..." << std::endl;
        curr_ctrl->tick();
        std::this_thread::sleep_for(dura);
    }

    std::cout << "killing thread..." << std::endl;

    //glutTimerFunc(GlobalConsts::SLEEP_TIME, timer, value);
}*/


void close_window() {
    std::cout << "window closing..." << std::endl;
    glutLeaveMainLoop();
    delete curr_ctrl;
}


int main(int argc, char** argv) {
    Py_Initialize();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GlobalConsts::SCREEN_WIDTH, GlobalConsts::SCREEN_HEIGHT);
	glutCreateWindow("Hex Grid Thing...");

	curr_ctrl = Controller::py_get_controller();
	curr_rend_ctrl = RenderController::get_render_controller();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutTimerFunc(GlobalConsts::SLEEP_TIME, timer, 0);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_pressed);
	glutKeyboardUpFunc(key_released);
	glutMouseFunc(mouse_click);
	glutMotionFunc(mouse_drag);
    glutCloseFunc(close_window);

	curr_rend_ctrl->init_gl(GlobalConsts::SCREEN_WIDTH, GlobalConsts::SCREEN_HEIGHT);
	curr_ctrl->init_board();
	//curr_ctrl->init_board(BOARD_WIDTH, BOARD_HEIGHT);

    //curr_ctrl->timer_thread = new std::thread(timer);

	glutMainLoop();

	//Py_Finalize();

	return 0;
}


#include "includes.h"

#include <GL/freeglut.h>

Renderable::Renderable() {
	RenderController* curr_rend_ctrl = RenderController::get_render_controller();
	curr_rend_ctrl->register_renderable(this);
}


//-----------------------------------------

RenderController* RenderController::curr_rend_ctrl = NULL;
std::string RenderController::print_string = std::string();


RenderController::RenderController() {
	this->scroll_map[GlobalConsts::LEFT]    = false;
	this->scroll_map[GlobalConsts::RIGHT]   = false;
	this->scroll_map[GlobalConsts::UP]      = false;
	this->scroll_map[GlobalConsts::DOWN]    = false;

	this->renderables = new std::vector< Renderable* >();

	this->width = 0;
	this->height = 0;

	this->x_offset = 0.0;
	this->y_offset = 0.0;
	this->zoom = GlobalConsts::START_ZOOM;
	this->view_range = GlobalConsts::START_VIEW_RANGE;
	this->rotation   = GlobalConsts::START_ROTATION;
}


void RenderController::init_gl(long width, long height) {
	this->width = width;
	this->height = height;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK,  GL_LINE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, this->width/((double) this->height),-1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	//glEnable(GL_LINE_SMOOTH);
	glLineWidth(5);


	//glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

}

void RenderController::resize(long width, long height) {
        this->width = width;
	this->height = height;

        if(this->height == 0) {
            this->height = 1;
	}

        glViewport(0, 0, this->width, this->height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, this->width/((double) this->height), 0.1, 1000.0);
        glMatrixMode(GL_MODELVIEW);
}

RenderController* RenderController::get_render_controller() {
	if(!RenderController::curr_rend_ctrl) {
		std::cout << "Starting render ctrl..." << std::endl;
		RenderController::curr_rend_ctrl = new RenderController();
	}

	return RenderController::curr_rend_ctrl;
}


void RenderController::register_renderable(Renderable* renderable) {
	std::cout << "registering..." << std::endl;
	this->renderables->push_back(renderable);
}

void RenderController::render() {
	Controller* curr_ctrl = Controller::get_controller();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector< GLfloat >* color = new std::vector< GLfloat >();
	color->push_back(1);
	color->push_back(1);
	color->push_back(0);
	this->render_string(2, 20, this->print_string, color);
	delete color;

	glLoadIdentity();

	double eye_x = this->x_offset * 1.5 * GlobalConsts::COS_60;
	double eye_y = this->y_offset * 1.0 * GlobalConsts::SIN_60;

	gluLookAt(
	    eye_x, eye_y, 15 * this->zoom,
	    eye_x, eye_y, 0,
		0, 1, 0
	);

	// set the rotation point... since our "origin" kinda changes... we need to go to it, rotate, then go back
	glTranslatef(0.0, this->y_offset * GlobalConsts::SIN_60, 0.0);
	glRotatef(this->rotation, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -this->y_offset * GlobalConsts::SIN_60, 0.0);

	int neg_x_view = this->x_offset - this->view_range / 2.0 - GlobalConsts::BOARD_CHUNK_SIZE;
	int pos_x_view = this->x_offset + this->view_range / 2.0;
	int neg_y_view = this->y_offset - this->view_range / 2.0 - GlobalConsts::BOARD_CHUNK_SIZE;
	int pos_y_view = this->y_offset + this->view_range / 2.0;

	if(neg_y_view < 0) {
		neg_y_view += (-neg_y_view) % GlobalConsts::BOARD_CHUNK_SIZE;
	} else {
		neg_y_view -=  neg_y_view % GlobalConsts::BOARD_CHUNK_SIZE;
	}

	if(pos_y_view < 0) {
		pos_y_view += (-pos_y_view) % GlobalConsts::BOARD_CHUNK_SIZE;
	} else {
		pos_y_view -=  pos_y_view % GlobalConsts::BOARD_CHUNK_SIZE;
	}

	int temp = neg_x_view;

	if(neg_x_view < 0) {
		neg_x_view += (-neg_x_view) % GlobalConsts::BOARD_CHUNK_SIZE;
	} else {
		neg_x_view -=  neg_x_view % GlobalConsts::BOARD_CHUNK_SIZE;
	}

	if(pos_x_view < 0) {
		pos_x_view += (-pos_x_view) % GlobalConsts::BOARD_CHUNK_SIZE;
	} else {
		pos_x_view -=  pos_x_view % GlobalConsts::BOARD_CHUNK_SIZE;
	}

	//std::cout << "size: " << this->renderables->size() << std::endl;
	for(int i = 0; i < this->renderables->size(); i++) {
		this->renderables->at(i)->render(neg_x_view, pos_x_view, neg_y_view, pos_y_view);
	}
}



void RenderController::render_for_select() {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	gluPerspective(45.0, float(this->width)/float(this->height), 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// TODO: Do something about duplicated code...

	glLoadIdentity();

	double eye_x = this->x_offset * 1.5 * GlobalConsts::COS_60;
	double eye_y = this->y_offset * 1.0 * GlobalConsts::SIN_60;

	gluLookAt(
	    eye_x, eye_y, 15 * this->zoom,
	    eye_x, eye_y, 0,
		0, 1, 0
	);

	// set the rotation point... since our "origin" kinda changes... we need to go to it, rotate, then go back
	glTranslatef(0.0, this->y_offset * GlobalConsts::SIN_60, 0.0);
	glRotatef(this->rotation, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -this->y_offset * GlobalConsts::SIN_60, 0.0);

	int neg_x_view = this->x_offset - this->view_range / 2.0;
	int pos_x_view = this->x_offset + this->view_range / 2.0;
	int neg_y_view = this->y_offset - this->view_range / 2.0;
	int pos_y_view = this->y_offset + this->view_range / 2.0;

	Controller* curr_ctrl = Controller::get_controller();

        for(int i = neg_x_view; i <= pos_x_view; i++) {
            for(int j = neg_y_view; j <= pos_y_view; j++) {
                Hexagon* curr_hex = curr_ctrl->gameboard->get_hexagon_list()->at(i)->at(j);
                glLoadName(curr_hex->name);

                double x = i * 1.5 * GlobalConsts::COS_60;
                double y = j * 1.0 * GlobalConsts::SIN_60;

                if(i % 2 != 0) {
                    y += 0.5 * GlobalConsts::SIN_60;
                }

                curr_hex->render_for_select(x, y);

            }
        }

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK,  GL_LINE);

	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
}


Hexagon* RenderController::get_clicked_hex(double x, double y) {
	GLuint buff[64] = {0};
	GLint hits = 0;
	GLint view[4];

	glSelectBuffer(64, buff);

	glGetIntegerv(GL_VIEWPORT,view);

	glRenderMode(GL_SELECT);

	//Clearing the name's stack
	//This stack contains all the info about the objects
	glInitNames();

	//Now fill the stack with one element (or glLoadName will generate an error)
	glPushName(0);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(x, y, 1.0, 1.0, view);

	this->render_for_select();

	glPopMatrix();

	//get number of objects drawed in that area
	//and return to render mode
	hits = glRenderMode(GL_RENDER);
	glMatrixMode(GL_MODELVIEW);

	Hexagon* clicked_hex = NULL;

	Controller* curr_ctrl = Controller::get_controller();

	// TODO: buggy... needs to be fixed.
	for(int i = 0; i < hits; i++) {
		clicked_hex = curr_ctrl->get_hex_by_name(buff[i * 4 + 3]);
		if(clicked_hex) {
			break;
		}
	}

	return clicked_hex;
}


void RenderController::render_string(int x, int y, std::string curr_string) {
	std::vector< GLfloat >* default_color = new std::vector< GLfloat >();
	default_color->push_back(0);
	default_color->push_back(0);
	default_color->push_back(0);

	this->render_string(x, y, curr_string, default_color);

	delete default_color;
}

void RenderController::render_string(int x, int y, std::string curr_string, std::vector< GLfloat >* color) {
	glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();
			glOrtho(0, this->curr_rend_ctrl->width, 0, this->curr_rend_ctrl->height, -1.0f, 1.0f);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity();
				glColor3fv(color->data());
				glRasterPos2i(x, this->curr_rend_ctrl->height - y);
				glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)curr_string.data());
				glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	glPopMatrix();
}


void RenderController::tick() {
    if(this->scroll_map[GlobalConsts::LEFT]) {
        this->x_offset -= 0.5 * this->zoom;
    }

    if(this->scroll_map[GlobalConsts::RIGHT]) {
        this->x_offset += 0.5 * this->zoom;
    }

    if(this->scroll_map[GlobalConsts::UP]) {
        this->y_offset += 0.5 * this->zoom;
    }

    if(this->scroll_map[GlobalConsts::DOWN]) {
        this->y_offset -= 0.5 * this->zoom;
    }
}


void RenderController::set_scroll(char direction) {
	this->scroll_map[direction] = true;
}

void RenderController::clear_scroll(char direction) {
    this->scroll_map[direction] = false;
}


void RenderController::zoom_map(double zoom_amount) {
	if(zoom_amount > 1) {
		if(this->zoom < GlobalConsts::MAX_ZOOM) {
			this->zoom *= zoom_amount;
			this->view_range *= zoom_amount;
		}
	} else {
		if(GlobalConsts::MIN_ZOOM < this->zoom) {
			this->zoom *= zoom_amount;
			this->view_range *= zoom_amount;
		}
	}
}


double RenderController::get_zoom() {
	return this->zoom;
}


void RenderController::set_rotation(double rotation) {
	this->rotation = rotation;
}

double RenderController::get_rotation() {
	return this->rotation;
}


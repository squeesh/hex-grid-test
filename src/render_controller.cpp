#include "includes.h"


Renderable::Renderable() {
	RenderController* curr_rend_ctrl = RenderController::get_render_controller();
	curr_rend_ctrl->register_renderable(this);
}


//-----------------------------------------

RenderController* RenderController::curr_rend_ctrl = NULL;
std::string RenderController::print_string = std::string();


RenderController::RenderController() {
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector< GLfloat >* color = new std::vector< GLfloat >();
	color->push_back(1);
	color->push_back(1);
	color->push_back(0);
	curr_ctrl->render_string(2, 20, this->print_string, color);
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

	for(int i = 0; i < this->renderables->size(); i++) {
		this->renderables->at(i)->render(neg_x_view, pos_x_view, neg_y_view, pos_y_view);
	}
}


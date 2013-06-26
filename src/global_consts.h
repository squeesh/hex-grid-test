#ifndef GLOBAL_CONSTS_H
#define GLOBAL_CONSTS_H


class GlobalConsts {
	public:
        static const GLlong SLEEP_TIME;

		static const GLdouble COS_60;
		static const GLdouble SIN_60;

		static const char LEFT;
		static const char RIGHT;
		static const char UP;
		static const char DOWN;

		static const int MOUSE_LEFT;
		static const int MOUSE_MIDDLE;
		static const int MOUSE_RIGHT;
		static const int MOUSE_SCROLL_UP;
		static const int MOUSE_SCROLL_DOWN;

		static const int MOUSE_DOWN;
		static const int MOUSE_UP;
        static const int MOUSE_DRAG;

        static const int KEY_UP;
        static const int KEY_DOWN;

        static const int RENDER_LINES;
        static const int RENDER_TRIANGLES;

		static const GLlong SCREEN_WIDTH;
		static const GLlong SCREEN_HEIGHT;

		static const GLlong BOARD_WIDTH;
		static const GLlong BOARD_HEIGHT;

		static const GLlong BOARD_CHUNK_SIZE;

		static const GLdouble MIN_ZOOM;
		static const GLdouble MAX_ZOOM;
		static const GLdouble START_ZOOM;

		static const GLdouble START_ROTATION;

		static const GLdouble START_VIEW_RANGE;

		static const GLdouble MAX_PATHABLE_SLOPE;

		// TODO: this should be const...
		static std::map< const char*, std::vector< GLdouble >*, cmp_str> RENDER_TRAY_COORDS;

		static std::vector< GLdouble > COLOR_GREY;
};




#endif

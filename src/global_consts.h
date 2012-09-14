#ifndef GLOBAL_CONSTS_H
#define GLOBAL_CONSTS_H


class GlobalConsts {
	public:
		static const double COS_60;
		static const double SIN_60;

		static const char LEFT;
		static const char RIGHT;
		static const char UP;
		static const char DOWN;

		static const int MOUSE_LEFT;
		static const int MOUSE_MIDDLE;
		static const int MOUSE_RIGHT;
		static const int MOUSE_SCROLL_UP;
		static const int MOUSE_SCROLL_DOWN;

		static const int RENDER_LINES;
		static const int RENDER_TRIANGLES;

		static const long BOARD_WIDTH;
		static const long BOARD_HEIGHT;

		static const double MIN_ZOOM;
		static const double MAX_ZOOM;
		static const double START_ZOOM;

		static const double START_ROTATION;

		static const double START_VIEW_RANGE;

		static const double MAX_PATHABLE_SLOPE;

		// TODO: this should be const...
		static std::map< const char*, std::vector< double >*, cmp_str> RENDER_TRAY_COORDS;
};




#endif

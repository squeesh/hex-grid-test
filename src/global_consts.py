# This file is loaded by main.exe
#    Careful referencing things from here that reference Controller.
#    You will get circular imports...

class GlobalConsts(object):
    PATH_SHOW_SEARCH    = True

    GENERATE_HILLS = True
    GENERATE_MOUNTAINS = GENERATE_HILLS

    BOARD_HEIGHT = 78
    BOARD_WIDTH  = 120
#    BOARD_HEIGHT = int(BOARD_HEIGHT * 0.5)
#    BOARD_WIDTH  = int(BOARD_WIDTH  * 0.5)

    BOARD_CHUNK_SIZE = 10 # MUST be in multiples of 2

    MIN_ZOOM = 0.5
    MAX_ZOOM = 17.0

    START_ZOOM = BOARD_WIDTH / 20.0
    START_ROTATION  = -30.0
    START_VIEW_RANGE = BOARD_WIDTH * 1.5

    MAX_PATHABLE_SLOPE = 2.0


    ####################################
    # don't touch

    LEFT  = 'L'
    RIGHT = 'R'
    UP    = 'U'
    DOWN  = 'D'

    # Defined by glut
    MOUSE_LEFT          = 0
    MOUSE_MIDDLE        = 1
    MOUSE_RIGHT         = 2
    MOUSE_SCROLL_UP     = 3
    MOUSE_SCROLL_DOWN   = 4

    RENDER_LINES     = 0
    RENDER_TRIANGLES = 1



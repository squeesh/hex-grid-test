# This file is loaded by main.exe
#    Careful referencing things from here that reference Controller.
#    You will get circular imports...

class GlobalConsts(object):
    PATH_SHOW_SEARCH    = True

    BOARD_HEIGHT = 78
    BOARD_WIDTH  = 120

    MIN_ZOOM = 0.5
    MAX_ZOOM = 17.0

    MAX_PATHABLE_SLOPE = 2.0


    ####################################
    # don't touch

    LEFT  = 'L'
    RIGHT = 'R'
    UP    = 'U'
    DOWN  = 'D'

    MOUSE_LEFT          = 0
    MOUSE_MIDDLE        = 1
    MOUSE_RIGHT         = 2
    MOUSE_SCROLL_UP     = 3
    MOUSE_SCROLL_DOWN   = 4

    RENDER_LINES     = 0
    RENDER_TRIANGLES = 1



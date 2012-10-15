# This file is loaded by main.exe
#    Careful referencing things from here that reference Controller.
#    You will get circular imports...

class GlobalConsts(object):
    PATH_SHOW_SEARCH    = False

    GENERATE_HILLS = False
    GENERATE_MOUNTAINS = GENERATE_HILLS

#    GENERATE_MOUNTAINS = True

    BOARD_HEIGHT = 40
    BOARD_WIDTH  = 40
#
#    BOARD_HEIGHT = 80
#    BOARD_WIDTH  = 120

#    BOARD_HEIGHT = 512
#    BOARD_WIDTH  = 512

#    BOARD_HEIGHT = int(BOARD_HEIGHT * 0.5)
#    BOARD_WIDTH  = int(BOARD_WIDTH  * 0.5)

#    BOARD_CHUNK_SIZE = 16 # MUST be in multiples of 2
    BOARD_CHUNK_SIZE = 40 # MUST be in multiples of 2

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

    # Event types
    MOUSE_DOWN = 0
    MOUSE_UP    = 1
    MOUSE_DRAG  = 2
    
    KEY_UP      = 3
    KEY_DOWN    = 4
    
    # Render modes
    RENDER_LINES     = 0
    RENDER_TRIANGLES = 1




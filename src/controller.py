from ctypes import *
controller_lib = cdll.LoadLibrary('./externs.so')
controller_lib.Controller_COS_60.restype = c_double
controller_lib.Controller_SIN_60.restype = c_double
controller_lib.Controller_get_rotation.restype = c_double
controller_lib.Controller_get_controller.restype = c_long
controller_lib.Controller_get_hexagon.restype = c_long
controller_lib.Controller_get_selected_hex.restype = c_long
controller_lib.Controller_get_clicked_hex.restype = c_long
controller_lib.Controller_get_width.restype = c_double
controller_lib.Controller_get_height.restype = c_double
controller_lib.Controller_get_zoom.restype = c_double

from itertools import cycle, islice
from random import random

from util import dist_between, a_star, try_catch_funcs

from global_consts import GlobalConsts
from hexagon import Hexagon
from board_object import BoardObject

from land_generation import RollingHills, MountainRange


@try_catch_funcs
class Controller(object):
    _curr_ctrl = None
    _c_pointer = None

    view_range = GlobalConsts.BOARD_WIDTH * 1.25

    COS_60  = controller_lib.Controller_COS_60()
    SIN_60  = controller_lib.Controller_SIN_60()

#    width   = None
#    height  = None
    
    player_input = None

    def __init__(self):
        self._c_pointer = controller_lib.Controller_get_controller()

    @staticmethod
    def get_controller():
        if Controller._curr_ctrl is None:
            try:
                Controller._curr_ctrl = Controller()
            except Exception:
                import traceback
                traceback.print_exc()
                raise

        return Controller._curr_ctrl

    def init_board(self):
        from player_input import PlayerInput
        self.player_input = PlayerInput()
        controller_lib.Controller_set_player_input(self.player_input._c_pointer)
        
        self.zoom = GlobalConsts.START_ZOOM
        self.rotation = GlobalConsts.START_ROTATION
        self.view_range = GlobalConsts.BOARD_WIDTH

        print 'Creating Board...'
        for j in range(GlobalConsts.BOARD_HEIGHT):
            for i in range(GlobalConsts.BOARD_WIDTH):
                x = i * 1.5
                y = j if not i%2 else j + 0.5

                curr_hex = Hexagon(x * self.COS_60, y * self.SIN_60)

                controller_lib.Controller_push_hexagon(curr_hex._c_pointer)

        print "Linking Segments..."
        self.link_segments()

        if GlobalConsts.GENERATE_HILLS:
            print 'Generating hills',
            for i in range(int(20 * (GlobalConsts.BOARD_WIDTH / 100.0))):
                print '.',
                x_start = int(random() * GlobalConsts.BOARD_WIDTH)
                y_start = int(random() * GlobalConsts.BOARD_WIDTH)
                RollingHills.generate(self.get_hexagon(x_start, y_start), 750 * (GlobalConsts.BOARD_WIDTH / 100.0), height_range=(0,  0.015))
                RollingHills.generate(self.get_hexagon(x_start, y_start), 750 * (GlobalConsts.BOARD_WIDTH / 100.0), height_range=(0, -0.0225))
            print

        if GlobalConsts.GENERATE_MOUNTAINS:
            print 'Generating mountains',
            for i in range(int(7 * (GlobalConsts.BOARD_WIDTH / 100.0))):
                print '.',
                x_start = int(random() * GlobalConsts.BOARD_WIDTH)
                y_start = int(random() * GlobalConsts.BOARD_WIDTH)
                MountainRange.generate(
                    self.get_hexagon(x_start, y_start),
                    Hexagon.NEIGHBOR_DIRECTION[int(random() * len(Hexagon.NEIGHBOR_DIRECTION))],
                    60 * (GlobalConsts.BOARD_WIDTH / 100.0), 2, height_range=(0.25, 1.0)
                )
            print

        for hex in Hexagon.get_all_hexagons():
            height = hex.get_height()
            height_percent = abs(height / 5.0);

            if height_percent > 1.0:
                height_percent = 1.0

            if height > 0.0:
                hex.set_hex_color(height_percent, 0.5 + height_percent / 2.0, height_percent)
            else:
                hex.set_hex_color(height_percent / 2.0, 0.5 - height_percent / 2.0, 0)

        hex_list = Hexagon.get_all_hexagons()

        BoardObject(hex_list[0])
        BoardObject(hex_list[15])

#        for hex in Hexagon.get_all_hexagons():
#            if not hex.is_pathable():
#                hex.set_hex_color(0.5, 0, 0.5)

    def get_neighbor_offset(self, index):
        EVEN_OFFSET = {
            'N':    ( 0,  1),
            'NE':   ( 1,  0),
            'SE':   ( 1, -1),
            'S':    ( 0, -1),
            'SW':   (-1, -1),
            'NW':   (-1,  0),
        }

        ODD_OFFSET = {
            'N':    ( 0,  1),
            'NE':   ( 1,  1),
            'SE':   ( 1,  0),
            'S':    ( 0, -1),
            'SW':   (-1,  0),
            'NW':   (-1,  1),
        }

        return islice(cycle([EVEN_OFFSET, ODD_OFFSET]), index, index+1).next()

    def link_segments(self):
        for i in range(GlobalConsts.BOARD_WIDTH):
            offset = self.get_neighbor_offset(i)

            for j in range(GlobalConsts.BOARD_HEIGHT):
                neighbor_dict = {}
                curr_hex = self.get_hexagon(i, j)

                for pos, (i_offset, j_offset) in offset.iteritems():
                    new_seg_i = i + i_offset
                    new_seg_j = j + j_offset

                    neighbor_hex = self.get_hexagon(new_seg_i, new_seg_j)

                    assert neighbor_hex not in neighbor_dict.values()
                    neighbor_dict[pos] = neighbor_hex

                curr_hex.set_neighbors(neighbor_dict)

        for curr_hex in Hexagon.get_all_hexagons():
            curr_hex.link_verticies()

    def get_hexagon(self, i, j):
        curr_c_hex = controller_lib.Controller_get_hexagon(c_int(i), c_int(j))
        return Hexagon.get_hexagon(curr_c_hex)

    def init_gl(self, width, height):
        controller_lib.Controller_init_gl(width, height)

    def resize(self, width, height):
        controller_lib.Controller_resize(width, height)

    def zoom_map(self, zoom_amount):
        controller_lib.Controller_zoom_map(c_double(zoom_amount))

    @property
    def rotation(self):
        return controller_lib.Controller_get_rotation()

    @rotation.setter
    def rotation(self, rotation):
        controller_lib.Controller_set_rotation(c_double(rotation))

    def get_width(self):
        return controller_lib.Controller_get_width()

    def get_height(self):
        return controller_lib.Controller_get_height()

    def get_zoom(self):
        return controller_lib.Controller_get_zoom()

    def add_x_offset(self, x_offset):
        controller_lib.Controller_add_x_offset(c_double(x_offset))

    def add_y_offset(self, y_offset):
        controller_lib.Controller_add_y_offset(c_double(y_offset))

    def set_scroll(self, direction):
        controller_lib.Controller_set_scroll(c_char(direction))

    def clear_scroll(self, direction):
        controller_lib.Controller_clear_scroll(c_char(direction))

    def tick(self):
        controller_lib.Controller_tick()

    def mouse_down(self, x, y, button):
        controller_lib.Controller_mouse_down(c_double(x), c_double(y), 0)

    def get_selected_hex(self):
        c_hex_obj = controller_lib.Controller_get_selected_hex()
        return Hexagon.get_hexagon(c_hex_obj)

    def set_selected_hex(self, curr_hex):
        controller_lib.Controller_set_selected_hex(curr_hex._c_pointer)

    def clear_selected_hex(self):
        controller_lib.Controller_clear_selected_hex()

    def get_clicked_hex(self, x, y):
        x = int(x)
        y = int(y)
        c_hex_obj = controller_lib.Controller_get_clicked_hex(c_int(x), c_int(y))
        return Hexagon.get_hexagon(c_hex_obj)

    def find_path(self, start_hex, end_hex):
        for hex in Hexagon.get_all_hexagons():
            if hex.is_pathable():
                hex.clear_select_color()

        for hex in a_star(start_hex, end_hex):
            hex.set_select_color(0, 1, 1)


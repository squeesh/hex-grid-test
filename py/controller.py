from ctypes import *
controller_lib = cdll.LoadLibrary('./bin/all_externs.so')
#controller_lib.Controller_COS_60.restype = c_double
#controller_lib.Controller_SIN_60.restype = c_double
controller_lib.Controller_get_rotation.restype = c_double
controller_lib.Controller_get_controller.restype = c_long
controller_lib.Controller_get_hexagon.restype = c_long
#controller_lib.Controller_get_selected_hex.restype = c_long
controller_lib.Controller_get_clicked_hex.restype = c_long
controller_lib.Controller_get_width.restype = c_double
controller_lib.Controller_get_height.restype = c_double
controller_lib.Controller_get_zoom.restype = c_double

from itertools import cycle, islice
from random import random

from global_consts import GlobalConsts
from hexagon import Hexagon
from board_object import BoardObject

from land_generation import RollingHillsGen, MountainRangeGen, RoadGen


class Controller(object):
    _curr_ctrl = None
    _c_pointer = None

    view_range = GlobalConsts.BOARD_WIDTH * 1.25

    #COS_60  = controller_lib.Controller_COS_60()
    #SIN_60  = controller_lib.Controller_SIN_60()

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

                curr_hex = Hexagon(x * GlobalConsts.COS_60, y * GlobalConsts.SIN_60)

                controller_lib.Controller_push_hexagon(curr_hex._c_pointer)

        print "Linking Segments..."
        self.link_segments()

        if GlobalConsts.GENERATE_HILLS:
            print 'Generating hills',
            for i in range(int(20 * (GlobalConsts.BOARD_WIDTH / 100.0))):
                print '.',
                x_start = int(random() * GlobalConsts.BOARD_WIDTH)
                y_start = int(random() * GlobalConsts.BOARD_WIDTH)
                RollingHillsGen.generate(self.get_hexagon(x_start, y_start), 750 * (GlobalConsts.BOARD_WIDTH / 100.0), height_range=(0,  0.015))
                RollingHillsGen.generate(self.get_hexagon(x_start, y_start), 750 * (GlobalConsts.BOARD_WIDTH / 100.0), height_range=(0, -0.0225))
            print

        if GlobalConsts.GENERATE_MOUNTAINS:
            print 'Generating mountains',
            for i in range(int(7 * (GlobalConsts.BOARD_WIDTH / 100.0))):
                print '.',
                x_start = int(random() * GlobalConsts.BOARD_WIDTH)
                y_start = int(random() * GlobalConsts.BOARD_WIDTH)
                MountainRangeGen.generate(
                    self.get_hexagon(x_start, y_start),
                    Hexagon.NEIGHBOR_DIRECTION[int(random() * len(Hexagon.NEIGHBOR_DIRECTION))],
                    60 * (GlobalConsts.BOARD_WIDTH / 100.0), 2, height_range=(0.25, 1.0)
                )
            print

        road_hexagons = []
        for i in range(3):
            while True:
                curr_hex = self.get_hexagon(int(random() * GlobalConsts.BOARD_WIDTH), int(random() * GlobalConsts.BOARD_HEIGHT))
                if curr_hex.is_pathable():
                    road_hexagons.append(curr_hex)
                    break

        RoadGen.generate(
            road_hexagons[0], road_hexagons[1]
        )
        RoadGen.generate(
            road_hexagons[1], road_hexagons[2]
        )
        RoadGen.generate(
            road_hexagons[2], road_hexagons[0]
        )


        from timeit import Timer
        from functools import partial

        def get_execution_time(function, *args, **kwargs):
            """Return the execution time of a function in seconds."""
            numberOfExecTime = kwargs.pop('numberOfExecTime', 20)
            return Timer(partial(function, *args, **kwargs)).timeit(numberOfExecTime)

        from util import a_star
        # return a_star(hex_a, hex_b)

        for i in range(3):
            print 'cpp:', get_execution_time(Hexagon.find_path, road_hexagons[i-1], road_hexagons[i])
            print 'pyc:', get_execution_time(a_star, road_hexagons[i-1], road_hexagons[i])
            print '---'

        # while True:
        #     curr_hex = self.get_hexagon(int(random() * GlobalConsts.BOARD_WIDTH), int(random() * GlobalConsts.BOARD_HEIGHT))
        #     if curr_hex.is_pathable():
        #         break

        # # print 'dist: ', dist_between(curr_hex, curr_hex.get_neighbor('N').get_neighbor('N')), 'should be 2'
        # # print 'dist: ', dist_between(curr_hex, curr_hex.get_neighbor('S').get_neighbor('S')), 'should be 2'
        # # print 'dist: ', dist_between(curr_hex, curr_hex.get_neighbor('S').get_neighbor('S').get_neighbor('SE').get_neighbor('SE')), 'should be 3ish'

        # RoadGen.generate(
        #     curr_hex, curr_hex.get_neighbor('N').get_neighbor('N')
        # )

        # Hexagon.find_path(curr_hex, curr_hex.get_neighbor('N').get_neighbor('N'))

        # from util import dist_between
        # print dist_between(road_hexagons[0], road_hexagons[0].get_neighbor('N'))
        # print 1 / dist_between(road_hexagons[0], road_hexagons[0].get_neighbor('S'))
        # print dist_between(road_hexagons[0], road_hexagons[0].get_neighbor('NW'))

        height_list = [hex.get_height() for hex in Hexagon.get_all_hexagons()]

        max_height = max(height_list)
        min_height = min(height_list)

        if max_height and min_height:
            for hex in Hexagon.get_all_hexagons():
                height = hex.get_height()

                if height > 0.0:
                    height_percent = (height / max_height)

                    if height_percent < 0.5:
                        curr_percent = height_percent * 2.0

                        red     = curr_percent * 0.7
                        green   = 0.5 + curr_percent * 0.2
                        blue    = curr_percent * 0.7
                    else:
                        curr_percent = (height_percent - 0.5) * 2.0

                        red     = 0.7 + curr_percent * 0.3
                        green   = 0.7 + curr_percent * 0.3
                        blue    = 0.7 + curr_percent * 0.3
                else:
                    height_percent = abs(height / min_height)

                    if height_percent < 0.5:
                        curr_percent = height_percent * 2.0

                        red     = curr_percent * 0.3
                        green   = 0.5 - curr_percent * 0.5
                        blue    = 0
                    else:
                        curr_percent = (height_percent - 0.5) * 2.0

                        red     = 0.3 + curr_percent * 0.7
                        green   = 0
                        blue    = 0

                hex.set_hex_color(red, green, blue)
        else:
            for hex in Hexagon.get_all_hexagons():
                hex.set_hex_color(0, 0.5, 0)

        img_list = [
            'media/test_a.png',
            'media/test_b.png',
        ]

        for i in range(1):
            while True:
                curr_hex = self.get_hexagon(int(random() * GlobalConsts.BOARD_WIDTH), int(random() * GlobalConsts.BOARD_HEIGHT))
                if curr_hex.is_pathable():
                    board_obj = BoardObject(curr_hex, img_list[i])
                    break

        # orig_hex = self.get_hexagon(int(random() * GlobalConsts.BOARD_WIDTH), int(random() * GlobalConsts.BOARD_HEIGHT))
        # for i in range(10):
        #     curr_hex = orig_hex
        #     for j in range(13):
        #         curr_hex.set_select_color(1, 0, 1)
        #         k = 0.0
        #         for neigh_hex in curr_hex.get_neighbors().values():
        #             k += 1.0 / 6.0
        #             neigh_hex.set_select_color(1, k, 0)
        #         curr_hex = curr_hex.get_neighbor("N").get_neighbor("N").get_neighbor("N")

        #     orig_hex = orig_hex.get_neighbor("NE").get_neighbor("SE").get_neighbor("NE").get_neighbor("SE")

        board_obj.set_selected(True)

#        for hex in Hexagon.get_all_hexagons():
#            if not hex.is_pathable():
#                hex.set_hex_color(0.5, 0, 0.5)

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

    def get_neighbor_offset(self, index):
        return islice(cycle([self.EVEN_OFFSET, self.ODD_OFFSET]), index, index+1).next()

#    def get_neighbor_offset(self, index):
#        EVEN_OFFSET = {
#            'N':    ( 0,  1),
#            'NE':   ( 1,  0),
#            'SE':   ( 1, -1),
#            'S':    ( 0, -1),
#            'SW':   (-1, -1),
#            'NW':   (-1,  0),
#        }
#
#        ODD_OFFSET = {
#            'N':    ( 0,  1),
#            'NE':   ( 1,  1),
#            'SE':   ( 1,  0),
#            'S':    ( 0, -1),
#            'SW':   (-1,  0),
#            'NW':   (-1,  1),
#        }
#
#        return islice(cycle([EVEN_OFFSET, ODD_OFFSET]), index, index+1).next()

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

#    def resize(self, width, height):
#        controller_lib.Controller_resize(width, height)

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
        from board_object import BoardObject
        for board_obj in BoardObject.get_all():
            if board_obj.curr_path:
                next_hex = board_obj.curr_path[0]
                board_obj.curr_path = board_obj.curr_path[1:]
                board_obj.move_to_hex(next_hex)

    def get_clicked_hex(self, x, y):
        for hex in Hexagon.get_all_hexagons():
            if hex.is_pathable():
                hex.clear_select_color()

        x = int(x)
        y = int(y)
        c_hex_obj = controller_lib.Controller_get_clicked_hex(c_int(x), c_int(y))
        return Hexagon.get_hexagon(c_hex_obj)

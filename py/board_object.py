from PIL import Image
from ctypes import *
board_object_lib = cdll.LoadLibrary('./bin/all_externs.so')
board_object_lib.BoardObject_new.restype = c_long
board_object_lib.BoardObject_is_selected.restype = c_bool

# from util import a_star_2 as a_star


class BoardObject(object):
    _c_pointer = None
    _board_obj_cache = {}

    curr_path = None
    text_data = None

    def __init__(self, base_hex, texture_path):
        self.curr_path = []

        im = Image.open(texture_path)
        self.text_data = im.convert("RGBA").tostring("raw", "RGBA")

        self._c_pointer = board_object_lib.BoardObject_new(base_hex._c_pointer)
        self._board_obj_cache[self._c_pointer] = self

        board_object_lib.BoardObject_set_tex_data(self._c_pointer, self.text_data)

    def set_destination(self, dest_hex):
        from hexagon import Hexagon

        start_hex = self.get_base_hex()

        # self.curr_path = a_star(start_hex, dest_hex)
        self.curr_path = Hexagon.find_path(start_hex, dest_hex)

    @staticmethod
    def get_board_object(c_board_obj):
        return BoardObject._board_obj_cache.get(c_board_obj)

    @staticmethod
    def get_all():
        return BoardObject._board_obj_cache.values()

    def is_selected(self):
        return board_object_lib.BoardObject_is_selected(self._c_pointer)

    def set_selected(self, selected):
        board_object_lib.BoardObject_set_selected(self._c_pointer, c_bool(selected))

    def get_base_hex(self):
        from hexagon import Hexagon
        hex_ptr = board_object_lib.BoardObject_get_base_hex(self._c_pointer)
        return Hexagon.get_hexagon(hex_ptr)

    def move_to_hex(self, curr_hex):
        board_object_lib.BoardObject_move_to_hex(self._c_pointer, curr_hex._c_pointer)

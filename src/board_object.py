from ctypes import *
board_object_lib = cdll.LoadLibrary('./externs.so')

board_object_lib.BoardObject_new.restype = c_long

from util import try_catch_funcs


@try_catch_funcs
class BoardObject(object):
    _c_pointer = None
    _board_obj_cache = {}
    
    curr_path = None
    
    def __init__(self, base_hex):
        self._c_pointer = board_object_lib.BoardObject_new(base_hex._c_pointer)
        
        self._board_obj_cache[self._c_pointer] = self
    
    def set_destination(self, dest_hex_addr):
        start_hex = self.parent_hex()
        dest_hex = Hexagon.get_hexagon(start_hex_addr)

        self.curr_path = a_star(start_hex, end_hex)

    @staticmethod
    def get_board_object(c_board_obj):
        return BoardObject._board_obj_cache.get(c_board_obj)

    @staticmethod
    def get_all_board_objects():
        return BoardObject._board_obj_cache.values()
    

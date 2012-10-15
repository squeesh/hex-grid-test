from ctypes import *
hexagon_lib = cdll.LoadLibrary('./externs.so')
hexagon_lib.Hexagon_new.restype = c_long
hexagon_lib.Hexagon_get_vertex.restype = c_long
hexagon_lib.Hexagon_get_neighbor.restype = c_long
hexagon_lib.Hexagon_get_height.restype = c_double
hexagon_lib.Hexagon_get_slope.restype = c_double
hexagon_lib.Hexagon_get_last_x.restype = c_double
hexagon_lib.Hexagon_get_last_y.restype = c_double
hexagon_lib.Hexagon_is_pathable.restype = c_bool

from util import RoundList, try_catch_funcs


@try_catch_funcs
class Hexagon(object):
    _c_pointer = None
    _hex_cache = {}

    NEIGHBOR_DIRECTION = RoundList('N', 'NE', 'SE', 'S', 'SW', 'NW') # Order is important!

    VERTEX_POSITIONS = ('W', 'NW', 'NE', 'E', 'SE', 'SW')

    SEGMENT_VERTEX_MAP = {
        'N':    (('NW', 'SW'),  ('NE',  'SE')),
        'NE':   (('NE', 'W'),   ('E',   'SW')),
        'SE':   (('E',  'NW'),  ('SE',  'W')),
        'S':    (('SW', 'NW'),  ('SE',  'NE')),
        'SW':   (('W',  'NE'),  ('SW',  'E')),
        'NW':   (('W',  'SE'),  ('NW',  'E')),
    }

    MAX_PATHABLE_SLOPE = 2.0

    def __init__(self, x, y, color=(0, 1, 0)):
        self._c_pointer = hexagon_lib.Hexagon_new()
        self.set_border_color(*color)

        hexagon_lib.Hexagon_set_last_x(self._c_pointer, c_double(x))
        hexagon_lib.Hexagon_set_last_y(self._c_pointer, c_double(y))

        self._hex_cache[self._c_pointer] = self

    def set_border_color(self, red, green, blue):
        hexagon_lib.Hexagon_set_border_color(self._c_pointer, c_double(red), c_double(green), c_double(blue))

    def set_hex_color(self, red, green, blue):
        hexagon_lib.Hexagon_set_hex_color(self._c_pointer, c_double(red), c_double(green), c_double(blue))

    def set_select_color(self, red, green, blue):
        hexagon_lib.Hexagon_set_select_color(self._c_pointer, c_double(red), c_double(green), c_double(blue))

    def clear_select_color(self):
        hexagon_lib.Hexagon_clear_select_color(self._c_pointer)

    def set_height(self, height):
        hexagon_lib.Hexagon_set_height(self._c_pointer, c_double(height))

    def add_height(self, height):
        hexagon_lib.Hexagon_add_height(self._c_pointer, c_double(height))

    def get_height(self):
        return hexagon_lib.Hexagon_get_height(self._c_pointer)

    def get_slope(self):
        return hexagon_lib.Hexagon_get_slope(self._c_pointer)

    def get_vertex(self, position):
        return hexagon_lib.Hexagon_get_vertex(self._c_pointer, position)

    def set_neighbors(self, neighbor_dict):
        for position, neighbor_hex in neighbor_dict.iteritems():
            hexagon_lib.Hexagon_set_neighbor(self._c_pointer, position, neighbor_hex._c_pointer)

    def get_neighbors(self):
        output = {}

        for position in self.NEIGHBOR_DIRECTION:
            output[position] = Hexagon.get_hexagon(hexagon_lib.Hexagon_get_neighbor(self._c_pointer, position))

        return output

    def get_neighbor(self, position):
        return Hexagon.get_hexagon(hexagon_lib.Hexagon_get_neighbor(self._c_pointer, position))

    def link_verticies(self):
        for position, curr_hex in self.get_neighbors().iteritems():
            for self_pos, curr_pos in self.SEGMENT_VERTEX_MAP[position]:
                self_vert = self.verticies[self_pos]
                curr_vert = curr_hex.verticies[curr_pos]

                if self_vert != curr_vert:
                    self.verticies[self_pos] = self_vert
                    curr_hex.verticies[curr_pos] = self_vert
                    Vertex.verticies_to_delete.append(curr_vert)
                else:
                    assert id(self_vert) == id(curr_vert)
                    assert self_vert.c_vert_obj == curr_vert.c_vert_obj

    @property
    def verticies(self):
        curr_hex = self

        class VertexManager(object):
            def __getitem__(self, position):
                if position not in curr_hex.VERTEX_POSITIONS:
                    raise IndexError

                c_vert = hexagon_lib.Hexagon_get_vertex(curr_hex._c_pointer, position)
                py_vert = Vertex._verticies_list.get(c_vert, None)

                if not py_vert:
                    py_vert = Vertex()
                    py_vert.c_vert_obj = c_vert
                    Vertex._verticies_list[c_vert] = py_vert

                return py_vert

            def __setitem__(self, position, value):
                hexagon_lib.Hexagon_set_vertex(curr_hex._c_pointer, position, value.c_vert_obj)

            def __iter__(self):
                for position in curr_hex.VERTEX_POSITIONS:
                    yield self[position]

        return VertexManager()

    @staticmethod
    def get_hexagon(c_hex_obj):
        return Hexagon._hex_cache.get(c_hex_obj)

    @staticmethod
    def get_all_hexagons():
        return Hexagon._hex_cache.values()

    def get_last_coord(self):
        py_last_x = hexagon_lib.Hexagon_get_last_x(self._c_pointer)
        py_last_y = hexagon_lib.Hexagon_get_last_y(self._c_pointer)

        return (py_last_x, py_last_y)

    def is_pathable(self):
        return hexagon_lib.Hexagon_is_pathable(self._c_pointer)
    
    def get_board_object(self):
        return hexagon_lib.Hexagon_get_board_object(self._c_pointer)


class Vertex(object):
    _verticies_list = {}
    verticies_to_delete = []

    c_vert_obj = None

    def cleanup_verticies(self):
        pass

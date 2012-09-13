from ctypes import *
hexagon_lib = cdll.LoadLibrary('./controller.so')

hexagon_lib.Hexagon_new_hexagon.restype = c_long
hexagon_lib.Hexagon_get_vertex.restype = c_long
hexagon_lib.Hexagon_get_neighbor.restype = c_long

hexagon_lib.Hexagon_get_height.restype = c_double
hexagon_lib.Hexagon_get_slope.restype = c_double
hexagon_lib.Hexagon_get_last_x.restype = c_double
hexagon_lib.Hexagon_get_last_y.restype = c_double

hexagon_lib.Hexagon_is_pathable.restype = c_bool

from util import RoundList


class Hexagon(object):
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

    c_hex_obj = None

    def __init__(self, x, y, color=(0, 1, 0)):
        self.c_hex_obj = hexagon_lib.Hexagon_new_hexagon()
        self.set_color(*color)

#        print
#        print x, '|', y
        hexagon_lib.Hexagon_set_last_x(self.c_hex_obj, c_double(x))
        hexagon_lib.Hexagon_set_last_y(self.c_hex_obj, c_double(y))

        self._hex_cache[self.c_hex_obj] = self

    def set_color(self, red, green, blue):
        hexagon_lib.Hexagon_set_color(self.c_hex_obj, c_double(red), c_double(green), c_double(blue))

    def set_select_color(self, red, green, blue):
        hexagon_lib.Hexagon_set_select_color(self.c_hex_obj, c_double(red), c_double(green), c_double(blue))

    def clear_select_color(self):
        hexagon_lib.Hexagon_clear_select_color(self.c_hex_obj)

    def set_height(self, height):
        hexagon_lib.Hexagon_set_height(self.c_hex_obj, c_double(height))

    def add_height(self, height):
        hexagon_lib.Hexagon_add_height(self.c_hex_obj, c_double(height))

    def get_height(self):
        return hexagon_lib.Hexagon_get_height(self.c_hex_obj)

    def get_slope(self):
        return hexagon_lib.Hexagon_get_slope(self.c_hex_obj)

    def get_vertex(self, position):
        return hexagon_lib.Hexagon_get_vertex(self.c_hex_obj, position)

    def set_neighbors(self, neighbor_dict):
        for position, neighbor_hex in neighbor_dict.iteritems():
            hexagon_lib.Hexagon_set_neighbor(self.c_hex_obj, position, neighbor_hex.c_hex_obj)

    def get_neighbors(self):
        output = {}

        for position in self.NEIGHBOR_DIRECTION:
            output[position] = Hexagon.get_hexagon(hexagon_lib.Hexagon_get_neighbor(self.c_hex_obj, position))

        return output

    def get_neighbor(self, position):
        return Hexagon.get_hexagon(hexagon_lib.Hexagon_get_neighbor(self.c_hex_obj, position))

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

                c_vert = hexagon_lib.Hexagon_get_vertex(curr_hex.c_hex_obj, position)
                py_vert = Vertex._verticies_list.get(c_vert, None)

                if not py_vert:
                    py_vert = Vertex()
                    py_vert.c_vert_obj = c_vert
                    Vertex._verticies_list[c_vert] = py_vert

                return py_vert

            def __setitem__(self, position, value):
                hexagon_lib.Hexagon_set_vertex(curr_hex.c_hex_obj, position, value.c_vert_obj)

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
        py_last_x = hexagon_lib.Hexagon_get_last_x(self.c_hex_obj)
        py_last_y = hexagon_lib.Hexagon_get_last_y(self.c_hex_obj)

        return (py_last_x, py_last_y)

    def is_pathable(self):
        return hexagon_lib.Hexagon_is_pathable(self.c_hex_obj)


class Vertex(object):
    _verticies_list = {}
    verticies_to_delete = []

    c_vert_obj = None

    def cleanup_verticies(self):
        pass

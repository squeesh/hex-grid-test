print 'player'
from ctypes import *
input_lib = cdll.LoadLibrary('./all_externs.so')
input_lib.PlayerInput_new.restype = c_long

from global_consts import GlobalConsts

from controller import Controller


class PlayerInput(object):
    """
        Methods are called on this class when a Player Input event occurs
    """

    _c_pointer = None

    def __init__(self):
        self._c_pointer = input_lib.PlayerInput_new(c_long(id(self)))

    def mouse_left_click(self, x, y):
        curr_ctrl = Controller.get_controller()
        curr_hex = curr_ctrl.get_clicked_hex(x, y)

        if curr_hex and curr_hex.get_board_object():
            curr_board_obj = curr_hex.get_board_object()
            if curr_board_obj:
                from board_object import BoardObject
                for board_obj in BoardObject.get_all():
                    board_obj.set_selected(False)

                curr_board_obj.set_selected(True)

    def mouse_left_release(self, x, y):
        pass

    def mouse_middle_click(self, x, y):
        pass

    def mouse_middle_release(self, x, y):
        pass

    def mouse_right_click(self, x, y):
        curr_ctrl = Controller.get_controller()
        curr_hex = curr_ctrl.get_clicked_hex(x, y)

        if curr_hex and curr_hex.is_pathable():
            from board_object import BoardObject
            for board_obj in BoardObject.get_all():
                if board_obj.is_selected():
                    board_obj.set_destination(curr_hex)

#            sel_hex = curr_ctrl.get_selected_hex()
#            if sel_hex:
#                curr_ctrl.find_path(sel_hex, curr_hex)

    def mouse_right_release(self, x, y):
        pass

    def mouse_scroll_up(self, x, y):
        curr_ctrl = Controller.get_controller()
        curr_ctrl.zoom_map(1 / 1.25)

    def mouse_scroll_down(self, x, y):
        curr_ctrl = Controller.get_controller()
        curr_ctrl.zoom_map(1.25)

    def mouse_left_drag(self, x_diff, y_diff):
        pass

    def mouse_middle_drag(self, x_diff, y_diff):
#        print 'x diff: ', x_diff
#        print 'y diff: ', y_diff
#        print

        curr_ctrl = Controller.get_controller()

        curr_ctrl.add_x_offset(-x_diff / 30.0 * curr_ctrl.get_zoom())
        curr_ctrl.add_y_offset(-y_diff / 30.0 * curr_ctrl.get_zoom())

    def mouse_right_drag(self, x_diff, y_diff):
        pass

    def key_down(self, key, x, y):
        curr_ctrl = Controller.get_controller()

#        if ord(key) == 27:
#            curr_ctrl.clear_selected_hex()

        if   key == 'w':
            curr_ctrl.set_scroll(GlobalConsts.UP)
        elif key == 's':
            curr_ctrl.set_scroll(GlobalConsts.DOWN)
        elif key == 'a':
            curr_ctrl.set_scroll(GlobalConsts.LEFT)
        elif key == 'd':
            curr_ctrl.set_scroll(GlobalConsts.RIGHT)

        elif key == '+':
            curr_ctrl.zoom_map(1 / 1.25)
        elif key == '-':
            curr_ctrl.zoom_map(1.25)
        elif key == '*':
            curr_ctrl.rotation += 2.0
        elif key == '/':
            curr_ctrl.rotation -= 2.0

    def key_up(self, key, x, y):
        curr_ctrl = Controller.get_controller()

        if key == 'w':
            curr_ctrl.clear_scroll(GlobalConsts.UP)
        elif key == 's':
            curr_ctrl.clear_scroll(GlobalConsts.DOWN)
        elif key == 'a':
            curr_ctrl.clear_scroll(GlobalConsts.LEFT)
        elif key == 'd':
            curr_ctrl.clear_scroll(GlobalConsts.RIGHT)


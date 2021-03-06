from ctypes import cdll
util_lib = cdll.LoadLibrary('./bin/all_externs.so')

from collections import defaultdict
import math
import types
import traceback
import sys

from global_consts import GlobalConsts


class RoundList(list):
    def __init__(self, *args):
        super(RoundList, self).__init__(args)

    def __getitem__(self, index):
        return super(RoundList, self).__getitem__(index % len(self))


# def a_star(start, goal):
#     closedset  = []    # The set of nodes already evaluated.
#     openset    = [start]    # The set of tentative nodes to be evaluated, initially containing the start node
#     came_from  = {}    # The map of navigated nodes.

#     g_score = {}
#     f_score = {}

#     g_score[start] = 0    # Cost from start along best known path.
#     # Estimated total cost from start to goal through y.
#     f_score[start] = g_score[start] + heuristic_cost_estimate(start, goal)

#     while openset:
#         curr_f_score = f_score[openset[0]]
#         current = openset[0]

#         for item in openset:
#             if f_score[item] < curr_f_score:
#                 curr_f_score = f_score[item]
#                 current = item #the node in openset having the lowest f_score[] value

#         if GlobalConsts.PATH_SHOW_SEARCH and current.is_pathable():
#             current.set_select_color(1, 0, 0)

#         if current == goal:
#             return reconstruct_path(came_from, goal)

#         openset.remove(current) # remove current from openset
#         closedset.append(current) # add current to closedset

#         for neighbor in current.get_neighbors().values():
#             if not neighbor.is_pathable():
#                 closedset.append(neighbor)

#             if neighbor in closedset:
#                 continue
#             tentative_g_score = g_score[current] + dist_between(current, neighbor)

#             if neighbor not in openset or tentative_g_score < g_score[neighbor]:
#                 openset.append(neighbor) # add neighbor to openset
#                 came_from[neighbor] = current
#                 g_score[neighbor] = tentative_g_score
#                 f_score[neighbor] = g_score[neighbor] + heuristic_cost_estimate(neighbor, goal)

    # return []


def reconstruct_path(came_from, current_node):
    if current_node in came_from:
        p = reconstruct_path(came_from, came_from[current_node])
        return p + [current_node]
    else:
        return [current_node]


# def heuristic_cost_estimate(curr_node, goal):
#     # if curr_node.get_improvement('road'):
#     #     hex_cost = 0
#     # else:
#     #     hex_cost = ((curr_node.get_slope()+1) ** 10)

#     return dist_between(curr_node, goal)


def dist_between(curr_node, neighbor):
    from controller import Controller

    board_width  = GlobalConsts.BOARD_WIDTH * 1.5 * GlobalConsts.COS_60
    board_height = GlobalConsts.BOARD_HEIGHT * 1.0 * GlobalConsts.SIN_60

    curr_last_x, curr_last_y   = curr_node.get_last_coord()
    neigh_last_x, neigh_last_y = neighbor.get_last_coord()

    x_diff = neigh_last_x - curr_last_x
    y_diff = neigh_last_y - curr_last_y
    half_width = board_width / 2.0
    half_height = board_height / 2.0

    if x_diff > half_width:
        x_diff -= board_width
    elif x_diff < -half_width:
        x_diff += board_width

    if y_diff > half_height:
        y_diff -= board_height
    elif y_diff < -half_height:
        y_diff += board_height

    return math.sqrt(x_diff**2 + y_diff**2)

def a_star(start_node, goal_node):
    open_list = [start_node]
    closed_list = []
    cost_dict = defaultdict(dict)
    parent_dict = {}

    cost_dict[start_node]['g'] = 0.0
    cost_dict[start_node]['h'] = get_h_cost(start_node, goal_node)
    cost_dict[start_node]['f'] = cost_dict[start_node]['h']

    curr_node = start_node

    while open_list:
        min_f_node = open_list[0]
        for node in open_list[1:]:
            if cost_dict[node]['f'] < cost_dict[min_f_node]['f']:
                min_f_node = node

        curr_node = min_f_node

        if curr_node == goal_node:
            return reconstruct_path(parent_dict, goal_node)

        # if GlobalConsts.PATH_SHOW_SEARCH and curr_node.is_pathable():
        #     curr_node.set_select_color(1, 0, 0)

        open_list.remove(curr_node)
        if curr_node not in closed_list:
            closed_list.append(curr_node)

        for neighbor in curr_node.get_neighbors().values():
            if not neighbor.is_pathable() and neighbor not in closed_list:
                closed_list.append(neighbor)

            if neighbor in closed_list:
                continue

            tentative_g_score = get_g_cost(curr_node, neighbor) + cost_dict[curr_node]['g']

            if neighbor not in open_list or tentative_g_score < cost_dict[neighbor]['g']:
                parent_dict[neighbor] = curr_node
                open_list.append(neighbor)
                cost_dict[neighbor]['g'] = get_g_cost(curr_node, neighbor) + cost_dict[curr_node]['g']
                cost_dict[neighbor]['h'] = get_h_cost(neighbor, goal_node)
                cost_dict[neighbor]['f'] = cost_dict[neighbor]['g'] + cost_dict[neighbor]['h']

    return []

def get_g_cost(start_node, end_node):
    if end_node.get_improvement('road'):
        return 1
    else:
        return (end_node.get_slope() + 1.1) ** 10

def get_h_cost(start_node, end_node):
    return (dist_between(start_node, end_node) * 1.0 / 0.866025) * 1.5

# Wraps all functions of a class in a try / catch
# forces exit on error
def try_catch_funcs(curr_class):
   all_attrs = dir(curr_class)
   base_attrs = dir(object)

   cls_attrs = list(set(all_attrs) - set(base_attrs))

   for curr_attr_str in cls_attrs:
       curr_attr = getattr(curr_class, curr_attr_str)
       if hasattr(curr_attr, '__call__'):
           old_func = curr_attr

           def get_func(curr_func):
               def _try_catch(*args, **kwargs):
                   try:
                       return curr_func(*args, **kwargs)
                   except Exception, e:
                       info = sys.exc_info()
                       traceback.print_exc()

                       # force exit if python error...
                       util_lib.Util_force_exit()

               return _try_catch

           if isinstance(old_func, types.FunctionType):
               old_func = get_func(old_func)

               old_func.__name__ = curr_attr_str
               old_func = staticmethod(old_func)

           else:
               old_func = get_func(old_func)

               old_func.__name__ = curr_attr_str

           setattr(curr_class, curr_attr_str, old_func)

   return curr_class


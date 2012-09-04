from random import random


def add_mountain(center_seg, height, radius):
    if radius > 1:
        for pos in center_seg.NEIGHBOR_DIRECTION:
            add_mountain(center_seg.get_neighbor(pos), height/float(radius), radius-1)

    center_seg.add_height(height)

class RollingHills(object):
    @staticmethod
    def generate(start_segment, distance, height_range=(0, 2)):
        distance = int(distance)

        min_height, max_height = height_range
        curr_seg = start_segment
        curr_dir = int(random() * len(curr_seg.NEIGHBOR_DIRECTION))
        effected_radius = 1

        for i in range(distance):
            add_mountain(curr_seg, (random() * (max_height - min_height)) + min_height, effected_radius)

            curr_dir = curr_dir + int(random() * 2) - 1
            curr_seg = curr_seg.get_neighbor(curr_seg.NEIGHBOR_DIRECTION[curr_dir])
            effected_radius = random() * 3 + 1

class MountainRange(object):
    distance_range = (1, 1)

    @staticmethod
    def generate(curr_seg, direction, total_dist=100, distance=0, height_range=(0.125, 0.75), orig_dir=None):
        if total_dist < 0:
            return

        if orig_dir is None:
            orig_dir = curr_seg.NEIGHBOR_DIRECTION.index(direction)

        height = (random() * (height_range[1] - height_range[0])) + height_range[0]
        radius = int(random() * 4) + 2
        add_mountain(curr_seg, height, radius)

        curr_dir = curr_seg.NEIGHBOR_DIRECTION.index(direction)
        curr_seg = curr_seg.get_neighbor(curr_seg.NEIGHBOR_DIRECTION[curr_dir])
#
#        print distance
        if distance > 1:
            MountainRange.generate(curr_seg, curr_seg.NEIGHBOR_DIRECTION[curr_dir], total_dist-1, distance-1, orig_dir=orig_dir)
        else:
            curr_dir = curr_dir + int(random() * 3) - 1

            if curr_dir > orig_dir + 1:
                curr_dir = orig_dir + 1
            elif curr_dir < orig_dir - 1:
                curr_dir = orig_dir - 1

#            print curr_seg.NEIGHBOR_DIRECTION[curr_dir]
#            print curr_seg.NEIGHBOR_DIRECTION[curr_dir], '|', curr_dir

            distance = (random() * MountainRange.distance_range[1]) + MountainRange.distance_range[0]

            MountainRange.generate(curr_seg, curr_seg.NEIGHBOR_DIRECTION[curr_dir], total_dist-1, distance, orig_dir=orig_dir)


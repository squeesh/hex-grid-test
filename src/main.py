# This is statement is required by the build system to query build info
if __name__ == '__build__':
    raise Exception

#from OpenGL.GL import *
from OpenGL.GLUT import *
#from OpenGL.GLU import *

from numpy import *
import sys
from time import time

from controller import Controller
from hexagon import Hexagon


curr_ctrl = None

# Some api in the chain is translating the keystrokes to this octal string
# so instead of saying: ESCAPE = 27, we use the following.
ESCAPE = '\033'

# Number of the glut window.
window = 0

SLEEP_TIME = 10
SHOW_FPS = True
RESOLUTION = (1024, 768)

def keyPressed(key, x, y):
#    print args[0]
    # If escape is pressed, kill everything.
    if key == ESCAPE:
        sys.exit()
#    elif key == 'i':
#        curr_ctrl.set_rotate(curr_ctrl.UP)
#    elif key == 'k':
#        curr_ctrl.set_rotate(curr_ctrl.DOWN)
#    elif key == 'j':
#        curr_ctrl.set_rotate(curr_ctrl.LEFT)
#    elif key == 'l':
#        curr_ctrl.set_rotate(curr_ctrl.RIGHT)
#
    elif key == 'w':
        curr_ctrl.set_scroll(curr_ctrl.UP)
    elif key == 's':
        curr_ctrl.set_scroll(curr_ctrl.DOWN)
    elif key == 'a':
        curr_ctrl.set_scroll(curr_ctrl.LEFT)
    elif key == 'd':
        curr_ctrl.set_scroll(curr_ctrl.RIGHT)
#
    elif key == '+':
        curr_ctrl.zoom /= 1.25
    elif key == '-':
        curr_ctrl.zoom *= 1.25
#
#    curr_ctrl.print_stuff = True

def keyReleased(*args):
#    if args[0] in 'ikjl':
#        curr_ctrl.set_rotate(None)

    if args[0] == 'w':
        curr_ctrl.clear_scroll(curr_ctrl.UP)
    elif args[0] == 's':
        curr_ctrl.clear_scroll(curr_ctrl.DOWN)
    elif args[0] == 'a':
        curr_ctrl.clear_scroll(curr_ctrl.LEFT)
    elif args[0] == 'd':
        curr_ctrl.clear_scroll(curr_ctrl.RIGHT)

def timer(value):
    curr_ctrl.tick()
    glutTimerFunc(SLEEP_TIME, timer, value)

prev_time = time()
frames = 0
def render():
    global frames
    global prev_time

    if SHOW_FPS:
        curr_time = time()
        if curr_time - prev_time >= 1:
            print 'fps:', frames
            frames = 0
            prev_time = curr_time

        frames += 1

    curr_ctrl.render()

def resize(width, height):
    curr_ctrl.resize(width, height)

def mouse_click(button, state, x, y):
    if(state == GLUT_DOWN):
        curr_ctrl.mouse_down(x, y, button)

def main():
    global window
    global curr_ctrl
    curr_ctrl = Controller.get_controller()

    glutInit(sys.argv)

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
    glutInitWindowSize(*RESOLUTION)
    glutInitWindowPosition(0, 0)
    window = glutCreateWindow("Hex Grid Thing...")
    glutDisplayFunc(render)

    # glutFullScreen()

    glutIdleFunc(render)
    glutTimerFunc(SLEEP_TIME, timer, 0)
    glutReshapeFunc(resize)
    glutKeyboardFunc(keyPressed)
    glutKeyboardUpFunc(keyReleased)
    glutMouseFunc(mouse_click)

    curr_ctrl.init_gl(*RESOLUTION)
    curr_ctrl.init_board()

    glutMainLoop()

print "Hit ESC key to quit."
main()



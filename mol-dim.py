import numpy as np
from OpenGL.GL import *
from OpenGL.GLU import *
import glfw

# Simulation parameters
N = 200
positions = np.random.rand(N, 3) * 10
velocities = np.random.randn(N, 3) * 0.1
dt = 0.01

# Camera control
zoom = 30
rotate_x, rotate_y = 0, 0
last_mouse_pos = [0, 0]
mouse_pressed = False

# Sphere rendering
quadric = gluNewQuadric()

def compute_forces(pos):
    forces = np.zeros_like(pos)
    for i in range(len(pos)):
        for j in range(i + 1, len(pos)):
            r_vec = pos[i] - pos[j]
            r = np.linalg.norm(r_vec)
            if r == 0:
                continue
            f = 24 * ((2 / r**14) - (1 / r**8)) * r_vec
            forces[i] += f
            forces[j] -= f
    return forces

def update():
    global positions, velocities
    forces = compute_forces(positions)
    velocities += forces * dt
    positions += velocities * dt

def draw_particles():
    for p in positions:
        glPushMatrix()
        glTranslatef(*p)
        glColor3f(0.2, 0.5, 1.0)
        gluSphere(quadric, 0.2, 12, 12)
        glPopMatrix()

def setup_lighting():
    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)
    light_pos = [10.0, 10.0, 20.0, 1.0]
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, [1.0, 1.0, 1.0, 1.0])
    glLightfv(GL_LIGHT0, GL_SPECULAR, [1.0, 1.0, 1.0, 1.0])
    glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE)

# Callbacks for interaction
def scroll_callback(window, xoffset, yoffset):
    global zoom
    zoom -= yoffset
    zoom = max(5, min(zoom, 100))

def mouse_button_callback(window, button, action, mods):
    global mouse_pressed
    if button == glfw.MOUSE_BUTTON_LEFT:
        mouse_pressed = (action == glfw.PRESS)

def cursor_pos_callback(window, xpos, ypos):
    global rotate_x, rotate_y, last_mouse_pos
    if mouse_pressed:
        dx = xpos - last_mouse_pos[0]
        dy = ypos - last_mouse_pos[1]
        rotate_x += dy * 0.5
        rotate_y += dx * 0.5
    last_mouse_pos = [xpos, ypos]

def key_callback(window, key, scancode, action, mods):
    global zoom
    if action == glfw.PRESS:
        if key == glfw.KEY_EQUAL or key == glfw.KEY_KP_ADD:  # "+" key
            zoom -= 2
        elif key == glfw.KEY_MINUS or key == glfw.KEY_KP_SUBTRACT:
            zoom += 2
        zoom = max(5, min(zoom, 100))

def main():
    if not glfw.init():
        return
    window = glfw.create_window(800, 600, "3D Molecular Dynamics", None, None)
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    glfw.set_scroll_callback(window, scroll_callback)
    glfw.set_cursor_pos_callback(window, cursor_pos_callback)
    glfw.set_mouse_button_callback(window, mouse_button_callback)
    glfw.set_key_callback(window, key_callback)

    glEnable(GL_DEPTH_TEST)
    setup_lighting()

    while not glfw.window_should_close(window):
        update()

        width, height = glfw.get_framebuffer_size(window)
        glViewport(0, 0, width, height)

        # Background color (white)
        glClearColor(1.0, 1.0, 1.0, 1.0)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        # Projection
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(45, width / height, 0.1, 100.0)

        # Camera and model transform
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        gluLookAt(0, 0, zoom, 0, 0, 0, 0, 1, 0)
        glRotatef(rotate_x, 1, 0, 0)
        glRotatef(rotate_y, 0, 1, 0)

        draw_particles()

        glfw.swap_buffers(window)
        glfw.poll_events()

    glfw.terminate()

if __name__ == "__main__":
    main()


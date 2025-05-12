#include <GL/glut.h>

float angle = 0.0f;

void drawCube(float size) {
    float hs = size / 2.0f; // Half size

    glBegin(GL_QUADS);
    
    // Front
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(hs, hs, hs);
    glVertex3f(-hs, hs, hs);
    glVertex3f(-hs, -hs, hs);
    glVertex3f(hs, -hs, hs);

    // Back
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(hs, hs, -hs);
    glVertex3f(-hs, hs, -hs);
    glVertex3f(-hs, -hs, -hs);
    glVertex3f(hs, -hs, -hs);

    // Top
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(hs, hs, -hs);
    glVertex3f(-hs, hs, -hs);
    glVertex3f(-hs, hs, hs);
    glVertex3f(hs, hs, hs);

    // Bottom
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(hs, -hs, hs);
    glVertex3f(-hs, -hs, hs);
    glVertex3f(-hs, -hs, -hs);
    glVertex3f(hs, -hs, -hs);

    // Right
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(hs, hs, -hs);
    glVertex3f(hs, hs, hs);
    glVertex3f(hs, -hs, hs);
    glVertex3f(hs, -hs, -hs);

    // Left
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-hs, hs, hs);
    glVertex3f(-hs, hs, -hs);
    glVertex3f(-hs, -hs, -hs);
    glVertex3f(-hs, -hs, hs);

    glEnd();
}

void drawMenger(int depth, float size) {
    if (depth == 0) {
        drawCube(size);
        return;
    }

    float newSize = size / 3.0f;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                int sum = abs(x) + abs(y) + abs(z);
                if (sum > 1) { // Skip center and middle axes
                    glPushMatrix();
                    glTranslatef(x * newSize, y * newSize, z * newSize);
                    drawMenger(depth - 1, newSize);
                    glPopMatrix();
                }
            }
        }
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(angle, 1.0f, 1.0f, 0.0f);

    drawMenger(3, 2.0f); // depth=3 gives a good balance of detail and performance

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = 1.0f * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    angle += 0.5f;
    if (angle > 360) angle -= 360;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Menger Sponge Fractal - OpenGL");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}


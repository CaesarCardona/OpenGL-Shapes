#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

GLuint shaderProgram;
float angle = 0.0f;

const char* vertexShaderSource = R"(
#version 120
attribute vec3 position;
void main() {
    gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 120
void main() {
    gl_FragColor = vec4(gl_FragCoord.x/800.0, gl_FragCoord.y/600.0, 0.5, 1.0);
}
)";

GLuint createShaderProgram() {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    return program;
}

void drawCube() {
    GLfloat vertices[][3] = {
        {1,1,1}, {-1,1,1}, {-1,-1,1}, {1,-1,1},     // Front
        {1,1,-1}, {-1,1,-1}, {-1,-1,-1}, {1,-1,-1}  // Back
    };

    GLint faces[][4] = {
        {0, 1, 2, 3}, // Front
        {4, 5, 6, 7}, // Back
        {4, 5, 1, 0}, // Top
        {3, 2, 6, 7}, // Bottom
        {0, 3, 7, 4}, // Right
        {1, 5, 6, 2}  // Left
    };

    glBegin(GL_QUADS);
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 4; ++j)
            glVertex3fv(vertices[faces[i][j]]);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(angle, 1.0f, 1.0f, 0.0f);

    glUseProgram(shaderProgram);
    drawCube();
    glUseProgram(0);

    glutSwapBuffers();
}

void timer(int value) {
    angle += 1.0f;
    if (angle > 360) angle -= 360;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void init() {
    glewInit();
    glEnable(GL_DEPTH_TEST);
    shaderProgram = createShaderProgram();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = (float)w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shader Cube - OpenGL");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}


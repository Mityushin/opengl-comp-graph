//
// Created by Dmitry Mityushin on 28.10.18.
//

#include <GL/glut.h>
#include <math.h>
#include <error.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLfloat red = 0.0f, green = 0.0f, blue = 0.0f;

GLfloat obs_speed = 0.1f;
GLfloat obs_x = 0.0f, obs_y = 0.0f, obs_z = 5.0f;

void init(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glShadeModel(GL_FLAT);
    glLineStipple(3, 0xAAAA);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(red, green, blue);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // View transform
    gluLookAt(obs_x, obs_y, obs_z,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Model transform
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex3f(-1.0f, -4.5f, 0.0f);
    glVertex3f(-1.0f, 4.5f, 0.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glPushMatrix();
    glTranslatef(-1.0f, -0.75f, 0.0f);
    glScalef(0.5f, 0.5f, 1.0f);
    glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
    glutWireOctahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, 0.5f, 0.0f);
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    glutWireCone(0.5, 2.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, 2.1f, 0.0f);
    glRotatef(15.0f, 0.0f, 0.0f, -1.0f);
    glutWireTeapot(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, -2.0f, 0.0f);
    glutWireTorus(0.1, 0.6, 20, 40);
    glPopMatrix();

    glFlush();
}

void spinObserver(void) {

    obs_x -= obs_speed * 0.075 * cos(0.9 * obs_z);
    obs_z -= obs_speed * 0.075 * sin(0.9 * obs_x);

    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0)
        h = 1;
//    GLfloat ratio = (GLfloat) (w * 1.0 / h);

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

//    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
//    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glOrtho(-3.0, 3.0, -3.0, 3.0, -3.0, 20.0);


    glMatrixMode(GL_MODELVIEW);
}

// При нажатии левой кнопки зарегистрировать
// функцию фоновой обработки (поворота)
// При нажатии правой – отменить регистрацию
void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                glutIdleFunc(spinObserver);
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                glutIdleFunc(NULL);
            }
            break;
        default:
            break;
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
        case 32:
            obs_speed = 0.1f;
            break;
        case 43:
            obs_speed += 0.05f;
            break;
        case 45:
            obs_speed -= 0.05f;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(200, 100);

    glutCreateWindow("Hello, labs");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}

//
// Created by Dmitry Mityushin on 28.10.18.
//

#include <GL/glut.h>
#include <math.h>
#include <error.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FIGURE_VERTICAL -1.0f

GLfloat red = 0.0f, green = 0.0f, blue = 0.0f;

GLfloat obs_speed = 0.1f;
GLfloat obs_x = 0.0f, obs_y = 0.0f, obs_z = 5.0f;

GLboolean octahedron_is_visible = GL_FALSE;
GLboolean octahedron_is_rotated = GL_FALSE;
GLfloat octahedron_translate[3] = {1.0f, -0.75f, 0.0f};
GLfloat octahedron_scale[3] = {0.5f, 0.5f, 1.0f};
GLfloat octahedron_rotate[4], octahedron_rotate_default[4] = {0.0f};
GLfloat octahedron_rotate_rotated[4] = {-60.0f, 1.0f, 0.0f, 0.0f};

GLboolean cone_is_visible = GL_FALSE;
GLboolean cone_is_rotated = GL_FALSE;
GLfloat cone_translate[3] = {2.0f, 0.5f, 0.0f};
GLfloat cone_rotate[4], cone_rotate_default[4] = {0.0f};
GLfloat cone_rotate_rotated[4] = {45.0f, 0.0f, 1.0f, 0.0f};

GLboolean teapot_is_visible = GL_FALSE;
GLfloat teapot_translate[3] = {-1.5f, 2.1f, 0.0f};
GLfloat teapot_rotate[4] = {15.0f, 0.0f, 0.0f, -1.0f};

GLboolean torus_is_visible = GL_FALSE;
GLfloat torus_translate[3] = {-2.0f, -2.0f, 0.0f};

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

    if (octahedron_is_visible) {
        glPushMatrix();
        glTranslatef(octahedron_translate[0], octahedron_translate[1], octahedron_translate[2]);
        glScalef(octahedron_scale[0], octahedron_scale[1], octahedron_scale[2]);
        glRotatef(octahedron_rotate[0], octahedron_rotate[1], octahedron_rotate[2], octahedron_rotate[3]);
        glutWireOctahedron();
        glPopMatrix();
    }
    if (cone_is_visible) {
        glPushMatrix();
        glTranslatef(cone_translate[0], cone_translate[1], cone_translate[2]);
        glRotatef(cone_rotate[0], cone_rotate[1], cone_rotate[2], cone_rotate[3]);
        glutWireCone(0.5, 2.0, 20, 20);
        glPopMatrix();
    }
    if (teapot_is_visible) {
        glPushMatrix();
        glTranslatef(teapot_translate[0], teapot_translate[1], teapot_translate[2]);
        glRotatef(teapot_rotate[0], teapot_rotate[1], teapot_rotate[2], teapot_rotate[3]);
        glutWireTeapot(1.0);
        glPopMatrix();
    }
    if (torus_is_visible) {
        glPushMatrix();
        glTranslatef(torus_translate[0], torus_translate[1], torus_translate[2]);
        glutWireTorus(0.1, 0.6, 20, 40);
        glPopMatrix();
    }
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
        case '+':
            obs_speed += 0.05f;
            break;
        case '-':
            obs_speed -= 0.05f;
            break;
        case 'v':
            octahedron_translate[0] = FIGURE_VERTICAL;
            cone_translate[0] = FIGURE_VERTICAL;
            teapot_translate[0] = FIGURE_VERTICAL;
            torus_translate[0] = FIGURE_VERTICAL;
            break;
        case '1':
            if (octahedron_is_visible == GL_FALSE) {
                octahedron_is_visible = GL_TRUE;
                break;
            }
            for (int i = 0; i < 4; ++i) {
                octahedron_rotate[i] = octahedron_is_rotated ? octahedron_rotate_default[i] : octahedron_rotate_rotated[i];
            }
            octahedron_is_rotated = (GLboolean) !octahedron_is_rotated;
            break;
        case '2':
            if (cone_is_visible == GL_FALSE) {
                cone_is_visible = GL_TRUE;
                break;
            }
            for (int i = 0; i < 4; ++i) {
                cone_rotate[i] = cone_is_rotated ? cone_rotate_default[i] : cone_rotate_rotated[i];
            }
            cone_is_rotated = (GLboolean) !cone_is_rotated;
            break;
        case '3':
            teapot_is_visible = GL_TRUE;
            break;
        case '4':
            torus_is_visible = GL_TRUE;
            break;
        default:
            break;
    }
    glutPostRedisplay();
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

//    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}

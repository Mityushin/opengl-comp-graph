//
// Created by Dmitry Mityushin on 28.10.18.
//

#include <GL/glut.h>
#include <math.h>
#include <error.h>
#include <stdio.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FIGURE_VERTICAL -1.0f

GLfloat default_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};

GLfloat obs_speed = 0.025f;
GLfloat obs_angle[3] = {0.0, 0.0, 0.0};
GLfloat obs_angle_h[3] = {3.0, 3.0, 3.0};
GLfloat obs_x = 0.0f, obs_y = 0.0f, obs_z = 5.0f;

//region octahedron

GLfloat octahedron_color[4] = {64.0f, 0.0f, 0.0f, 1.0f};
GLboolean octahedron_is_visible = GL_TRUE;
GLboolean octahedron_is_rotated = GL_FALSE;
GLfloat octahedron_translate[3] = {1.0f, -0.75f, 0.0f};
GLfloat octahedron_scale[3] = {0.5f, 0.5f, 1.0f};
GLfloat octahedron_rotate[4], octahedron_rotate_default[4] = {0.0f};
GLfloat octahedron_rotate_rotated[4] = {-60.0f, 1.0f, 0.0f, 0.0f};
//endregion

//region cone

GLfloat cone_color[4] = {0.0f, 64.0f, 0.0f, 1.0f};
GLboolean cone_is_visible = GL_TRUE;
GLboolean cone_is_rotated = GL_FALSE;
GLfloat cone_translate[3] = {2.0f, 0.5f, 0.0f};
GLfloat cone_rotate[4], cone_rotate_default[4] = {0.0f};
GLfloat cone_rotate_rotated[4] = {45.0f, 0.0f, 1.0f, 0.0f};
//endregion

//region teapot

GLfloat teapot_color[4] = {0.0f, 0.0f, 64.0f, 1.0f};
GLboolean teapot_is_visible = GL_TRUE;
GLfloat teapot_translate[3] = {-1.5f, 2.1f, 0.0f};
GLfloat teapot_rotate[4] = {15.0f, 0.0f, 0.0f, -1.0f};
//endregion

//region torus

GLfloat torus_color[4] = {64.0f, 0.0f, 64.0f, 1.0f};
GLboolean torus_is_visible = GL_TRUE;
GLfloat torus_translate[3] = {-2.0f, -2.0f, 0.0f};
//endregion

GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};

GLfloat diffuse_material[4] = {128.0, 0.0, 0.0, 1.0};

GLfloat mat_specular_mirror[] = {10.0, 10.0, 10.0, 10.0};
GLfloat mat_shininess_mirror[] = {50.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 0.0};
GLfloat mat_shininess[] = {128.0};

GLuint loadBMP_custom(const char *imagepath) {
    // Данные, прочитанные из заголовка BMP-файла
    unsigned char header[54]; // Каждый BMP-файл начинается с заголовка, длиной в 54 байта
    unsigned int dataPos;     // Смещение данных в файле (позиция данных)
    unsigned int width, height;
    unsigned int imageSize;   // Размер изображения = Ширина * Высота * 3
    // RGB-данные, полученные из файла
    unsigned char *data;
    FILE *file = fopen(imagepath, "rb");
    if (!file) {
        printf("Изображение не может быть открыто\n");
        return 0;
    }
    if (fread(header, 1, 54, file) != 54) { // Если мы прочитали меньше 54 байт, значит возникла проблема
        printf("Некорректный BMP-файл\n");
        exit(1);
    }
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Некорректный BMP-файл\n");
        exit(1);
    }
    // Читаем необходимые данные
    dataPos = *(unsigned int *) &(header[0x0A]); // Смещение данных изображения в файле
    imageSize = *(unsigned int *) &(header[0x22]); // Размер изображения в байтах
    width = *(unsigned int *) &(header[0x12]); // Ширина
    height = *(unsigned int *) &(header[0x16]); // Высота
    // Некоторые BMP-файлы имеют нулевые поля imageSize и dataPos, поэтому исправим их
    if (imageSize == 0) imageSize = width * height * 3; // Ширину * Высоту * 3, где 3 - 3 компоненты цвета (RGB)
    if (dataPos == 0) dataPos = 54; // В таком случае, данные будут следовать сразу за заголовком

    // Создаем буфер
    data = malloc(sizeof(unsigned char) * imageSize);
    // Считываем данные из файла в буфер
    fread(data, 1, imageSize, file);
    // Закрываем файл, так как больше он нам не нужен
    fclose(file);
    // Создадим одну текстуру OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);
    // Сделаем созданную текстуру текущий, таким образом все следующие функции будут работать именно с этой текстурой
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Передадим изображение OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    return textureID;
}

void init(void) {
    glShadeModel(GL_FLAT);
    glLineStipple(3, 0xAAAA);

    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_ambient[] = {0.0, 0.0, 1.0, 1.0};

    glClearColor(0.0, 0.0, 0.3, 1.0);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position); //Положение источника света
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); //Интенсивность фонового света
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); //Интенсивность диффузного света
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light); //Интенсивность зеркального света

    glEnable(GL_LIGHTING); //Включить расчёт освещения
    glEnable(GL_LIGHT0); //Включить первый источник
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); //Локальная точка наблюдателя

    GLuint texture = loadBMP_custom("bicycle.bmp");
}

void displayTeapot() {
    if (teapot_is_visible) {
        glEnable(GL_TEXTURE_2D);
        glColor4fv(teapot_color);
        glPushMatrix();
        glTranslatef(teapot_translate[0], teapot_translate[1], teapot_translate[2]);
        glRotatef(teapot_rotate[0], teapot_rotate[1], teapot_rotate[2], teapot_rotate[3]);
        glutSolidTeapot(1.0);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}

void displayCone() {
    if (cone_is_visible) {
        glColor4fv(cone_color);
        glPushMatrix();
        glTranslatef(cone_translate[0], cone_translate[1], cone_translate[2]);
        glRotatef(cone_rotate[0], cone_rotate[1], cone_rotate[2], cone_rotate[3]);
        glutSolidCone(0.5, 2.0, 20, 20);
        glPopMatrix();
    }
}

void displayOctahedron() {
    if (octahedron_is_visible) {
        glColor4fv(octahedron_color);
        glPushMatrix();
        glTranslatef(octahedron_translate[0], octahedron_translate[1], octahedron_translate[2]);
        glScalef(octahedron_scale[0], octahedron_scale[1], octahedron_scale[2]);
        glRotatef(octahedron_rotate[0], octahedron_rotate[1], octahedron_rotate[2], octahedron_rotate[3]);
        glutSolidOctahedron();
        glPopMatrix();
    }
}

void displayTorus() {
    if (torus_is_visible) {
        glColor4fv(torus_color);
        glPushMatrix();
        glTranslatef(torus_translate[0], torus_translate[1], torus_translate[2]);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_mirror); //Зеркальный цвет материала
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_mirror);//Показатель зеркального отражения
        glutSolidTorus(0.1, 0.6, 20, 40);
        glPopMatrix();
    }
}

void displayLine() {
    glColor4fv(default_color);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex3f(-1.0f, -4.5f, 0.0f);
    glVertex3f(-1.0f, 4.5f, 0.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void displaySun() {
//    glColor3d(1.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(light_position[0], light_position[1], light_position[2]);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(obs_x, obs_y, obs_z,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

//    glRotatef(obs_angle[0], 1.0, 0.0, 0.0);
//    glRotatef(obs_angle[1], 0.0, 1.0, 0.0);
//    glRotatef(obs_angle[2], 0.0, 0.0, 1.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular); //Зеркальный цвет материала
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);//Показатель зеркального отражения

    float whiteColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, whiteColor);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, whiteColor);

    displaySun();

    float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, clearColor);

    displayLine();
    displayOctahedron();
    displayCone();
    displayTeapot();
    displayTorus();
    glFlush();
}

void spinObserver(void) {

    GLfloat radius = 1.0f;
//    obs_x = (GLfloat) (sin(time(NULL)) * radius);
//    obs_z = (GLfloat) (cos(time(NULL)) * radius);

//    obs_x -= obs_speed * 0.075 * cos(0.9 * obs_z);
//    obs_z -= obs_speed * 0.075 * sin(0.9 * obs_x);

    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0)
        h = 1;
    GLfloat ratio = (GLfloat) (w * 1.0 / h);

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
//    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
//    glOrtho(-10.0, 10.0,
//            -10.0, 10.0,
//            -10.0, 10.0);


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
                octahedron_rotate[i] = octahedron_is_rotated ? octahedron_rotate_default[i]
                                                             : octahedron_rotate_rotated[i];
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

void special(int key, int x, int y) {
    int modifiers = glutGetModifiers();
    switch (key) {
        case GLUT_KEY_UP:
            obs_angle[0] += obs_angle_h[0];
            break;
        case GLUT_KEY_DOWN:
            obs_angle[0] -= obs_angle_h[0];
            break;
        case GLUT_KEY_LEFT:
            obs_angle[1] += obs_angle_h[1];
            break;
        case GLUT_KEY_RIGHT:
            obs_angle[1] -= obs_angle_h[1];
            break;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(200, 100);

    glutCreateWindow("Hello, labs");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();

    return 0;
}

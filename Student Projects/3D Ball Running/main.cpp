#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio> // FIXED: Required for sprintf

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int NUM_OBS = 5;

float ballX = 0.0f;
float ballZ = -5.0f;

float speed = 0.2f;

float obsX[NUM_OBS] = { 0, 2, -2, 1, -1 };
float obsZ[NUM_OBS] = { -20, -35, -50, -70, -90 };

bool gameOver = false;
int score = 0;

GLuint groundTexture;
bool hasTexture = false;

// ================= RESET =================
void resetGame()
{
    ballX = 0.0f;
    ballZ = -5.0f;

    speed = 0.2f;
    score = 0;
    gameOver = false;

    obsZ[0] = -20;
    obsZ[1] = -35;
    obsZ[2] = -50;
    obsZ[3] = -70;
    obsZ[4] = -90;

    obsX[0] = 0;
    obsX[1] = 2;
    obsX[2] = -2;
    obsX[3] = 1;
    obsX[4] = -1;
}

// ================= INIT =================
void init()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Dark blue background looks better than pure black
    glEnable(GL_DEPTH_TEST);

    // Enable Lighting and Colors
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); // FIXED: Allows glColor3f to work alongside lighting

    GLfloat lightPos[] = { 0, 10, 5, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat lightColor[] = { 1, 1, 1, 1 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

    // Texture Loading
    glEnable(GL_TEXTURE_2D);

    int w, h, c;
    unsigned char* data = stbi_load("ground.jpg", &w, &h, &c, 0);

    if (data)
    {
        hasTexture = true;
        glGenTextures(1, &groundTexture);
        glBindTexture(GL_TEXTURE_2D, groundTexture);

        // Texture parameters so it repeats seamlessly
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Warning: ground.jpg not found. Using solid color instead." << std::endl;
    }
}

// ================= COLLISION =================
bool checkCollision(float bx, float bz, float ox, float oz)
{
    float dx = bx - ox;
    float dz = bz - oz;
    return (dx * dx + dz * dz) < 1.0;
}

// ================= DRAW =================
void drawGround()
{
    // FIXED: Actually draw the ground so the player has reference for movement
    glColor3f(0.5f, 0.5f, 0.5f); // Base color if texture fails

    if (hasTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_QUADS);
    // Draw a long runway. We use ballZ so the ground moves with the player.
    float startZ = ballZ + 10.0f;
    float endZ = ballZ - 150.0f;

    // Texture coordinates mapped to simulate movement
    glTexCoord2f(0.0f, startZ * 0.1f); glVertex3f(-10.0f, -0.5f, startZ);
    glTexCoord2f(10.0f, startZ * 0.1f); glVertex3f(10.0f, -0.5f, startZ);
    glTexCoord2f(10.0f, endZ * 0.1f);  glVertex3f(10.0f, -0.5f, endZ);
    glTexCoord2f(0.0f, endZ * 0.1f);  glVertex3f(-10.0f, -0.5f, endZ);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void drawBall()
{
    glPushMatrix();
    glTranslatef(ballX, 0, ballZ);
    glColor3f(1, 0, 0); // Red
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

void drawObstacles()
{
    glColor3f(0, 0, 1); // Blue
    for (int i = 0; i < NUM_OBS; i++)
    {
        glPushMatrix();
        glTranslatef(obsX[i], 0, obsZ[i]);
        glutSolidCube(1);
        glPopMatrix();
    }
}

// ================= DISPLAY =================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        ballX, 3, ballZ + 5,
        ballX, 0, ballZ - 10,
        0, 1, 0
    );

    drawGround();
    drawBall();
    drawObstacles();

    // FIXED: Disable lighting before drawing 2D text so it doesn't turn black
    glDisable(GL_LIGHTING);

    // ===== SCORE =====
    glColor3f(1, 1, 1);
    glRasterPos3f(ballX - 2, 3, ballZ - 5);

    char text[50];
    snprintf(text, sizeof(text), "Score: %d", score); // Safer than sprintf

    for (int i = 0; text[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);

    // ===== GAME OVER OVERLAY =====
    if (gameOver)
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(1, 0, 0); // Red text

        glRasterPos2f(340, 320);
        const char* msg = "GAME OVER";
        for (int i = 0; msg[i]; i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);

        glRasterPos2f(320, 280);
        const char* msg2 = "Press R to Restart";
        for (int i = 0; msg2[i]; i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg2[i]);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    // Turn lighting back on for the next 3D frame
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

// ================= INPUT =================
void specialKeys(int key, int x, int y)
{
    if (gameOver) return;

    if (key == GLUT_KEY_LEFT && ballX > -2.0f) // Added limits so ball doesn't fly off screen
        ballX -= 0.5f;

    if (key == GLUT_KEY_RIGHT && ballX < 2.0f)
        ballX += 0.5f;
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'r' || key == 'R')
        resetGame();
}

// ================= UPDATE =================
void update(int value)
{
    if (!gameOver)
    {
        ballZ -= speed;
        score++;
        speed += 0.00005f; // Made speed scale slightly faster for fun

        for (int i = 0; i < NUM_OBS; i++)
        {
            if (checkCollision(ballX, ballZ, obsX[i], obsZ[i]))
            {
                gameOver = true;
            }

            if (obsZ[i] > ballZ + 5)
            {
                obsZ[i] = ballZ - (40 + rand() % 50); // Pushed obstacles further back to prevent instant pop-in
                obsX[i] = (rand() % 3) - 1;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

// ================= MAIN =================
int main(int argc, char** argv)
{
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);

    glutCreateWindow("3D Ball Runner");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc([](int w, int h) {
        if (h == 0) h = 1; // Prevent divide by zero
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float)w / h, 1.0, 200.0);
        glMatrixMode(GL_MODELVIEW);
        });

    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
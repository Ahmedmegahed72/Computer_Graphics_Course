#define STB_IMAGE_IMPLEMENTATION
#include<GL/stb_image.h>
#include<iostream>
#include<cmath>
#include<Windows.h>
#include<gl/glut.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

GLuint backTexture;
GLuint gameOverTexture;
GLuint winTexture;
GLuint wallTexture;
GLuint floorTexture;
GLuint doorTexture;

void loadGameOverTexture(const char* filename) {
    int width, height, nrchannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrchannels, 0);
    if (data) {
        GLenum format = (nrchannels == 4) ? GL_RGBA : GL_RGB;
        glGenTextures(1, &gameOverTexture);
        glBindTexture(GL_TEXTURE_2D, gameOverTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        //If texture exceeds size:repeat image.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_TEXTURE_WRAP_S:Horizontal wrapping.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_TEXTURE_WRAP_T:Vertical wrapping.

        //When texture zooms:smooth pixels.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        cout << "Failed to Load Your Gameover Image!" << endl;
    }

}

void drawGameOver()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, gameOverTexture);

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);

    glTexCoord2f(0, 1); glVertex2f(0, 0);
    glTexCoord2f(1, 1); glVertex2f(800, 0);
    glTexCoord2f(1, 0); glVertex2f(800, 600);
    glTexCoord2f(0, 0); glVertex2f(0, 600);

    glEnd();

    glDisable(GL_TEXTURE_2D); //otherwise next objects may accidentally use same texture.
    glEnable(GL_LIGHTING);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void loadWinTexture(const char* filename) {
    int width, height, nrchannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrchannels, 0);
    if (data) {
        GLenum format = (nrchannels == 4) ? GL_RGBA : GL_RGB;
        glGenTextures(1, &winTexture);
        glBindTexture(GL_TEXTURE_2D, winTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        cout << "Failed to Load Your Win Image!" << endl;
    }

}

void drawWinTexture()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, winTexture);

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);

    glTexCoord2f(0, 1); glVertex2f(0, 0);
    glTexCoord2f(1, 1); glVertex2f(800, 0);
    glTexCoord2f(1, 0); glVertex2f(800, 600);
    glTexCoord2f(0, 0); glVertex2f(0, 600);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

GLuint loadTexture(const char* filename)
{
    GLuint textureID;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (!data)
    {
        cout << "Failed to load texture: " << filename << endl;
        return 0;
    }

    GLenum format;

    if (nrChannels == 4)
        format = GL_RGBA;
    else
        format = GL_RGB;

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}

//===global variables====

float ratio;
bool fullScreenMode = true;

bool showStartText = true;
int startTime = 0;

int gameStartTime = 0;
int gameDuration = 60;
int timeLeft = 60;

float obstacleZ = 0.0f;
float obstacleDir = 1.0f;

bool doorOpen = false;
float doorAngle = 0.0f;

bool showDoorMsg = false;
int doorMsgStartTime = 0;

float trnsxd = 11.85;
float trnsyd = 1.25;
float trnszd = 4.5;
int level = 0;
int final = 0;

bool isGameOver = false;
bool isWin = false;

bool printedWin = false;

// ====== Player Variables (Part 2) ======
int player_i = 1, player_j = 1; // choose the index of the place you want to make the player in
float playerX = (player_i - 4.5f) * 3.0f;
float playerY = 1.5f;
float playerZ = (player_j - 4.5f) * 3.0f;

float speed = 3.0f;
float cameraAngle = 0.0f; // Mouse Control

int key_i = 7, key_j = 5;
bool hasKey = false;
float keyX = (key_i - 4.5f) * 3.0f;
float keyZ = (key_j - 4.5f) * 3.0f;
// ====== Function Declarations ======
void drawWall(float, float, float);
void drawFloor();
void drawDoor();
void drawMaze();
void drawPlayer();
void drawKey();
void background();
void display();
void reshsape(int, int);
void update(int);
void keyboard(unsigned char, int, int);
void mouseMotion(int, int);
void specialKeyboard(int, int, int);
bool isWall(float, float);
void checkKey();
void drawText(float, float, string, float = 1, float = 1, float = 1);
void checkDoor();
void restartGame();
// ====== Maze ======
int Maze[10][10] = {
{1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,1,0,0,0,0,1},
{1,0,1,0,1,0,1,1,0,1},
{1,0,1,0,0,0,0,1,0,1},
{1,0,1,1,1,1,0,1,0,1},
{1,0,0,0,0,1,0,0,0,1},
{1,1,1,1,0,1,1,1,0,0},
{1,0,0,1,0,0,0,1,0,1},
{1,0,0,0,0,1,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1}
};

//===Maze2===
int Maze2[10][10] = {
{1,1,1,1,1,0,1,1,1,1},
{1,0,1,0,0,0,1,0,0,1},
{1,0,1,0,1,0,1,1,0,1},
{1,0,0,0,1,0,0,1,0,1},
{1,1,1,0,1,1,0,1,0,1},
{1,0,0,0,0,1,0,0,0,1},
{1,0,1,1,0,1,1,1,0,1},
{1,0,0,1,0,0,0,1,0,1},
{1,0,0,0,0,1,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1}
};

// ====== MAIN ======
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(800, 600);
    glutCreateWindow("Maze Project");
    loadGameOverTexture("C:\\Users\\LENOVO\\Documents\\OpenGL Programming\\images\\youlose.png");
    loadWinTexture("C:\\Users\\LENOVO\\Documents\\OpenGL Programming\\images\\you won.png");
    wallTexture = loadTexture("C:\\Users\\LENOVO\\Documents\\OpenGL Programming\\images\\Stone wall.jpg");
    floorTexture = loadTexture("C:\\Users\\LENOVO\\Documents\\OpenGL Programming\\images\\ground1.jpg");
    doorTexture = loadTexture("C:\\Users\\LENOVO\\Documents\\OpenGL Programming\\images\\door.jpg");
    glutFullScreen();
    glutSetWindow(glutGetWindow());
    //glutSetCursor(GLUT_CURSOR_NONE);

    background();
    startTime = glutGet(GLUT_ELAPSED_TIME);
    gameStartTime = glutGet(GLUT_ELAPSED_TIME);


    glutDisplayFunc(display);
    glutReshapeFunc(reshsape);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion); // Mouse Control
    glutSetCursor(GLUT_CURSOR_NONE);
    glutSpecialFunc(specialKeyboard);

    glutMainLoop();
    return 0;
}

// ====== DRAW FUNCTIONS (Part 1) ======

void drawWall(float x, float y, float z)
{
    glPushMatrix();
    glTranslatef(x, y, z);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wallTexture);

    glColor3f(1, 1, 1);

    float s = 1.5f;

    glBegin(GL_QUADS);

    // FRONT
    glNormal3f(0, 0, 1); //This surface faces: positive Z direction. Lighting uses normals to know: where light hits.
    glTexCoord2f(0, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);

    // BACK
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(s, s, -s);

    // LEFT
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

    // RIGHT
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(s, s, s);

    // TOP
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, s, s);
    glTexCoord2f(1, 0); glVertex3f(s, s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

    // BOTTOM
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, -s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, -s, s);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void drawFloor()
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, floorTexture);

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    //Why 5? Texture repeats 5 times across floor. Without this: texture stretches badly.
    glTexCoord2f(0, 0);
    glVertex3f(-15, 0, -15);

    glTexCoord2f(5, 0);
    glVertex3f(15, 0, -15);

    glTexCoord2f(5, 5);
    glVertex3f(15, 0, 15);

    glTexCoord2f(0, 5);
    glVertex3f(-15, 0, 15);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void drawDoor()
{
    glPushMatrix();

    // door position
    glTranslatef(trnsxd, 0.0f, trnszd);

    // rotate in level 2
    if (level == 1)
    {
        glRotatef(90, 0, 1, 0);
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, doorTexture);

    glColor3f(1, 1, 1);

    float w = 0.15f;   // thickness
    float h = 2.5f;    // height
    float z = 1.5f;    // width

    glBegin(GL_QUADS);

    // ===== FRONT =====
    glNormal3f(1, 0, 0);

    glTexCoord2f(0, 0); glVertex3f(w, 0, -z);
    glTexCoord2f(1, 0); glVertex3f(w, 0, z);
    glTexCoord2f(1, 1); glVertex3f(w, h, z);
    glTexCoord2f(0, 1); glVertex3f(w, h, -z);

    // ===== BACK =====
    glNormal3f(-1, 0, 0);

    glTexCoord2f(0, 0); glVertex3f(-w, 0, z);
    glTexCoord2f(1, 0); glVertex3f(-w, 0, -z);
    glTexCoord2f(1, 1); glVertex3f(-w, h, -z);
    glTexCoord2f(0, 1); glVertex3f(-w, h, z);

    // ===== LEFT =====
    glNormal3f(0, 0, -1);

    glTexCoord2f(0, 0); glVertex3f(-w, 0, -z);
    glTexCoord2f(1, 0); glVertex3f(w, 0, -z);
    glTexCoord2f(1, 1); glVertex3f(w, h, -z);
    glTexCoord2f(0, 1); glVertex3f(-w, h, -z);

    // ===== RIGHT =====
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0); glVertex3f(w, 0, z);
    glTexCoord2f(1, 0); glVertex3f(-w, 0, z);
    glTexCoord2f(1, 1); glVertex3f(-w, h, z);
    glTexCoord2f(0, 1); glVertex3f(w, h, z);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void drawMaze()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int cell;

            if (level == 0)
                cell = Maze[i][j];
            else
                cell = Maze2[i][j];

            if (cell == 1)
            {
                float x = (j - 4.5f) * 3.0f;
                float z = (i - 4.5f) * 3.0f;
                drawWall(x, 1.25f, z);
            }
        }
    }
}


void drawKey()
{
    if (hasKey) return;

    float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    glPushMatrix();

    glTranslatef(keyX, 0.5f + sin(t * 3.0f) * 0.15f, keyZ);
    glRotatef(t * 60.0f, 0, 1, 0);


    glColor3f(1.0f, 0.85f, 0.0f);

    // stick
    glPushMatrix();
    glScalef(0.1f, 0.1f, 0.6f);
    glutSolidCube(1);
    glPopMatrix();

    // ring
    glPushMatrix();
    glTranslatef(0, 0.2f, 0);
    glutSolidTorus(0.03, 0.08, 10, 15);
    glPopMatrix();

    glPopMatrix();
}
// ====== PLAYER (Part 2) ======

void drawPlayer()
{
    glPushMatrix();
    glTranslatef(playerX, playerY, playerZ);
    glColor3f(0, 0, 1);
    glutSolidCube(1);
    glPopMatrix();
}

// ====== BACKGROUND ======

void background()
{
    glClearColor(0.15f, 0.15f, 0.18f, 1);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // ===== Ambient Light =====
    GLfloat ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //soft gray light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // ===== Main Light =====
    GLfloat lightPos[] = { 0.0f, 15.0f, 0.0f, 1.0f }; //Light above scene
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; //Strong white light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse); //Diffuse Uses Surface Normals

    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //Creates shiny highlights
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glShadeModel(GL_SMOOTH); //Smooth color transition between vertices

    glEnable(GL_NORMALIZE); //Fixes lighting calculations after scaling
}

// ====== DISPLAY ======

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ===== calculate time left =====
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    timeLeft = gameDuration - (currentTime - gameStartTime) / 1000;
    if (timeLeft < 0)
        timeLeft = 0;

    /// ====== Camera ======

//    gluLookAt(
//    0, 30, 0,
//    0, 0, 0,
//    0, 0, -1
//);

//    // حساب اتجاه الكاميرا من الماوس
    float camX = sin(cameraAngle * 3.14 / 180);
    float camZ = -cos(cameraAngle * 3.14 / 180);

    //// كاميرا بتلف حوالين اللاعب
    gluLookAt(
        playerX, 10, playerZ + 10,              // مكان الكاميرا
        playerX + camX, 0, playerZ + camZ,      // بتبص فين
        0, 1, 0
    );

    //===== lose condition =====
    if (timeLeft <= 0 && !isWin)
    {
        isGameOver = true;
    }

    //===== draw game over screen =====
    if (isGameOver) {
        drawGameOver();
        glutSwapBuffers();
        return;
    }

    else
    {
        drawFloor();
        drawMaze();
        drawDoor();
        drawPlayer();
        drawKey();
    }

    //===== draw win screen =====
    if (isWin && hasKey && timeLeft > 0)
    {
        drawWinTexture();
        glutSwapBuffers();
        return;
    }

    if (!isWin && !isGameOver)
    {
        timeLeft = gameDuration - (currentTime - gameStartTime) / 1000;
    }

    // start message
    if (showStartText)
    {
        drawText(350, 300, "Use WASD to move");

        if (currentTime - startTime > 4000)
        {
            showStartText = false;
        }
    }

    // timer
    string timerText = "Time Left : " + to_string(timeLeft);

    if (timeLeft <= 5)
    {
        drawText(55, 530, timerText, 1, 0, 0); // أحمر
    }
    else
    {
        drawText(55, 530, timerText, 0, 1, 0); //اخضر
    }


    if (!hasKey)
    {
        drawText(55, 550, "Find the Golden Key!", 1, 1, 0);
    }
    else
    {
        drawText(55, 550, "Key is Found!, Escape the Maze", 1, 1, 0);
    }

    if (level == 0)
        drawText(55, 570, "Level 1", 1, 1, 1);
    else
        drawText(55, 570, "Level 2", 1, 1, 1);

    //    if (hasKey)
    //{
    //    drawText(380, 260, "Key Found!", 0, 1, 0);
    //}

    if (showDoorMsg)
    {
        int t = glutGet(GLUT_ELAPSED_TIME);

        drawText(350, 300, "No Key? No Escape!", 1, 0, 0);

        if (t - doorMsgStartTime > 2000)
        {
            showDoorMsg = false;
        }
    }


    glutSwapBuffers();
}

// ====== RESHAPE ======
void reshsape(int w, int h)
{
    if (h == 0) h = 1;
    ratio = w / (float)h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, ratio, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
}

// ====== upadte screen ======
void update(int v)
{

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);

    if (doorOpen && trnszd != 1.5 && level == 0)
    {
        trnszd -= 0.25;// سرعة الفتح
    }
    if (doorOpen && trnsxd != -1.5 && level == 1)
    {
        trnsxd -= 0.25;// سرعة الفتح
    }
    float dx = playerX - 13.5f;
    float dz = playerZ - 4.5f;

    if (sqrt(dx * dx + dz * dz) < 0.2f && hasKey)
    {
        if (level == 0 && timeLeft != 0)
        {
            // نروح Level 2
            level = 1;

            // reset
            playerX = (1 - 4.5f) * 3.0f;
            playerZ = (1 - 4.5f) * 3.0f;

            hasKey = false;

            // مكان مفتاح جديد
            key_i = 8;
            key_j = 1;
            keyX = (key_i - 4.5f) * 3.0f;
            keyZ = (key_j - 4.5f) * 3.0f;

            //مكان الباب الجديد
            trnsxd = 1.5;
            trnsyd = 1.25;
            trnszd = -12.0;

            drawText(55, 570, "Level 2", 1, 1, 1);


            doorOpen = false;


        }

    }

    //===== win condition =====
    if (level == 1)
    {
        float dx = playerX - trnsxd;
        float dz = playerZ - trnszd;

        if (sqrt(dx * dx + dz * dz) < 4.0f && hasKey) //sqrt(dx * dx + dz * dz) calculates the distance between the player and the door
        {
            isWin = true;
        }
    }




}

// ====== KEYBOARD (WASD Movement) ======

void keyboard(unsigned char key, int x, int y)
{
    key = tolower(key);
    //cout << "KEY PRESSED:" << key << endl;

    float newX = playerX;
    float newZ = playerZ;
    //cout <<"Befor: " << newX << " " << newZ << '\n';
    if (key == 'q' || key == 27) exit(0);
    if (key == 'w')
        newZ -= speed;

    if (key == 'r')
    {
        if (timeLeft <= 0 || isWin)
        {
            restartGame();
        }
    }

    if (key == 's')
        newZ += speed;

    if (key == 'a')
        newX -= speed;

    if (key == 'd')
        newX += speed;
    //cout << "After: " << newX << " " << newZ << '\n';
    if (!isWall(newX, newZ))
    {
        playerX = newX;
        playerZ = newZ;
        checkKey();
        checkDoor();
    }
}

// ====== FULLSCREEN ======

void specialKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_F1) {
        fullScreenMode = !fullScreenMode;
        if (fullScreenMode) {
            glutFullScreen();
        }
        else {
            glutReshapeWindow(GetSystemMetrics(SM_CXSCREEN) / 2,
                GetSystemMetrics(SM_CYSCREEN) / 2);
            glutPositionWindow(GetSystemMetrics(SM_CXSCREEN) / 4,
                GetSystemMetrics(SM_CYSCREEN) / 4);
        }
    }
}


// ====== Is Wall ? ======


bool isWall(float x, float z)
{
    int i = (z / 3.0f + 4.5f);
    int j = (x / 3.0f + 4.5f);

    if (i < 0 || i >= 10 || j < 0 || j >= 10)
        return true;

    // ===== Maze collision =====
    bool wall;

    if (level == 0)
        wall = (Maze[i][j] == 1);
    else 
        wall = (Maze2[i][j] == 1);

    return wall;
    // ===== Door collision =====

    float doorX, doorZ;

    if (level == 0)
    {
        doorX = 13.5f;
        doorZ = 4.5f;
    }
    else
    {
        doorX = trnsxd;
        doorZ = trnszd;
    }

    bool nearDoor =
        fabs(x - doorX) < 1.0f &&
        fabs(z - doorZ) < 1.0f;

    // لو قريب من الباب ومفيش مفتاح امنع الحركة
    if (nearDoor && !hasKey)
        return true;
}


void checkKey()
{
    float dx = playerX - keyX;
    float dz = playerZ - keyZ;

    if (sqrt(dx * dx + dz * dz) < 1)
    {
        hasKey = true;
        doorOpen = true;
        PlaySound(TEXT("key.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }
}

//=======PART4==========

//message


void drawText(float x, float y, string text, float r, float g, float b)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);

    //  لون النص
    glColor3f(r, g, b);

    glRasterPos2f(x, y);

    for (int i = 0; i < (int)text.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }

    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

//======check door==
void checkDoor()
{
    float doorX, doorZ;

    if (level == 0)
    {
        doorX = 13.5f;
        doorZ = 4.5f;
    }
    else
    {
        doorX = trnsxd;
        doorZ = trnszd;
    }

    float dx = playerX - doorX;
    float dz = playerZ - doorZ;

    float dist = sqrt(dx * dx + dz * dz);

    if ((level == 0 && dist < 4.0f) ||
        (level == 1 && dist < 2.0f))
    {
        if (!hasKey)
        {
            showDoorMsg = true;
            doorMsgStartTime = glutGet(GLUT_ELAPSED_TIME);
        }
        else
        {
            doorOpen = true;
        }
    }
}


//====mouse motion==

void mouseMotion(int x, int y)
{
    int centerX = 400;

    int deltaX = x - centerX;

    cameraAngle += deltaX * 0.1f; // higer speed

    //360 deg
    if (cameraAngle > 360) cameraAngle -= 360;
    if (cameraAngle < 0)   cameraAngle += 360;

    glutWarpPointer(centerX, 300);

    glutPostRedisplay();
}

void restartGame()
{
    // reset player
    player_i = 1;
    player_j = 1;
    playerX = (player_i - 4.5f) * 3.0f;
    playerZ = (player_j - 4.5f) * 3.0f;

    // reset level
    level = 0;

    // reset key
    hasKey = false;
    key_i = 7;
    key_j = 5;
    keyX = (key_i - 4.5f) * 3.0f;
    keyZ = (key_j - 4.5f) * 3.0f;

    // reset door
    trnsxd = 11.85;
    trnsyd = 1.25;
    trnszd = 4.5;
    doorOpen = false;

    // reset game state
    isGameOver = false;
    isWin = false;
    printedWin = false;

    // reset time
    gameStartTime = glutGet(GLUT_ELAPSED_TIME);
}


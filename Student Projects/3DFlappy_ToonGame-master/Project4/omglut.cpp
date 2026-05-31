
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION

#include <Windows.h>
#include <GL/glut.h>
#include <GL/stb_image.h>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>

const int WIN_W = 1000;
const int WIN_H = 650;

GLuint TEX_BIRD, TEX_PIPE, TEX_GROUND, TEX_SKY;

GLuint loadTexture(const char* path) {
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, ch;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(path, &w, &h, &ch, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        printf("[TEX] OK: %s (%dx%d)\n", path, w, h);
    }
    else {
        printf("[TEX] FAIL: %s -> %s\n", path, stbi_failure_reason());
        unsigned char* fb = new unsigned char[64 * 64 * 4];
        for (int i = 0;i < 64;i++) for (int j = 0;j < 64;j++) {
            int k = (i * 64 + j) * 4;
            unsigned char c = ((i / 8 + j / 8) % 2) ? 180 : 60;
            fb[k] = c;fb[k + 1] = c;fb[k + 2] = c;fb[k + 3] = 255;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, fb);
        delete[] fb;
    }
    return id;
}

// =============================================================
//  [CAMERA]
// =============================================================
struct Camera {
    float eyeX, eyeY, eyeZ;
    float atY, targetY, shake;

	void update(float birdY) {
        targetY = birdY + 1.0f;
        atY += (targetY - atY) * 0.09f;
        if (shake > 0.0f) shake -= 0.05f;
        if (shake < 0.0f) shake = 0.0f;
    }
	void apply() {
        float sx = 0, sy = 0;
        if (shake > 0) {
            sx = ((rand() % 200) - 100) * 0.0008f * shake;
            sy = ((rand() % 200) - 100) * 0.0008f * shake;
        }
        gluLookAt(eyeX + sx, atY + sy, eyeZ,
            eyeX + sx, atY + sy, 0.0f,
            0.0f, 1.0f, 0.0f);
    }
} cam = { 0,0,9,0,0,0 };

// =============================================================
//  [LIGHTING]
// =============================================================
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);

    GLfloat p0[] = { 6,12,6,1 };
    GLfloat a0[] = { 0.20f,0.18f,0.14f,1 };
    GLfloat d0[] = { 1.0f,0.92f,0.75f,1 };
    GLfloat s0[] = { 0.6f,0.55f,0.4f,1 };
    glLightfv(GL_LIGHT0, GL_POSITION, p0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, a0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, d0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, s0);

    GLfloat p1[] = { -8,4,3,1 };
    GLfloat a1[] = { 0,0,0,1 };
    GLfloat d1[] = { 0.15f,0.25f,0.5f,1 };
    glLightfv(GL_LIGHT1, GL_POSITION, p1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, a1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, d1);

	GLfloat ga[] = { 0.12f,0.12f,0.12f,1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ga);
}

// =============================================================
//  [3D ENVIRONMENT] - textured box
// =============================================================
void draw3DBox(float x, float y, float z,
    float w, float h, float d,
	GLuint tex, float uR = 1, float vR = 1)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    //glColor3f(1, 1, 1);
    float x2 = x + w, y2 = y + h, z2 = z + d;

	glNormal3f(0, 0, 1);  glBegin(GL_QUADS);// front face
    glTexCoord2f(0, 0);glVertex3f(x, y, z2); glTexCoord2f(uR, 0);glVertex3f(x2, y, z2);
    glTexCoord2f(uR, vR);glVertex3f(x2, y2, z2); glTexCoord2f(0, vR);glVertex3f(x, y2, z2);
    glEnd();

	glNormal3f(0, 0, -1); glBegin(GL_QUADS);// back face
    glTexCoord2f(0, 0);glVertex3f(x2, y, z); glTexCoord2f(uR, 0);glVertex3f(x, y, z);
    glTexCoord2f(uR, vR);glVertex3f(x, y2, z); glTexCoord2f(0, vR);glVertex3f(x2, y2, z);
    glEnd();

	glNormal3f(-1, 0, 0); glBegin(GL_QUADS);// left face
    glTexCoord2f(0, 0);glVertex3f(x, y, z); glTexCoord2f(uR, 0);glVertex3f(x, y, z2);
    glTexCoord2f(uR, vR);glVertex3f(x, y2, z2); glTexCoord2f(0, vR);glVertex3f(x, y2, z);
    glEnd();

	glNormal3f(1, 0, 0);  glBegin(GL_QUADS);// right face
    glTexCoord2f(0, 0);glVertex3f(x2, y, z2); glTexCoord2f(uR, 0);glVertex3f(x2, y, z);
    glTexCoord2f(uR, vR);glVertex3f(x2, y2, z); glTexCoord2f(0, vR);glVertex3f(x2, y2, z2);
    glEnd();

	glNormal3f(0, 1, 0);  glBegin(GL_QUADS);// top face
    glTexCoord2f(0, 0);glVertex3f(x, y2, z); glTexCoord2f(uR, 0);glVertex3f(x2, y2, z);
    glTexCoord2f(uR, vR);glVertex3f(x2, y2, z2); glTexCoord2f(0, vR);glVertex3f(x, y2, z2);
    glEnd();

	glNormal3f(0, -1, 0); glBegin(GL_QUADS);// bottom face
    glTexCoord2f(0, 0);glVertex3f(x, y, z2); glTexCoord2f(uR, 0);glVertex3f(x2, y, z2);
    glTexCoord2f(uR, vR);glVertex3f(x2, y, z); glTexCoord2f(0, vR);glVertex3f(x, y, z);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

float skyScroll = 0, groundScroll = 0;

void drawSky() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TEX_SKY);

    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(1, 0);
    glTexCoord2f(1, 1); glVertex2f(1, 1);
    glTexCoord2f(0, 1); glVertex2f(0, 1);
    glEnd();

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();

  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}
void drawGround() {
    float u = fmodf(groundScroll, 1.0f);
    draw3DBox(-22, -3.4f, -7, 44, 0.6f, 14, TEX_GROUND, 10.0f + u * 10.0f, 3.0f);
}

// =============================================================
//  [ANIMATION] - Bird
// =============================================================
struct BirdAnim {
    float wing, wingDir, tilt;
    void update(float vy) {
        wing += 0.18f * wingDir;
        if (wing > 1)wingDir = -1;
        if (wing < -1)wingDir = 1;
        float tgt = vy * 220.0f;
        if (tgt > 50)tgt = 50;
        if (tgt < -85)tgt = -85;
        tilt += (tgt - tilt) * 0.15f;
    }
} ba = { 0,1,0 };

// =============================================================
//  GAME STATE & PHYSICS
// =============================================================
enum EGameState { GS_MENU, GS_PLAYING, GS_DEAD };
EGameState gameState = GS_MENU;

float birdY = 0, birdVY = 0;
const float BIRD_X = -2.5f, GRAVITY = -0.013f, FLAP = 0.23f;

// =============================================================
//  [COLLISION] - Pipes
// =============================================================
const int NPIPES = 4;
const float PIPE_SPEED = 0.075f, PIPE_W = 0.6f, PIPE_D = 0.6f;

struct Pipe { float x, gapY, gapH;
bool scored; } pipes[NPIPES];

bool hitsPipe(const Pipe& p) {
    const float BR = 0.28f;
    if (BIRD_X + BR < p.x - PIPE_W / 2)
        return false;
    if (BIRD_X - BR > p.x + PIPE_W / 2)
        return false;
    if (birdY - BR >= p.gapY - p.gapH && birdY + BR <= p.gapY + p.gapH) return false;
    return true;
}

void resetPipes() {
    for (int i = 0; i < NPIPES; i++) {
        pipes[i].x = 5.0f + i * 5.8f;
		pipes[i].gapY = 0.5f + (rand() % 40) * 0.03f;  
        pipes[i].gapH = 1.4f;                          
        pipes[i].scored = false;
    }
}

int score = 0, hiScore = 0;
float health = 100.0f;
const float DMG = 35.0f;

// =============================================================
//  PARTICLES
// =============================================================
struct Pt { float x, y, z, vx, vy, vz, life;bool on; } spark[50];

void burst(float x, float y, float z) {
    for (int i = 0;i < 50;i++) {
        if (spark[i].on)continue;
        spark[i] = { x,y,z,
            ((rand() % 200) - 100) * 0.005f,
            ((rand() % 200) - 100) * 0.005f + 0.07f,
            ((rand() % 100) - 50) * 0.003f,
            1.0f,true };
    }
}

void updateParticles() {
    for (int i = 0;i < 50;i++) {
        if (!spark[i].on)continue;
        spark[i].x += spark[i].vx; spark[i].y += spark[i].vy; spark[i].z += spark[i].vz;
        spark[i].vy -= 0.003f; spark[i].life -= 0.035f;
        if (spark[i].life <= 0)spark[i].on = false;
    }
}

void drawParticles() {
    glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    for (int i = 0;i < 50;i++) {
        if (!spark[i].on)continue;
        glColor3f(1.0f, spark[i].life * 0.5f, 0.0f);
        glVertex3f(spark[i].x, spark[i].y, spark[i].z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// =============================================================
//  DRAW BIRD
// =============================================================
void drawBird() {
    glPushMatrix();
    glTranslatef(BIRD_X, birdY, 0);
    glRotatef(ba.tilt, 0, 0, 1);
    float s = 0.52f;

    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, TEX_BIRD);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING); glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);glVertex3f(-s, -s, 0.02f);
    glTexCoord2f(1, 0);glVertex3f(s, -s, 0.02f);
    glTexCoord2f(1, 1);glVertex3f(s, s, 0.02f);
    glTexCoord2f(0, 1);glVertex3f(-s, s, 0.02f);
    glEnd();
    glDisable(GL_BLEND); glDisable(GL_TEXTURE_2D); glEnable(GL_LIGHTING);

    // [ANIMATION] wing
    glColor3f(1.0f, 0.62f, 0.05f);
    glPushMatrix();
    glTranslatef(-0.04f, 0.02f, 0.03f);
    glRotatef(ba.wing * 60.0f, 1, 0, 0);
    glNormal3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(-s * 0.5f, -s * 0.32f, 0);
    glVertex3f(s * 0.5f, -s * 0.32f, 0);

    glVertex3f(s * 0.38f, s * 0.28f, 0);
    glVertex3f(-s * 0.38f, s * 0.28f, 0);
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

// =============================================================
//  DRAW PIPES
// =============================================================
// =============================================================
//  DRAW PIPES
// =============================================================
// =============================================================
//  DRAW PIPES - النسخة المعدلة لتبدو قطعة واحدة
// =============================================================
void drawPipes() {
    for (int i = 0; i < NPIPES; i++) {
        float px = pipes[i].x - PIPE_W / 2;
        float pz = -PIPE_D / 2;
        float top = pipes[i].gapY + pipes[i].gapH;
        float bot = pipes[i].gapY - pipes[i].gapH;

        
        glColor3f(0.2f, 0.85f, 0.2f);
        draw3DBox(px, -3.4f, pz,
            PIPE_W, bot - (-3.4f), PIPE_D,
            TEX_PIPE, 1.0f, 1.0f);

        // Bottom cap
        glColor3f(0.15f, 0.75f, 0.15f);
        draw3DBox(px - 0.12f, bot - 0.35f, pz - 0.08f,
            PIPE_W + 0.24f, 0.35f, PIPE_D + 0.16f,
            TEX_PIPE, 1.0f, 1.0f);

        // Top pipe
        glColor3f(0.2f, 0.85f, 0.2f);
        draw3DBox(px, top, pz,
            PIPE_W, 20.0f, PIPE_D,
            TEX_PIPE, 1.0f, 1.0f);

        // Top cap
        glColor3f(0.15f, 0.75f, 0.15f);
        draw3DBox(px - 0.12f, top, pz - 0.08f,
            PIPE_W + 0.24f, 0.35f, PIPE_D + 0.16f,
            TEX_PIPE, 1.0f, 1.0f);
    }
}

// =============================================================
//  HUD
// =============================================================
void hudTxt(float x, float y, const char* s, float r, float g, float b, float sc) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* c = s; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void begin2D() {
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
}

void end2D() {
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}

void drawHUD() {
    begin2D();
    float bx = WIN_W / 2.0f - 160, by = WIN_H - 46.0f, bw = 320, bh = 26;

    glColor3f(0.08f, 0.04f, 0.04f);
    glBegin(GL_QUADS);
    glVertex2f(bx - 4, by - 4);glVertex2f(bx + bw + 4, by - 4);
    glVertex2f(bx + bw + 4, by + bh + 4);glVertex2f(bx - 4, by + bh + 4);
    glEnd();

    glColor3f(0.13f, 0.06f, 0.06f);
    glBegin(GL_QUADS);
    glVertex2f(bx, by);glVertex2f(bx + bw, by);
    glVertex2f(bx + bw, by + bh);glVertex2f(bx, by + bh);
    glEnd();

    float r = health / 100.0f;
    glColor3f(0.9f - r * 0.35f, r * 0.45f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(bx, by + 2);glVertex2f(bx + bw * r, by + 2);
    glVertex2f(bx + bw * r, by + bh - 2);glVertex2f(bx, by + bh - 2);
    glEnd();

    glColor3f(0.75f, 0.1f, 0.1f); glLineWidth(2.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(bx, by);glVertex2f(bx + bw, by);
    glVertex2f(bx + bw, by + bh);glVertex2f(bx, by + bh);
    glEnd();

    hudTxt(bx + bw * r / 2 - 38, by + 5, "HEALTH", 1, 1, 1, 0.85f);

    char buf[64];
    sprintf(buf, "SCORE: %d", score);
    hudTxt(20, WIN_H - 30, buf, 1.0f, 1.0f, 0.0f, 1.0f);

    sprintf(buf, "BEST: %d", hiScore);
    hudTxt(WIN_W - 160, WIN_H - 30, buf, 0.2f, 1.0f, 0.3f, 1.0f);
    end2D();
}

void drawMenu() {
    begin2D();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0, 0, 0.52f);
    glBegin(GL_QUADS);
    glVertex2f(WIN_W / 2 - 270, WIN_H / 2 + 10); glVertex2f(WIN_W / 2 + 270, WIN_H / 2 + 10);
    glVertex2f(WIN_W / 2 + 270, WIN_H / 2 + 185); glVertex2f(WIN_W / 2 - 270, WIN_H / 2 + 185);
    glEnd(); glDisable(GL_BLEND);

    hudTxt(WIN_W / 2 - 120, WIN_H / 2 + 140, "FLAPPY TOON 3D", 1.0f, 0.88f, 0.1f, 1);
    hudTxt(WIN_W / 2 - 140, WIN_H / 2 + 90, "PRESS SPACE TO START", 1.0f, 1.0f, 0.1f, 1);
    hudTxt(WIN_W / 2 - 195, WIN_H / 2 + 55, "SPACE / UP / CLICK = FLAP    ESC = QUIT", 0.75f, 0.75f, 0.75f, 1);
    end2D();
}

void drawDead() {
    begin2D();
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0, 0, 0.62f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(WIN_W, 0); glVertex2f(WIN_W, WIN_H); glVertex2f(0, WIN_H);
    glEnd(); glDisable(GL_BLEND);

    hudTxt(WIN_W / 2 - 80, WIN_H / 2 + 80, "GAME OVER", 1.0f, 0.18f, 0.18f, 1);

    char buf[64];
    sprintf(buf, "SCORE: %d", score);
    hudTxt(WIN_W / 2 - 60, WIN_H / 2 + 30, buf, 1.0f, 0.9f, 0.2f, 1);

    sprintf(buf, "BEST: %d", hiScore);
    hudTxt(WIN_W / 2 - 60, WIN_H / 2 - 5, buf, 0.35f, 1.0f, 0.35f, 1);

    hudTxt(WIN_W / 2 - 145, WIN_H / 2 - 60, "PRESS SPACE TO RETRY", 1.0f, 1.0f, 0.1f, 1);
    end2D();
}

// =============================================================
//  RESET
// =============================================================
void resetGame() {
    birdY = 0;birdVY = 0;health = 100;score = 0;
    skyScroll = groundScroll = 0;
    cam.atY = 0;cam.shake = 0;
    ba = { 0,1,0 };
    for (int i = 0;i < 50;i++) spark[i].on = false;
    resetPipes();
    gameState = GS_PLAYING;
}
// =============================================================
//  UPDATE
// =============================================================
void update(int) {
    if (gameState == GS_PLAYING) {
        birdVY += GRAVITY; birdY += birdVY;
        ba.update(birdVY);          // [ANIMATION]
        skyScroll += 0.0035f;
        groundScroll += 0.022f;
        cam.update(birdY);          // [CAMERA]

        for (int i = 0;i < NPIPES;i++) {
            pipes[i].x -= PIPE_SPEED;

            if (pipes[i].x + PIPE_W / 2 < BIRD_X - 7.0f) {
                pipes[i].x = BIRD_X + 20.0f + (rand() % 40) * 0.1f;
                pipes[i].gapY = 0.5f + (rand() % 40) * 0.03f;  // ← نفس القيم
                pipes[i].gapH = 1.4f;
                pipes[i].scored = false;
            }

            if (!pipes[i].scored && BIRD_X > pipes[i].x + PIPE_W / 2) {
                pipes[i].scored = true; score++;
                if (score > hiScore) hiScore = score;
            }

            if (hitsPipe(pipes[i])) {         // [COLLISION]
                health -= DMG; cam.shake = 0.6f;
                birdVY = 0.10f; pipes[i].x -= 1.8f;
                burst(BIRD_X, birdY, 0);
                if (health <= 0) {
                    health = 0;
                    birdVY = 0;      // ← وقف الحركة فوراً
					cam.shake = 0;   
                    gameState = GS_DEAD;
                }
            }
        }

        if (birdY < -2.2f) {
            birdY = -2.2f;
            if (birdVY < 0) {
                health -= DMG;
                birdVY = 0.18f;        // ارتداد فوري للفوق
                burst(BIRD_X, birdY, 0);
                cam.shake = 0.45f;
                if (health <= 0) {
                    health = 0;
                    birdVY = 0;
                    birdY = -2.2f;
                    cam.shake = 0;
                    gameState = GS_DEAD;
                }
            }
        }

        // نقص الـ timer كل frame
        
        if (birdY > 5.8f) { birdY = 5.8f;birdVY = -0.04f; }
        
        updateParticles();
    }
    else if (gameState == GS_MENU) {
        float t = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
        birdY = sinf(t * 2.0f) * 0.35f;
        ba.update(cosf(t * 2.0f) * 0.35f * 2.0f);
        skyScroll += 0.0012f; groundScroll += 0.007f;
        cam.update(birdY);
    }
    // GS_DEAD — مفيش حاجة، الطائر ثابت في مكانه
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// =============================================================
//  DISPLAY
// =============================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cam.apply();            
    setupLighting();        
    drawSky();             
    drawGround();
    drawPipes();
    drawBird();             
    drawParticles();
    if (gameState == GS_MENU)   drawMenu();
    else if (gameState == GS_DEAD) { drawHUD(); drawDead(); }
    else                   drawHUD();
    glutSwapBuffers();
}
void cleanup();
// =============================================================
//  [KEYBOARD & MOUSE]
// =============================================================
void keyboard(unsigned char key, int, int) {
    if (key == ' ' || key == 'w' || key == 'W') {
        if (gameState == GS_MENU)    resetGame();
        else if (gameState == GS_PLAYING) birdVY = FLAP;
        else if (gameState == GS_DEAD)    resetGame();
    }
    if (key == 27) { cleanup(); exit(0); }
}

void special(int key, int, int) {
    if (key == GLUT_KEY_UP) {
        if (gameState == GS_MENU)    resetGame();
        else if (gameState   == GS_PLAYING) birdVY = FLAP;
        else if (gameState == GS_DEAD)    resetGame();
    }
}

void mouse(int btn, int st, int, int) {
    if (btn == GLUT_LEFT_BUTTON && st == GLUT_DOWN) {
        if (gameState == GS_MENU)    resetGame();
        else if (gameState == GS_PLAYING) birdVY = FLAP;
        else if (gameState == GS_DEAD)    resetGame();
    }
}

// =============================================================
//  RESHAPE
// =============================================================
void reshape(int w, int h) {
    if (h == 0)h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(60.0, (double)w / h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

// =============================================================
//  MEMORY CLEANUP - free all GPU & heap resources
// =============================================================
void cleanup() {
    // Delete all textures from GPU memory
    GLuint texIDs[4] = { TEX_BIRD, TEX_PIPE, TEX_GROUND, TEX_SKY };
    glDeleteTextures(4, texIDs);
    printf("[MEM] Textures freed.\n");
}

// =============================================================
//  MAIN
// =============================================================
int main(int argc, char** argv) {
    srand((unsigned)GetTickCount());
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(80, 50);
    glutCreateWindow("Flappy Toon 3D");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.50f, 0.78f, 0.96f, 1.0f);

    // [TEXTURE] - ?? ??? ??? ?????? ??????
    TEX_BIRD = loadTexture("Bird.jpg");
    TEX_PIPE = loadTexture("pipeBottom.jpg");
    TEX_GROUND = loadTexture("Grass.jpg");
    TEX_SKY = loadTexture("photo0jpg.jpg");

    setupLighting();    // [LIGHTING]
    resetPipes();

    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);    
    glutSpecialFunc(special);      
    glutMouseFunc(mouse);          
    glutTimerFunc(16, update, 0);

    atexit(cleanup);   // free memory when window closes
    glutMainLoop();
    return 0;
}
#include <gl/freeglut.h>
#include<iostream>
#include<math.h>
#include<cstdlib>
#include<cstdio>
#define PI 3.1415
using namespace std;

double enemyXposition[1000];
double enemyYposition[1000];
double enemy_speed_x[1000];

int hasLost;
int currentEnemies[1000];
int enemy_count;
int totalPoints=0;
int currentMouseXPosition;
int currentMouseYPosition;
int timePassed=0;
int meteoriteSpawnRate=200;
int points;


bool hyperAsteriods = false;
bool megaAsteriods = false;

void drawPlayerShape(double x, double y)
{

    glPushMatrix();
    glTranslated(x,y,0);
    glRotated(0.25*timePassed,0,0,1);
    glColor3f(0.0,1.0,1.0);
    glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * PI; i += PI / 4)
        glVertex3f(cos(i) * 15, sin(i) * 15, 0.0);
    glEnd();
    glPopMatrix();
}


void drawEnemyShape(double x, double y)
{

    glPushMatrix();
    glTranslated(x,y,0);
    glRotated(-0.50*timePassed,0,0,1);
    if(hyperAsteriods==false)
    {
        glColor3f(1.0,1.0,0.0);
        glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * PI; i += PI / 4)
            glVertex3f(cos(i) * 10, sin(i) * 10, 0.0);
        glEnd();
    }
    if(hyperAsteriods==true)
    {
        glColor3f(1.0,0.5,0.0);
        glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * PI; i += PI / 4)
            glVertex3f(cos(i) * 20, sin(i) * 20, 0.0);
        glEnd();
    }
    if(megaAsteriods==true)
    {
        glColor3f(1.0,0.0,0.0);
        glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * PI; i += PI / 4)
            glVertex3f(cos(i) * 40, sin(i) * 40, 0.0);
        glEnd();
    }
    glPopMatrix();
}


void canvas(void)
{
    if(meteoriteSpawnRate>10)
    {
        meteoriteSpawnRate=150-timePassed/150;
    }
    if(meteoriteSpawnRate<=100)
    {
        hyperAsteriods=true;
    }
    if(meteoriteSpawnRate<=50)
    {
        hyperAsteriods=false;
        megaAsteriods=true;
    }
    timePassed++;
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    char info[1000];
    glColor3f(1,1,1);

    sprintf(info,"Timelapse: %d seconds",timePassed/400);
    glRasterPos2i(600, 520);
    glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const unsigned char *)info);

    sprintf(info,"Points: %d",totalPoints);
    glRasterPos2i(600, 550);
    glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const unsigned char *)info);

    if(hyperAsteriods==true)
    {
        glColor3f(1,1,1);
        sprintf(info,"Hyper asteroids round, be careful!!");
        glRasterPos2i(20, 550);
        glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const unsigned char *)info);
    }

    if(megaAsteriods==true)
    {
        glColor3f(1,1,1);
        sprintf(info,"Mega asteroids round, watch out!!!!!!!!!!!");
        glRasterPos2i(20, 550);
        glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const unsigned char *)info);
    }


    if (timePassed%meteoriteSpawnRate==0)
    {
        int i;
        for (i=0; i<1000; i++)
        {
            if (currentEnemies[i]==0)
            {
                currentEnemies[i]=1;
                enemyXposition[i]=(rand()%2) * 1000;
                enemyYposition[i]=-100+rand()%1000;
                enemy_speed_x[i]=0.06*sqrt(sqrt(timePassed)*timePassed/1000);
                break;
            }
        }
        enemy_count++;

    }

    drawPlayerShape(currentMouseXPosition,currentMouseYPosition);


    int i;

    for (i=0; i<1000; i++)
        if (currentEnemies[i]==1)
        {
            enemyXposition[i]+=enemy_speed_x[i];

            drawEnemyShape(enemyXposition[i],enemyYposition[i]);

            if (enemyYposition[i]>620 || enemyXposition[i]<-20 || enemyXposition[i]>820)
            {
                currentEnemies[i]=0;
                if(!hyperAsteriods&&!megaAsteriods)
                    totalPoints++;
                else if(hyperAsteriods)
                    totalPoints+=2;
                else if(megaAsteriods)
                {
                    totalPoints+=5;
                }
                enemy_count--;
            }
        }


    for (int j=0; j<1000; j++)
    {
        if (currentEnemies[j]==1)
        {
            double diff_player_enemy_x= currentMouseXPosition-enemyXposition[j];
            double diff_player_enemy_y= currentMouseYPosition-enemyYposition[j];
            if (diff_player_enemy_x<0)
                diff_player_enemy_x*=-1;
            if (diff_player_enemy_y<0)
                diff_player_enemy_y*=-1;
            if (diff_player_enemy_x<10 &&diff_player_enemy_y<10 &&!hyperAsteriods &&!megaAsteriods)
            {
                hasLost=1;
            }
            if (diff_player_enemy_x<20 &&diff_player_enemy_y<20 &&hyperAsteriods &&!megaAsteriods)
            {
                hasLost=1;
            }
            if (diff_player_enemy_x<40 &&diff_player_enemy_y<40 &&!hyperAsteriods &&megaAsteriods)
            {
                hasLost=1;
            }
        }
    }

    if (hasLost &&!hyperAsteriods &&!megaAsteriods)
    {
        glColor3f(0,1,0);
        glRasterPos2i(275, 275);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char *)"Serios, ai pierdut jocul?");
    }

    if (hasLost &&hyperAsteriods &&!megaAsteriods)
    {
        glColor3f(0,1,0);
        glRasterPos2i(225, 275);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char *)"Cred ca puteai mai mult, dar din pacate ai pierdut!");
    }
    if (hasLost &&!hyperAsteriods &&megaAsteriods)
    {
        glColor3f(0,1,0);
        glRasterPos2i(275, 275);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char *)"Esti o legenda, chiar daca ai pierdut!");
    }


    glutSwapBuffers();
    glFlush();
    if (!hasLost)
        glutPostRedisplay();
}
void windowResize(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, 800, 0.0, 600.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mousefunc(int x, int y)
{
    currentMouseXPosition=x;
    currentMouseYPosition=600-y;
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize (800, 600);
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("Asteroids");

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glMatrixMode (GL_PROJECTION);
    glOrtho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
    glutDisplayFunc(canvas);
    glutReshapeFunc(windowResize);
    glutPassiveMotionFunc(mousefunc);
    glutMainLoop();
}

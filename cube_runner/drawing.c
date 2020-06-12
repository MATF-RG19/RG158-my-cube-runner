#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "image.h"

#include "drawing.h"

#define BROJ_PREPREKA 3000

//Boje za prepreke
#define RED1 (1)
#define RED2 (0)
#define RED3 (0)
#define CITY1 (1)
#define CITY2 (0)
#define CITY3 (1)
#define YELLOW1 (1)
#define YELLOW2 (1)
#define YELLOW3 (0)
#define LIME1 (0)
#define LIME2 (1)
#define LIME3 (0)
#define AQUA1 (0)
#define AQUA2 (1)
#define AQUA3 (1)
#define PINK1 (1)
#define PINK2 (0.08)
#define PINK3 (0.588)
#define LIGHT1 (1)
#define LIGHT2 (0.9)
#define LIGHT3 (1)
#define SAND1 (0.97)
#define SAND2 (0.55)
#define SAND3 (0.38)
#define BLUE1 (0)
#define BLUE2 (0)
#define BLUE3 (0.82)

//Struktura koja predstavlja sve prepreke
typedef struct{
    float x;
    float y ;
    float z;
} Kocka;


//iscrtavanje koordinatnog sistema za pomoc
void draw_coo(){
         glDisable(GL_LIGHTING);
         glBegin(GL_LINES);
         glColor3f (1, 0, 0);
         glVertex3f(-10, 0, 0);
         glVertex3f(0, 0, 0);
      
         glColor3f (0, 1, 0);
         glVertex3f(0, 10, 0);
         glVertex3f(0, 0, 0);
      
         glColor3f (0, 0, 1);
         glVertex3f(0, 0, -10);
         glVertex3f(0, 0, 0);
         glEnd();
         glEnable(GL_LIGHTING);
}  


//funkcija za iscrtavanje staze
void draw_road(GLuint names, float z_koordinata) {
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, names);
        glEnable(GL_TEXTURE_2D);
        
        glBegin(GL_POLYGON);
        glTranslatef(0 , 0, z_koordinata);
		glTexCoord2f(0, 0);
		glVertex3f(-40, 0.1, -3);
			
		glTexCoord2f(1, 0);
		glVertex3f(40, 0.1, -3);
			
		glTexCoord2f(1, 1);
		glVertex3f(40, 0.1, 720);
			
		glTexCoord2f(0, 1);
		glVertex3f(-40, 0.1, 720);
		glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
	    glColor3f(0.211,0.211,0.211);
	    glScalef(20.0, .05, 1000);
        glTranslatef(0, 0, 1.96);
	    glutSolidCube(4);
    glPopMatrix();
}


void draw_ball(float x_koordinata, double levitacija, float z_koordinata, int score)
{

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);


    glPushMatrix();
        glColor3f(0 ,0, 1);
        glTranslatef(x_koordinata,0.5 + levitacija, z_koordinata);
        glScalef(1 , 0.5, 1);
        glRotatef((score % 360), 0, 1.0, 0);
        glutSolidSphere(0.3, 50, 50);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0, 0 , 1);
        glTranslatef(x_koordinata, 0.6 + levitacija , z_koordinata);
        glRotatef(-(score % 360), 0, 1.0, 0);
        glutSolidSphere(0.18, 50, 50);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(x_koordinata, 0.7 + levitacija , z_koordinata-0.09);
        glRotatef(-(score % 360), 0, 1.0, 0);
        glutSolidSphere(0.05, 50, 50);
    glPopMatrix();


    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);     
}

float boja_1(int boja){
     switch (boja)
        {
        case (0): return BLUE1; break;
        case (1): return RED1; break;
        case (2): return CITY1; break;
        case (3): return YELLOW1; break;
        case (4): return LIME1; break;
        case (5): return AQUA1; break;
        case (6): return PINK1; break;
        case (7): return LIGHT1; break;
        case (8): return SAND1; break;
        default:
            return CITY1; break;
        }
}

float boja_2(int boja){
     switch (boja)
        {
        case (0): return BLUE2; break;
        case (1): return RED2; break;
        case (2): return CITY2; break;
        case (3): return YELLOW2; break;
        case (4): return LIME2; break;
        case (5): return AQUA2; break;
        case (6): return PINK2; break;
        case (7): return LIGHT2; break;
        case (8): return SAND2; break;
        default:
            return CITY2; break;
        }
}

float boja_3(int boja){
     switch (boja)
        {
        case (0): return BLUE3; break;
        case (1): return RED3; break;
        case (2): return CITY3; break;
        case (3): return YELLOW3; break;
        case (4): return LIME3; break;
        case (5): return AQUA3; break;
        case (6): return PINK3; break;
        case (7): return LIGHT3; break;
        case (8): return SAND3; break;
        default:
            return CITY3; break;
        }
}
























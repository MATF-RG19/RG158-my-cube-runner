/* PROJEKAT IZ RACUNARSKE GRAFIKE 
    
        OGNJEN JANKOVIC     

            88/2017
*/


#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<GL/glut.h>

void on_reshape(int width, int height){

         glViewport(0, 0, width, height);
      

         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         gluPerspective(60, (float) width / height, 1, 100);
}

//funkcija za iscrtavanje staze
void draw_road() {
    glPushMatrix();
	    glColor3f(0.211,0.211,0.211);
	    glScalef(10.0, .05, 25);
	    glutSolidCube(4);
    glPopMatrix();
}

void on_display(void);
            
int main(int argc, char **argv){

         GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
         GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
         GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
      
         GLfloat ambient_coeffs[] = { 0.3, 0.7, 0.3, 1 };
         GLfloat diffuse_coeffs[] = { 0.2, 1, 0.2, 1 };
         GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
         GLfloat shininess = 30;
     

         glutInit(&argc, argv);
         glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
      

         glutInitWindowSize(900, 900);
         glutInitWindowPosition(500, 500);
         glutCreateWindow(argv[0]);
      

         glutReshapeFunc(on_reshape);
         glutDisplayFunc(on_display);
	 
      

         glClearColor(0.5, 0.9, 0.4, 0);
         glEnable(GL_DEPTH_TEST);
     	
         glEnable(GL_LIGHTING);
      

         glEnable(GL_LIGHT0);
	 glEnable(GL_COLOR_MATERIAL);      		

         glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
         glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
         glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
      

         glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
         glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
         glMaterialf(GL_FRONT, GL_SHININESS, shininess);
         glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
         glEnable(GL_COLOR_MATERIAL);
	
         glutMainLoop();

	
         return 0;
}

//iscrtavanje koordinatnog sistema za pomoc
static void draw_coo(){
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

void on_display(void){
        
        GLfloat light_position[] = {0,1,0, 0 };
              
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
              
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
                
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	/* pogled kamere */
	gluLookAt(0, 2, -7, 0, 0, 0, 0, 1, 0);	
	 
	draw_coo();

	draw_road();
	

        glutSwapBuffers();
}

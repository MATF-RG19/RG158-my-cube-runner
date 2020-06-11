/* PROJEKAT IZ RACUNARSKE GRAFIKE 
    
        OGNJEN JANKOVIC     

            88/2017
*/


#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<GL/glut.h>
#include<string.h>
#include "image.h"

#define BR_REDOVA_ZA_PREPREKE 200
#define BR_STAT_KOCKI 1000
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


/* Imena fajlova sa teksturama. */
#define FILENAME0 "planete.bmp"
#define FILENAME1 "put.bmp"
#define FILENAME2 "endgame.bmp"
#define FILENAME3 "startgame.bmp"

/* Identifikatori tekstura. */
static GLuint names[4];



static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_timer2(int value);
static void on_reshape(int width, int height);
static void on_release(unsigned char key,int x,int y);
static void on_display(void);
static void startgame();
static void game_over();
static float boja_1(int boja);
static float boja_2(int boja);
static float boja_3(int boja);
static void random_prepreke(int j, int w, int m );


/*Promenljive koje sluze za kontrolisanje igre*/
static int start = 0;
static int score = 0;
static int boja = 1;
static double levitacija = 0.0;
static double levIndex = 0.005;


//Promenljive zaduzene za kretanje i pozicioniranje objekta
static int kretanja[] = {0, 0};
static float x_koordinata = 0;
static float z_koordinata = 1;
static float kameraZ = -2;
static float tackaPogleda_z = 7;
static float brzina = 0.2;


//Struktura koja predstavlja sve prepreke
typedef struct{
    float x;
    float y ;
    float z;
} Kocka;

//Nizovi vezani za prepeke
static int brKockiZaRed[BR_REDOVA_ZA_PREPREKE];
Kocka prepreka[BROJ_PREPREKA];
Kocka statPrep[BR_STAT_KOCKI];

//Funkcije za kontrolisanje kolizije
static float rastojanje(Kocka koc);
static void kolizija();


//inicijalizacija prepreka
static void postavke(){
    int i;
    for( i=0 ; i < BR_REDOVA_ZA_PREPREKE; i++)
        brKockiZaRed[i] = (int)rand() % 30;
    
    for(i=0; i < BROJ_PREPREKA; i++) {
        prepreka[i].x = -35 + ( (int)rand() % 70) ;
        prepreka[i].y = 0.5;
        prepreka[i].z = -1;
    }
}



void on_reshape(int width, int height){

    glViewport(0, 0, width, height);
      
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 100);
}


//funkcija za iscrtavanje staze
void draw_road() {
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, names[1]);
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
    glutCreateWindow("My Cube Runner");
      
    /*Objekat koji predstavlja teskturu ucitanu iz fajla*/
    Image * image;

    /*Ukljucuju se teksture.*/
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    /*
    *Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz fajla*/
    image = image_init(0, 0);

    /*Generisu se identifikatori tekstura*/
    glGenTextures(4, names);

    /* Kreira se prva tekstura. */
    image_read(image, FILENAME0);

    glBindTexture(GL_TEXTURE_2D, names[0]);
        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se druga tekstura. */
    image_read(image, FILENAME1);

    glBindTexture(GL_TEXTURE_2D, names[1]);
        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                image->width, image->height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /*Kreira se treca tekstura*/
    image_read(image, FILENAME2);

    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /*Kreira se cetvrta tekstura*/
    image_read(image, FILENAME3);
  
    glBindTexture(GL_TEXTURE_2D, names[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                    image->width, image->height, 0, 
                    GL_RGB, GL_UNSIGNED_BYTE, image->pixels); 

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);



    glutReshapeFunc(on_reshape);
    glutDisplayFunc(startgame);
	glutKeyboardFunc(on_keyboard);
    glutKeyboardUpFunc(on_release);
      

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
	
    srand(time(NULL));
    postavke();
    glutFullScreen();
    glutMainLoop();

	
    return 0;
}


//iscrtavanje koordinatnog sistema za pomoc
/*static void draw_coo(){
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
}  */

void draw_ball(){

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

//Iscrtavanje prepreka(ceo nivo)
void draw_squares(){
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
    int i = 0; //indeks pomeranja po X osi
    int j = 20; //indeks pomeranja po Z osi
    int t = 0; //indeks niza staticnih prepeka
    float h = 0;
    int m = 0; //indeks niza random prepreka
    
//Zid po X osi    
   for(i ; i<36 ; i++) { 
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(-4-i,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -4-i;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(4+i,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = 4+i;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
   }

   j++;
   
  //Suzavanje hodnika 
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(-4,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -4;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
     glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(-3.2,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -3.2;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(4,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = 4;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(3.2,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = 3.2;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
   
    j++; 


//Hodnik-pocetak   
     for(j ; j<60 ; j++ ) { 
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(-3.2, 0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -3.2;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(3.2, 0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = 3.2;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
   }
//Hodnik-kraj
    j+=5;

//Otvoren put - random iscrtavanje prepreka
    i = 0;
    int w=0;
    
//    random_prepreke(j, w, m);

    for(j, w; j<160; w++, j += 5){
        int q = brKockiZaRed[w]+1;
        for( q; q >= 0; q--){
            glPushMatrix();
                glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
                glTranslatef( prepreka[m].x ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            prepreka[m].z = j;
            m++;
        }
    }
    
    
    j++;

//Iscrtavanje galerije 

    //Zid po X osi    
   for(i=0 ; i<36 ; i++) { 
       if(i == 11 || i == 12)
        continue;
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(-4-i,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -4-i;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(4+i,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = 4+i;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
   }
    
    
    j++;
     //ka spolja
    int k=0; int b=0; int c=0;
    for(int p = 0; p < 3 ; p++) {
        
      c = j+13;  b=j+6; k=0;
    for(i = 0; j < c; j++, i++){
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(3 + i ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3+i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(-3 - i ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = -3-i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        if(j > b){
            glPushMatrix();
                glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
                glTranslatef(0 + k ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            statPrep[t].x = 0+k;
            statPrep[t].y = 0.5;
            statPrep[t].z = j;
            t++;
            
            glPushMatrix();
                glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
                glTranslatef(0 - k,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            statPrep[t].x = 0-k;
            statPrep[t].y = 0.5;
            statPrep[t].z = j;
            t++;
            
            k++;
        }
    }
    
      c = j+13; i-=2; b = j + 5; k-=2;
    for(i; j < c; j++, i--){
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(3 + i ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3+i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(-3 - i ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = -3-i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        if(j < b){
            glPushMatrix();
                glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
                glTranslatef(0 + k ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            statPrep[t].x = 0+k;
            statPrep[t].y = 0.5;
            statPrep[t].z = j;
            t++;
            
            glPushMatrix();
                glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
                glTranslatef(0 - k,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            statPrep[t].x = 0-k;
            statPrep[t].y = 0.5;
            statPrep[t].z = j;
            t++;
            
                k--;
            }
        }
    }

    //Prostor za random grenerisane prepreke

    j += 5;
    
//Otvoren put - random iscrtavanje prepreka
    i = 0;
     b=0;
    
//    random_prepreke(j, w, m);

    for(j, w; j<345; w++, j += 5){
        int q = brKockiZaRed[w]+1;
        for( q; q >= 0; q--){
            glPushMatrix();
                glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
                glTranslatef( prepreka[m].x ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            prepreka[m].z = j;
            m++;
        }
    }

    
    j+=5;

//kraj random prepreka


    //Kosina 
    //Zid po X osi    
   for(i=0 ; i<36 ; i++) { 
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(-4-i,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
        statPrep[t].x = -4-i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
    
    
    glPushMatrix();
        glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
        glTranslatef(4+i,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
        statPrep[t].x = 4+i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
   }
    
    j++;
    i=0;
    h=i;
    
    for(int p = 0; p < 3; p++, h=h ){
        
         c = j+15;  
    for(i ; j < c; j++, h+=0.5){
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(3.5 - h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3.5-h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(-3.5 - h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = -3.5-h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
    }
    
    h-=0.5;
    c = j+5;
    for(j; j < c; j++){
         glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(3.5 - h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3.5-h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(-3.5 - h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = -3.5-h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
    }
    
      c = j+15; i-=0.5;
    for(i; j < c; j++, h=h-0.5){
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(3.5 - h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3.5-h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(-3.5 - h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = -3.5-h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        }
        
        c = j+5; h+=0.5;
    for(j; j < c; j++){
         glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(3.5 + h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3.5+h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
            glTranslatef(-3.5 + h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = -3.5+h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        }
    }
     j += 5;

    //Otvoren put - random iscrtavanje prepreka
    i = 0;
     b=0;
    
    for(j, w; j<571; w++, j += 5){
        int q = brKockiZaRed[w]+1;
        for( q; q >= 0; q--){
            glPushMatrix();
                glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
                glTranslatef( prepreka[m].x ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            prepreka[m].z = j;
            m++;
        }
    }

}



//FUNKCIJE NA TASTATURI************************************
static void on_keyboard(unsigned char key, int x, int y){
    
    switch (key) {
        /*Na esc dugme izlazi se iz igrice*/
        case 27:
            exit(0);
            break;
        case 'd':
        case 'D':
            kretanja[1] = 1;
            break;
        case 'a':
        case 'A':
            kretanja[0] = 1;
            break;
            
        case 'w':
        case 'W':
            if(!start){
                start = 1;
                glutDisplayFunc(on_display);
                glutTimerFunc(20, on_timer, 0); 
            }
            break;    

        case 'r':
        case 'R':
            score = 0;
            start = 0;
            boja = 1;
            levitacija = 0;
            x_koordinata = 0;
            z_koordinata = 1;
            brzina = 0.2;
            kameraZ = -2;
            tackaPogleda_z = 7;
            glutDisplayFunc(on_display);
            glutPostRedisplay();
            break;                          
    }
}




static float rastojanje(Kocka koc){
    
    float x = pow((koc.x - x_koordinata), 2);
    float y = pow((koc.y - 0.5), 2);
    float z = pow((koc.z - z_koordinata), 2);

    return sqrt(x + y + z);
}


static void kolizija(){
    int i;
    
    for(i=0; i < BROJ_PREPREKA; i++){
        Kocka koc1 = prepreka[i]; 
        Kocka koc2 = statPrep[i];
        float u1 = rastojanje(koc1);
        float u2 = rastojanje(koc2);
        
        if(u1 < 0.65  ||  u2 < 0.65 ){
            start = 0; 
            glutDisplayFunc(game_over);
            break;
        }
    }
}



//kad se pusti taster za kretanje
static void on_release(unsigned char key, int x, int y){
    
    switch (key){
    case 'a':
    case 'A':
        kretanja[0] = 0;
        break;
    case 'd':
    case 'D':
        kretanja[1] = 0;
        break;        
    }
}






//prikaz na ekranu
void on_display(void){
        
        GLfloat light_position[] = {0,1,0, 0 };
              
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
              
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
                
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	/* pogled kamere */
	gluLookAt(x_koordinata, 2.5, kameraZ, 
              x_koordinata, 1, tackaPogleda_z, 
              0, 1, 0);
	
      /*Prikaz skora na ekranu*/
    glPushMatrix();
        glColor3f(1,1,1);
        glRasterPos3f(x_koordinata + 5,  5, z_koordinata + 5);
    
        char score_display[50] = "SCORE: ";
        char score_value[50];
    
        sprintf(score_value, " %d ", score);
        strcat(score_display, score_value);
        int len = (int)strlen(score_display);
        int i;
        for (i = 0; i < len; i++){
            glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, score_display[i]);
        }
    glPopMatrix();


    /*Postavljamo pozadinu*/
    glPushMatrix();
        glRotatef(100,1,0,0);
        glTranslatef(0 ,80 + z_koordinata , -20 );
        glScalef(14,1,24);
  
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, names[0]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-10, 0, -5);
			
			glTexCoord2f(1, 0);
			glVertex3f(10, 0, -5);
			
			glTexCoord2f(1, 3);
			glVertex3f(10, 0, 5);
			
			glTexCoord2f(0, 3);
			glVertex3f(-10, 0, 5);
		glEnd();
	
        glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
	 
	//draw_coo();
    draw_ball();
    draw_squares();
	draw_road();
	

        glutSwapBuffers();
}


static void on_timer(int value){
     
    if(value != 0)
        return;
    

    if( (int)ceil(z_koordinata) % 570 == 0)
    {
        postavke();
        draw_squares();
        z_koordinata = 0;
        kameraZ = -3;
        tackaPogleda_z = 7;
        boja = (boja + 1) % 9;
        
    }
    
    if(score % 1200 == 0)
        brzina += 0.05;

    z_koordinata += brzina;
    kameraZ += brzina;
    tackaPogleda_z += brzina;        
    
    levitacija += levIndex;

    if (levitacija == 0.08)
        levIndex *= (-1);
    if(levitacija == -0.08) 
        levIndex < 0 ? levIndex *= (-1) : levIndex;
    
             
    if( start ) {
        score += 1;
        glutTimerFunc(20, on_timer, 0);
    }
    
    kolizija();

     if(kretanja[0] && x_koordinata < 39)
         x_koordinata += 0.3;
    
     if(kretanja[1] && x_koordinata > -39)
         x_koordinata -= 0.3;
    
    glutPostRedisplay();
}


/*Funkcija kojom se postavlja startgame izgled ekrana*/
void startgame(){
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 3.5, 0,
              0, 0, 0,
              1, 0, 0);
    
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, names[3]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-2, 0, -3);
			
			glTexCoord2f(0, 1);
			glVertex3f(2, 0, -3);
			
			glTexCoord2f(1, 1);
			glVertex3f(2, 0, 3);
			
			glTexCoord2f(1, 0);
			glVertex3f(-2, 0, 3);
		glEnd();
        
	glDisable(GL_TEXTURE_2D);
    

    glutSwapBuffers();
	
}

/*Funkcija kojom se postavlja gameover izgled ekrana*/
void game_over(){
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*Pozicioniramo kameru na odgovarajucu poziciju*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 3.5, 0,
              0, 0, 0,
              1, 0, 0);
    /*Postavljamo teksturu, tj sliku*/
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, names[2]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-2, 0, -3);
			
			glTexCoord2f(0, 1);
			glVertex3f(2, 0, -3);
			
			glTexCoord2f(1, 1);
			glVertex3f(2, 0, 3);
			
			glTexCoord2f(1, 0);
			glVertex3f(-2, 0, 3);
		glEnd();
        
	glDisable(GL_TEXTURE_2D);
    
    /*Ispisujemo score na ekran*/
    
    glPushMatrix();
        glColor3f(1,1,1);
        glRasterPos3f(0.3, 0.1, -1.3);
        char score_display[50] = " ";
        char score_value[50];
        sprintf(score_value, " %d ", score);
        strcat(score_display, score_value);

        int len = (int)strlen(score_display);
        int i;
        for ( i = 0; i < len; i++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_display[i]);
        }
    glPopMatrix();
    
    glutSwapBuffers();
	
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

void random_prepreke (int j, int w, int m) {
    for(j, w; j<160; w++, j += 5){
        int q = brKockiZaRed[w]+1;
        for( q; q >= 0; q--){
            glPushMatrix();
                glColor3f(boja_1(boja), boja_2(boja), boja_3(boja));
                glTranslatef( prepreka[m].x ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            prepreka[m].z = j;
            m++;
        }
    }
}

static void on_timer2(int value){
     
    if(value != 0)
        return;

    int br = 0;
    levIndex = 0.02;
    levitacija = 0;
    levitacija += levIndex;
    glutTimerFunc(20, on_timer2, 0);
    
    if(levitacija == 1.0) {
        glutDisplayFunc(game_over);
        levIndex = 0.005;
        levitacija = 0;
    }
    glutPostRedisplay();
}

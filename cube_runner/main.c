/* PROJEKAT IZ RACUNARSKE GRAFIKE 
    
        OGNJEN JANKOVIC     

            88/2017
*/


#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<GL/glut.h>

#define BR_REDOVA_ZA_PREPREKE 550
#define BR_STAT_KOCKI 1000
#define BROJ_PREPREKA 5000

static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_reshape(int width, int height);
static void on_release(unsigned char key,int x,int y);
static void on_display(void);

/*Promenljiva koje sluzi za kontrolisanje igre*/
static int start = 0;

//Promenljive zaduzene za kretanje i pozicioniranje objekta
static int kretanja[] = {0, 0};
static float x_koordinata = 0;
static float z_koordinata = 0;
static float kameraZ = -3;
static float tackaPogleda_z = 7;


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


//inicijalizacija prepreka
static void postavke(){
    int i;
    for( i=0 ; i < BR_REDOVA_ZA_PREPREKE; i++)
        brKockiZaRed[i] = (int)rand() % 30;
    
    for(i=0; i < BROJ_PREPREKA; i++) {
        prepreka[i].x = -35 + ( (int)rand() % 70) ;
//         prepreka[i].z = 45 + (int)rand() % 10;
        prepreka[i].y = 0.5;
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
      

         glutReshapeFunc(on_reshape);
         glutDisplayFunc(on_display);
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

void draw_ball(){

        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);


        glPushMatrix();
            glColor3f(0 ,0, 1);
            glTranslatef(x_koordinata,0.5, z_koordinata);
//             glScalef(0, 0, 10);
            glutSolidCube(0.4);
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
    int m = 0;
    
//Zid po X osi    
   for(i ; i<36 ; i++) { 
    glPushMatrix();
        glColor3f(1 ,0, 0);
        glTranslatef(-4-i,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -4-i;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    glPushMatrix();
        glColor3f(1 ,0, 0);
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
        glColor3f(1 ,0, 0);
        glTranslatef(-4,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -4;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
     glPushMatrix();
        glColor3f(1 ,0, 0);
        glTranslatef(-3.2,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -3.2;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    glPushMatrix();
        glColor3f(1 ,0, 0);
        glTranslatef(4,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = 4;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    glPushMatrix();
        glColor3f(1 ,0, 0);
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
        glColor3f(1 ,0, 0);
        glTranslatef(-3.2, 0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -3.2;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    glPushMatrix();
        glColor3f(1 ,0, 0);
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
    
    for(j, w; j<160; w++, j += 3){
        int q = brKockiZaRed[w]+1;
        for( q; q >= 0; q--, m++){
            glPushMatrix();
                glColor3f(1 ,0, 0);
                glTranslatef( prepreka[m].x ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            
            prepreka[m].z = j;
        }
    }
    
    
    j++;

//Iscrtavanje galerije 

    //Zid po X osi    
   for(i=0 ; i<36 ; i++) { 
    glPushMatrix();
        glColor3f(1 ,0, 0);
        glTranslatef(-4-i,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
    statPrep[t].x = -4-i;
    statPrep[t].y = 0.5;
    statPrep[t].z = j;
    t++;
    
    
    glPushMatrix();
        glColor3f(1 ,0, 0);
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
            glColor3f(1 ,0, 0);
            glTranslatef(3 + i ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3+i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        
        glPushMatrix();
            glColor3f(1 ,0, 0);
            glTranslatef(-3 - i ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = -3-i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        if(j > b){
            glPushMatrix();
                glColor3f(1 ,0, 0);
                glTranslatef(0 + k ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            statPrep[t].x = 0+k;
            statPrep[t].y = 0.5;
            statPrep[t].z = j;
            t++;
            
            glPushMatrix();
                glColor3f(1 ,0, 0);
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
            glColor3f(1 ,0, 0);
            glTranslatef(3 + i ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3+i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(1 ,0, 0);
            glTranslatef(-3 - i ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = -3-i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        if(j < b){
            glPushMatrix();
                glColor3f(1 ,0, 0);
                glTranslatef(0 + k ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            statPrep[t].x = 0+k;
            statPrep[t].y = 0.5;
            statPrep[t].z = j;
            t++;
            
            glPushMatrix();
                glColor3f(1 ,0, 0);
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
    
    for(j, w; j<345; w++, j += 3){
        int q = brKockiZaRed[w]+1;
        for( q; q >= 0; q--, m++){
            glPushMatrix();
                glColor3f(1 ,0, 0);
                glTranslatef( prepreka[m].x ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            
            prepreka[m].z = j;
        }
    }

    
    j+=5;

//kraj random prepreka


    //Kosina 
    //Zid po X osi    
   for(i=0 ; i<36 ; i++) { 
    glPushMatrix();
        glColor3f(1 ,0, 0);
        glTranslatef(-4-i,0.5, j);
        glutSolidCube(0.8);
    glPopMatrix();
        statPrep[t].x = -4-i;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
    
    
    glPushMatrix();
        glColor3f(1 ,0, 0);
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
            glColor3f(1 ,0, 0);
            glTranslatef(3.5 - h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3.5-h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(1 ,0, 0);
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
            glColor3f(1 ,0, 0);
            glTranslatef(3.5 - h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3.5-h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(1 ,0, 0);
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
            glColor3f(1 ,0, 0);
            glTranslatef(3.5 - h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3.5-h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(1 ,0, 0);
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
            glColor3f(1 ,0, 0);
            glTranslatef(3.5 + h ,0.5, j);
            glutSolidCube(0.8);
        glPopMatrix();
        statPrep[t].x = 3.5+h;
        statPrep[t].y = 0.5;
        statPrep[t].z = j;
        t++;
        
        glPushMatrix();
            glColor3f(1 ,0, 0);
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
    
    for(j, w; j<571; w++, j += 3){
        int q = brKockiZaRed[w]+1;
        for( q; q >= 0; q--, m++){
            glPushMatrix();
                glColor3f(1 ,0, 0);
                glTranslatef( prepreka[m].x ,0.5, j);
                glutSolidCube(0.8);
            glPopMatrix();
            
            prepreka[i].z = m;
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
                glutTimerFunc(20, on_timer, 0); 
            }
            break;                              
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
	
	 
	draw_coo();
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
    //    postavke();
        z_koordinata = 0;
        kameraZ = -3;
        tackaPogleda_z = 7;
    }
    
    z_koordinata += 0.5;
    kameraZ += 0.5;
    tackaPogleda_z += 0.5;        
             
             
    if( start )
        glutTimerFunc(20, on_timer, 0);
    
    
     if(kretanja[0] && x_koordinata < 39.4)
         x_koordinata += 0.3;
    
     if(kretanja[1] && x_koordinata > -39.4)
         x_koordinata -= 0.3;
    
    glutPostRedisplay();
}

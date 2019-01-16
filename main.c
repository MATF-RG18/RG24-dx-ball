#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

//Deklaracija callback funkcija
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int);
static void initialization(void);   //inicijalizacija promenljivih

#define PI (3*3.1415926535)     //Konstanta

static int animation_ongoing;   //Animacija pokrenuta ili ne;
static double Xsphere,Ysphere;  //Koordinate x,y za kuglu
static double Xcylinder;        //Koordinata x za valjak
static int *targets = NULL;     //Niz koji cuva postojanje/nepostojanje meta
static double *targetsCentar = NULL;    //Niz koji cuva pozicije (x,y) meta
static double Xsphere1,Ysphere1,Xsphere0,Ysphere0;  //Prethodne i trenutne koordinate kugle
static double move;         //Pomeraj

int main(int argc, char **argv){
    
    //inicijalizacija GLUT-a
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    //Postavljanje velicine prozora, pozicije, i kreiranje prozora
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("DX BALL");

    //Registracija callback funkcija
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    
    //Pozivanje funkcije inicijalizacije
    initialization();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //Ulazak u glavnu petlju
    glutMainLoop();
    
    return 0;
}

void initialization(){
    
    //Podesavanje pozadine i SMOOTH secenja
    glClearColor(0.3,0.3,0.3,0.3);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    animation_ongoing = 0;
    move = 0.15;
    
    //Pocetne pozicije kugle
    Xsphere0 = -0.1;
    Ysphere0 = -4.1;
    
    //Random broj koji odredjuje na koju stranu ce kugla krenuti
    srand(time(NULL));
    double randomBroj = rand()%2;
    if(randomBroj < 1)
        Xsphere1 = 0;
    else
        Xsphere1 = -0.2;
    
    //Sledeca y koordinata kugle
    Ysphere1 = -4;
    
    //Trenutne koordinate kugle
    Xsphere = Xsphere1;
    Ysphere = Ysphere1;
    
    //Pocetna koordinata x valjka
    Xcylinder = 0;
    
    //Alokacija memorije za niz i 
    //Postavljanje svih elemenata niza na 1 (koje oznacava postojanje mete)
    if( (targets = (int *) malloc(sizeof(int)*25)) == NULL ){
        exit(0);
    }

    for(int i=0; i<25; i++)
        targets[i] = 1;

    //Alokacija memorije za niz i 
    //Postavljanje pozicije svih meta
    if( (targetsCentar = (double *) malloc(sizeof(double)*50)) == NULL ){
        exit(0);
    }
    
    int k=0;
    for(double j=5; j>0; j--){
        for(double i=-j+1; i<j; i+=0.5){
            if(k%2)
                targetsCentar[k] = j;
            else
                targetsCentar[k] = i;
            k++;
        }
    }
}


static void on_reshape(int width, int height){
    
    //Podesavanje viewport-a
    glViewport(0, 0, width, height);

    //Podesavanje projekcije
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 15);
}

//''Odaziv na odredjene tastere na ekranu''
static void on_keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27:        //Izlaz iz programa na 'esc'
            exit(0);
            break;
            
        case 'g':       //Pokretanje programa
        case 'G':
            if (!animation_ongoing) {
                glutTimerFunc(50, on_timer, 0);
                animation_ongoing = 1;
            }
            break;

        case 's':       //Stopiranje
        case 'S':
            animation_ongoing = 0;
            break;
            
        case 'r':       //Restartovanje programa i inicijalizacija koordinata kugle i cilindra
        case 'R':
            animation_ongoing = 0;
            Xsphere = 0;
            Ysphere = -4;
            Xcylinder = 0;
            break;
    }
}


static void on_timer(int value){
    
    if (value != 0)
        return;
    
    int indikator = 1;
    
    //Ako je kugla dosla do desne ivice odozdo
    if( Xsphere > 5.5 && Ysphere0 <= Ysphere1 ){
        Xsphere -= move;
        Ysphere += move;
        indikator = 0;
    }
    //Ako je kugla dosla do desne ivice odozgo
    else if( Xsphere > 5.5 && Ysphere0 >= Ysphere1 ){
        Xsphere -= move;
        Ysphere -= move;
        indikator = 0;
    }
    //Ako je kugla dosla do leve ivice odozgo
    else if( Xsphere < -5.5 && Ysphere0 >= Ysphere1 ){
        Xsphere += move;
        Ysphere -= move;
        indikator = 0;
    }
    //Ako je kugla dosla do leve ivice odozdo
    else if( Xsphere < -5.5 && Ysphere0 <= Ysphere1 ){
        Xsphere += move;
        Ysphere += move;
        indikator = 0;
    }
    //Ako je kugla dosla do gornje ivice sa desne strane
    else if( Ysphere > 5.5 && Xsphere0 >= Xsphere1 ){
        Xsphere -= move;
        Ysphere -= move;
        indikator = 0;
    }
    //Ako je kugla dosla do gornje ivice sa leve strane
    else if( Ysphere > 5.5 && Xsphere0 <= Xsphere1 ){
        Xsphere += move;
        Ysphere -= move;
        indikator = 0;
    }
    //Ako je kugla udarila u valjak sa desne strane
    else if( Ysphere < -3.9 && Xsphere0 >= Xsphere1 && ( Xsphere <= Xcylinder + 2.6  && Xsphere >= Xcylinder - 2.6 ) ){
        Xsphere -= move;
        Ysphere += move;
        indikator = 0;
    }
    //Ako je kugla udarila u valjak sa leve strane
    else if( Ysphere < -3.9 && Xsphere0 <= Xsphere1 && ( Xsphere <= Xcylinder + 2.6  && Xsphere >= Xcylinder - 2.6 ) ){
        Xsphere += move;
        Ysphere += move;
        indikator = 0;
    }
    else{
        //Provera da li je kugla udarila u metu i njeno ponasanje ako jeste
        for(int i=0; i<50; i+=2){
            
            //Ako meta postoji
            if( targets[i/2] ){
                
                if( Xsphere <= (targetsCentar[i]+0.4) && Xsphere >= (targetsCentar[i]-0.4) && 
                           Ysphere >= (targetsCentar[i+1]-0.4) && Ysphere <= (targetsCentar[i+1]+0.4) ){
                    
                    //Provera da li je udarila sa leve, desne, gornje ili donje ivice
                    double x1 = fabs(targetsCentar[i] - 0.4 - Xsphere0);
                    double x2 = fabs(targetsCentar[i] + 0.4 - Xsphere0);
                    double y1 = fabs(targetsCentar[i+1] - 0.4 - Ysphere0);
                    double y2 = fabs(targetsCentar[i+1] + 0.4 - Ysphere0);
                    
                    if( x1 < x2 && x1 < y1 && x1 < y2 ){
                        Xsphere -= move;
                        if( Ysphere0 >= Ysphere1 )
                            Ysphere += move;
                        else
                            Ysphere -= move;
                    }
                    else if( x2 < x1 && x2 < y1 && x2 < y2 ){
                        Xsphere += move;
                        if( Ysphere0 >= Ysphere1 )
                            Ysphere -= move;
                        else
                            Ysphere += move;
                    }
                    else if( y1 < x1 && y1 < x2 && y1 < y2 ){
                        Ysphere -= move;
                        if( Xsphere0 >= Xsphere1 )
                            Xsphere -= move;
                        else
                            Xsphere += move;
                    }
                    else{
                        Ysphere += move;
                        if( Xsphere0 >= Xsphere1 )
                            Xsphere += move;
                        else
                            Xsphere += move;
                    }
                    
                    //Unistavamo metu u koju je udarila kugla
                    targets[i/2] = 0;
                    indikator = 0;
                }
            }
        }
    }
    
    
    //Prirodan nastavak kretanja kugle kad ne udari o ivice
    if(indikator){
        
        if( Xsphere0 >= Xsphere1 && Ysphere0 >= Ysphere1){
            Xsphere -= move;
            Ysphere -= move;
        }
        else if( Xsphere0 <= Xsphere1 && Ysphere0 >= Ysphere1){
            Xsphere += move;
            Ysphere -= move;
        }
        else if( Xsphere0 >= Xsphere1 && Ysphere0 <= Ysphere1){
            Xsphere -= move;
            Ysphere += move;
        }
        else if( Xsphere0 <= Xsphere1 && Ysphere0 <= Ysphere1){
            Xsphere += move;
            Ysphere += move;
        }
    }
    
    
    //Ako je kugla promasila valjak i otisla ispod, prekida se akcija
    if( Ysphere <= -5 ){                                         /* GAME OVER */
        
        animation_ongoing = 0;
    }                                                           
        
    //Ponovna postavka koordinata kugle
    Xsphere0 = Xsphere1;
    Ysphere0 = Ysphere1;
    Xsphere1 = Xsphere;
    Ysphere1 = Ysphere;
    
    //Ponovno iscrtavanje
    glutPostRedisplay();

    //Nastaviti sa animacijom ako je animation_ongoing=1
    if (animation_ongoing)
        glutTimerFunc(5, on_timer, 0);
}


//Crtanje meta
void draw_target(){
    
    for(int i=0; i<50; i+=2){
            
        if( targets[i/2] == 1 ){
            glPushMatrix();
                glTranslatef(targetsCentar[i],targetsCentar[i+1],0);
                glutSolidCube(0.5);
            glPopMatrix();
        }
    }
}


void cylinder_normal_and_vertex(float u, float v){
    
    glNormal3f(sin(v)/2,0,cos(v)/2);        //Izracunavanja za funkciju draw_cylinder
    glVertex3f(sin(v)/2,u/2,cos(v)/2);
}


void draw_cylinder(){
    
    float u, v;
    
    //Translacija i rotacija valjka
    glTranslatef(0,-4.7,0);
    glTranslatef(PI/4,0,0);
    glRotatef(90,0,0,1);
    
    //Crtanje valjka
    for (u = 0; u < PI; u += PI / 50) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + 0.01; v += PI / 50) {
            cylinder_normal_and_vertex(u, v);
            cylinder_normal_and_vertex(u + PI / 50, v);
        }
        glEnd();
    }
}


static void on_display(void){
    
    //Podesavanje osvetljena
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
      
    GLfloat ambient_coeffs[] = { 0.3, 0.7, 0.3, 1 };
    GLfloat diffuse_coeffs[] = { 0.2, 1, 0.2, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 30;
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GLfloat light_position[] = {1,3,1,0};

    //Podesavanje tacke pogleda
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    //Crtanje plavog okvira
    glPushMatrix();
        ambient_coeffs[0] = 0.3;
        ambient_coeffs[1] = 0.3;
        ambient_coeffs[2] = 0.7;
        diffuse_coeffs[0] = 0.2;
        diffuse_coeffs[1] = 0.4;
        diffuse_coeffs[2] = 0.6;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        glBegin(GL_QUADS);
            glVertex3f(5.75,-5.75,0);
            glVertex3f(5.6,-5.75,0);
            glVertex3f(5.6,5.75,0);
            glVertex3f(5.75,5.75,0);
        glEnd();
    
        glBegin(GL_QUADS);
            glVertex3f(5.6,5.75,0);
            glVertex3f(5.6,5.6,0);
            glVertex3f(-5.75,5.6,0);
            glVertex3f(-5.75,5.75,0);
        glEnd();
        
        glBegin(GL_QUADS);
            glVertex3f(-5.75,5.6,0);
            glVertex3f(-5.6,5.6,0);
            glVertex3f(-5.6,-5.75,0);
            glVertex3f(-5.75,-5.75,0);
        glEnd();
    glPopMatrix();
    
    //Crtanje zelene kugle
    glPushMatrix();                                         
        ambient_coeffs[0] = 0.3;
        ambient_coeffs[1] = 0.7;
        ambient_coeffs[2] = 0.3;
        diffuse_coeffs[0] = 0.2;
        diffuse_coeffs[1] = 1;
        diffuse_coeffs[2] = 0.2;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        glTranslatef(Xsphere,Ysphere,0);
        glutSolidSphere(0.25, 20, 20);
    glPopMatrix();
    
    //Crtanje plavog valjka
    glPushMatrix();
    
        ambient_coeffs[0] = 0.3;
        ambient_coeffs[1] = 0.3;
        ambient_coeffs[2] = 0.7;
        diffuse_coeffs[0] = 0.2;
        diffuse_coeffs[1] = 0.4;
        diffuse_coeffs[2] = 0.7;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        
        glTranslatef(Xcylinder,0,0);
    
    draw_cylinder();
    
    glPopMatrix();
    
    //Crtanje meta (kockica)
    glPushMatrix();
    
        ambient_coeffs[0] = 0.2;
        ambient_coeffs[1] = 0.2;
        ambient_coeffs[2] = 0.6;
        diffuse_coeffs[0] = 0.2;
        diffuse_coeffs[1] = 0.2;
        diffuse_coeffs[2] = 0.6;
    
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    
        draw_target();
        
    glPopMatrix();
    
    glutSwapBuffers();
}

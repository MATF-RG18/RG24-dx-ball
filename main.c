#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

//Deklaracija callback funkcija
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void initialization(void);   //inicijalizacija promenljivih

#define PI (3*3.1415926535)     //Konstanta

static double Xsphere,Ysphere;  //Koordinate x,y za kuglu
static double Xcylinder;        //Koordinata x za valjak

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
    glClearColor(0.5, 0.5, 0.5, 0.5);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    //Pocetne koordinate kugle
    Xsphere = 0;
    Ysphere = -3.9;
    
    //Pocetna koordinata x valjka
    Xcylinder = 0;
}


static void on_reshape(int width, int height){
    
    //Podesavanje viewport-a
    glViewport(0, 0, width, height);

    //Podesavanje projekcije
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 15);
}


static void on_keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27:        //Izlaz iz programa na 'esc'
            exit(0);
            break;
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
    

    glutSwapBuffers();
}

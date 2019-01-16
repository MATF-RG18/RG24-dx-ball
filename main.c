#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_mouse(int button, int state, int x, int y);
static void on_motion(int x, int y);
static void on_timer(int value);
static void on_display(void);
static void initialization(void);

void setText();
void setWin(char *r);

static int mouse_x, mouse_y;
static double nivo;
static int life;
static int animation_ongoing;
static double PI;
static double Xsphere;
static double Ysphere;
static double move;
static double Xsphere1;
static double Ysphere1;
static double Xsphere0;
static double Ysphere0;
static double Xcylinder;
static int result;
static int *targets = NULL;
static double *targetsCentar = NULL;
static int lose;
static int win;

int main(int argc, char **argv){
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("DX BALL");

    glutKeyboardFunc(on_keyboard);
    glutMouseFunc(on_mouse);
    glutMotionFunc(on_motion);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    
    nivo = 3;
    life = 2;
    result = 0;
    
    initialization();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glutMainLoop();

    return 0;
}


void initialization(){
    
    if(nivo == 3)
        animation_ongoing = 0;
    
    glClearColor(0.3,0.3,0.3,0.3);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    PI = 3*3.1415926535;
    move = 0.12;
    
    mouse_x = 0;
    mouse_y = 0;
    
    lose = 1;
    win = 0;

    Xsphere0 = -0.1;
    Ysphere0 = -4.1;
    
    srand(time(NULL));
    double randomBroj = rand()%2;
    if(randomBroj < 1)
        Xsphere1 = 0;
    else
        Xsphere1 = -0.2;
    
    Ysphere1 = -4;
    
    Xsphere = Xsphere1;
    Ysphere = Ysphere1;
    
    Xcylinder = 0;
    
    if( (targets = (int *) malloc(sizeof(int)*nivo*nivo)) == NULL ){
        exit(0);
    }

    for(int i=0; i<nivo*nivo; i++)
        targets[i] = 1;

    
    if( (targetsCentar = (double *) malloc(sizeof(double)*2*nivo*nivo)) == NULL ){
        exit(0);
    }
    
    int k=0;
    for(double j=nivo; j>0; j--){
        for(double i=-j+1; i<j; i+=0.5){
            if(k%2)
                targetsCentar[k] = j;
            else
                targetsCentar[k] = i;
            k++;
        }
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


static void on_keyboard(unsigned char key, int x, int y){
    
    switch (key) {
    case 27:
        exit(0);
        break;

    case 'g':
    case 'G':
        if (!animation_ongoing && lose && !win) {
            glutTimerFunc(50, on_timer, 0);
            animation_ongoing = 1;
        }
        break;

    case 's':
    case 'S':
        animation_ongoing = 0;
        break;
        
    case 'r':
    case 'R':
        animation_ongoing = 0;
        Xsphere = -0.1;
        Ysphere = -4.1;
        Xcylinder = 0;
        break;
        
    case 'n':
    case 'N':
        if(life == 0){
            life = 2;
            nivo = 3;
            result = 0;
            initialization();
        }
        break;
    }
}


static void on_timer(int value){
    
    if (value != 0)
        return;
    
    int indikator = 1;
    
    if( Xsphere > 5.5 && Ysphere0 <= Ysphere1 ){
        Xsphere -= move;
        Ysphere += move;
        indikator = 0;
    }
    else if( Xsphere > 5.5 && Ysphere0 >= Ysphere1 ){
        Xsphere -= move;
        Ysphere -= move;
        indikator = 0;
    }
    else if( Xsphere < -5.5 && Ysphere0 >= Ysphere1 ){
        Xsphere += move;
        Ysphere -= move;
        indikator = 0;
    }
    else if( Xsphere < -5.5 && Ysphere0 <= Ysphere1 ){
        Xsphere += move;
        Ysphere += move;
        indikator = 0;
    }
    else if( Ysphere > 5.5 && Xsphere0 >= Xsphere1 ){
        Xsphere -= move;
        Ysphere -= move;
        indikator = 0;
    }
    else if( Ysphere > 5.5 && Xsphere0 <= Xsphere1 ){
        Xsphere += move;
        Ysphere -= move;
        indikator = 0;
    }
    else if( Ysphere < -3.9 && Xsphere0 >= Xsphere1 && ( Xsphere <= Xcylinder + 2.6  && Xsphere >= Xcylinder - 2.6 ) ){
        Xsphere -= move;
        Ysphere += move;
        indikator = 0;
    }
    else if( Ysphere < -3.9 && Xsphere0 <= Xsphere1 && ( Xsphere <= Xcylinder + 2.6  && Xsphere >= Xcylinder - 2.6 ) ){
        Xsphere += move;
        Ysphere += move;
        indikator = 0;
    }
    else{
        
        for(int i=0; i<2*nivo*nivo; i+=2){
            
            if( targets[i/2] ){
                
                if( Xsphere <= (targetsCentar[i]+0.4) && Xsphere >= (targetsCentar[i]-0.4) && 
                           Ysphere >= (targetsCentar[i+1]-0.4) && Ysphere <= (targetsCentar[i+1]+0.4) ){
                    
                    double x1 = fabs(targetsCentar[i] - 0.4 - Xsphere0);
                    double x2 = fabs(targetsCentar[i] + 0.4 - Xsphere0);
                    double y1 = fabs(targetsCentar[i+1] - 0.4 - Ysphere0);
                    double y2 = fabs(targetsCentar[i+1] + 0.4 - Ysphere0);
                    
                    if( x1 < x2 && x1 < y1 && x1 < y2 ){
                        Xsphere -= move;
                        if( Ysphere0 >= Ysphere1 )
                            Ysphere -= move;
                        else
                            Ysphere += move;
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
                            Xsphere -= move;
                        else
                            Xsphere += move;
                    }
                    
                    targets[i/2] = 0;
                    indikator = 0;
                    result++;
                    
                    if(result == 50){
                        animation_ongoing = 0;
                        win = 1;
                    }
                }
            }
        }
    }
    
    
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
    
    
    if( Ysphere <= -5 ){                                         /* GAME OVER */
        
        life--;
        
        if(life != 0){
            on_keyboard('r',0,0);
        }
        else{
            animation_ongoing = 0;
            lose = 0;
        }
    }                                                           
        
    Xsphere0 = Xsphere1;
    Ysphere0 = Ysphere1;
    Xsphere1 = Xsphere;
    Ysphere1 = Ysphere;
    
    for(int i=1; i<2*nivo*nivo; i+=2){
        if( targets[(i-1)/2] && targetsCentar[i] <= -3.9 ){       /* GAME OVER */
            animation_ongoing = 0;
            life = 0;
            lose = 0;
        }
                                                            
        
        targetsCentar[i] -= nivo * 0.0004;
    }
    
    for(int i=0; i<nivo*nivo; i++){
        if(targets[i] == 1)
            break;
        
        if( (i == nivo*nivo-1) && !win){
            nivo++;
            initialization();
            on_keyboard('s',0,0);
        }
    }
    
    
    glutPostRedisplay();

    if (animation_ongoing)
        glutTimerFunc(5, on_timer, 0);
}


static void on_reshape(int width, int height){
    
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 15);
}


void cylinder_normal_and_vertex(float u, float v){
    
    glNormal3f(sin(v)/2,0,cos(v)/2);
    glVertex3f(sin(v)/2,u/2,cos(v)/2);
}


void draw_cylinder(){
    
    float u, v;
    
    glTranslatef(0,-4.7,0);
    glTranslatef(PI/4,0,0);
    glRotatef(90,0,0,1);
    
    for (u = 0; u < PI; u += PI / 50) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + 0.01; v += PI / 50) {
            cylinder_normal_and_vertex(u, v);
            cylinder_normal_and_vertex(u + PI / 50, v);
        }
        glEnd();
    }
}


void draw_target(){
    
    for(int i=0; i<2*nivo*nivo; i+=2){
            
        if( targets[i/2] == 1 ){
            glPushMatrix();
                glTranslatef(targetsCentar[i],targetsCentar[i+1],0);
                glutSolidCube(0.5);
            glPopMatrix();
        }
    }
}


static void on_mouse(int button, int state, int x, int y)
{
    /* Pamti se pozicija pokazivaca misa. */
    mouse_x = x;
    mouse_y = y;
}

static void on_motion(int x, int y)
{
    /* Promena pozicije pokazivaca misa. */
    int delta_x;

    /* Izracunava se promena pozicije pokazivaca misa. */
    delta_x = x - mouse_x;

    /* Pamti se nova pozicija pokazivaca misa. */
    mouse_x = x;

    if(animation_ongoing){
        if(delta_x > 0){
            if( Xcylinder < 3 )
                Xcylinder += delta_x * 0.01;
        }
        else if( delta_x < 0 ){
            if( Xcylinder > -3 )
                Xcylinder += delta_x * 0.01;
        }
    }
        
    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();
}


void setWin(char *r){
    
    char text[24], *t;
    
    glPushMatrix();
    
        glRasterPos3f(-0.5,0,0.5);
        sprintf(text,"%s", r);
        
        for(t = text; *t != '\0'; t++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*t);
    
    glPopMatrix();
}


void setText(){
    
    char text[24], *t;
    
    glPushMatrix();
        glBegin(GL_LINE_STRIP);
            glVertex3f(-5,-4.2,0.5);
            glVertex3f(-5,-5.2,0.5);
            glVertex3f(-5,-5.2,0.5);
            glVertex3f(-3.5,-5.2,0.5);
            glVertex3f(-3.5,-5.2,0.5);
            glVertex3f(-3.5,-4.2,0.5);
            glVertex3f(-5,-4.2,0.5);
        glEnd();
    
    glPopMatrix();
    
    glPushMatrix();
        glBegin(GL_LINE_STRIP);
            glVertex3f(2.9,-4.2,0.5);
            glVertex3f(2.9,-5.2,0.5);
            glVertex3f(2.9,-5.2,0.5);
            glVertex3f(4.8,-5.2,0.5);
            glVertex3f(4.8,-5.2,0.5);
            glVertex3f(4.8,-4.2,0.5);
            glVertex3f(2.9,-4.2,0.5);
            glVertex3f(3.8,-4.2,0.5);
            glVertex3f(3.8,-5.2,0.5);
        glEnd();
    
    glPopMatrix();
    
    
    glPopMatrix();
    
        glPushMatrix();
        glRasterPos3f(-4.8,-4.5,0.5);
        sprintf(text, "REZULTAT");
        
        for(t = text; *t != '\0'; t++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*t);
    
    glPopMatrix();
    
    glPushMatrix();
        glRasterPos3f(-4.4,-5,0.5);
        sprintf(text, "%d",result);
        
        for(t = text; *t != '\0'; t++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*t);
    
    glPopMatrix();
    
    glPushMatrix();
        glRasterPos3f(3,-4.5,0.5);
        sprintf(text, "NIVO");
        
        for(t = text; *t != '\0'; t++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*t);
    
    glPushMatrix();
        glRasterPos3f(3.2,-5,0.5);
        sprintf(text, "%.f",nivo-2);
        
        for(t = text; *t != '\0'; t++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*t);
    
    glPopMatrix();
    
    glPopMatrix();
        
    glPushMatrix();
        glRasterPos3f(4,-4.5,0.5);
        sprintf(text, "ZIVOT");
        
        for(t = text; *t != '\0'; t++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*t);
    
    glPopMatrix();
    
    glPushMatrix();
        glRasterPos3f(4.2,-5,0.5);
        sprintf(text, "%d",life);
        
        for(t = text; *t != '\0'; t++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*t);
    
    glPopMatrix();
    
}


static void on_display(void){
    
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
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
    
    glPushMatrix();
    
    if(nivo == 3){
        ambient_coeffs[0] = 0.05*nivo;
        ambient_coeffs[1] = 0.05*nivo;
        ambient_coeffs[2] = 0.2*nivo;
        diffuse_coeffs[0] = 0.05*nivo;
        diffuse_coeffs[1] = 0.05*nivo;
        diffuse_coeffs[2] = 0.2*nivo;
    }
    else if(nivo == 4){
        ambient_coeffs[0] = 0.2*nivo;
        ambient_coeffs[1] = 0.05*nivo;
        ambient_coeffs[2] = 0.05*nivo;
        diffuse_coeffs[0] = 0.2*nivo;
        diffuse_coeffs[1] = 0.05*nivo;
        diffuse_coeffs[2] = 0.05*nivo;
    }
    else{
        ambient_coeffs[0] = 0.05*nivo;
        ambient_coeffs[1] = 0.2*nivo;
        ambient_coeffs[2] = 0.05*nivo;
        diffuse_coeffs[0] = 0.05*nivo;
        diffuse_coeffs[1] = 0.2*nivo;
        diffuse_coeffs[2] = 0.05*nivo;
    }
    
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    
    
    draw_target();
    
    glPopMatrix();
    
    glPushMatrix();
        ambient_coeffs[0] = 0.1;
        ambient_coeffs[1] = 0.1;
        ambient_coeffs[2] = 0.1;
        diffuse_coeffs[0] = 0.1;
        diffuse_coeffs[1] = 0.1;
        diffuse_coeffs[2] = 0.1;
    
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        
        setText();
        
    glPopMatrix();
    
    glPushMatrix();
        if(win){
            glDisable(GL_LIGHTING);
            glColor3f(0,1,0);
            
            setWin("WIN!");
            
            glEnable(GL_LIGHTING);
        }
    glPopMatrix();
    
    glPushMatrix();
        if(!lose){
            glDisable(GL_LIGHTING);
            glColor3f(1,0,0);
            
            setWin("LOSE!");
            
            glEnable(GL_LIGHTING);
        }
    glPopMatrix();

    glutSwapBuffers();
}

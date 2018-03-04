#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;


char title[] = "2D plane" ,c;
GLfloat anglePyramid = 0.0f,aspect=0.0f;
GLfloat angleCube = 0.0f;
int refreshMills = 15,b=0,d=0,t=0,k=0,f=0;
GLdouble a=0.0,width,height;
double rotation=0.0,angle=90.0;
float i=0.0f,o=0.1f,p=10.0f;
GLboolean diff,spec,off=false;

const double MAX_INTENSITY=255.0;

double m[16];
float g[16];

void setm()
{
    memset(m,0,sizeof(m));
    for(int i=0; i<16; i+=5)m[i]=1;
}

void setg()
{
    memset(g,0,sizeof(g));
    for(int i=0; i<16; i+=5)g[i]=1;
}


void lookAt(float eyeX, float eyeY, float eyeZ ,float lookX, float lookY, float lookZ, float headX,float headY,float headZ){
    float x=(eyeX-lookX);
    float y=(eyeY-lookY);
    float z=(eyeZ-lookZ);
    float sum= sqrt(pow(x,2.0)+pow(y,2.0)+pow(z,2.0));
//    for(int i=0;i<3;i++)cout<<Zv[i]<<" ";


    float i1=headX, j1=headY, k1=headZ;
    float i2=x, j2=y, k2=z;

    float x1=((k2)*(j1)-(j2)*(k1));
    float y1=(-(k2)*(i1)+(i2)*(k1));
    float z1=((j2)*(i1)-(i2)*(j1));
    float sum1= sqrt(pow(x1,2.0)+pow(y1,2.0)+pow(z1,2.0));
  //  for(int i=0;i<3;i++)cout<<Xv[i]<<" ";

    float i3=headX, j3=headY, k3=headZ;
    int i4=x1, j4=y1, k4=z1;

    float x2=((k4)*(j3)-(j4)*(k3));
    float y2=(-(k4)*(i3)+(i4)*(k3));
    float z2=((j4)*(i3)-(i4)*(j3));
    float sum2= sqrt(pow(x2,2.0)+pow(y2,2.0)+pow(z2,2.0));
    setg();
    g[0]=x1/sum1;
    g[4]=y1/sum1;
    g[8]=z1/sum1;
    g[1]=x2/sum2;
    g[5]=y2/sum2;
    g[9]=z2/sum2;
    g[3]=x/sum;
    g[7]=y/sum;
    g[11]=z/sum;
    g[15]=1;




    glTranslatef(-eyeX,-eyeY,-eyeZ);

    glMultMatrixf(g);


}
void light(){

GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1 };
GLfloat light_specular[] = { 1, 1, 1, 1 };
GLfloat light_position[] = { 0, 20.0, -5, 1.0 };
GLfloat light_position2[] = { 10.5, 20.0, -5, 1.0 };

 glEnable( GL_LIGHT0);
		glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv( GL_LIGHT0, GL_POSITION, light_position);
		GLfloat spot_direction[] = { 0.0, -6.0, 0.0 };


		//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
		//glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);


		//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
	//	glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
//		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 5.0);
//        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 2.0);
}





void rotateAroundLine(double x1, double y1, double z1,
                      double x2, double y2, double z2)
{
// inverse translate
    glTranslatef(x1,y1,z1);

// inverse align
    setm();
    double A=x2-x1, B=y2-y1, C=z2-z1;
    double l= sqrt(B*B+C*C);
    double v= sqrt(A*A+B*B+C*C);

    m[0]=l/v;
    m[1]= -(A*B) /(l*v);
    m[2]= -(A*C) /(l*v);
    m[5]= C/l;
    m[6]= -B/l;
    m[8]=A/v;
    m[9]= B/v;
    m[10]= C/v;

    glMultMatrixd(m);


    // rotate
    glRotatef(rotation,0,0,1);


// Aligning with Z axis
    setm();
    m[0]=l/v;
   m[2]=A/v;
    m[4]= -(A*B) /(l*v);
    m[5]= C/l;
    m[6]= B/v;
    m[8]= -(A*C) /(l*v);
    m[9]= -B/l;
    m[10]= C/v;

    glMultMatrixd(m);

// Translate
    glTranslatef(-x1,-y1,-z1);
glEnable(GL_LINE_SMOOTH);
glLineWidth(5.0);
// draw Line
    glColor3f(1,0,1);
    glBegin(GL_LINES);
    glVertex3f(x1,y1,z1);
    glVertex3f(x2,y2,z2);

    glEnd();


}


void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glShadeModel(GL_SMOOTH);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

#define DEG2RAD 3.14159/180.0

void drawWheel(float cx, float cy, float cz, float r, int num_segments)
{
    glEnable(GL_LINE_SMOOTH);
     glLineWidth(5.0);
    glBegin(GL_LINES);
    for (int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);
        float z = r * cosf(theta);//calculate the y component
        glVertex3f(x + cx, y + cy, z+cz);//output vertex
    }
    glEnd();
}
void spobject(float cx,float cy,float cz,float r)
{
    glBegin(GL_LINE_STRIP);
    for(float x = -3; x <= 3; x += 0.25)
        glVertex2f(x, a + b * x + c * x * x + d * x * x * x);
    glEnd();

}
void pyramid(double x, double y,  double xp,double y1, double y2,  double z,double zp)
{
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_INDEX_ARRAY );

    GLfloat vertices[] = {x,  y,  z, // front top right corner
                          xp,  y1,  z, // front top left corner
                          xp,  y2,  z, // front bottom left corner
                          x,  y,  zp, // back bottom right corner
                          xp,  y1,  zp, // front top right corner
                          xp,  y2,  zp // front top left corner
                         }; // bottom right corner

    GLubyte indices[] = {0,1,2, // first triangle (bottom left - top left - top right)
                         3,4,5
                        }; // second triangle (bottom left - top right - bottom right)
    GLubyte indices2[] = {  0,2,5,3,
                         2,5,4,1,
                         0,1,4,3
                         };

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
    glDrawElements(GL_QUADS, 12, GL_UNSIGNED_BYTE, indices2);

}
void cube2(double x, double y,  double x2, double y2,double x3,double y3, double x4, double y4,  double z, double zp) {


glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_INDEX_ARRAY );

    GLfloat vertices[] = {x,  y,  z, // front top right corner
                          x2,  y2,  z, // front top left corner
                          x3,  y3,  z, // front bottom left corner
                          x4,  y4,  z, // back bottom right corner
                          x,  y,  zp, // front top right corner
                          x2,  y2,  zp, // front top left corner
                          x3,  y3,  zp, // front bottom left corner
                          x4,  y4,  zp,   // back bottom right corner
                         }; // bottom right corner

    GLubyte indices[] = {0,1,2,3, // first triangle (bottom left - top left - top right)
                         4,0,3,7,
                         4,5,6,7,
                         1,5,6,2,
                         0,4,5,1,
                         2,6,7,3
                        }; // second triangle (bottom left - top right - bottom right)

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);


}
void cube(double x, double y,  double xp, double yp,  double z, double zp){

	glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_INDEX_ARRAY );

    GLfloat vertices[] = {x,  y,  z, // front top right corner
                          xp,  y,  z, // front top left corner
                          xp,  yp,  z, // front bottom left corner
                          x,  yp,  z, // back bottom right corner
                          x,  y,  zp, // front top right corner
                          xp,  y,  zp, // front top left corner
                          xp,  yp,  zp, // front bottom left corner
                          x,  yp,  zp,   // back bottom right corner
                         }; // bottom right corner

    GLubyte indices[] = {0,1,2,3, // first triangle (bottom left - top left - top right)
                         4,0,3,7,
                         4,5,6,7,
                         1,5,6,2,
                         0,4,5,1,
                         2,6,7,3
                        }; // second triangle (bottom left - top right - bottom right)

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);

}


void room(void){
	glEnableClientState(GL_VERTEX_ARRAY);
	//Anlegen eines Arrays mit den Punktkooridanten
	static GLfloat vertices[] = { -8,-5,-5,  12,-5,-5,  12,10,-5,  -8,10,-5, -8,10,5,  -8,-5,5,  12,-5,5,  12,10,5};

	glVertexPointer(3, GL_FLOAT, 0, vertices);

	//Nummerierung der Punkte des Wuerfels
	static GLubyte frontIndices[] = {0,1,2,3};
	static GLubyte rightIndices[] = { 1,6,7,2};
	static GLubyte bottomIndices[] = {0,5,6,1};
	static GLubyte leftIndices[] = {3,4,5,0};
	//static GLubyte bottomIndices[] = {0, 1 , 2 ,3};

	glColor3f(0.05f, 0.05f, 0.85f);     // Red
    GLfloat mat_ambient1[] = { 0,0,.5, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient1);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, frontIndices);
	glColor3f(0.5f, 0.0f, 0.90f);
	GLfloat mat_ambient2[] = { .5,0,.9, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient2);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rightIndices);
	glColor3f(0.0f, 0.0f, 0.0f);
	GLfloat mat_ambient3[] = { 0,0,0, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient3);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, bottomIndices);
	glColor3f(0.5f, 0.85f, 0.6f);
	GLfloat mat_ambient4[] = { .5,.85,.6, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient4);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, leftIndices);

}

void almari()
{

    //glColor3f((185/255.0), (122/255.0), (87/255.0));
    GLfloat mat_ambient[] = { (185/255.0), (122/255.0), (87/255.0), 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    cube(-8.0,-5.0,-6.5,5.0,0.0,-3.0);

    //glColor3f((99/MAX_INTENSITY), (24/MAX_INTENSITY), (1/MAX_INTENSITY));
    GLfloat mat_ambient1[] = { (99/255), (24/255), (1/255), 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient1);
    cube(-6.5,-5.0,-6.2,5.0,0.0,-1.5);

     // glColor3f((136/MAX_INTENSITY), (0/MAX_INTENSITY), (21/MAX_INTENSITY));
      GLfloat mat_ambient2[] = { (136/255), 0 , (21/255), 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient2);
    cube(-6.5,-5.0,-6.2,5.0,-1.5,-3.0);

    //handle
   // glColor3f((39/MAX_INTENSITY), (34/MAX_INTENSITY), (28/MAX_INTENSITY));
   //glColor3f(1.0,1.0,1.0);
   GLfloat mat_ambient3[] = { 1,1,1, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient3);
    cube(-6.2,-.2,-5.8,0,-1.0,-1.1);
    cube(-5.85,-.8,-5.8,-.2,-1.0,-1.1);
    cube(-6.2,-1,-5.8,-.8,-1.0,-1.1);

   // glTranslatef(0,0,-1.7);

    cube(-6.2,-.2,-5.8,0,-1.6,-1.7);
    cube(-5.85,-.8,-5.8,-.2,-1.6,-1.7);
    cube(-6.2,-1,-5.8,-.8,-1.6,-1.7);



}

void bed()
{  GLfloat mat_ambient[] = { (39/255), (34/255), (28/255), 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);

    cube(6.0,-3.0,12.0,-2.5,1.0,-4.0);

    //leg
    cube(6.0,-5.0,6.2,-3.0,-3.8,-4.0);
    cube(6.0,-5.0,6.2,-3.0,1.0,.8);
    cube(11.8,-5.0,12.0,-3.0,1.0,.8);
    cube(11.8,-5.0,12.0,-3.0,-3.8,-4.0);

    //head
    cube(6.0,-3.0,12.0,-1.0,-3.8,-4.0);

    //bed shit
    //glColor3f(.5,0.5,0);
    GLfloat mat_ambient1[] = { .5,.5,0, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient1);
    cube(6.0,-2.5,12.0,-2.3,1.0,-4.0);
    cube(5.9,-5.0,6.0,-2.3,1.0,-4.0);

    //pillow
    //glColor3f(1,1,1);
    GLfloat mat_ambient2[] = { 1,1,1, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient2);
    cube(7.0,-2.3,9.0,-2.0,-3,-4.0);
    cube(9.5,-2.3,11.5,-2.0,-3,-4.0);

}

void table(void){

    GLfloat mat_ambient[] = { 0.65, 0.35, 0.05, 1.0 };
    GLfloat mat_diffuse[] = { 0.65, 0.35, 0.05, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {10};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    if(diff)
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    if(spec){
        glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    }
    cube(-1.0,-2.0,2.0,-1.7,0.0,-1.5);
    //leg
    cube(-1.0,-5.0,-0.9,-2.0,0.0,-.2);
    cube(-1.0,-5.0,-0.9,-2.0,-1.3,-1.5);
    cube(1.9,-5.0,2.0,-2.0,0.0,-.2);
    cube(1.9,-5.0,2.0,-2.0,-1.3,-1.5);

}

void horse()
{
   // GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 0, 0.0, 1.0 };
    GLfloat mat_shininess[] = {10};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    cube2(0.4,-.2,.8,0,0.7,.5,0.3,0,-.5,-.8);

    cube2(1.6,-.7,2.9,-.7,2.9,.2,1.6,.2,-.5,-.8);

    GLfloat mat_ambient1[] = { 0.2, 0, 0.0, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient1);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    cube2(.8,0,1.4,-.4,1.6,.1,0.7,.5,-.5,-.8);

    GLfloat mat_ambient2[] = { 0.0, 0, 0.0, 1.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient2);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    //hair
    cube2(0.7,.5,1.6,.1,1.7,.2,.9,.6,-.5,-.8);

    cube2(2.9,0,3.1,-.7,3.2,-.7,2.9,.2,-.5,-.8);
    //leg
     cube(1.6,-1.2,1.7,-.7,-.5,-.6);
     cube(1.6,-1.2,1.7,-.7,-.7,-.8);
     cube(2.6,-1.2,2.7,-.7,-.5,-.6);
     cube(2.6,-1.2,2.7,-.7,-.7,-.8);
     //belt
    GLfloat mat_ambient3[] = { 0.0, 0, 0.2, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient3);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    cube2(1.4,-.5,1.6,-.7,1.8,.2,1.6,.2,-.5,-.8);
}

void plane()
{  GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0,0,.95, 1.0 };
    GLfloat mat_ambient2[] = { 1,0,.95, 1.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    //glColor3f(1,0,.95);
    pyramid(-0.2,.5,0.3,0,1.0,-.5,-.8);

    //glColor3f(1,0,.95);
    pyramid(1.8,1.0,2.3,1.0,2,-.5,-.8);
   // glColor3f(1,0,.95);
    pyramid(0.7,1.0,1.3,1.0,2,-.5,-.8);

    cube(.9,.1,1.2,.2,.1,-1.4);

     glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient2);
    //glColor3f(0,0,.95);
    cube(0.3,0,2.3,1.0,-.5,-.8);


    glEnable(GL_LINE_SMOOTH);
     glLineWidth(5.0);

    glBegin(GL_LINES);
    //glColor3f(0.05,0.15,.15);
    GLfloat mat_ambient3[]= { 0.05,0.15,.15, 1.0 };
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient3);
    glVertex3f(.7,0,-.6);
    glVertex3f(.5,-.5,-.6);
    glVertex3f(1.5,0,-.6);
    glVertex3f(1.7,-.5,-.6);
    glEnd();

    drawWheel(.5,-.7,-.5,.25,1000);
    drawWheel(1.7,-.7,-.5,.25,1000);



}
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '=':
            angle+=10.0;
            break;
        case '-':
            angle-=10.0;
            break;
        case'k':
            k++;
            break;
        case'r':
            i=0.0f,o=0.1f,p=10.0f,off=false;
            break;
        case '2':
            diff=true;
            break;

            case '3':
            spec=true;
            break;
        case 'I':
          i+=.2f;
          break;
        case'0' :
            off=true;
            break;
        case 'O':
          o+=.2f;
          break;
    case 'P':
        p+=.2f;
        break;


    case 'i':
        i-=.2f;
        break;
    case 'o':
        o-=.2f;
        break;
    case 'p':
        p-=.2f;
        break;

    }
}
void totalscene()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
        gluLookAt(i,o,p, 0,0,0 ,0,1,0);
      //     glTranslatef(-2.5f, 0.0f, -8.0f);
           room();
           table();
          almari();
        bed();
         glTranslatef(-0.9f, -0.5f, 0.0f);
           glPushMatrix();
           if(k%2==1) { rotateAroundLine(-.2,-1,-.5,-.4,1,-.5); }
           //horse();
           plane();
           glPopMatrix();


}
void display() {
    // gluLookAt(2,2,1, 2,1,1, 0,1,0);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     totalscene();

    /*  glViewport(width/2,height/2,width/2, height/2);
      table();
     totalscene();
     glViewport(0,height/2,width/2, height/2);
     tablediff();
     totalscene();
     glViewport(0, 0, width/2, height/2);
     totalscene();
     glViewport(width/2, 0, width/2, height/2);
     totalscene();
     */


     if(off)glDisable(GL_LIGHT0);

      glutSwapBuffers();
}

void timer(int value) {
    rotation+=1;
   glutPostRedisplay();      // Post re-paint request to activate display()
   glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}


void reshape(GLsizei w, GLsizei h) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   width=w;
   height=h;
   if (height == 0) height = 1;                // To prevent divide by 0
    aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window

   glViewport(0, 0, width, height);


   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(60.0f, aspect, 0.1f, 100.0f);

    //gluLookAt(0,a,0, -1.5,0,-6.0, 0,1,0);

}

int main(int argc, char** argv) {




   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(1200, 600);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);
   glutReshapeFunc(reshape);      // Create window with the given title
   glutDisplayFunc(display);
   glutKeyboardFunc(key);
   glEnable(GL_LIGHTING);
   //glDisable(GL_LIGHTING);
   light();
          // Register callback handler for window re-paint event

//   glutKeyboardFunc(key);     // Register callback handler for window re-size event
   initGL();                       // Our own OpenGL initialization
   glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
   glutMainLoop();
               // Enter the infinite event-processing loop
   return 0;
}



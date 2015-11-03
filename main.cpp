#include "mesh.h"
#include "LapMatrixMesh.h"
#include "Matrix.h"
#include "DLRS.h"
#include "Model_OBJ.h"
#include "LaplacianSmooth.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <fstream>
#include <ctime>
#include <cmath>
#include <string.h>


using namespace std;

static int window_width, window_height;
static int start_x, start_y;
static double current_rotation[16], start_rotation[16];
static Mesh mesh;

static void arcball_rotation(int start_x, int start_y, int end_x, int end_y);
static void mouse(int button, int state, int x, int y);
static void motion(int x, int y);
static void init(void);
static void reshape(int w, int h);
static void display(void);
static int flag=0;//decide to display orignal or DLRS
static float times=1;
#define Big 3;
#define Small 4;


void arcball_rotation(int start_x, int start_y, int end_x, int end_y)
{
	double sx, sy, sz, ex, ey, ez;
	double scale;
	double sl, el;
	double dotprod;

	// find vectors from center of window
	sx = start_x - (window_width  / 2);
	sy = start_y - (window_height / 2);
	ex = end_x -   (window_width  / 2);
	ey = end_y -   (window_height / 2);

	// invert y coordinates (raster versus device coordinates)
	sy = -sy;
	ey = -ey;

	// scale by inverse of size of window
	if (window_width > window_height) {
		scale = 1.0 / (double) window_height;
	} else {
		scale = 1.0 / (double) window_width;
	}

	sx *= scale;
	sy *= scale;
	ex *= scale;
	ey *= scale;

	// project points to unit circle
	sl = hypot(sx, sy);
	el = hypot(ex, ey);

	if (sl > 1.0) {
		sx /= sl;
		sy /= sl;
		sl = 1.0;
	}

	if (el > 1.0) {
		ex /= el;
		ey /= el;
		el = 1.0;
	}

	// project up to unit sphere - find Z coordinate
	sz = sqrt(1.0 - sl * sl);
	ez = sqrt(1.0 - el * el);

	// rotate (sx,sy,sz) into (ex,ey,ez)

	// compute angle from dot-product of unit vectors (and double it).
	// compute axis from cross product.
	dotprod = sx * ex + sy * ey + sz * ez;
	glRotatef(2.0 * acos(dotprod) * 180.0 / M_PI,
						sy * ez - ey * sz,
						sz * ex - ez * sx,
						sx * ey - ex * sy);

}


void mouse(int button, int state, int x, int y)
{ 
	if (state == GLUT_DOWN) {
		// Start of drag.  
		int i;

		// Store off mouse location and current rotation matrix
		start_x = x;
		start_y = y;
		for (i = 0; i < 16; i++) start_rotation[i] = current_rotation[i];
	} else {
		// nothing to do here, as rotation has already been 
		// multiplied into current_rotation
		;
	}

	// redraw.
	glutPostRedisplay();
}

void motion(int x, int y)
{
  // Mouse has been dragged.

  // Update the current rotation matrix.
  glLoadIdentity();
  arcball_rotation(start_x, start_y, x, y);
  glMultMatrixd(start_rotation);
  glGetDoublev(GL_MODELVIEW_MATRIX, current_rotation);

  // redraw.
  glutPostRedisplay();
}


void init(void)
{
	// Use depth buffering for hidden surface elimination.
	glEnable(GL_DEPTH_TEST);

	// Enable a single OpenGL light.
	// White diffuse light.
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	// Put the light at infinity in the direction (1,1,1)
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// Turn on the light and enable lighting.
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	// Set up a perspective view, with square aspect ratio
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 50 degree fov, uniform aspect ratio, near = 1, far = 10
	gluPerspective(20.0,
								 1.0,
								 0.1, 100.0);

	// Initialize rotation of the cube
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, current_rotation);
	glEnable(GL_NORMALIZE);
}

void reshape(int w, int h)
{
	// store window dimensions (needed to compute rotations)
	window_width = w;
	window_height = h;

	// Always use the largest square viewport possible
	if (w > h) {
		glViewport((w - h) / 2, 0, h, h);
	} else {
		glViewport(0, (h - w) / 2, w, w);
	}
}

void display(void)
{
	GLfloat red[] = {1.0, 0.0, 0.0,2.0};
	GLfloat green[] = {0.0, 1.0, 0.0, 1.0};

	// Clear the back buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Make the cube red
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);

	// Set up viewing transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// look from (0,0,5) to (0,0,0) with up vector of (0,1,0)
	gluLookAt(0.0, 0.0, 5.0,
						0.0, 0.0, 0.0,
						0.0, 1.0, 0.0);

	// apply the current rotation
	glMultMatrixd(current_rotation);

	 
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	//glPolygonMode(GL_BACK,GL_LINE);
	glBegin(GL_TRIANGLES);
	//std::vector<Point3d> Vertices;//存放点的集合
	//std::vector<Vector> VertexNormals;//存放点法向的集合，与点集一一对应
	if(flag==0)
	{
		//printf("mesh.Faces.size:%d\n",mesh.Faces.size());
		for(int i=0;i<mesh.Faces.size();++i)
		{
			glNormal3f(mesh.VertexNormals[mesh.Faces[i].P0Index].X,
						mesh.VertexNormals[mesh.Faces[i].P0Index].Y,
						mesh.VertexNormals[mesh.Faces[i].P0Index].Z);
			glVertex3f(mesh.Vertices[mesh.Faces[i].P0Index].X*times,
						mesh.Vertices[mesh.Faces[i].P0Index].Y*times,
						mesh.Vertices[mesh.Faces[i].P0Index].Z*times);

			glNormal3f(mesh.VertexNormals[mesh.Faces[i].P1Index].X,
						mesh.VertexNormals[mesh.Faces[i].P1Index].Y,
						mesh.VertexNormals[mesh.Faces[i].P1Index].Z);
			glVertex3f(mesh.Vertices[mesh.Faces[i].P1Index].X*times,
						mesh.Vertices[mesh.Faces[i].P1Index].Y*times,
						mesh.Vertices[mesh.Faces[i].P1Index].Z*times);
			
			glNormal3f(mesh.VertexNormals[mesh.Faces[i].P2Index].X,
						mesh.VertexNormals[mesh.Faces[i].P2Index].Y,
						mesh.VertexNormals[mesh.Faces[i].P2Index].Z);
			glVertex3f(mesh.Vertices[mesh.Faces[i].P2Index].X*times,
						mesh.Vertices[mesh.Faces[i].P2Index].Y*times,
						mesh.Vertices[mesh.Faces[i].P2Index].Z*times);
		}
	}else
	{
		
		for(int i=0;i<mesh.Faces.size();++i)
		{
			glNormal3f(mesh.VertexNormals[mesh.Faces[i].P0Index].X,
						mesh.VertexNormals[mesh.Faces[i].P0Index].Y,
						mesh.VertexNormals[mesh.Faces[i].P0Index].Z);
			glVertex3f(mesh.Vertices[mesh.Faces[i].P0Index].X*times,
						mesh.Vertices[mesh.Faces[i].P0Index].Y*times,
						mesh.Vertices[mesh.Faces[i].P0Index].Z*times);

			glNormal3f(mesh.VertexNormals[mesh.Faces[i].P1Index].X,
						mesh.VertexNormals[mesh.Faces[i].P1Index].Y,
						mesh.VertexNormals[mesh.Faces[i].P1Index].Z);
			glVertex3f(mesh.Vertices[mesh.Faces[i].P1Index].X*times,
						mesh.Vertices[mesh.Faces[i].P1Index].Y*times,
						mesh.Vertices[mesh.Faces[i].P1Index].Z*times);
			
			glNormal3f(mesh.VertexNormals[mesh.Faces[i].P2Index].X,
						mesh.VertexNormals[mesh.Faces[i].P2Index].Y,
						mesh.VertexNormals[mesh.Faces[i].P2Index].Z);
			glVertex3f(mesh.Vertices[mesh.Faces[i].P2Index].X*times,
						mesh.Vertices[mesh.Faces[i].P2Index].Y*times,
						mesh.Vertices[mesh.Faces[i].P2Index].Z*times);
		}
	}


	glEnd();


	// Swap front and back buffers
	glutSwapBuffers();
}

void processMenuEvents(int option)
{

	//option，就是传递过来的value的值。
    switch(option)
    {
    	case 0:
    		flag=0;
    		glutPostRedisplay();
    		break;
    	
    	case 1:
    		flag=1;
    		CalculateDLRS(mesh,1);
    		glutPostRedisplay();
    		break;
    	case 3:
    		times*=2.0;
    		glutPostRedisplay();
    		break;
    	case 4:
    		times/=2.0;
    		glutPostRedisplay();
    		break;
    	case 5:
    		flag=5;
    		LaplacianSmooth(mesh,1);
    		glutPostRedisplay();
    		break;
    	default:
    		break;
    }
    

}

void createGLUTMenus()
{
	int menu;
 
    // 创建菜单并告诉GLUT，processMenuEvents处理菜单事件。
    menu = glutCreateMenu(processMenuEvents);
         
    //给菜单增加条目
    glutAddMenuEntry("orignal",0);
    glutAddMenuEntry("DLRS",1);
    glutAddMenuEntry("LaplacianSmooth",5);
    glutAddMenuEntry("BIG",3);
    glutAddMenuEntry("SMALL",4);
    
    
    // 把菜单和鼠标右键关联起来。
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}





int main(int argc,char* argv[])
{
	if(argc<2)
	{
		cout<<"input error"<<endl;
		exit(1);
	}
	
	
	mesh.MeshLoadOBJ(argv[1]);
	mesh.CaculateVertexNormals();
	//CalculateDLRS(mesh,new_mesh,1);

/////////////shading////////////////////
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("test");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	

	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	createGLUTMenus();
	
	glutMainLoop();
	return 0;

	// CalculateDLRS(mesh,new_mesh,10);
	// //Output(mesh,argv[2]);

}
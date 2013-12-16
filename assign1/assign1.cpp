// assign1.cpp : Defines the entry point for the console application.
//

/*
CSCI 480 Computer Graphics
Assignment 1: Height Fields
C++ starter code
*/

#include "stdafx.h"
#include <pic.h>
#include <windows.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/gl.h>
int g_iMenuId;
//variables to count screenshoot times.
int counter = 170,n = 0;
//screenshot name.
char buffer[50];
int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROLSTATE;

CONTROLSTATE g_ControlState = ROTATE;

/* state of the world */
float g_vLandRotate[3] = {0.0, 0.0, 0.0};
float g_vLandTranslate[3] = {0.0, 0.0, 0.0};
float g_vLandScale[3] = {1.0, 1.0, 1.0};

/* see <your pic directory>/pic.h for type Pic */
Pic * g_pHeightData;

/* Write a screenshot to the specified filename */
void saveScreenshot (char *filename)
{
	int i, j;
	Pic *in = NULL;

	if (filename == NULL)
		return;

	/* Allocate a picture buffer */
	in = pic_alloc(640, 480, 3, NULL);

	printf("File to save to: %s\n", filename);

	for (i=479; i>=0; i--) {
		glReadPixels(0, 479-i, 640, 1, GL_RGB, GL_UNSIGNED_BYTE,
			&in->pix[i*in->nx*in->bpp]);
	}

	if (jpeg_write(filename, in))
		printf("File saved Successfully\n");
	else
		printf("Error in Saving\n");

	pic_free(in);
}

void myinit()
{
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* clear buffer */
	//enable z-buffer here
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	
	
	//rotate, scale and translate
	glRotatef(g_vLandRotate[0], 1.0, 0.0,0.0);
	glRotatef(g_vLandRotate[1], 0.0, 1.0,0.0);
	glRotatef(g_vLandRotate[2], 0.0, 0.0,1.0);
	glScalef(g_vLandScale[0],g_vLandScale[1],g_vLandScale[2]);
	glTranslatef(g_vLandTranslate[0],g_vLandTranslate[1],g_vLandTranslate[2]);
	//use triangles to render the objects
	glScalef(10.7,10.7,1);
	glTranslatef(-0.5,-0.5,0);
	//comment this to get the frame wire height field
	
	glPolygonOffset(1,1);
	glEnable(GL_POLYGON_OFFSET_FILL);
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < g_pHeightData->nx-1; i ++){
		for(int j = 0; j < g_pHeightData->ny-1; j ++){
			glColor3f(1,0,0);
		glColor3f(PIC_PIXEL(g_pHeightData, (GLint)(i),(GLint)(j), 0)/255.0,
			 PIC_PIXEL(g_pHeightData,(GLint)(i), (GLint)(j), 1)/255.0,
			 PIC_PIXEL(g_pHeightData,(GLint)(i), (GLint)(j), 2)/255.0);

		  glVertex3f((GLfloat)(i/(g_pHeightData->nx * 10.0/10.0)),(GLfloat)(j/(g_pHeightData->ny * 10.0 / 10.0))
			  ,(GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)(j), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint)i, (GLint)j, 1)+
			  PIC_PIXEL(g_pHeightData,(GLint)i, (GLint)j, 2))/255.0/3.0) );

		glColor3f(PIC_PIXEL(g_pHeightData, (GLint)((i+1)), (GLint)j, 0)/255.0,
			 PIC_PIXEL(g_pHeightData,(GLint)((i+1)),(GLint)(j), 1)/255.0,
			 PIC_PIXEL(g_pHeightData,(GLint)((i+1)), 
			 (GLint)(j), 2)/255.0);

		  glVertex3f((GLfloat)((i+1)/(g_pHeightData->nx * 10.0/10.0)),(GLfloat)(j/(g_pHeightData->ny * 10.0 / 10.0))
			  ,(GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 2))/255.0/3.0) );

		glColor3f(PIC_PIXEL(g_pHeightData,(GLint)(i),(GLint)((j+1)), 0)/255.0,
			PIC_PIXEL(g_pHeightData,(GLint)(i), (GLint)(j+1), 1)/255.0,
			PIC_PIXEL(g_pHeightData,(GLint)(i), (GLint)(j+1), 2)/255.0);

		  glVertex3f((GLfloat)(i/(g_pHeightData->nx * 10.0/10.0)),
			  (GLfloat)((j+1)/(g_pHeightData->ny * 10.0 / 10.0)),
			  (GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 2))/255.0/3.0) );


		 
		  glColor3f(PIC_PIXEL(g_pHeightData, (GLint)(i+1),(GLint)(j+1), 0)/255.0,
			 PIC_PIXEL(g_pHeightData,(GLint)(i+1), (GLint)(j+1), 1)/255.0,
			 PIC_PIXEL(g_pHeightData,(GLint)(i+1), (GLint)(j+1), 2)/255.0);

		  glVertex3f((GLfloat)((i+1)/(g_pHeightData->nx * 10.0/10.0)),
			  (GLfloat)((j+1)/(g_pHeightData->ny * 10.0 / 10.0)),
			  (GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) (i+1), (GLint)(j+1), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint)(i+1), (GLint)(j+1), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint)(i+1), (GLint)(j+1), 2))/255.0/3.0) );

		 glColor3f(PIC_PIXEL(g_pHeightData, (GLint)((i+1)), (GLint)j, 0)/255.0,
			 PIC_PIXEL(g_pHeightData,(GLint)((i+1)),(GLint)(j), 1)/255.0,
		 PIC_PIXEL(g_pHeightData,(GLint)((i+1)), 
			 (GLint)(j), 2)/255.0);

		  glVertex3f((GLfloat)((i+1)/(g_pHeightData->nx * 10.0/10.0)),
			  (GLfloat)(j/(g_pHeightData->ny * 10.0 / 10.0)),
			  (GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 2))/255.0/3.0) );

		glColor3f(PIC_PIXEL(g_pHeightData,(GLint)(i),(GLint)((j+1)), 0)/255.0,
			PIC_PIXEL(g_pHeightData,(GLint)(i), (GLint)(j+1), 1)/255.0,
			PIC_PIXEL(g_pHeightData,(GLint)(i), (GLint)(j+1), 2)/255.0);

		  glVertex3f((GLfloat)(i/(g_pHeightData->nx * 10.0/10.0)),
			  (GLfloat)((j+1)/(g_pHeightData->ny * 10.0 / 10.0)),
			  (GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 2))/255.0/3.0) );
	}
	  }


	glEnd();//finishing render objects
	
	glDisable(GL_POLYGON_OFFSET_FILL);
	
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < g_pHeightData->nx-1; i ++){
		for(int j = 0; j < g_pHeightData->ny-1; j ++){
			glColor3f(0,1,1);
		

		  glVertex3f((GLfloat)(i/(g_pHeightData->nx * 10.0/10.0)),(GLfloat)(j/(g_pHeightData->ny * 10.0 / 10.0))
			  ,(GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)(j), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint)i, (GLint)j, 1)+
			  PIC_PIXEL(g_pHeightData,(GLint)i, (GLint)j, 2))/255.0/3.0) );

		

		  glVertex3f((GLfloat)((i+1)/(g_pHeightData->nx * 10.0/10.0)),(GLfloat)(j/(g_pHeightData->ny * 10.0 / 10.0))
			  ,(GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 2))/255.0/3.0) );

		

		  glVertex3f((GLfloat)(i/(g_pHeightData->nx * 10.0/10.0)),
			  (GLfloat)((j+1)/(g_pHeightData->ny * 10.0 / 10.0)),
			  (GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 2))/255.0/3.0) );


		 
		 

		  glVertex3f((GLfloat)((i+1)/(g_pHeightData->nx * 10.0/10.0)),
			  (GLfloat)((j+1)/(g_pHeightData->ny * 10.0 / 10.0)),
			  (GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) (i+1), (GLint)(j+1), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint)(i+1), (GLint)(j+1), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint)(i+1), (GLint)(j+1), 2))/255.0/3.0) );

	

		  glVertex3f((GLfloat)((i+1)/(g_pHeightData->nx * 10.0/10.0)),
			  (GLfloat)(j/(g_pHeightData->ny * 10.0 / 10.0)),
			  (GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint) ((i+1)), (GLint)(j), 2))/255.0/3.0) );

		

		  glVertex3f((GLfloat)(i/(g_pHeightData->nx * 10.0/10.0)),
			  (GLfloat)((j+1)/(g_pHeightData->ny * 10.0 / 10.0)),
			  (GLfloat)((PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 0)
			  +PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 1)+
			  PIC_PIXEL(g_pHeightData,(GLint) (i), (GLint)((j+1)), 2))/255.0/3.0) );
	}
	  }


	glEnd();//finishing render objects
	glutSwapBuffers();//swap buffers 
	//use sprintf to manipulate the name of screenshot images.
	n = sprintf (buffer, "%d .jpeg", counter);
	counter = counter+1;
	//saveScreenshot((char*)buffer);
	
}

void menufunc(int value)
{
	switch (value)
	{
	case 0:
		exit(0);
		break;
	}
}

void doIdle()
{
	/* do some stuff... */

	/* make the screen update */
	glutPostRedisplay();
}

/* converts mouse drags into information about 
rotation/translation/scaling */
void mousedrag(int x, int y)
{
	int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};

	switch (g_ControlState)
	{
	case TRANSLATE:  
		if (g_iLeftMouseButton)
		{
			g_vLandTranslate[0] += vMouseDelta[0]*0.01;
			g_vLandTranslate[1] -= vMouseDelta[1]*0.01;
		}
		if (g_iMiddleMouseButton)
		{
			g_vLandTranslate[2] += vMouseDelta[1]*0.01;
		}
		break;
	case ROTATE:
		if (g_iLeftMouseButton)
		{
			g_vLandRotate[0] += vMouseDelta[1];
			g_vLandRotate[1] += vMouseDelta[0];
		}

		if (g_iMiddleMouseButton)
		{
			g_vLandRotate[2] += vMouseDelta[1];
		}
		break;
	case SCALE:
		if (g_iLeftMouseButton)
		{
			g_vLandScale[0] *= 1.0+vMouseDelta[0]*0.01;
			g_vLandScale[1] *= 1.0-vMouseDelta[1]*0.01;
		}
		if (g_iMiddleMouseButton)
		{
			g_vLandScale[2] *= 1.0-vMouseDelta[1]*0.01;
		}
		break;
	}
	g_vMousePos[0] = x;
	g_vMousePos[1] = y;
}

void mouseidle(int x, int y)
{
	g_vMousePos[0] = x;
	g_vMousePos[1] = y;
}

void mousebutton(int button, int state, int x, int y)
{

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		g_iLeftMouseButton = (state==GLUT_DOWN);
		break;
	case GLUT_MIDDLE_BUTTON:
		g_iMiddleMouseButton = (state==GLUT_DOWN);
		break;
	case GLUT_RIGHT_BUTTON:
		g_iRightMouseButton = (state==GLUT_DOWN);
		break;
	}

	switch(glutGetModifiers())
	{
	case GLUT_ACTIVE_CTRL:
		g_ControlState = TRANSLATE;
		break;
	case GLUT_ACTIVE_SHIFT:
		g_ControlState = SCALE;
		break;
	default:
		g_ControlState = ROTATE;
		break;
	}

	g_vMousePos[0] = x;
	g_vMousePos[1] = y;
}
void myReshape(int w, int h)
{
GLfloat aspect = (GLfloat) w / (GLfloat) h;
glViewport(0, 0, w, h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
if (w <= h) /* aspect <= 1 */
glOrtho(-2.0, 2.0, -2.0/aspect, 2.0/aspect, -10.0, 10.0);
else /* aspect > 1 */
glOrtho(-2.0*aspect, 2.0*aspect, -2.0, 2.0, -10.0, 10.0);
glMatrixMode(GL_MODELVIEW);
}
int _tmain(int argc, _TCHAR* argv[])
{
	// I've set the argv[1] to spiral.jpg.
	// To change it, on the "Solution Explorer",
	// right click "assign1", choose "Properties",
	// go to "Configuration Properties", click "Debugging",
	// then type your texture name for the "Command Arguments"
	if (argc<2)
	{  
		printf ("usage: %s heightfield.jpg\n", argv[0]);
		exit(1);
	}

	g_pHeightData = jpeg_read((char*)argv[1], NULL);
	if (!g_pHeightData)
	{
		printf ("error reading %s.\n", argv[1]);
		exit(1);
	}
	glMatrixMode (GL_MODELVIEW);
	glutInit(&argc,(char**)argv);
	glMatrixMode (GL_MODELVIEW);
	/*
	create a window here..should be double buffered and use depth testing

	the code past here will segfault if you don't have a window set up....
	replace the exit once you add those calls.
	*/

	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (640, 480);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	//exit(0);
	glutReshapeFunc(myReshape);
	/* tells glut to use a particular display function to redraw */
	glutDisplayFunc(display);

	/* allow the user to quit using the right mouse button menu */
	g_iMenuId = glutCreateMenu(menufunc);
	glutSetMenu(g_iMenuId);
	glutAddMenuEntry("Quit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* replace with any animate code */
	glutIdleFunc(doIdle);

	/* callback for mouse drags */
	glutMotionFunc(mousedrag);
	/* callback for idle mouse movement */
	glutPassiveMotionFunc(mouseidle);
	/* callback for mouse button changes */
	glutMouseFunc(mousebutton);

	
	


	/* do initialization */
	myinit();

	glutMainLoop();
	return 0;
}
#include <GL/glut.h>									//GLUT library
#include "UniformGrid.h"
#include "ColormapRenderer.h"
#include <iostream>										
#include <math.h>										//For expf()



float Xmin	= -5, Xmax	= 5;							//Range of variable x that we sample
float Ymin	= -5, Ymax	= 5;							//Range of variable y that we sample
int   Nx	= 30, Ny	= 20;							//Number of samples taken along the x and y axes


float fov	 = 60;										//Perspective projection parameters
float aspect = 1; 
float z_near = 0.1; 
float z_far  = 30;

float eye_x = 10, eye_y = 10, eye_z = 10;				//Modelview (camera extrinsic) parameters
float c_x   = 0,  c_y   = 0,  c_z   = 0;
float up_x  = 0,  up_y  = 0,  up_z  = 1;


float f(float x, float y)								//A simple two-variable function to plot 
{														//The function is samples 
	float ret = 8*expf(-(x*x+y*y)/5);
	return ret;
}


UniformGrid			grid(Nx,Ny,Xmin,Ymin,Xmax,Ymax);
ColormapRenderer	renderer;
int					drawing_style = 0;
int					colormap      = 0;


void viewing(int W, int H)								//Window resize function, sets up viewing parameters (GLUT callback function)
{
	glMatrixMode(GL_MODELVIEW);							//1. Set the modelview matrix (including the camera position and view direction)
	glLoadIdentity ();										
	gluLookAt(eye_x,eye_y,eye_z,c_x,c_y,c_z,up_x,up_y,up_z);

	glMatrixMode (GL_PROJECTION);						//2. Set the projection matrix
	glLoadIdentity ();
	gluPerspective(fov,float(W)/H,z_near,z_far);

	glViewport(0,0,W,H);								//3. Set the viewport to the entire size of the rendering window
}



void draw()												//Render the grid (GLUT callback function)
{
	renderer.draw(grid);
}


void keyboard(unsigned char c,int,int)					//Press space to cycle through the renderer's drawing styles and colormaps
{
	switch(c)
	{
	case ' ':
		{
		  drawing_style = (++drawing_style) % 4;		//Ask renderer to use another drawing-style mode
		  renderer.setDrawingStyle((ColormapRenderer::DRAW_STYLE)drawing_style);
		  break;
		}
	case 'c':
		{
		  colormap = (++colormap) % 3;					//Ask renderer to use another colormap
		  renderer.setColormapType((ColormapRenderer::COLORMAP_TYPE)colormap);
		  break;
		}
	}
			
	glutPostRedisplay();    
}	


void createGrid()										//Create scalars for the grid. For this, we simply sample the function f(x,y)
{														//at all the grid points
	for(int i=0;i<grid.numPoints();++i)
	{
	   float p[2];				
	   grid.getPoint(i,p);								//Get coordinates of current grid point
	   
	   float value = f(p[0],p[1]);						//Evaluate f(x,y) at current point
	   
	   grid.pointScalars().setC0Scalar(i,value);		//Store the function value as scalar for the current point
	}   
}


int main(int argc,char* argv[])							//Main program
{
   cout<<"In the graphics window, press"<<endl;
   cout<<"  space: to toggle through various color-mapping modes."<<endl;
   cout<<"  c: to toggle through various color maps."<<endl;

   glutInit(&argc, argv);								//1. Initialize the GLUT toolkit
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);			//2. Ask GLUT to create next windows with a RGB framebuffer and a Z-buffer too
   glutInitWindowSize(500,500);							//3. Tell GLUT how large are the windows we want to create next
   glutCreateWindow("1. Uniform grid (color mapping scalars)");		
														//4. Create our window
   
   createGrid();										//5. Create scalar values for the grid vertices
   
   glutKeyboardFunc(keyboard);   
   glutDisplayFunc(draw);								//6. Add a drawing callback to the window	
   glutReshapeFunc(viewing);							//7. Add a resize callback to the window
   glutMainLoop();										//8. Start the event loop that displays the graph and handles window-resize events
   
   return 0;
}





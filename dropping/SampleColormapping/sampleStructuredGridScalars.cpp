#include <math.h>										//For sin(), cos()
#include <GL/glut.h>									//GLUT library
#include "StructuredGrid.h"
#include "ColormapRenderer.h"
#include <iostream>




float Rmin	= 3,  Rmax	= 12;							//Range of polar radius coordinate that we sample
float amin	= 0, amax	= 350;							//Range of polar angle coordinate that we sample (in degrees)
int   Na	= 100, Nr	= 30;							//Number of samples taken along the angle and radius axes


float fov	 = 60;										//Perspective projection parameters
float aspect = 1; 
float z_near = 0.1; 
float z_far  = 40;

float eye_x = 10, eye_y = 10, eye_z = 20;				//Modelview (camera extrinsic) parameters
float c_x   = 0,  c_y   = 0,  c_z   = 0;
float up_x  = 0,  up_y  = 0,  up_z  = 1;


ColormapRenderer	renderer;
StructuredGrid		grid(Na,Nr);
int					drawing_style = 0;
int					colormap      = 0;

float f(float a, float r)								//A simple two-variable function to plot 
{														//The function is defined in polar coordinates 
	float ret = sin(3*a)*cos(r);
	return ret;
}




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


void createGrid()										//Create a structured grid with scalar values. For illustration purposes, we create
{														//a grid whose vertices sample a disk sector.
	int counter=0;										//Vertex index in the storage-order
	for(int j=0;j<Nr;++j)
	  for(int i=0;i<Na;++i,++counter)
	  {
		float a = amin + i*(amax-amin)/Na;				//Compute the polar coordinates of current vertex	
		float a_rad = a*M_PI/180;						//Convert the angle coordinate from degrees to radians
		float R = Rmin + j*(Rmax-Rmin)/Nr;		
		
		float p[2];
		p[0] = R*cos(a_rad);							//Compute the Cartesian vertex coordinates from its polar coordinates
		p[1] = R*sin(a_rad);
		
		grid.setPoint(counter,p);						//Set vertex coordinates of next vertex in the grid
	    
		float value = f(a_rad,R);						//Evaluate f() at current point

		grid.pointScalars().setC0Scalar(counter,value);	//Store the function value as scalar for the current point
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
   glutCreateWindow("2. Structured grid (color mapping scalars)");	
														//4. Create our window
   
   createGrid();										//5. Create the structured grid and add scalar values to its vertices
   
   glutKeyboardFunc(keyboard);   
   glutDisplayFunc(draw);								//6. Add a drawing callback to the window	
   glutReshapeFunc(viewing);							//7. Add a resize callback to the window
   glutMainLoop();										//8. Start the event loop that displays the graph and handles window-resize events
   
   return 0;
}





//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "Angel.h"
#include <iostream>
using namespace std;
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;


GLuint projection;
GLuint model_view;




const int NumVertices = 36*8; //(6 faces)(2 triangles/face)(3 vertices/triangle)
const int hair = 4;

point4 points[NumVertices];
color4 colors[NumVertices];
point4 crossh[hair] = { point4 ( 2,  0,  0, 10),
					   point4 (-2,  0,  0, 10),
					   point4 ( 0,  2,  0, 10),
					   point4 ( 0, -2,  0, 10) };


float sizex = 10;
float sizey = 10;
float sizez = 10;
// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {   //initial cube with center -10, -10, +10
    point4( -12.5, -12.5,  15, 30.0 ),
    point4( -12.5,  -7.5,  15, 30.0 ),
    point4(  -7.5,  -7.5,  15, 30.0),
    point4(  -7.5, -12.5,  15, 30.0 ),
    point4( -12.5, -12.5,  10, 30.0 ),
    point4( -12.5,  -7.5,  10, 30.0 ),
    point4(  -7.5,  -7.5,  10, 30.0 ),
    point4(  -7.5, -12.5,  10, 30.0 )
};

int stat = 0;
void changevertices() //change vertices in vertices array
{

	switch(stat)
	{
	case 0 : //-10 10 10
		sizex = 0; sizey = 20; sizez = 0; stat++; break;
	case 1 : // 10 10 10
		sizex = 20; sizey = 20; sizez = 0; stat++; break;
	case 2 : // 10 -10 10
		sizex = 20; sizey = 0; sizez = 0; stat++; break;
	case 3 : // 10 -10 10
		sizex = 0; sizey = 0; sizez = -20; stat++; break;
	case 4 : // -10 -10 -10
		sizex = 0; sizey = 20; sizez = -20; stat++; break;
	case 5 : // 10 10 -10
		sizex = 20; sizey = 20; sizez = -20; stat++; break;
	case 6 : // 10 -10 -10
		sizex = 20; sizey = 0; sizez = -20; stat++; break;

	}
	//store each different vertices in vertices array everytime to create each cube
	vertices[0] = point4( -12.5+sizex, -12.5+sizey,  15+sizez, 30.0);
	vertices[1] = point4( -12.5+sizex,  -7.5+sizey,  15+sizez, 30.0);
	vertices[2] = point4(  -7.5+sizex,  -7.5+sizey,  15+sizez, 30.0);
	vertices[3] = point4(  -7.5+sizex, -12.5+sizey,  15+sizez, 30.0);
	vertices[4] = point4( -12.5+sizex,  -12.5+sizey,  10+sizez, 30.0);
	vertices[5] = point4( -12.5+sizex,   -7.5+sizey,  10+sizez, 30.0);
	vertices[6] = point4(  -7.5+sizex,  -7.5+sizey,  10+sizez, 30.0);
	vertices[7] = point4(  -7.5+sizex, -12.5+sizey,  10+sizez, 30.0);

}
// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 0.5, 0.0, 0.5, 1.0 ),  // some color
    color4( 0.0, 1.0, 1.0, 1.0 ),   // cyan
	color4( 0.0, 0.0, 1.0, 1.0 )  // blue
};

color4 cycle_colors[8] = {  //colors to change with command 'c'
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
	color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 0.5, 0.0, 0.5, 1.0 ),  // some color
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
	
};
//----------------------------------------------------------------------------

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = -1;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 }; //angle array to rotate object

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
int cold = 0;
void
quad( int a, int b, int c, int d )   //put vertices in points array and put color in colors array
{
    colors[Index] = vertex_colors[cold]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[cold]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[cold]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[cold]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[cold]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[cold]; points[Index] = vertices[d]; Index++;
}


int cycle = 0;
void cyclecolor() //change colors 
{
	Index = 0;
	for (int i = 0; i < NumVertices; i ++)
	{

		if (i != 0 && (i % 36) == 0)
		{
			cycle++;	
			if (cycle == 8)    //this makes color cycle
			{ 
				cycle = 0;
			}
		}
		colors[Index] = vertex_colors[cycle];
		Index++;
	}
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors ); //draw new cubes with different color
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
	cold++;
	changevertices();
}


//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	for (int i = 0; i < 8; i++) //create 6 cubes
	{
		colorcube();
	}

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) +sizeof(crossh),  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(colors)+sizeof(points), sizeof(crossh), crossh);
	
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader31.glsl", "fshader31.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );

	model_view = glGetUniformLocation(program, "model_view");

	projection = glGetUniformLocation(program, "projection");

    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

float height = 0;
float depth = -2;
float width = 0;
float narrow = 1;
float angle = 60;
float aspect = 1;
void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	mat4 tran = Translate(width,height,depth);  //use translate function for command 'i' 'j' 'k' 'm'
	mat4 scal = Scale(narrow,1,1); //use scale function for command 'n' ,'h'
	

	mat4  transform = ( RotateX( Theta[Xaxis] ) *   //model_view matrix in v shader
			RotateY( Theta[Yaxis] ) *
			RotateZ( Theta[Zaxis] ) ) * scal * tran;
	glUniformMatrix4fv(model_view,1,GL_TRUE,transform);   


	
	mat4 pers = Perspective(angle,aspect,0.1,2000.0);  //perspective projection using vshader
	glUniformMatrix4fv(projection,1,GL_TRUE,pers);

	

    glDrawArrays( GL_TRIANGLES, 0, NumVertices );  //draw cubes
	glDrawArrays( GL_LINES,NumVertices*2,hair);   //draw cross hair
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033:  // Escape key
	case 'q': case 'Q': 
	    exit( EXIT_SUCCESS );
	    break;
	case 'C' : case 'c' :
		cyclecolor(); 
		break;
	case 'i' : case 'I' :
		depth += 0.01;
		break;
	case 'j' : case 'J' :
		width += 0.01;
		break;
	case 'k' : case 'K' :
		width -= 0.01;
		break;
	case 'm' : case 'M' :
		depth -= 0.01;
		break;
	case 'n' : case 'N' :
		narrow -= 0.01;
		break;
	case 'w' : case 'W' :
		narrow += 0.01;
		break;
    }
}

void specialkey(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		height -= 0.01;
		break;
	case GLUT_KEY_DOWN:
		height += 0.01;
		break;
	case GLUT_KEY_LEFT:
		Theta[Yaxis] -= 1;
		break;
	case GLUT_KEY_RIGHT:
		Theta[Yaxis] += 1;
		break;
	case GLUT_KEY_F5:
		Axis = Yaxis;
		break;
	case GLUT_KEY_F6:
		Axis = -1;
		break;
	case GLUT_KEY_F10: //reset the variable 
		Theta[Xaxis] = 0;
		Theta[Yaxis] = 0;
		narrow = 1;
		height = 0;
		width = 0;
		depth = -2;
		break;
	}
	glutPostRedisplay();
}



void
idle( void )
{
    Theta[Axis] += 0.01;    //rotation +0.01 degree

    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }
	glutPostRedisplay();
    
}
//----------------------------------------------------------------------------



int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "Color Cube" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutSpecialFunc(specialkey);
    glutIdleFunc( idle );
    glutMainLoop();
    return 0;
}

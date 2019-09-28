#include<GL/glut.h>
#include<unistd.h>
#include<math.h>
#define pi 3.141
#define DEG2RAD 3.14159/180.0


int stage, i;


void delay(int m)
{
    for(int d=0; d<2000000 * m; d++);
}


void myInit(void) 
{ 
    // making background color black as first  
    glClearColor(0.0, 0.0, 0.0, 1.0); 
      
    // making picture color green (in RGB mode), as middle argument is 1.0 
    glColor3f(1.0, 1.0, 1.0); 
      
    // breadth of picture boundary is 1 pixel 
    glPointSize(1.0); 
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity(); 
      
    // setting window dimension in X- and Y- direction 
    gluOrtho2D(-780, 780, -420, 420); 
} 

void circle(float X, float Y, float radius){

	glBegin(GL_POLYGON); 
	for (float i = 0; i < (2 * pi); i += 0.001) 
	{ 
		float x = radius * cos(i); 
		float y = radius * sin(i); 
		glVertex2i(x + X, y + Y); 
	} 
	glEnd(); 
}

void ellipse(int radiusX, int radiusY,int offsetX, int offsetY){
	int e;

	glBegin(GL_POLYGON);

	for(e=0;e<360;e++)
	{
		float rad = e*DEG2RAD;
		glVertex2f(cos(rad)*radiusX + offsetX,
		sin(rad)*radiusY + offsetY);
	}

	glEnd();
}
 

void puppet(void)  
{ 
	glColor3f(0/255.0, 0/255.0, 0/255.0);
	glClear(GL_COLOR_BUFFER_BIT); 
	for(int k =0; k < 100000; k++){	
		glClear(GL_COLOR_BUFFER_BIT);
		
		float w = glutGet(GLUT_WINDOW_WIDTH);
		float h = glutGet(GLUT_WINDOW_HEIGHT);		
		float x, y, i; 

		// Reference point used to draw rest of the puppet.  			
		float Y = h/3;
		float X = w/2;

		int radiusX = 50;
		int radiusY = 10;

		int r = 50; // Size of the head 
		
		// Strings
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);
	    	glVertex2f(w/2 + 40, 0);
	    	glVertex2f(X + r + 10 + 2* radiusX,  Y + r + 10);
		glEnd();

		glBegin(GL_LINES);
	    	glVertex2f(w/2 - 40, 0);
	    	glVertex2f(X - r - 10 - 2* radiusX,  Y + r + 10);
		glEnd();
		
		glBegin(GL_LINES);
	    	glVertex2f(w/2 + 20, 0);
	    	glVertex2f(X + r + 10 +  radiusX,  Y + r + 10);
		glEnd();

		glBegin(GL_LINES);
	    	glVertex2f(w/2 - 20, 0);
	    	glVertex2f(X - r - 10 -  radiusX,  Y + r + 10);
		glEnd();

		
		//Walls 
		glColor3f(138/255.0, 109/255.0, 81/255.0);
		
		// Right
		glBegin(GL_POLYGON);		
		glVertex2i(w - 40, h - 250); 
		glVertex2i(w, h - 100);
		glVertex2i(w, 0);
		glVertex2i(w - 40, 0); 
		glEnd();
			
		// Left
		glBegin(GL_POLYGON);		
		glVertex2i(40, h - 250); 
		glVertex2i(0, h - 100);
		glVertex2i(0, 0);
		glVertex2i(40, 0); 
		glEnd();
			
			
		
		// Curtains 
		glColor3f(232/255.0, 60/255.0, 60/255.0);
		circle(w/2, -400, 500);
		glColor3f(189/255.0, 34/255.0, 34/255.0);
		circle(w/2 + 400, -400, 500);
		circle(w/2 - 400, -400, 500);
		glColor3f(232/255.0, 60/255.0, 60/255.0);
		circle(-350, 0, 500);
		circle(w + 350, 0, 500);

		
		//Floor 
		glColor3f(94/255.0, 64/255.0, 37/255.0);
		glBegin(GL_POLYGON);
		glVertex2i( 40, h - 250); 
		glVertex2i( w - 40,  h - 250);
		glVertex2i(w, h - 100);
		glVertex2i( 0 , h - 100);
		glEnd(); 
		
		glColor3f(50/255.0, 20/255.0, 0/255.0);
		glBegin(GL_POLYGON);
		glVertex2i(w, h - 100);
		glVertex2i( 0 , h - 100);
		glVertex2i( 0 , h - 80);
		glVertex2i( w , h - 80);
		glEnd(); 

		// Ground (Black)
		glColor3f(0/255.0, 0/255.0, 0/255.0);
		glBegin(GL_POLYGON);
		glVertex2i( 0 , h - 80);
		glVertex2i( w , h - 80);
		glVertex2i( w , h );
		glVertex2i( 0 , h );
		
		glEnd(); 


		glColor3f(1.0, 1.0, 1.0);
		// Head of the puppet
		glColor3f(209/255.0, 175/255.0, 90/255.0);
		circle(X, Y, r);

	
		int e;
		
		// Arms 
		
		glColor3f(209/255.0, 175/255.0, 90/255.0);
		ellipse(radiusX, radiusY, X - r - 10 - 1.5*(radiusX),  Y + r + 10);
		ellipse(radiusX, radiusY, X + r + 10 + 1.5*(radiusX),  Y + r + 10);
		glColor3f(255/255.0, 123/255.0, 0/255.0);
		ellipse(radiusX, radiusY, X + r + 10 + radiusX/2,  Y + r + 10);
		ellipse(radiusX, radiusY, X - r - 10 - radiusX/2,  Y + r + 10);
		
		// Legs
		glColor3f(209/255.0, 175/255.0, 90/255.0);
		ellipse(radiusY, radiusX, X + r - 30,  Y + r + 150 + (1.5*radiusX));
		ellipse(radiusY, radiusX, X - r + 30,  Y + r + 150 + (1.5*radiusX));

		glColor3f(68/255.0, 99/255.0, 173/255.0);
		ellipse(radiusY, radiusX, X + r - 30,  Y + r + 150 + radiusX/2);
		ellipse(radiusY, radiusX, X - r + 30,  Y + r + 150 + radiusX/2);
		
		// Torso of the puppet 
		glColor3f(255/255.0, 123/255.0, 28/255.0);
		glBegin(GL_POLYGON);
		glVertex2i(X - r - 10, Y + r); 
		glVertex2i(X + r + 10, Y + r);
		glVertex2i(X + r - 15, Y + r + 150);
		glVertex2i(X - r + 15, Y + r + 150);
		glEnd(); 
	

		//Curtains
		float curtainLeft = w/2;
		float curtainRight = w/2;

 		for(int i = 0; i < w/2; i ++){
			glColor3f(232/255.0, 60/255.0, 60/255.0);
			glClear(GL_COLOR_BUFFER_BIT);
			// glLoadIdentity();
			delay(1);
			

			glBegin(GL_POLYGON);
			glVertex2i(0, 0); 
			glVertex2i(curtainLeft - i, 0);
			glVertex2i(curtainLeft - i, h -120);
			glVertex2i(0, h -120);
			glEnd(); 

			// Right
			glBegin(GL_POLYGON);
			glVertex2i(curtainRight +  i, 0); 
			glVertex2i(w, 0);
			glVertex2i(w,h-120);
			glVertex2i(curtainRight + i, h-120);
			glEnd(); 
		
			glutSwapBuffers();
		
			//Audience
			glColor3f(40/255.0, 40/255.0, 40/255.0);
			ellipse(50, 25, w/2, h-40);
			circle(w/2, h-90, 35);

			ellipse(60, 30, w/2 + 390,  h-40);
			circle(w/2+ 390, h-100, 50);

			ellipse(60, 30, w/2 - 320, h-40);
			circle(w/2 - 320, h-100, 50);

			ellipse(60, 30, w/2 + 160, h-40);
			circle(w/2 + 160, h-100, 50);

			ellipse(60 , 30, w/2 - 130, h-40);
			circle(w/2 - 130, h-100, 50);

			ellipse(50, 25, w/2 - 500, h-40);
			circle(w/2 - 500, h-90, 35);
		}


			


		glFlush(); 
	
	}
	
} 

void drawBitmapText(char *string,float x,float y,float z,void* style) 
{  
	char *c;
	glRasterPos3f(x, y-100,z);
    

	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(style, *c);		
	}
}




void drawStrokeText(char* string,int x,int y,int z)
{
	  char *c;
	  glPushMatrix();
	  glTranslatef(x,y,z);
	  glScalef(.28f,-0.7f,z);	
	
          glPointSize(3.0);
	  for (c=string; *c != '\0'; c++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN , *c);
	  }
	  glPopMatrix();
}

void init()
{
	glClearColor(1.0,1.0,0.0,0.0); 
}


void reshape(int w,int h) 
{ 
 
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluOrtho2D(0,w,h,0); 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 

}



void keyPressed (unsigned char key, int x, int y) { 
	if(key == 'a'){
		stage = 2;
	}	 
}  




	
void welcome(void){

	float w = glutGet(GLUT_WINDOW_WIDTH);
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	
	glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity();
		

	glColor3f(1,1, 1);
    
	void* font =  GLUT_BITMAP_HELVETICA_18 ;
	drawBitmapText("Pawan Bhandarkar",w/5,h*2/5,0 ,font);
	drawBitmapText("4NM16CS090",w*3/5,h*2/5,0,font);
	drawBitmapText("Pavan N",w/5,h/2,0,font);
	drawBitmapText("4NM16CS091",w*3/5,h/2,0,font);
	drawBitmapText("Pavithra B",w/5,h*3/5,0,font);
	drawBitmapText("4NM16CS092",w*3/5,h*3/5,0,font);
	drawStrokeText("A Puppet Show",w/2-180,h/5*3.5,0);
	glutSwapBuffers();
}



void render(void)
{	 	
	i = 0;
 	float w = glutGet(GLUT_WINDOW_WIDTH);
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	

	while( i < h){
		i++;
		delay(1);
		glClear(GL_COLOR_BUFFER_BIT); 
		glLoadIdentity();
			

		glColor3f(1,1, 1);
	    
		void* font =  GLUT_BITMAP_HELVETICA_18 ;
		drawBitmapText("Pawan Bhandarkar",w/5,h*2/5 - i,0 ,font);
		drawBitmapText("4NM16CS090",w*3/5,h*2/5 - i,0,font);
		drawBitmapText("Pavan N",w/5,h/2-i,0,font);
		drawBitmapText("4NM16CS091",w*3/5,h/2- i,0,font);
		drawBitmapText("Pavithra B",w/5,h*3/5- i,0,font);
		drawBitmapText("4NM16CS092",w*3/5,h*3/5- i,0,font);
		drawStrokeText("A Puppet Show",w/2-180,h/5*3.5- i,0);
		glutSwapBuffers(); 
		
	}	
} 


void display(void){
	//welcome();
	//delay(200);
	//render();
	puppet();

}


int main(int argc, char* argv[])
{ 
	stage = 1;
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 

	// giving window size in X- and Y- direction 
	glutInitWindowSize(1366, 768); 
	glutInitWindowPosition(0, 0); 

	// Giving name to window 
        glutCreateWindow("Puppet Show"); 
	myInit(); 

  	glutDisplayFunc(display);
	glutIdleFunc(display);
        glutReshapeFunc(reshape); 
	glutKeyboardFunc(keyPressed);
	glutMainLoop(); 

	return 0;
}

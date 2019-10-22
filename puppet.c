#include<GL/glut.h>
#include<unistd.h>
#include<math.h>
#include <iostream>
#define PI 3.141
#define DEG2RAD 3.14159/180.0

#define RIGHT_ARM 0
#define LEFT_ARM 1
#define RIGHT_LEG 2
#define LEFT_LEG 3


int stage, i;
float w;   // Stores window width
float h;   // Stores window height
int k;     // global parameter that decides the display  
int c;	   // Controls the curtain movement
int t;     // Used to record time of button click 

int open = 0;  // Whether the cutain is being closed or opened 

float ra, la, rl, ll;  // Right Arm, Left Arm, Right Leg and Left Leg angles 



float max( float a, float b){
	return a>b ? a : b;

}

float min( float a, float b){
	return a<b ? a : b;

}



float slope(float x0, float y0, float x1, float y1){

	return (y1 - y0)/(x1- x0);

}


float distance(float x0, float y0, float x1, float y1){

	return sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
}

int closest_limb(float x, float y){


	// RIGHT ARM (745, 320) -> (870, 320)
	// LEFT ARM (625, 320) -> (500, 320)
	// RIGHT LEG (705, 450) -> (705, 590)
	// LEFT LEG (665, 450) -> (665, 590)

	float right_arm = distance(x,y, 745, 320);
	float left_arm = distance(x,y, 625, 320);
	float right_leg = distance(x,y,705, 450);
	float left_leg = distance(x,y, 665, 450);

	float closest = min(min(min(right_arm, left_arm), right_leg), left_leg);
	
	if(closest == right_arm){
		float m =  slope(745, 320, x,y);
		float angle  = atan(m)* 180 / PI;
		printf("Slope =  %f. Angle = %f. ", m , angle);
		ra = angle;
		return RIGHT_ARM;	
	}
	else if(closest == left_arm){
		float m = slope(625, 320, x,y);
		float angle  = atan(m)* 180 / PI;
		la = -angle;
		printf("Slope =  %f. Angle = %f. ", m , angle);
		return LEFT_ARM;	
	}
	else if(closest == right_leg){
		float m = slope(705, 520, x,y);
		float angle  = atan(m)* 180 / PI;
		rl = angle;
		printf("Slope =  %f. Angle = %f. ", m , angle);
		return RIGHT_LEG;	
	}
	else{
		float m = slope(665, 520, x,y);
		float angle  = atan(m)* 180 / PI;
		ll = -angle;
		printf("Slope =  %f. Angle = %f. ", m , angle);
		return LEFT_LEG;	
	}
}


int track;

void track_mouse(){
	
	//TODO : Use popen command to get real time mouse positions
	// watch -ptn 0 "xdotool getmouselocation"
	
}

void mouse(int mouse, int state, int x, int y){
    switch(mouse){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){
		track = 1;
		int limb = closest_limb(x,y);
		printf("Clicked (%d, %d). Closest limb: %d\n ", x, y, limb);
                glutPostRedisplay();
            }
	    else{
		printf("Mouse released\n ");
		track = 0;
		}

        break;

    }

}




void delay(int m)
{
    for(int d=0; d<2000000 * m; d++);
}


void myInit(void) 
{ 
    k = 0;
    ra = la = rl = ll = 0;

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
	for (float i = 0; i < (2 * PI); i += 0.001) 
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


void rotate(float x, float y, float angle){

	glTranslatef(x,y,0.0); // 3. Translate to the object's position.
	glRotatef(angle,0.0,0.0,1.0); // 2. Rotate the object.
	glTranslatef(-x,-y,0.0); // 1. Translate to the origin.
}
 

void puppet(void)  
{ 
	glColor3f(0/255.0, 0/255.0, 0/255.0);
	glClear(GL_COLOR_BUFFER_BIT); 

	float x, y, i; 

	// Reference point used to draw rest of the puppet.  			
	float Y = h/3;
	float X = w/2;

	int radiusX = 50;
	int radiusY = 10;

	int r = 50; // Size of the head 

	// Floor  =====================================================================

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

	
	// Strings  =====================================================================
	glColor3f(1, 1, 1);

	// Right Arm
	rotate(X + r + 10 , Y + r + 10, ra);
	glBegin(GL_LINES);
    	glVertex2f(w/2 + 160, -200);
    	glVertex2f(X + r + 10 + 2* radiusX,  Y + r + 10);
	glEnd();
	rotate(X + r + 10 , Y + r + 10, -ra);

	// Left Arm 
	rotate(X - r - 10 , Y + r + 10, -la);
	glBegin(GL_LINES);
    	glVertex2f(w/2 - 160, -200);
    	glVertex2f(X - r - 10 - 2* radiusX,  Y + r + 10);
	glEnd();
	rotate(X - r - 10 , Y + r + 10, la);
	
	// Right Leg
	rotate(X + r - 30 , Y + r + 150, 0.5* rl);
	glBegin(GL_LINES);
    	glVertex2f(w/2 + 20, 0);
    	glVertex2f(w/2 + 20,  Y + r + 150 );
	glEnd();
	rotate(X + r - 30 , Y + r + 150, -0.5*rl);
	
	// Left Leg
	rotate(X - r + 30 , Y + r + 150, 0.5* -ll);
	glBegin(GL_LINES);
    	glVertex2f(w/2 - 20, 0);
    	glVertex2f(w/2 - 20,  Y + r + 150 );
	glEnd();
	rotate(X - r + 30 , Y + r + 150, 0.5*ll);

	
	//Walls =====================================================================
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
		
		
	
	// Curtains  =====================================================================
	glColor3f(232/255.0, 60/255.0, 60/255.0);
	circle(w/2, -400, 500);
	glColor3f(189/255.0, 34/255.0, 34/255.0);
	circle(w/2 + 400, -400, 500);
	circle(w/2 - 400, -400, 500);
	glColor3f(232/255.0, 60/255.0, 60/255.0);
	circle(-350, 0, 500);
	circle(w + 350, 0, 500);

	
	// Ground  =======================================================================
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2i( 0 , h - 80);
	glVertex2i( w , h - 80);
	glVertex2i( w , h );
	glVertex2i( 0 , h );
	glEnd(); 


	// Head  =====================================================================
	glColor3f(209/255.0, 175/255.0, 90/255.0);
	circle(X, Y, r);


	int e;
	// Arms  =====================================================================

	// Right  
	rotate(X - r - 10 , Y + r + 10, -la);
	glColor3f(209/255.0, 175/255.0, 90/255.0);
	ellipse(radiusX, radiusY, X - r - 10 - 1.5*(radiusX),  Y + r + 10);
	glColor3f(255/255.0, 123/255.0, 0/255.0);
	ellipse(radiusX, radiusY, X - r - 10 - radiusX/2,  Y + r + 10);
	rotate(X - r - 10 , Y + r + 10, la);
	
	// Left
	rotate(X + r + 10 , Y + r + 10, ra);
	glColor3f(209/255.0, 175/255.0, 90/255.0);
	ellipse(radiusX, radiusY, X + r + 10 + 1.5*(radiusX),  Y + r + 10);
	glColor3f(255/255.0, 123/255.0, 0/255.0);
	ellipse(radiusX, radiusY, X + r + 10 + radiusX/2,  Y + r + 10);
	rotate(X + r + 10 , Y + r + 10, -ra);


	// Legs =====================================================================

	// Right
	rotate(X + r - 30 , Y + r + 150, rl);
	glColor3f(209/255.0, 175/255.0, 90/255.0);
	ellipse(radiusY, radiusX, X + r - 30,  Y + r + 150 + (1.5*radiusX));
	glColor3f(68/255.0, 99/255.0, 173/255.0);
	ellipse(radiusY, radiusX, X + r - 30,  Y + r + 150 + radiusX/2);
	rotate(X + r - 30 ,Y + r + 150, -rl);
	
	// Left
	rotate(X - r + 30 , Y + r + 150, -ll);
	glColor3f(209/255.0, 175/255.0, 90/255.0);
	ellipse(radiusY, radiusX, X - r + 30,  Y + r + 150 + (1.5*radiusX));
	glColor3f(68/255.0, 99/255.0, 173/255.0);
	ellipse(radiusY, radiusX, X - r + 30,  Y + r + 150 + radiusX/2);
	rotate(X - r + 30 ,Y + r + 150, ll);
	
	// Torso =====================================================================

	glColor3f(255/255.0, 123/255.0, 28/255.0);
	glBegin(GL_POLYGON);
	glVertex2i(X - r - 10, Y + r); 
	glVertex2i(X + r + 10, Y + r);
	glVertex2i(X + r - 15, Y + r + 150);
	glVertex2i(X - r + 15, Y + r + 150);
	glEnd(); 


	// Moving Curtains  ==========================================================
 
	float curtainLeft = w/2;
	float curtainRight = w/2;

	{
		if(open){
			c = min(t++, w/2);
		}		
		else {
			c = max(t--, 0);		
		}
		
		glColor3f(232/255.0, 60/255.0, 60/255.0);

		delay(1);
		
		// Left
		glBegin(GL_POLYGON);
		glVertex2i(0, 0); 
		glVertex2i(curtainLeft - c, 0);
		glVertex2i(curtainLeft - c, h -120);
		glVertex2i(0, h -120);
		glEnd(); 

		// Right
		glBegin(GL_POLYGON);
		glVertex2i(curtainRight +  c, 0); 
		glVertex2i(w, 0);
		glVertex2i(w,h-120);
		glVertex2i(curtainRight + c, h-120);
		glEnd();
		
	}



	//Audience  =====================================================================
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

		
	glutSwapBuffers(); // Helps prevent flickering 
	glFlush(); 



	
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


void reshape(int width,int height) 
{ 
 
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluOrtho2D(0,width ,height,0); 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 

}



void keyPressed (unsigned char key, int x, int y) { 

	if(key == 'r'){
		// Raises the Right Arm 
		ra = max(-20 * 3, ra - 20);
	}	

	if(key == 'f'){
		// Lowers the Right Arm
		ra = min(20 , ra + 20);
	} 

	if(key == 'q'){
		// Raises the Left Arm 
		la = max(-20 * 3, la - 20);
	}	

	if(key == 'a'){
		// Lowers the Left Arm
		la = min(20 , la + 20);
	} 


	if(key == 'v'){
		// Raises the Right Leg 
		rl = max(-20 * 3, rl - 20);
	}

	if(key == 'c'){
		// Lowers the Right Arm
		rl = min(20 , rl + 20);
	} 
	
	if(key == 'z'){
		// Raises the Left Leg 
		ll = max(-20 * 3, ll - 20);
	}

	if(key == 'x'){
		// Lowers the Left Leg
		ll = min(20 , ll + 20);
	} 
	if(key == '`'){
		// Reset show
		k = 0;	
	}

	if(key == 'o'){

		pid_t pid = fork();

		if (pid == 0) {
		   system("mpg123 spring.mp3");
		   exit(0);
		}

		open = 1;	
		t = c; 	
	}

	if(key == 'p'){
		// Close Curtain

		open = 0;	
		t = c;
	}	
	
}  




	
void welcome(void){

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
	t = 0;
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


	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	
	if(k < 1)
		welcome();
	else if (k < 2)
		delay(200);
	else if (k < 3)
		render();
	else
		puppet();

	k++;
}


int main(int argc, char* argv[])
{ 
	stage = 1;	
	c = 0; 


	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 

	

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
    	glutMouseFunc(mouse);
	glutMainLoop(); 

	return 0;
}

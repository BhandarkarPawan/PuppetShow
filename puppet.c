#include<GL/glut.h>
#include<unistd.h>
#include<math.h>
#include<iostream>
#include<cstring>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>


#define PI 3.141
#define DEG2RAD 3.14159/180.0

#define RIGHT_ARM 0
#define LEFT_ARM 1
#define RIGHT_LEG 2
#define LEFT_LEG 3



// Variable Declaration section ===================================================

// Stores the reference points of the limbs for calculating angles 
float ra_x, ra_y, la_x, la_y;
float rl_x, rl_y, ll_x, ll_y;

int stage, i;
float w;   // Stores window width
float h;   // Stores window height
int k;     // global parameter that decides the display  
int c;	   // Controls the curtain movement
int t;     // Used to record time of button click 

int open = 0;  // Whether the cutain is being closed or opened 

float ra, la, rl, ll;  // Right Arm, Left Arm, Right Leg and Left Leg angles 

FILE *fp;  // To access the command for mouse tracking 
char mouse_pos[40];

float pos_Y, pos_X, r;

pid_t pid;

// Math functions =================================================================

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
		
	// Use Euclidean distance to find out which is the closest reference point
	// i.e Find out the closest limb and move that limb

	float m, angle; 

	float right_arm = distance(x,y, ra_x, ra_y);
	float left_arm = distance(x,y, la_x, la_y);
	float right_leg = distance(x,y, rl_x, rl_y);
	float left_leg = distance(x,y, ll_x, ll_y);

	float closest = min(min(min(right_arm, left_arm), right_leg), left_leg);
	
	if(closest == right_arm){
		// Moving right Arm 
		m =  slope(ra_x, ra_y, x,y);
		angle  = atan(m)* 180 / PI;

		if (angle < 0)
			ra = max(-20 * 3, angle);
		else 
			ra = min(20 , angle);

		return RIGHT_ARM;	
	}
	else if(closest == left_arm){
		// Moving left Arm 
		m = slope(la_x, la_y, x,y);
		angle  = atan(m)* 180 / PI;


		if (angle < 0)
			la = - max(-20 , angle);
		else 
			la = - min(20 * 3, angle);

		return LEFT_ARM;	
	}
	else if(closest == right_leg){
		// Moving right leg 
		m = slope(rl_x, rl_y, x,y);
		angle  = atan(m)* 180 / PI;

		if (angle < 0)
			rl = max(-20 * 3, angle);
		else 
			rl = min(20 , angle);

		return RIGHT_LEG;	
	}
	else{
		// Moving left leg 
		m = slope(ll_x, ll_y, x,y);
		angle  = atan(m)* 180 / PI; 

		if (angle < 0)
			ll =  - max(-20 , angle);
		else 
			ll = - min(20 * 3 , angle);

		return LEFT_LEG;	
	}

	printf("Slope =  %f. Angle = %f. ", m , angle);
}


int get_number(char s[]){
	char c;
	int digit,number=0;
	for(int k=0;k<strlen(s);k++)
	{
		c = s[k];
		if(c>='0' && c<='9') //to confirm it's a digit
		{
			digit = c - '0';
			number = number*10 + digit;
		}
	}
	return number;
}


int track;  // Used to check whether mouse is being pressed 

void track_mouse(){
	
	// Find the position of the mouse while it is pressed down 
	 /* Open the command for reading. */
	fp = popen("/usr/bin/xdotool getmouselocation", "r");
	  if (fp == NULL) {
	    printf("Failed to run xdotool.\n" );
	    exit(1);
	  }

	char x_pos[10];
	char y_pos[10]; 

	fgets(mouse_pos, sizeof(mouse_pos)-1, fp);
	int i; 

	for(i = 0; i < 15; i++){
		if(mouse_pos[i] == ' '){
			x_pos[i] = '\0';
			break;
		}

		x_pos[i] = mouse_pos[i];	
	}
	
	int x = get_number(x_pos);	


	for(int j = i+1; j < 15; j++){
		if(mouse_pos[j] == ' '){
			y_pos[j-i] = '\0';
			break;
		}
		y_pos[j-i-1] = mouse_pos[j];	
	}		


	int y = get_number(y_pos);
	
	printf("%d, %d\n", x, y );  
	
	closest_limb(x,y);
  	
	pclose(fp);
	
	
	
}


int dance = 0;
int p_c = 2;
int head = 0; // Check if head is clicked to move entire puppet 

void mouse(int mouse, int state, int x, int y){
    // React to mouse press 
    switch(mouse){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){
		
		if(x > 100 && y > pos_Y + r + 10 && x < 200 && y < pos_Y + r + 80){
			printf("Button 1 clicked\n");
			dance = 1;


			if(pid > 0){
				system("ps");
				printf("ABOUT TO KILL MUSIC\n");
				system("pkill -9 play");
				printf("ABOUT TO KILL PID : %d\n", pid);
				kill(pid , SIGKILL);
				wait(NULL);
				system("ps");
			}

			pid = fork();
			if(pid == 0) 
				system("play jeet.mp3");

		
				
		}

		else if(x > 100 && y > pos_Y + r + 150 && x < 200 && y < pos_Y + r + 220){
			printf("Button 2 clicked\n");
			dance = 2;
			if(pid > 0){
				system("ps");
				printf("ABOUT TO KILL MUSIC\n");
				system("pkill -9 play");
				printf("ABOUT TO KILL PID : %d\n", pid);
				kill(pid , SIGKILL);
				wait(NULL);
				system("ps");
			}

			pid = fork();
			if(pid == 0) 
				system("play barso.mp3");

		}


		track_mouse();
		track = 1;
		int limb = closest_limb(x,y);
		printf("Dance = %d, Clicked (%d, %d). Closest limb: %d\n ", dance, x, y, limb);
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


// Text Commands =============================================================

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

int step = 1;

// Buttons ================================================================

void buttons(void){

	void* font =  GLUT_BITMAP_HELVETICA_18 ;

	glColor3f(1,1,1);
	glBegin(GL_POLYGON);

	glVertex2i(100, pos_Y + 60);
	glVertex2i(200, pos_Y + 60);
	glVertex2i(200, pos_Y + 130);
	glVertex2i(100, pos_Y + 130);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2i(100, pos_Y + 200);
	glVertex2i(200, pos_Y + 200);
	glVertex2i(200, pos_Y + 270);
	glVertex2i(100, pos_Y + 270);
	glEnd();


	glColor3f(0,0,0);
	drawBitmapText("Music 1" , 120, pos_Y + r + 150 ,0,font);

	glColor3f(0,0,0);
	drawBitmapText("Music 2" , 120, pos_Y + r + 290 ,0,font);


}


 
// Main Puppet =====================================================================

void puppet(int X, int Y )  
{ 


	glColor3f(0/255.0, 0/255.0, 0/255.0);
	glClear(GL_COLOR_BUFFER_BIT); 

	float x, y, i; 


	int radiusX = 50;
	int radiusY = 10;

	r = 50; // Size of the head 
	
	// Floor  =====================================================================


	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2i( 40, h - 250); 
	glVertex2i( w - 40,  h - 250);
	glColor3f(94/255.0, 64/255.0, 37/255.0);
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

	
	// Right
	glBegin(GL_POLYGON);	
	glColor3f(0,0,0);
	glVertex2i(w - 40, 0); 
	glVertex2i(w - 40, h - 250); 
	glColor3f(138/255.0, 109/255.0, 81/255.0);
	glVertex2i(w, h - 100);
	glVertex2i(w, 0);

	glEnd();
		
	// Left
	glBegin(GL_POLYGON);	
	glColor3f(0,0,0);
	glVertex2i(40, 0); 	
	glVertex2i(40, h - 250); 
	glColor3f(138/255.0, 109/255.0, 81/255.0);
	glVertex2i(0, h - 100);
	glVertex2i(0, 0);

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

	// Dance ====================================================================

	if( dance == 1){
	// Do the First type of dance 
	if( k % 50 == 0){
		if(step == 1){
			ra = 30;
			la = -30;
			rl = 30;
			ll = -30;	
			
			step = 2;			
			}

		else if(step == 2){
			ra = 0;
			la = 0;
			rl = 0;
			ll = 0;	
			
			step = 3;			
			}

		else if(step == 3){
			ra = -30;
			la = 30;
			rl = -30;
			ll = 30;	
			
			step = 4;			
			}

		else if(step == 4){
			ra = 0;
			la = 0;
			rl = 0;
			ll = 0;	
			
			step = 1;			
			}
		}
	}

	if( dance == 2){
	// Do the Second type of dance 
	if( k % 50 == 0){
		if(step == 1){
			ra = -30;
			la = -30;
			rl = 30;
			ll = 30;	
			
			step = 2;			
			}

		else if(step == 2){
			ra = 0;
			la = 0;
			rl = 0;
			ll = 0;	
			
			step = 3;			
			}

		else if(step == 3){
			ra = 30;
			la = 30;
			rl = -30;
			ll = -30;	
			
			step = 4;			
			}

		else if(step == 4){
			ra = 0;
			la = 0;
			rl = 0;
			ll = 0;	
			
			step = 1;			
			}
		}
	}



	// Arms  =====================================================================

	// Right  
	rotate(X - r - 10 , Y + r + 10, -la);
	glColor3f(209/255.0, 175/255.0, 90/255.0);
	ellipse(radiusX, radiusY, X - r - 10 - 1.5*(radiusX),  Y + r + 10);
	glColor3f(255/255.0, 123/255.0, 0/255.0);
	ellipse(radiusX, radiusY, X - r - 10 - radiusX/2,  Y + r + 10);
	rotate(X - r - 10 , Y + r + 10, la);
	
	la_x = X - r - 10 - radiusX/2;
	la_y = Y + r + 10;

	// Left
	rotate(X + r + 10 , Y + r + 10, ra);
	glColor3f(209/255.0, 175/255.0, 90/255.0);
	ellipse(radiusX, radiusY, X + r + 10 + 1.5*(radiusX),  Y + r + 10);
	glColor3f(255/255.0, 123/255.0, 0/255.0);
	ellipse(radiusX, radiusY, X + r + 10 + radiusX/2,  Y + r + 10);
	rotate(X + r + 10 , Y + r + 10, -ra);

	ra_x = X + r + 10  + radiusX/2;
	ra_y = Y + r + 10;

	// Legs =====================================================================

	// Right
	rotate(X + r - 30 , Y + r + 150, rl);
	glColor3f(209/255.0, 175/255.0, 90/255.0);
	ellipse(radiusY, radiusX, X + r - 30,  Y + r + 150 + (1.5*radiusX));
	glColor3f(68/255.0, 99/255.0, 173/255.0);
	ellipse(radiusY, radiusX, X + r - 30,  Y + r + 150 + radiusX/2);
	rotate(X + r - 30 ,Y + r + 150, -rl);
	
	rl_x = X + r - 30;
	rl_y = Y + r + 150 + radiusX/2;

	// Left
	rotate(X - r + 30 , Y + r + 150, -ll);
	glColor3f(209/255.0, 175/255.0, 90/255.0);
	ellipse(radiusY, radiusX, X - r + 30,  Y + r + 150 + (1.5*radiusX));
	glColor3f(68/255.0, 99/255.0, 173/255.0);
	ellipse(radiusY, radiusX, X - r + 30,  Y + r + 150 + radiusX/2);
	rotate(X - r + 30 ,Y + r + 150, ll);

	ll_x = X - r + 30;
	ll_y = Y + r + 150 + radiusX/2;
	
	// Torso =====================================================================

	glColor3f(255/255.0, 123/255.0, 28/255.0);
	glBegin(GL_POLYGON);
	glVertex2i(X - r - 10, Y + r); 
	glVertex2i(X + r + 10, Y + r);

	glColor3f(255/255.0, 255/255.0, 28/255.0);
	glVertex2i(X + r - 15, Y + r + 150);
	glVertex2i(X - r + 15, Y + r + 150);


	glEnd(); 

	buttons();



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
		


		delay(1);
		
		// Left
		glBegin(GL_POLYGON);
		glColor3f(232/255.0, 30/255.0, 30/255.0);
		glVertex2i(curtainLeft - c, 0);
		glVertex2i(curtainLeft - c, h -120);
		glColor3f(232/255.0, 70/255.0, 70/255.0);
		glVertex2i(0, h -120);
		glVertex2i(0, 0); 
		glEnd(); 

		// Right
		glBegin(GL_POLYGON);
		glColor3f(232/255.0, 30/255.0, 30/255.0);
		glVertex2i(curtainRight + c, h-120);
		glVertex2i(curtainRight +  c, 0); 
		glColor3f(232/255.0, 70/255.0, 70/255.0);
		glVertex2i(w, 0);
		glVertex2i(w,h-120);

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
	
	if(key == 'g'){
		// Stop Dance
		dance = 0;
	}


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
			
		open = 1;	
		t = c; 	

		pid = fork();
		printf("\nThe music process is : %d\n", pid);

		if (pid == 0) {   
			system("play spring.mp3");
		}
		

	}

	if(key == 'p'){
		// Close Curtain

		open = 0;	
		t = c;
	}	
	
}  






char pawan[50] = "Pawan Bhandarkar";
char pavan[50] = "Pavan N";
char pavithra[50] = "Pavithra B";
char pawan_usn[50] = "4NM16CS090";
char pavan_usn[50] = "4NM16CS091";
char pavithra_usn[50] = "4NM16CS092";
char title[50] = "A Puppet Show";
	
void welcome(void){

	glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity();
		

	glColor3f(1,1, 1);
    
	void* font =  GLUT_BITMAP_HELVETICA_18 ;
	drawBitmapText(pawan,w/5,h*2/5,0 ,font);
	drawBitmapText(pawan_usn,w*3/5,h*2/5,0,font);
	drawBitmapText(pavan,w/5,h/2,0,font);
	drawBitmapText(pavan_usn,w*3/5,h/2,0,font);
	drawBitmapText(pavithra,w/5,h*3/5,0,font);
	drawBitmapText(pavithra_usn,w*3/5,h*3/5,0,font);
	drawStrokeText(title,w/2-180,h/5*3.5,0);
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
		drawBitmapText(pawan,w/5,h*2/5 - i,0 ,font);
		drawBitmapText(pawan_usn,w*3/5,h*2/5 - i,0,font);
		drawBitmapText(pavan,w/5,h/2 - i,0,font);
		drawBitmapText(pavan_usn,w*3/5,h/2 - i,0,font);
		drawBitmapText(pavithra,w/5,h*3/5 - i,0,font);
		drawBitmapText(pavithra_usn,w*3/5,h*3/5 - i,0,font);
		drawStrokeText(title,w/2-180,h/5*3.5 - i,0);
		glutSwapBuffers(); 
		
	}	
} 


void display(void){
	
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

	pos_Y = h/3;
	pos_X = w/2;

	if(k < 1)
		welcome();
	else if (k < 2)
		delay(200);
	else if (k < 3)
		render();
	else

		puppet(pos_X, pos_Y);

	k++;


	if(track == 1)
		track_mouse();
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

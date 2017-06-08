
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

class particle{
	private:
		float v_x;
		float v_y;

		float r;
		float g;
		float b;

	public:
		particle(){
			v_x = 0.0f;
			v_y = 0.0f;

			r = 0.0f;
			g = 0.0f;
			b = 0.0f;
		}

		void setV_x(float x){	v_x = x;	}
		void setV_y(float y){ 	v_y = y;	}

		void setR(float _r){ 	r = _r;	}
		void setG(float _g){	g = _g;	}
		void setB(float _b){	b = _b;	}

		float getV_x( ){	v_x = v_x;	}
		float getV_y( ){ 	v_y = v_y;	}

		float getR( ){ 	return r;	}
		float getG( ){	return g;	}
		float getB( ){	return b;	}
};

void init();
void myreshape(int w, int h);
void myidle();
void mydisplay();
void mykeyboard(unsigned char key, int x, int y);
void DoSpecial(int key, int x, int y);
void MyMenu(int value);


static float g_aspect = 1.0f;
static float g_delta_x = 0.0f;


static float g_eye_x = 0.0f;
static float g_eye_y= 0.0f;

static float g_fovy = 70.0f;

static int numOfPolygons = 1;

static float time_delta = 0.01f;
static float v_x = 0.0f;
static float v_y = 0.0f;

static float r, g, b = 1.0;

static particle *p;

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Graphics HW1");

	init();

	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutIdleFunc(myidle);
	
	glutSpecialFunc(DoSpecial);
	glutKeyboardFunc(mykeyboard);

	glutCreateMenu(MyMenu);
	glutAddMenuEntry("emit 1", 1);
	glutAddMenuEntry("emit 10", 2);
	glutAddMenuEntry("emit 50", 3);
	glutAddMenuEntry("emit 100", 4);
	glutAddMenuEntry("emit 500", 5);
	glutAddMenuEntry("emit 1000", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();

	return	0;
}

void init(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		// white

	p = new particle[numOfPolygons];
	srand(time(NULL));
	for(int i =0; i < numOfPolygons; i++){
		r =  rand() / (float) RAND_MAX;
		g =  rand() / (float) RAND_MAX;
		b =  rand() / (float) RAND_MAX;

		v_x = (rand() / (float) RAND_MAX *6 )-3;
		v_y = (rand() / (float) RAND_MAX)*3;

		p[i].setR(r);
		p[i].setG (g);
		p[i].setB (b);

		p[i].setV_x(v_x);
		p[i].setV_y(v_y);
	}

}

void myreshape(int w, int h){
	std::cout << "calling myreshape" << std::endl;

	g_aspect = (float) w / (float) h;

	glViewport(0, 0, w, h);

	glutPostRedisplay();
}

void myidle(){

	if(g_eye_x <-5.0f)	g_eye_x = -5.0f;
	else if(g_eye_x > 5.0f)	g_eye_x = 5.0f;

	if(g_eye_y < 0.0f)	g_eye_y = 0.0f;
	else if(g_eye_y > 10.0f)	g_eye_y = 10.0f;

	if(time_delta > 1.0f	){
		time_delta = 0.0f;
		init();
	}

	time_delta += 0.01f;

	glutPostRedisplay();
}


void mydisplay(){
	std::cout << "calling display()" << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// camera intrinsic parameter
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy, g_aspect, 0.01f, 1000.0f);

	// camera extrinsic parameter
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(g_eye_x, g_eye_y, 2.0f, 
		g_eye_x, g_eye_y, 0.0f,
		0.0f, 1.0f, 0.0f);

	// (0, 0, 0)
	glColor3f(0.0f, 0.0f, 0.0f);	
	glBegin(GL_POLYGON);
	glVertex3f( 0.01f,  0.01f,  0.0f);
	glVertex3f(-0.01f,  0.01f,  0.0f);
	glVertex3f(-0.01f, -0.01f,  0.0f);
	glVertex3f( 0.01f, -0.01f,  0.0f);
	glEnd();


	// emit
	for(int i= 0; i<numOfPolygons; i++){
		glColor3f(p[i].getR(), p[i].getG(), p[i].getB());	

		glTranslatef(p[i].getV_x()*time_delta, 
			p[i].getV_y()*time_delta - 0.5*9.8*time_delta*time_delta, 0);
		
		glBegin(GL_POLYGON);
		glVertex3f( 0.015f,  0.015f,  0.0f);
		glVertex3f(-0.015f,  0.015f,  0.0f);
		glVertex3f(-0.015f, -0.015f,  0.0f);
		glVertex3f( 0.015f, -0.015f,  0.0f);
		glEnd();	
		
	}
	glutSwapBuffers();
}


void mykeyboard(unsigned char key, int x, int y){
	std::cout << "calling mykeyboard" << std::endl;
	switch (key){
		case '+' :
		case '=' :
			g_fovy -= 0.3f;
			break;
		case '-' :
		case '_' :
			g_fovy += 0.3f;
			break;
		}
	glutPostRedisplay();
}

void DoSpecial(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			g_eye_x += -0.1f;
			break;

		case GLUT_KEY_RIGHT:
			g_eye_x += 0.1f;
			break;

		case GLUT_KEY_UP:
			g_eye_y += 0.1f;
			break;

		case GLUT_KEY_DOWN:
			g_eye_y += -0.1f;
			break;

		case GLUT_KEY_HOME:
			g_eye_x = 0;
			g_eye_y = 0;
			g_fovy = 70.f;
			break;
	}
	glutPostRedisplay();
}

void MyMenu(int value){
	switch(value){
		case 1:
			numOfPolygons = 1;
			init();
			break;
		case 2:
			numOfPolygons = 10;
			init();
			break;
		case 3:
			numOfPolygons = 50;
			init();
			break;
		case 4:
			numOfPolygons = 100;
			init();
			break;
		case 5:
			numOfPolygons = 500;
			init();
			break;
		case 6:
			numOfPolygons = 1000;
			init();
			break;
	}
}

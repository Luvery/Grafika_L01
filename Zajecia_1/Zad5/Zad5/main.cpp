#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

vector< float > points;
string text = "Ile wierzcholkow: ";

void Text()
{
	string fullText = text;
	string str = to_string(points.size() / 2);
	fullText += str;	
	glLoadIdentity();
	glColor3f(0.5f, 1.0f, -0.5f);
	glRasterPos2f(10, 10);
	for (int i = 0; i < fullText.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fullText[i]);
	}
}

void myDisplay(void)
{

	glClearColor(0.7f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800, 0.0, 600); ////////////

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3f(0.98f, 0.625f, 1.0f);
	for (size_t i = 0; i < points.size(); i += 2)
	{
		glVertex2d(points[i], points[i + 1]);
	}
	glEnd();

	Text();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.3f, 0.7f, 0.1f);
	for (size_t i = 0; i < points.size(); i += 2)
	{
		glVertex2f(points[i], points[i + 1]);
	}
	glEnd();
	glFlush();
}

void myMouse(int button, int state, int x, int y)
{
	int yy;
	yy = glutGet(GLUT_WINDOW_HEIGHT);
	y = yy - y;
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		points.push_back(x);
		points.push_back(y);
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Glut nr 5");
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutMainLoop();
}
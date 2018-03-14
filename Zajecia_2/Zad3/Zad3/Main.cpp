﻿// OpenGL_PYRAMID_ConsoleApplication.cpp : Defines the entry point for the console application.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "gltools_extracted.h"



// Wielkoci obrotów
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
// Zmiana przestrzeni widocznej i okna.
// Wywoływana w momencie zmiany rozmiaru okna
void ChangeSize(int w, int h)
{
	GLfloat fAspect;
	// Zabezpieczenie przed dzieleniem przez zero
	if (h == 0)
		h = 1;
	// Zrównanie wielkości widoku i okna
	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w / (GLfloat)h;
	// Ustalenie układu współrzędnych
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Utworzenie rzutowania perspektywicznego
	gluPerspective(35.0f, fAspect, 1.0, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Ta funkcja wykonuje wszystkie konieczne inicjalizacje kontekstu renderowania.
// Tutaj, konfiguruje i inicjalizuje oświetlenie sceny
void SetupRC()
{
	GLuint texture;
	int x, y;
	char pixels[256 * 256];
	GLint iWidth = 256;
	GLint iHeight = 256;
	GLint iComponents = GL_RGB;
	GLenum eFormat = {
		GL_RGBA
	};
	// Wartoci i współrzdne światła
	GLfloat whiteLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat lightPos[] = { -10.f, 5.0f, 5.0f, 1.0f };
	glEnable(GL_DEPTH_TEST); // Usuwanie ukrytych powierzchni
	glEnable(GL_CULL_FACE); // Nie bdziemy prowadzi oblicze wntrza samolotu
	glFrontFace(GL_CCW); // Wielokty z nawiniciem przeciwnym do ruchu wskazówek zegara
						 // Włczenie owietlenia
	glEnable(GL_LIGHTING);
	// Konfiguracja i włączenie światła numer 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	// Włczenie ledzenia kolorów
	glEnable(GL_COLOR_MATERIAL);
	// Właciwoci owietlenia otoczenia i rozproszenia
	// bd ledzi wartoci podawane funkcji glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// Czarne tło
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Ładowanie tekstury
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate a name for the texture
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);


	for (y = 0; y < 256; y++)
	{
		for (x = 0; x < 256; x++)
			pixels[y * 256 + x] = rand() % 256;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 256, 256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}

// Reakcje na klawisze strzałek
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;
	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
	xRot = (GLfloat)((const int)xRot % 360);
	yRot = (GLfloat)((const int)yRot % 360);
	// Odwieenie zawartoci okna
	glutPostRedisplay();
}
// Wywoływana w celu przerysowania sceny
void RenderScene(void)
{
	GLTVector3 vNormal;
	GLTVector3 vCorners[15] = { { 0.0f, .80f, 0.0f }, // Góra 0
	{ -0.5f, 0.0f, -.30f }, // Lewy tył 1
	{ 0.5f, 0.0f, -0.30f }, // Prawy tył 2
	{ 0.5f, 0.0f, 0.3f }, // Prawy przód 3
	{ -0.5f, 0.0f, 0.3f }, // Lewy przód 4
	{ -0.25f, 0.0f, 0.6f } , // Przód 5
	{ -0.25f, 0.0f, -0.6f } , // 2 Tył lewy 6
	{ 0.25f, 0.0f, -0.6f },// 2 Tył prawy 7
	{ 0.25f, 0.0f, 0.6f } , // Przód 8
	{ -0.55f, 0.0f, 0.0f }, // Lewy tył 9 nad 1
	{ 0.55f, 0.0f, 0.0f }, // Prawy tył 10 nad 2
	{ 0.0f, 0.0f, 0.66f }, // Przód 11 pomiedzy 5 a 8
	{ 0.f, 0.0f, -0.66f },// Tyl 12 pomiedzy 6 a 7
	{ 0.0f, 0.0f, 0.0f } };
	
							 // Czyszczenie okna aktualnym kolorem czyszczącym
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Zapisanie stanu macierzy i wykonanie obrotów
	glPushMatrix();
	// Cofnięcie obiektów
	glTranslatef(0.0f, -0.25f, -4.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	// Rysowanie piramidy
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	// Podstawa piramidy - dwa trójkąty
	glNormal3f(0.0f, -1.0f, 0.0f);
	
	//Rysowanie podstawy - 5 trójkątów:
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[1]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[5]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[4]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[1]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[6]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[5]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[6]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[7]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[5]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[7]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[2]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[5]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[10]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[3]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[5]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[10]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[3]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[2]);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[8]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[5]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[3]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[1]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[4]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[9]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[2]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[10]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[3]);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[8]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[5]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[2]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[2]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[10]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[5]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[8]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[11]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[5]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(vCorners[6]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[12]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(vCorners[7]);



	//Ściany boczne
	gltGetNormalVector(vCorners[4], vCorners[5], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[4]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[5]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[8], vCorners[3], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[8]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[3]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[3], vCorners[10], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[3]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[10]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[2], vCorners[7], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[2]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[7]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[7], vCorners[12], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[7]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[12]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[6], vCorners[1], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[6]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[1]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[1], vCorners[9], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[1]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[9]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[9], vCorners[4], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[9]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[4]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[5], vCorners[11], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[5]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[11]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);
	
	gltGetNormalVector(vCorners[11], vCorners[8], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[11]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[8]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[10], vCorners[2], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[10]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);

	gltGetNormalVector(vCorners[12], vCorners[6], vCorners[0], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[12]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[6]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[0]);
	
	
	
	glEnd();
	// Odtworzenie stanu macierzy
	glPopMatrix();
	// Zamiana buforów
	glutSwapBuffers();
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Textured Pyramid");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
	return 0;
}

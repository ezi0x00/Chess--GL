#include<GL/glut.h>
#include<math.h>
#include"chess.cpp"
#include<string.h>
void rectangle(int a, int b, int c, int d);
void triangle(int a, int b, int c, int d, int e, int f);
void circle(int x, int y, int r);
void myinit();
void mreshape(int wx, int hx);
void board_layout();
void skeleton_box(int x, int y);
void clearbox(int x, int y);
void highlight(int x, int y);
int w = 1366, h = 685, d, offset = 2;
void message(char*);
void display();
chessboard c1(skeleton_box, clearbox, highlight, display, message);
void myinit()
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_POINT_SMOOTH);
}

void mreshape(int wx, int hx) // fix size of the window
{
	if (wx == w && hx == h)
	{
		board_layout();c1.redisplay();return;
	}
	if (wx != w || hx != h)
		glutReshapeWindow(w, h);
}
void rect_box(int, int, int, int);
void message_box() //define message box space
{
	glColor3f(0, 0, 0);
	glLineWidth(3);
	rectangle(750, 400, 1300, 650);
	glColor3f(1, 1, 1);
	rect_box(750, 400, 1300, 650);
}
void message(char *msg) //display message in message box after clearing it
{
	message_box();
	glRasterPos2f(770, 600);
	glColor3f(1, 1, 1);
	for (int i = 0;i < strlen(msg);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
}

void king(int x, int y, int color)
//function to display King wrt to (x,y) position. (x,y) is center of the King
{

	if (color == 0)
		glColor3ub(57, 94, 144);
	else
		glColor3ub(30, 5, 34);
	x = x*d + offset + d / 2;
	y = y*d + offset + d / 2;
	glLineWidth(1);
	glPointSize(1);
	rectangle(x - d / 14.285, y + d / 2.702, x + d / 14.285, y + d / 3.125);
	rectangle(x - d / 33.33, y + d / 2.439, x + d / 50, y + d / 3.33);
	circle(x, y + d / 5, d / 5.555);
	rectangle(x - d / 25, y + d / 10, x + d / 25, y + d / 20);
	rectangle(x - d / 11.11, y + d / 20, x + d / 11.11, y);
	rectangle(x - d / 16.667, y, x + d / 16.667, y - d / 4.167);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}
void knight(int x, int y, int color)
//function to display Knight wrt to (x,y) position. (x,y) is center of the Knight
{

	if (color == 0)
		glColor3ub(57, 94, 144);
	else
		glColor3ub(30, 5, 34);
	x = x*d + offset + d / 2;
	y = y*d + offset + d / 2;
	glLineWidth(2);
	glPointSize(1);
	glBegin(GL_LINES);
	glVertex2i(x, y + d / 4.347);
	glVertex2i(x - d / 9.09, y + d / 5);
	glVertex2i(x - d / 9.09, y + d / 5);
	glVertex2i(x - d / 14.285, y - d / 14.285);
	glVertex2i(x, y + d / 4.3478);
	glVertex2i(x + d / 4, y + d / 14.285);
	glVertex2i(x + d / 4, y + d / 14.285);
	glVertex2i(x + d / 5, y);
	glVertex2i(x + d / 5, y);
	glVertex2i(x + d / 12.5, y + d / 25);
	glVertex2i(x + d / 12.5, y + d / 25);
	glVertex2i(x + d / 16.667, y - d / 14.285);
	glEnd();
	glLineWidth(1);
	rectangle(x - d / 16.667, y - d / 14.285, x + d / 16.667, y - d / 4.167);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}
void queen(int x, int y, int color)
//function to display Queen wrt to (x,y) position. (x,y) is center of the Queen
{

	if (color == 0)
		glColor3ub(57, 94, 144);
	else
		glColor3ub(30, 5, 34);
	x = x*d + offset + d / 2;
	y = y*d + offset + d / 2;
	glLineWidth(1);
	glPointSize(1);
	triangle(x - d / 25, y + d / 2.778, x + d / 25, y + d / 2.778, x, y + d / 2.439);
	rectangle(x - d / 11.11, y + d / 2.778, x + d / 11.11, y + d / 3.333);
	circle(x, y + d / 5, d / 5.555);
	rectangle(x - d / 25, y + d / 10, x + d / 25, y + d / 20);
	rectangle(x - d / 11.11, y + d / 20, x + d / 11.11, y);
	rectangle(x - d / 16.667, y, x + d / 16.667, y - d / 4.166);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.111, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}
void bishop(int x, int y, int color)
//function to display Bishop wrt to (x,y) position. (x,y) is center of the Bishop
{

	if (color == 0)
		glColor3ub(57, 94, 144);
	else
		glColor3ub(30, 5, 34);
	x = x*d + offset + d / 2;
	y = y*d + offset + d / 2;
	glLineWidth(1);
	glPointSize(1);
	circle(x, y + d / 3.33, d / 7.692);
	circle(x, y + d / 6.667, d / 5.555);
	rectangle(x - d / 25, y + d / 20, x + d / 25, y - d / 50);
	rectangle(x - d / 11.11, y - d / 50, x + d / 11.11, y - d / 14.285);
	rectangle(x - d / 16.667, y - d / 14.285, x + d / 16.667, y - d / 4.167);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}
void rook(int x, int y, int color)
//function to display Rook wrt to (x,y) position. (x,y) is center of the Rook
{

	if (color == 0)
		glColor3ub(57, 94, 144);
	else
		glColor3ub(30, 5, 34);
	x = x*d + offset + d / 2;
	y = y*d + offset + d / 2;
	glLineWidth(1);
	glPointSize(1);
	rectangle(x - d / 8.333, y + d / 5, x + d / 8.333, y + d / 20);
	rectangle(x - d / 11.11, y + d / 20, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}
void pawn(int x, int y, int color)
{
	if (color == 0)
		glColor3ub(57, 94, 144);
	else
		glColor3ub(30, 5, 34);
	x = x*d + offset + d / 2;
	y = y*d + offset + d / 2;
	glLineWidth(1);
	glPointSize(1);
	circle(x, y + d / 20, d / 5);
	rectangle(x - d / 16.667, y - d / 14.285, x + d / 16.667, y - d / 4.167);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}

void triangle(int a, int b, int c, int d, int e, int f) //display unfilled triangle
{
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glVertex2i(a, b);
	glVertex2i(c, d);
	glVertex2i(e, f);
	glEnd();


}
void rectangle(int a, int b, int c, int d) //display filled rectangle
{
	glBegin(GL_POLYGON);
	glVertex2i(a, b);
	glVertex2i(c, b);
	glVertex2i(c, d);
	glVertex2i(a, d);
	glEnd();

}
void circle(int x, int y, int r) //display filled circle of radius 'r'
{

	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(r);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();

}
void clearbox(int, int);
void board_layout()	//display chess board
{
	glLineWidth(2);
	for (int i = 0;i < 8;i++)
		for (int j = 0;j < 8;j++)
			clearbox(i, j);
}
void rect_box(int a, int b, int c, int d)	//display unfilled rectangle
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(a, b);
	glVertex2i(c, b);
	glVertex2i(c, d);
	glVertex2i(a, d);
	glEnd();

}
void skeleton_box(int x, int y)		//display unfilled rectangle of white color
{
	glColor3f(1, 1, 1);
	glLineWidth(3);
	rect_box(x*d + offset, y*d + offset, (x + 1)*d + offset, (y + 1)*d + offset);
}
void clearbox(int x, int y)			//clear (x,y) box in chess board
{
	if ((x + y) % 2 == 0)
		glColor3ub(50, 205, 50);
	else
		glColor3ub(255, 250, 250);

	glBegin(GL_POLYGON);
	glVertex2i(x*d + offset, y*d + offset);
	glVertex2i(x*d + offset, (y + 1)*d + offset);
	glVertex2i((x + 1)*d + offset, (y + 1)*d + offset);
	glVertex2i((x + 1)*d + offset, y*d + offset);
	glEnd();
	skeleton_box(x, y);
}
void highlight(int x, int y)	//highlight box in chessboard
{
	glColor3ub(30, 144, 255);
	glLineWidth(3);
	rect_box(x*d + offset, y*d + offset, (x + 1)*d + offset, (y + 1)*d + offset);
}

void display()	//flush contents to display
{
	glFlush();
}
void initboard()	//initialise chess engine wrt UI elements 
{
	d = h;
	if (w < d)
		d = w;
	d = d / 8;
	board_layout();
	c1.setking(king);
	c1.setqueen(queen);
	c1.setbishop(bishop);
	c1.setrook(rook);
	c1.setknight(knight);
	c1.setpawn(pawn);
}
int m = 0;
void reset(int v)		//enable/disable mouse control for piece selection
{
	m = 0;
}
void mouse(int b, int s, int x, int y)
{
	if (m == 1)
		return;
	m = 1;
	y = h - y;
	if (b != GLUT_LEFT_BUTTON && s != GLUT_DOWN)
		return;
	if (x > (8 * d + offset) || y > (8 * d + offset))
		return;
	for (int i = 0;i < 8;i++)
		for (int j = 0;j < 8;j++)
			if (j*d + offset < x && i*d + offset < y && (j + 1)*d + offset >= x && (i + 1)*d + offset >= y)
			{
				x = j;y = i;
			}
	c1.select(x, y);
	glutTimerFunc(250, reset, 0);		//mouse is re-enabled after 250ms
}
void mainmenu(int id)	//menu to undo move on player request
{
	if (id == 1)
		c1.undo_move();
}
void initmenu()		//initialise menu 
{
	glutCreateMenu(mainmenu);
	glutAddMenuEntry("UNDO", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CHESS");

	myinit();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	initboard();
	initmenu();
	message_box();
	glutDisplayFunc(display);
	glutReshapeFunc(mreshape);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}

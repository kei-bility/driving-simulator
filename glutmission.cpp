#include "3dmath.h"
#include <stdio.h>
#include <math.h>
#include "glut.h"
#include "globjects.h"

/* マウスの相対位置→回転角の換算係数 */
#define SCALE (2.0 * 3.14159265358979323846)
#define M_PI 3.14159265358979323846

GLfloat light0pos[] = {-10.0f, 10.0f, 10.0f, 1.0f  };
GLfloat light1pos[] = { 0.0, -10.0, 0.0, 1.0 };
GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
GLfloat m_groundColor1[4],m_groundColor2[4];
GLfloat glfLightAmbient[]  = {0.1, 0.1, 0.1, 1.0};
GLfloat glfLightDiffuse[]  = {0.5, 0.5, 0.5, 1.0};
GLfloat glfLightSpecular[] = {0.0, 0.0, 0.0, 1.0};

int m_width=600,m_height=400;

GLfloat np=1.0;
CRealvector myposi(0,10,0.2),eyetarget(0,0,0);
CRealvector UpV=CRealvector(0,1,0);
static GLdouble mDscale=(myposi-eyetarget).length();
GLObjects objs;

static bool isShiftActive=false;
static int cx, cy;
/* マウスの絶対位置→ウィンドウ内での相対位置の換算係数 */
static double sx, sy;

//箱配置用のマージン（くっつかないようにする為の）
static const GLdouble dmargin=0.005;

static int isMouseDrag=0;
//0:NONE 1:LEFT 2:MIDDLE

void setViewP(const CRealvector &_eyeP,const CRealvector &_toP,const CRealvector &_upV){
	gluLookAt(_eyeP.x(),_eyeP.y(),_eyeP.z(),_toP.x(),_toP.y(),_toP.z(),_upV.x(),_upV.y(),_upV.z());
}

void setViewObjective(CRealvector &myposi,CRealvector &targetPosi,double dx,double dy){
	//printf("%f,%f\n",dx,dy);
	CRealvector toeyeP=targetPosi;
	CRealvector eyeP= myposi;

	//double m_leye=mDscale*(eyeP-toeyeP).length();
	double m_leye=mDscale;
	CRealvector tVV=m_leye*(toeyeP-eyeP).normalized();
	double m_eye_th1 = asin(tVV.y()/mDscale);
	double m_eye_th2 = atan2(tVV.x(),tVV.z());

	m_eye_th1 += (float)dy;
	m_eye_th2 += (float)dx;

	if (0.4999*M_PI<m_eye_th1) m_eye_th1=0.4999*M_PI;
	if (m_eye_th1<-0.4999*M_PI) m_eye_th1=-0.4999*M_PI;

	double lxz;
	CRealvector tmpV;
	tmpV.y()=m_leye*sin(m_eye_th1); 
	lxz=m_leye*cos(m_eye_th1);
	tmpV.x()=lxz*sin(m_eye_th2); tmpV.z()=lxz*cos(m_eye_th2);
	eyeP= -tmpV + toeyeP;

	eyetarget=toeyeP;
	myposi=eyeP;
}


void idle(void)
{
	glutPostRedisplay();
}
//
// マウスクリック時に呼ばれるコールバック関数
//
void  mouse(int button, int state, int x, int y)
{


	switch (button) {
		case GLUT_LEFT_BUTTON:
			switch (state) {
		case GLUT_DOWN:
			/* ドラッグ開始点位置を記録する */
			cx = x;
			cy = y;
			/* アニメーション開始 */
			isMouseDrag=1;
			glutIdleFunc(idle);
			break;
		case GLUT_UP:
			/* アニメーション終了 */
			isMouseDrag=0;
			glutIdleFunc(0);
			break;
		default:break;
			}		 
			break;
		case GLUT_RIGHT_BUTTON:
			switch (state) {
		case GLUT_DOWN:
			/* ドラッグ開始点位置を記録する */
			cx = x;
			cy = y;
			/* アニメーション開始 */
			isMouseDrag=2;
			glutIdleFunc(idle);
			break;
		case GLUT_UP:
			/* アニメーション終了 */
			isMouseDrag=0;
			glutIdleFunc(0);
			break;
		default:break;
			}
			break;
		default:break;
	}

}


//
// マウスドラッグ時に呼ばれるコールバック関数
//
void  motion( int x, int y )
{	
	/* マウスポインタの位置のドラッグ開始位置からの変位 (相対値) */


	if(isMouseDrag==1){
		double dx = (double)(x - cx) * sx;
		double dy = -(double)(y - cy) * sy;
		cx=x;
		cy=y;
		setViewObjective(myposi,eyetarget,dx,dy);
	}else if(isMouseDrag == 2){	
		double dx = (double)(x - cx) * sx;
		double dy = (double)(y - cy) * sy;
		cx=x;
		cy=y;
		double msc=0.1;
		if(isShiftActive)msc=0.5;
		else msc=0.2;
		double adist=sqrt(dx*dx+dy*dy);
		if(dy<0){
			mDscale -= msc;
		}else{
			mDscale += msc;
		}
		if(mDscale<0.1)mDscale=0.1;
		//printf("%f\n",mDscale);
		setViewObjective(myposi,eyetarget,0,0);
	}

}




void SetGroundColor(GLfloat *col1,GLfloat *col2){
	for(int i=0;i<4;i++){
		m_groundColor1[i]=col1[i];
		m_groundColor2[i]=col2[i];
	}
}


void renderOnlyGround(void){

	GLfloat color[][4] = {
		{ 1, 1, 1, 1.0 },
		{ 0.243, 0.271, 0.288, 1.0 },
		{ 0.2, 0.6, 0.2, 1.0 },

	};

	SetGroundColor(color[2],color[1]);
	glPushMatrix();


	bool bWB=false;

	for(double i = 0; i < 40; i+=0.5)
	{
		for(double j = 0; j < 40; j+=0.5)
		{
			if(bWB){
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_groundColor1);
			}else{
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,m_groundColor2);
			}
			bWB=!bWB;

			glBegin(GL_QUADS);
			glNormal3d(0.0, 1.0, 0.0);
			glVertex3d(-20.0 + i,  0.0, -20.0 + j);
			glVertex3d(-20.0 + i,  0.0, -19.5 + j);
			glVertex3d(-19.5 + i,  0.0, -19.5 + j);
			glVertex3d(-19.5 + i,  0.0, -20.0 + j);
			glEnd();
		}
		bWB=!bWB;

	}
	glPopMatrix();
}

void draw_cube(int i,int j,float cr,float cg,float cb){
	GLfloat col[] = { cr, cg, cb, np };
	glEnable(GL_BLEND);
	glPushMatrix();
	glTranslated((float)i/2.0+0.25,0.25+dmargin,(float)j/2.0+0.25);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
	glutSolidCube(0.5);
	glPopMatrix();
	glDisable(GL_BLEND);
}

struct mariocolor{
	double r,g,b;
};

struct mariocolor mc[3]={
	{254/255.0,209/255.0,114/255.0},
	{135/255.0,111/255.0,12/255.0},
	{253/255.0,24/255.0,29/255.0},
};

void draw_nankaLine(int snx,int sny,int blocks,double r,double g,double b){
	for(int i=0;i<blocks;i++){
		draw_cube(snx+i,sny,r,g,b);
	}
}

void draw_nankaLine(int snx,int sny,int blocks,struct mariocolor _mc){
	for(int i=0;i<blocks;i++){
		draw_cube(snx+i,sny,_mc.r,_mc.g,_mc.b);
	}
}

void draw_nanka(void){
	//glEnable(GL_COLOR_MATERIAL);
	draw_nankaLine(0,0,5,mc[2]);

	draw_nankaLine(-1,1,9,mc[2]);

	draw_nankaLine(-1,2,3,mc[1]);
	draw_nankaLine(2,2,2,mc[0]);
	draw_nankaLine(4,2,1,mc[1]);
	draw_nankaLine(5,2,1,mc[0]);

	draw_nankaLine(-2,3,1,mc[1]);
	draw_nankaLine(-1,3,1,mc[0]);
	draw_nankaLine(0,3,1,mc[1]);
	draw_nankaLine(1,3,3,mc[0]);
	draw_nankaLine(4,3,1,mc[1]);
	draw_nankaLine(5,3,3,mc[0]);

	draw_nankaLine(-2,4,1,mc[1]);
	draw_nankaLine(-1,4,1,mc[0]);
	draw_nankaLine(0,4,2,mc[1]);
	draw_nankaLine(2,4,3,mc[0]);
	draw_nankaLine(5,4,1,mc[1]);
	draw_nankaLine(6,4,3,mc[0]);

	draw_nankaLine(-2,5,2,mc[1]);
	draw_nankaLine(0,5,4,mc[0]);
	draw_nankaLine(4,5,4,mc[1]);

	draw_nankaLine(0,6,7,mc[0]);

	draw_nankaLine(-1,7,2,mc[1]);
	draw_nankaLine(1,7,1,mc[2]);
	draw_nankaLine(2,7,3,mc[1]);

	draw_nankaLine(-2,8,3,mc[1]);
	draw_nankaLine(1,8,1,mc[2]);
	draw_nankaLine(2,8,2,mc[1]);
	draw_nankaLine(4,8,1,mc[2]);
	draw_nankaLine(5,8,3,mc[1]);

	draw_nankaLine(-3,9,4,mc[1]);
	draw_nankaLine(1,9,4,mc[2]);
	draw_nankaLine(5,9,4,mc[1]);

	draw_nankaLine(-3,10,2,mc[0]);
	draw_nankaLine(-1,10,1,mc[1]);
	draw_nankaLine(0,10,1,mc[2]);
	draw_nankaLine(1,10,1,mc[0]);	
	draw_nankaLine(2,10,2,mc[2]);
	draw_nankaLine(4,10,1,mc[0]);
	draw_nankaLine(5,10,1,mc[2]);
	draw_nankaLine(6,10,1,mc[1]);
	draw_nankaLine(7,10,2,mc[0]);

	draw_nankaLine(-3,11,3,mc[0]);
	draw_nankaLine(0,11,6,mc[2]);
	draw_nankaLine(6,11,3,mc[0]);

	draw_nankaLine(-3,12,2,mc[0]);
	draw_nankaLine(-1,12,8,mc[2]);
	draw_nankaLine(7,12,2,mc[0]);

	draw_nankaLine(-1,13,3,mc[2]);
	draw_nankaLine(4,13,3,mc[2]);

	draw_nankaLine(-2,14,3,mc[1]);
	draw_nankaLine(5,14,3,mc[1]);

	draw_nankaLine(-3,15,4,mc[1]);
	draw_nankaLine(5,15,4,mc[1]);
	//glDisable(GL_COLOR_MATERIAL);
}

void display(void)
{	

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);


	/* 視点の移動 */
	setViewP(myposi,eyetarget,UpV);
	glEnable(GL_COLOR);
	objs.obj_axis(0.01,1.5);

	glDisable(GL_COLOR);
	
	///* 描画 */
	glPushMatrix();

	objs.obj_textureTest();
	glPopMatrix();
	//

	glutSwapBuffers();

}


void resize(int w, int h)
{
	/* マウスポインタ位置のウィンドウ内の相対的位置への換算用 */
	sx = M_PI / (double)w;
	sy = M_PI / (double)h;
	m_width=w;
	m_height=h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	setViewP(myposi,eyetarget,UpV);

}

void init(void)
{
	glClearColor(0.0, 0.0, 0.8, 1.0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, glfLightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, glfLightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, glfLightSpecular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, glfLightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glfLightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, glfLightSpecular);

	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable( GL_AUTO_NORMAL );
	glEnable(GL_COLOR_MATERIAL);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void key(unsigned char key , int x , int y) {
	switch(key){
		case 'u':myposi.y() +=1;setViewP(myposi,eyetarget,UpV);glutPostRedisplay();break;
		case 'd':myposi.y()-=1;setViewP(myposi,eyetarget,UpV);glutPostRedisplay();break;
		case 'n':if(np<=1)np+=0.1;glutPostRedisplay();break;
		case 'm':if(np>=0)np-=0.1;glutPostRedisplay();break;
		default:break;
	}
}


void skey(int key , int x , int y) {
	switch(key){
		case GLUT_KEY_LEFT:myposi.x()-=1;eyetarget.x()-=1;setViewP(myposi,eyetarget,UpV);glutPostRedisplay();break;
		case GLUT_KEY_UP:myposi.z()-=1;eyetarget.z()-=1;setViewP(myposi,eyetarget,UpV);glutPostRedisplay();break;
		case GLUT_KEY_RIGHT:myposi.x()+=1;eyetarget.x()+=1;setViewP(myposi,eyetarget,UpV);glutPostRedisplay();break;
		case GLUT_KEY_DOWN:myposi.z()+=1;eyetarget.z()+=1;setViewP(myposi,eyetarget,UpV);glutPostRedisplay();break;
		default:break;
	}

}

void Idle() {
	glutPostRedisplay();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(m_width,m_height);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
	glutKeyboardFunc(key);
	glutSpecialFunc(skey);
	glutIdleFunc(Idle);
	init();
	glutMainLoop();
	return 0;

}


/* ----------------------------------------------------------------------------------
     CG課題3：ドライビングシミュレーター
     2014/02/16
     5S 41 山谷佳祐
   ---------------------------------------------------------------------------------- */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "GLMetaseq.h"
#include "CGObjects.h"
#include "3DMATH.H"
#include "MyRacingCar.h"
#include <FTGL/ftgl.h>

#define FONT_FILE "C:\\Windows\\Fonts\\msgothic.ttc"
static FTPixmapFont* g_pFont;
unsigned long g_ulFontSize = 20;  //!< フォントサイズ
unsigned long g_ulGoalFontSize = 60;  //!< ゴールした後の表示用フォントサイズ

const GLdouble strcolor[][4] = {
	{ 0.16, 0.5, 0.73, 1.0},	// belize hole
	{ 0.56, 0.27, 0.68, 1.0},	// wisteria
	{ 0.75, 0.22, 0.169, 1.0},	// pomegranate
};

// 光源の位置
GLfloat light0pos[] = { 0.25, 1.0, 0.25, 1.0  };
GLfloat light1pos[] = { 0.0, -10.0, 0.0, 1.0 };

GLfloat m_groundColor1[4], m_groundColor2[4];

// 各光源の色
GLfloat glfLightAmbient[]  = {0.2, 0.2, 0.2, 1.0};
GLfloat glfLightDiffuse[]  = {0.2, 0.2, 0.2, 1.0};
GLfloat glfLightSpecular[] = {1.0, 1.0, 1.0, 1.0};

int windowWidth  = 800;
int windowHeight = 600;

GLfloat np = 1.0;

CRealvector myposi(3.0, 0.7, 0.5);	// カメラ位置(適当に調整)
CRealvector eyetarget(0.0, 0.2, 0.5);	// 注視点はランエボの初期位置
CRealvector upView = CRealvector(0, 1, 0);
static GLdouble mDscale = (myposi - eyetarget).length();
//CRealvector cardir(-1, 0, -10);		// 車の進行方向ベクトル

/*	デバッグ用の上からとかにするための視点
CRealvector myposi(5,20,6.2),eyetarget(0,0,0);
CRealvector upView = CRealvector(0, 1, 0);
static GLdouble mDscale = (myposi - eyetarget).length();
*/

static bool isShiftActive=false;
/* マウスの絶対位置→ウィンドウ内での相対位置の換算係数 */
static int cx, cy;
static double sx, sy;

//箱配置用のマージン（くっつかないようにする為の）
static const GLdouble margin = 0.005;

//0:NONE 1:LEFT 2:MIDDLE
static int isMouseDrag = 0;
// 1:flat, 2:smooth
int SHADINGMODE = 2;
// ゲームスタートと表示するかどうかに使うフラグ
bool isArrived = false;

CGObjects obj;
struct DataSet mydata;

MQO_MODEL crown;
MQO_MODEL AE86;
MQO_MODEL primera;
MQO_MODEL lapin;
MQO_MODEL barricade;
MQO_MODEL safetycone;
MQO_MODEL guardrail;
MQO_MODEL plant01;
MQO_MODEL plant02;
MQO_MODEL plant03;
MQO_MODEL plant04;
MQO_MODEL plant05;
MQO_MODEL dog;
MQO_MODEL cat;

// メタセコイアのモデルを変数に読み込む
void initModel()
{
	mqoInit();
	// 車関係
	crown = mqoCreateModel( "mqo/crown.mqo", 0.0035 );
	AE86 = mqoCreateModel( "mqo/86.mqo", 0.0015);
	primera = mqoCreateModel( "mqo/primera.mqo", 0.0029);
	lapin = mqoCreateModel( "mqo/lapin.mqo", 0.004);

	// 関係
	barricade = mqoCreateModel( "mqo/barricade.mqo", 0.0015);
	safetycone = mqoCreateModel( "mqo/safetycone.mqo", 0.001);
	guardrail = mqoCreateModel( "mqo/guardrail.mqo", 8);

	// 植物関係
	plant01 = mqoCreateModel( "mqo/plant01.mqo", 0.005);
	plant02 = mqoCreateModel( "mqo/plant02.mqo", 0.005);
	plant03 = mqoCreateModel( "mqo/plant03.mqo", 0.005);
	plant04 = mqoCreateModel( "mqo/plant04.mqo", 0.003);
	plant05 = mqoCreateModel( "mqo/plant05.mqo", 0.003);

	// 動物関係
	dog = mqoCreateModel( "mqo/dog.mqo", 0.002);
	cat = mqoCreateModel( "mqo/cat.mqo", 0.005);
}

// メタセコイアのモデルをすべて消す後処理を行う
void deleteAllModel()
{
	mqoDeleteModel( crown );
	mqoDeleteModel( AE86 );
	mqoDeleteModel( primera );
	mqoDeleteModel( lapin );
	mqoDeleteModel( barricade );
	mqoDeleteModel( safetycone );
	mqoDeleteModel( guardrail );
	mqoDeleteModel( plant01 );
	mqoDeleteModel( plant02 );
	mqoDeleteModel( plant03 );
	mqoDeleteModel( plant04 );
	mqoDeleteModel( plant05 );
	mqoDeleteModel( dog );
	mqoDeleteModel( cat );
	mqoCleanup();
}


/*!
 * 文字列描画
 * @param[in] str 文字列
 * @param[in] w,h ウィンドウサイズ
 * @param[in] x0,y0 文字列の位置(左上原点のスクリーン座標系,文字列の左下がこの位置になる)
 */
void DrawStrings(vector<wstring> &strs, int w, int h, int x0, int y0)
{
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
	
    glRasterPos2f(x0, y0);

    // フォントの初期化
    if(!g_pFont){
        g_pFont = new FTPixmapFont(FONT_FILE);
        if(g_pFont->Error()){
            cout << "Failed to open font " << FONT_FILE << endl;
            delete g_pFont;
            g_pFont = 0;
        }
        else{
			if (isArrived == true) {
				g_pFont->FaceSize(g_ulGoalFontSize);
			} else {
				g_pFont->FaceSize(g_ulFontSize);
			}
        }
    }

    // FTGLで文字列を描画
    if(g_pFont){
        for(int j = 0; j < (int)strs.size(); ++j){
            glRasterPos2f(x0, y0);
            strs[j].push_back('\0');
            g_pFont->Render(strs[j].c_str());
            y0 += g_pFont->LineHeight();
        }
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
}

void setViewPosition(const CRealvector &_eyeP, const CRealvector &_toP, const CRealvector &_upV)
{
	gluLookAt(_eyeP.x(),_eyeP.y(),_eyeP.z(),_toP.x(),_toP.y(),_toP.z(),_upV.x(),_upV.y(),_upV.z());
}

void setViewObjective(CRealvector &myposi, CRealvector &targetPosi, double dx, double dy)
{
	//printf("%f,%f\n",dx,dy);
	CRealvector toeyeP = targetPosi;
	CRealvector eyeP = myposi;

	//double m_leye=mDscale*(eyeP-toeyeP).length();
	double m_leye = mDscale;
	CRealvector tVV = m_leye * (toeyeP - eyeP).normalized();
	double m_eye_th1 = asin(tVV.y() / mDscale);
	double m_eye_th2 = atan2(tVV.x(), tVV.z());

	m_eye_th1 += (float)dy;
	m_eye_th2 += (float)dx;

	if (0.4999 * M_PI < m_eye_th1) m_eye_th1 = 0.4999 * M_PI;
	if (m_eye_th1 < -0.4999 * M_PI) m_eye_th1 = -0.4999 * M_PI;

	double lxz;
	CRealvector tmpV;
	tmpV.y() = m_leye * sin(m_eye_th1); 
	lxz = m_leye * cos(m_eye_th1);
	tmpV.x() = lxz * sin(m_eye_th2); 
	tmpV.z() = lxz * cos(m_eye_th2);
	eyeP = -tmpV + toeyeP;

	eyetarget = toeyeP;
	myposi = eyeP;
}

void init()
{
	mydata.player = new PlayerCar("player", 30);
	printf("/*--- Driving Simulation ver. 1.0 ---*/\n\n");
	printf("1本道をどんどん前へ進んでゴールのチェッカーボードを目指そう!!\n\n");
	printf("/*--- 操作方法 ---*/\n");
    printf(" ↑キー : アクセル,  ↓キー : ブレーキ\n");
    printf(" ←キー : 左へ曲がる,  →キー : 右へ曲がる\n");

	// ライティング(光源)の設定
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	// 光をあてる
	glLightfv(GL_LIGHT0, GL_AMBIENT,  glfLightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  glfLightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, glfLightSpecular);
	glLightfv(GL_LIGHT1, GL_AMBIENT,  glfLightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  glfLightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, glfLightSpecular);

	//glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);        // デプスバッファ有効
	glEnable( GL_AUTO_NORMAL );
	glEnable(GL_NORMALIZE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// シェーディングの設定
	if (SHADINGMODE)
		glShadeModel(GL_FLAT);      // フラットシェーディング
	else
		glShadeModel(GL_SMOOTH);    // スムーズシェーディング
	initModel();
	glClearColor(0.63f, 0.85f, 0.94f, 1.0f);       // ウィンドウ背景の色の設定
}

void reshape(int w, int h)
{
	// マウスポインタ位置のウィンドウ内の相対的位置への換算用
	sx = M_PI / (double)w;
	sy = M_PI / (double)h;
	windowWidth = w;
	windowHeight = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	setViewPosition(myposi, eyetarget, upView);
}

void renderScene()
{
	glPushMatrix();
	obj.renderEnv();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-8.0, 0.0, 0.5);
	glRotated(-90.0, 0.0, 1.0, 0.0);
	mqoCallModel( primera );
	glPopMatrix();

	glPushMatrix();
	glTranslated(10.0, 0.2, 0.5);
	glRotated(-90.0, 0.0, 1.0, 0.0);
	mqoCallModel( crown );
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.0, 0.21, -0.5);
	glRotated(90.0, 0.0, 1.0, 0.0);
	mqoCallModel( AE86 );
	glPopMatrix();

	glPushMatrix();
	glTranslated(17.0, 0.0, -0.5);
	glRotated(90.0, 0.0, 1.0, 0.0);
	mqoCallModel( lapin );
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.3, 4.0);
	mqoCallModel(barricade);
	glTranslated(-0.5, -0.03, 1.0);
	mqoCallModel(safetycone);
	glTranslated(1.2, 0.0, 0.0);
	mqoCallModel(safetycone);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.3, -4.0);
	mqoCallModel(barricade);
	glTranslated(-0.5, -0.03, -1.0);
	mqoCallModel(safetycone);
	glTranslated(1.2, 0.0, 0.0);
	mqoCallModel(safetycone);
	glPopMatrix();

	// 左上の花
	for (double i = -4.0; i > -8.0; i -= 1.0) {
		for (double j = -10.0; j < -2.0; j += 0.5) {
		
			glPushMatrix();
			glTranslated(j, 0.0, i);
			mqoCallModel(plant01);
			glPopMatrix();
		}
	}
	for (double i = -8.0; i > -12.0; i -= 1.0) {
		for (double j = -10.0; j < -2.0; j += 0.5) {
		
			glPushMatrix();
			glTranslated(j, 0.0, i);
			mqoCallModel(plant03);
			glPopMatrix();
		}
	}

	// 右下の花
	for (double i = -4.0; i > -8.0; i -= 1.0) {
		for (double j = 4.0; j < 10.0; j += 0.5) {
		
			glPushMatrix();
			glTranslated(j, 0.0, i);
			mqoCallModel(plant01);
			glPopMatrix();
		}
	}
	for (double i = -8.0; i > -12.0; i -= 1.0) {
		for (double j = 4.0; j < 10.0; j += 0.5) {
		
			glPushMatrix();
			glTranslated(j, 0.0, i);
			mqoCallModel(plant03);
			glPopMatrix();
		}
	}
	
	// 左下の花
	for (double i = 4.0; i < 8.0; i += 1.0) {
		for (double j = -10.0; j < -2.0; j += 0.5) {
		
			glPushMatrix();
			glTranslated(j, 0.0, i);
			mqoCallModel(plant02);
			glPopMatrix();
		}
	}
	// 右下の草と牛
	for (double i = 3.0; i < 10.0; i += 1.0) {
		for (double j = 3.0; j < 10.0; j += 0.5) {
		
			glPushMatrix();
			glTranslated(j, 0.0, i);
			mqoCallModel(plant05);
			glPopMatrix();
		}
	}
	
	// 犬
	glPushMatrix();
	glTranslated(6.0, 0.4, 1.5);
	glRotated(-90.0, 0.0, 1.0, 0.0);
	mqoCallModel( dog );
	glPopMatrix();

	// 猫
	glPushMatrix();
	glTranslated(-4.0, 0.0, -2.0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	mqoCallModel( cat );
	glPopMatrix();
	glPushMatrix();
	glTranslated(-6.0, 0.0, -2.0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	mqoCallModel( cat );
	glPopMatrix();
}

void display()
{
	GLfloat color[][4] = {
		{ 0.3, 0.3, 0.3, 1.0 },
		{ 0.8, 0.8, 0.8, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// ウィンドウを塗りつぶす

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

	// 視点の移動
	float xx, yy, zz;
	mydata.player->getpos(xx, yy, zz);
	float th = mydata.player->getdir();
	if (th != 0) {
		myposi.z() = 0.5 + zz;
		eyetarget.z() = 0.5 + zz;
		myposi.x() = 3.0 + xx;
		eyetarget.x() = 0.0 + xx;
	} else {
		// 角度に変化がない→直進
		myposi.x() = 3.0 + xx;
		eyetarget.x() = 0.0 + xx;
	}
	if (xx < -175) {
		isArrived = true;
	}
	
	setViewPosition(myposi, eyetarget, upView);

	glPushMatrix();
	renderScene();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-40.0, 0.0, 0.0);
	renderScene();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-80.0, 0.0, 0.0);
	renderScene();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-120.0, 0.0, 0.0);
	renderScene();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-160.0, 0.0, 0.0);
	renderScene();
	glPopMatrix();
	updateData(&mydata);
	mydata.player->draw();
	glPushMatrix();
	glTranslated(-175.0, 0.05, 0.0);
	obj.obj_checker();
	glPopMatrix();

	vector<wstring> strs;
	vector<wstring> goal;
    strs.push_back(L"Driving Simulation ver. 1.0");
    strs.push_back(L" ↑キー : アクセル,  ↓キー : ブレーキ");
    strs.push_back(L" ←キー : 左へ曲がる,  →キー : 右へ曲がる");
	if (isArrived == true) {
		goal.push_back(L" Goal !!");
		glColor3dv(strcolor[2]);
		DrawStrings(goal, windowWidth, windowHeight, 10, 18);
	} else {
		glColor3dv(strcolor[1]);
		DrawStrings(strs, windowWidth, windowHeight, 10, 18);
	}
	

	glutSwapBuffers();
}

void idle(void)
{
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) 
	{
	case GLUT_LEFT_BUTTON:
		switch (state) 
		{
		case GLUT_DOWN:
			// ドラッグ開始点位置を記録する
			cx = x;
			cy = y;
			// アニメーション開始
			isMouseDrag = 1;
			glutIdleFunc(idle);
			break;
		case GLUT_UP:
			// アニメーション終了
			isMouseDrag = 0;
			glutIdleFunc(0);
			break;
		default:
			break;
		}		 
		break;

	case GLUT_RIGHT_BUTTON:
		switch (state)
		{
		case GLUT_DOWN:
			// ドラッグ開始点位置を記録する
			cx = x;
			cy = y;
			// アニメーション開始
			isMouseDrag = 2;
			glutIdleFunc(idle);
			break;
		case GLUT_UP:
			// アニメーション終了
			isMouseDrag = 0;
			glutIdleFunc(0);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void motion(int x, int y)
{	
	// マウスポインタの位置のドラッグ開始位置からの変位 (相対値)d
	if (isMouseDrag == 1)
	{
		double dx = (double)(x - cx) * sx;
		double dy = -(double)(y - cy) * sy;
		cx = x;
		cy = y;
		setViewObjective(myposi, eyetarget, dx, dy);
	}
	else if (isMouseDrag == 2)
	{	
		double dx = (double)(x - cx) * sx;
		double dy = (double)(y - cy) * sy;
		cx = x;
		cy = y;
		double msc = 0.1;
		if (isShiftActive)	msc = 0.5;
		else msc = 0.2;
		double adist = sqrt(dx * dx + dy * dy);
		if (dy < 0)
		{
			mDscale -= msc;
		}
		else
		{
			mDscale += msc;
		}
		if (mDscale < 0.1)	mDscale = 0.1;
		//printf("%f\n",mDscale);
		setViewObjective(myposi, eyetarget, 0, 0);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'u':
		myposi.y() += 1;
		setViewPosition(myposi,eyetarget,upView);
		glutPostRedisplay();
		break;
	case 'd':
		myposi.y() -= 1;
		setViewPosition(myposi, eyetarget, upView);
		glutPostRedisplay();
		break;
	case 'n':
		if(np <= 1)		np += 0.1;
		glutPostRedisplay();
		break;
	case 'm':
		if(np >= 0)		np -= 0.1;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void specialKey(int key, int x, int y)
{
	mydata.player->control(&mydata, key);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Driving Simulator");
	init();
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSpecialFunc(specialKey);
	glutIdleFunc(idle);
	glutMainLoop();
	ReleaseDataSet(&mydata);
	deleteAllModel();
	return 0;
}


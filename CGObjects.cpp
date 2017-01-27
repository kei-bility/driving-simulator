
#include "ppmload.h"
#include "CGObjects.h"
#include "3DMATH.H"

CGObjects::CGObjects()
{

}

// 世界の中心に平面を書くための関数
// x-z平面上に幅w、高さdの平面を描画する
// 色を決めるための機能はないので関数を呼ぶ前に色を指定しなければならない
void CGObjects::obj_plane(GLdouble w, GLdouble d)
{
	GLdouble norm[3];
	glPushMatrix();	// 全体の世界に影響を与えないために行列をよけておく
	glBegin(GL_POLYGON);	// 多角形を描画するために頂点リストを並べますよ
	norm[0] = 1; norm[1] = 0; norm[2] = 0;	// 面の向きベクトル(法線ベクトル)の決定
	glNormal3dv(norm);		// 法線ベクトルを指定
	glVertex3d(w/2.0, 0, d/2.0);	// 右回りに頂点を決めていく:1点目
	glVertex3d(w/2.0, 0, -d/2.0);	// 右回りに頂点を決めていく:2点目
	glVertex3d(-w/2.0, 0, -d/2.0);	// 右回りに頂点を決めていく:3点目
	glVertex3d(-w/2.0, 0, d/2.0);	// 右回りに頂点を決めていく:4点目
	glEnd();	// ここで頂点リストは終わりですよ
	glPopMatrix();	// 行列を元に戻す
}

// 世界の中心にチェッカーボードを書くための関数
// x-z平面に幅w、高さdの平面を書く関数obj_planeを使って
// チェッカーボードを作る
void CGObjects::obj_checker(void)
{
	// チェッカーボードの色は白黒
	GLfloat color[][4] = {
		{0.0, 0.0, 0.0, 1.0}, // 黒
		{1.0, 1.0, 1.0, 1.0},	// 白
	};
	int bWB = 0;
	double i, j;
	for (i = -4; i < 4; i += 0.05) {
		for (j = -1; j < 1; j += 0.05) {
			glPushMatrix();
			
			if (bWB) 
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
			else 
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[1]);
			bWB = !bWB;
			// 箱の幅のぶんだけずらす
			// y方向つまり高さ方向にはずらさない
			glTranslated(i, 0, j);
			obj_plane(0.05, 0.05);
			glPopMatrix();
		}
		bWB = (int)abs(bWB - 1);
	}
}

// 直方体を書くための関数
// 横幅nx、高さny、縦幅nzの直方体を描画
// 色をつける機能はなし
void CGObjects::obj_cube(GLdouble nx, GLdouble ny, GLdouble nz)
{
	// 頂点の定義
	GLdouble vertex[][3] = {
		{-0.5, 0.0, -0.5},
		{0.5, 0.0, -0.5},
		{0.5, 1.0, -0.5},
		{-0.5, 1.0, -0.5},
		{-0.5, 0.0, 0.5},
		{0.5, 0.0, 0.5},
		{0.5, 1.0, 0.5},
		{-0.5, 1.0, 0.5}
	};
	// 面の定義
	int face[][4] = {
		{0, 1, 2, 3},
		{1, 5, 6, 2},
		{5, 4, 7, 6},
		{4, 0, 3, 7},
		{4, 5, 1, 0},
		{3, 2, 6, 7}
	};
	GLdouble normal[][3] = {
		{0.0, 0.0, -1.0},
		{1.0, 0.0, 0.0},
		{0.0, 0.0, 1.0},
		{-1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0},
		{0.0, 1.0, 0.0}
	};
	int i, j;
	glBegin(GL_QUADS);
	for (j = 0; j < 6; j++) {
		glNormal3dv(normal[j]);
		for (i = 0; i < 4; i++) {
			glVertex3d(vertex[face[j][i]][0] * nx,
				vertex[face[j][i]][1] * ny,
				vertex[face[j][i]][2] * nz);
		}
	}
	glEnd();
}

// 円柱
void CGObjects::cylinder(float radius, float height, int sides)
{
	double pi = 3.1415;
	//上面
	glNormal3d(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for(double i = 0; i < sides; i++) {
		double t = pi*2/sides * (double)i;
		glVertex3d(radius * cos(t), height, radius * sin(t));
	}
	glEnd();
	//側面
	glBegin(GL_QUAD_STRIP);
	for(double i=0;i<=sides;i=i+1){
		double t = i*2*pi/sides;
		glNormal3f((GLfloat)cos(t),0.0,(GLfloat)sin(t));
		glVertex3f((GLfloat)(radius*cos(t)),-height,(GLfloat)(radius*sin(t)));
		glVertex3f((GLfloat)(radius*cos(t)),height,(GLfloat)(radius*sin(t)));
	}
	glEnd();
	//下面
	glNormal3d(0.0, -1.0, 0.0);
	glBegin(GL_POLYGON);
	for(double i = sides; i >= 0; --i) {
		double t = pi*2/sides * (double)i;
		glVertex3d(radius * cos(t), -height, radius * sin(t));
	}
	glEnd();
}

void CGObjects::obj_depthtest(GLdouble nx, GLdouble ny, GLdouble nz){
	GLdouble vertex[][3] = {
		{  0.0,  1.0,  0.0 },
		{ -0.5,  0.0, -0.5 },
		{  0.5,  0.0, -0.5 },
		{  0.5,  0.0,  0.5 },
		{ -0.5,  0.0,  0.5 },
	};
	int face[][4] = {
		{ 0, 1, 2, 0 },
		{ 0, 2, 3, 0 },
		{ 0, 3, 4, 0 },
		{ 0, 4, 1, 0 },
		{ 1, 4, 3, 2 },
	};

	GLdouble normal[][3] = {
		{  0.0,  1.0, -1.0 },
		{  1.0,  1.0,  0.0 },
		{  0.0,  1.0,  1.0 },
		{ -1.0,  1.0,  0.0 },
	};

	GLdouble iro[][3] = {
		{  0.0,  1.0, 1.0 },
		{  1.0,  1.0,  0.0 },
		{  0.0,  1.0,  1.0 },
		{  1.0,  1.0,  0.0 },
	};

	int i, j;

	glBegin(GL_TRIANGLES);
	for(j = 0; j < 4; j++)	{
		glColor3dv(iro[j]);
		glNormal3dv(normal[j]);
		for(i = 0; i < 3; i++){
			glVertex3d(vertex[face[j][i]][0] * nx,
				vertex[face[j][i]][1] * ny,
				vertex[face[j][i]][2] * nz);
		}
	}
	glEnd();
	glBegin(GL_QUADS);
	glNormal3d(0.0, -1.0, 0.0);
	for(i = 0; i < 4; i++)	{
		glVertex3d(vertex[face[j][i]][0] * nx,
			vertex[face[j][i]][1] * ny,
			vertex[face[j][i]][2] * nz);
	}
	glEnd();
}

struct ppmimg *image=NULL;
GLenum minfilter = GL_NEAREST;
GLenum magfilter = GL_NEAREST;
GLenum env = GL_MODULATE;

GLenum wraps = GL_REPEAT;
GLenum wrapt = GL_REPEAT;

void CGObjects::obj_textureTest(void){
	static bool istexloaded=false;

	glDisable(GL_COLOR);
	if(!istexloaded){
		if(image==NULL)image=(struct ppmimg*)malloc(sizeof(ppmimg));
		image->dat=NULL;
		loadppmimage("mesi1.ppm",image);
		istexloaded=true;

		/* テクスチャ画像はバイト単位に詰め込まれている */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, env);

		/* テクスチャの割り当て */
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->iwidth, image->iheight, GL_RGB, GL_UNSIGNED_BYTE, image->dat);
	}
	glEnable(GL_TEXTURE_2D);
	double w=2.0,d=2.0;
	GLdouble norm[3];
	glPushMatrix();
	glBegin( GL_POLYGON );
	norm[0]=1;norm[1]=0;norm[2]=0;
	glNormal3dv (norm);
	glTexCoord2f(0.0, 0.0); glVertex3d( w/2.0 , 0 ,   d/2.0 );
	glTexCoord2f(0.0, 1.0); glVertex3d( w/2.0 , 0 ,  -d/2.0  );
	glTexCoord2f(1.0, 1.0); glVertex3d(-w/2.0 , 0 ,  -d/2.0  );
	glTexCoord2f(1.0, 0.0); glVertex3d(-w/2.0 , 0 ,   d/2.0  );
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}


void CGObjects::obj_stone(){
	GLfloat color[][4] = {
		{ 0.4, 0.4, 0.4, 1.0 },
		{ 0.8, 0.8, 0.8, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};

	//int i, j;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
	for(int i = 0; i < 40; i += 2)
	{
		if(i!=20 && i!=18){
			glPushMatrix();
			glTranslated(-5.95,0.002,-24.5+i);
			obj_cube(0.1, 0.125, 0.4);
			glPopMatrix();
		}
	}
}

void CGObjects::obj_signal(){
	GLfloat color[][4] = {
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 0.0, 0.8, 0.4, 1.0 },
		{ 0.7, 0.7, 0.0, 1.0 },
		{ 0.8, 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};

	int i;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[4]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);

	glPushMatrix();
	glTranslated(0.01, 0.0, 0.0);
	obj_cube(0.02, 0.8, 0.02);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.15, 0.8, 0.005);
	obj_cube(0.3, 0.1, 0.05);
	glPopMatrix();


	for(i = 0; i < 3; i++)
	{
		glPushMatrix();
		glTranslated(i * 0.1, 0.0, 0.0);


		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[i+1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[4]);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
		if(i == 0) glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color[i+1]);

		glPushMatrix();
		glTranslated(0.05, 0.81, 0.025);
		obj_cube(0.08, 0.08, 0.05);
		glPopMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color[4]);


		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[4]);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);

		glPushMatrix();
		glTranslated(0.05, 0.88, 0.05);
		obj_cube(0.08, 0.01, 0.1);
		glPopMatrix();


		glPopMatrix();
	}
}

void CGObjects::obj_house(){
	GLfloat color[][4] = {
		{ 0.0, 0.0, 0.8, 1.0 },
		{ 0.8, 0.7, 0.4, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
	glPushMatrix();
	glTranslated(0.0, 0.5, 0.0);
	obj_pyramid(1.2, 0.3, 1.2);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
	obj_cube(1.0, 0.5, 1.0);
}


void CGObjects::obj_building(){
	GLfloat color[][4] = {
		{ 0.5, 0.5, 0.6, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[1]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);

	obj_cube(2.0, 4.0, 1.0);
}

void CGObjects::obj_ground(){
	//GLfloat color[][4] = {
	//	{ 0.0, 0.3, 0.1, 1.0 },
	//	{ 0.0, 0.0, 0.0, 1.0 },
	//};
	GLfloat color[][4] = {
		{ 0.0, 0.3, 0.1, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};


	int i, j;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[1]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);

	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	/*
	glVertex3d(-100.0,  0.0, -100.0);
	glVertex3d( 100.0,  0.0, -100.0);
	glVertex3d( 100.0,  0.0,  100.0);
	glVertex3d(-100.0,  0.0,  100.0);
	*/
	for(i = 0; i < 40; i++)
	{
		for(j = 0; j < 40; j++)
		{
			glVertex3d(-20.0 + i,  0.0, -20.0 + j);
			glVertex3d(-19.0 + i,  0.0, -20.0 + j);
			glVertex3d(-19.0 + i,  0.0, -19.0 + j);
			glVertex3d(-20.0 + i,  0.0, -19.0 + j);
		}
	}
	glEnd();
}

void CGObjects::renderEnv(void){
	glPushMatrix();
	//glTranslatef(0,0,-1.5);
	//glRotatef(90,1,0,0);



	glPushMatrix();
	glTranslated(5.0, 0.0, 5.0);
	obj_stone();
	glPopMatrix();


	glPushMatrix();
	obj_ground();
	glPopMatrix();

	glPushMatrix();
	obj_road();
	glPopMatrix();

	glPushMatrix();
	glRotated((GLdouble)90, 0.0, 1.0, 0.0);
	obj_road();
	glPopMatrix();
	for(int i = -10; i < 10; i++)
	{

		if(i >= -1 && i <= 1) { continue; }

		for(int j = 0; j < 2; j++)
		{
			glPushMatrix();

			glTranslated(j*3.0 - 1.6, 0.0, i*2.0 + 1.0);
			obj_tree();
			glTranslated(0.25, 0.0, 0.25);
			obj_tree();
			glTranslated(0.25, 0.0, -0.25);
			obj_tree();

			glPopMatrix();
		}
	}


	glPushMatrix();
	glTranslated(-1.2, 0.0, -1.2);
	obj_signal();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.2, 0.0, 1.2);
	glRotated((GLdouble)180, 0.0, 1.0, 0.0);
	obj_signal();
	glPopMatrix();


	glPushMatrix();
	glTranslated(1.8, 0.0, -1.8);
	for(int i = 0; i < 5; i++)
	{
		obj_house();
		glTranslated(1.3, 0.0, 0.5 * ((i%2)*2-1));
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(12.8, 0.0, 2.4);
	for(int i = 0; i < 5; i++)
	{
		obj_house();
		glTranslated(1.3, 0.0, 0.5 * ((i%2)*2-1));
	}
	glPopMatrix();


	glPushMatrix();
	glTranslated(-1.8, 0.0, 1.8);
	obj_house();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-8.0, 0.0, 2.0);
	obj_building();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-12.0, 0.0, 2.0);
	obj_building();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-8.0, 0.0, -2.0);
	obj_building();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-12.0, 0.0, -2.0);
	obj_building();
	glPopMatrix();

	glPopMatrix();
}


void CGObjects::SetGroundColor(GLfloat *col1,GLfloat *col2){
	for(int i=0;i<4;i++){
		m_groundColor1[i]=col1[i];
		m_groundColor2[i]=col2[i];
	}
}



void CGObjects::renderOnlyGround(void){
	//GLfloat color[][4] = {
	//	{ 0.42, 0.572, 0.765, 1.0 },
	//	{ 0.243, 0.271, 0.288, 1.0 },
	//	{ 0.0, 0.0, 0.0, 1.0 },

	//};
	GLfloat color[][4] = {
		{ 0.22, 0.872, 0.265, 1.0 },
		{ 0.843, 0.271, 0.288, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },

	};
	SetGroundColor(color[2],color[1]);
	glPushMatrix();
	//glTranslatef(0,0,-1.5);
	//glRotatef(90,1,0,0);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[1]);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);

	glBegin(GL_QUADS);
	//glNormal3d(0.0, 1.0, 0.0);
	/*
	glVertex3d(-100.0,  0.0, -100.0);
	glVertex3d( 100.0,  0.0, -100.0);
	glVertex3d( 100.0,  0.0,  100.0);
	glVertex3d(-100.0,  0.0,  100.0);
	*/
	bool bWB=false;
	glNormal3d(0.0, 1.0, 0.0);
	for(double i = 0; i < 40; i+=0.5)
	{
		for(double j = 0; j < 40; j+=0.5)
		{
			if(bWB){
				//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
				//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_groundColor1);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 6.0);
			}else{
				//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[1]);
				//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,m_groundColor2);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 6.0);
			}
			bWB=!bWB;
			//glVertex3d(-20.0 + i,  0.0, -20.0 + j);
			//glVertex3d(-19.0 + i,  0.0, -20.0 + j);
			//glVertex3d(-19.0 + i,  0.0, -19.0 + j);
			//glVertex3d(-20.0 + i,  0.0, -19.0 + j);
			glVertex3d(-20.0 + i,  0.0, -20.0 + j);
			glVertex3d(-20.0 + i,  0.0, -19.5 + j);
			glVertex3d(-19.5 + i,  0.0, -19.5 + j);
			glVertex3d(-19.5 + i,  0.0, -20.0 + j);
		}
		bWB=!bWB;

	}
	glEnd();
	glPopMatrix();
}


void CGObjects::renderKyu(double r,int vx,int vy){
	GLfloat color[][4] = {
		{ 0.22, 0.872, 0.265, 1.0 },
		{ 0.843, 0.271, 0.288, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },

	};
	CRealvector **kyuposi;
	kyuposi=(CRealvector **)malloc(sizeof(CRealvector *)*(vy+2));
	for(int i=0;i<vy+2;i++){
		kyuposi[i] = (CRealvector *)malloc(sizeof(CRealvector)*(vx+2));
	}
	kyuposi[0][0].x() = 0;
	kyuposi[0][0].y() = r;
	kyuposi[0][0].z() = 0;
	for(int i=1;i < vy + 1;i++){
		kyuposi[0][i].x() = -r*sin((double)i/vy*G_PI);
		kyuposi[0][i].y() = r*cos((double)i/vy*G_PI);
		kyuposi[0][i].z() = 0;
	}

	for(int i = 0; i < vy+1; i++)
	{
		for(int j = 0; j < vx+1; j++){
			//if(i == 0){
			//	kyuposi[j][i].x() = 0;
			//	kyuposi[j][i].y() = r;
			//	kyuposi[j][i].z() = 0;
			//}else if(i == vy-1){
			//	kyuposi[j][i].x() = 0;
			//	kyuposi[j][i].y() = -r;
			//	kyuposi[j][i].z() = 0;
			//}else{
			kyuposi[j][i].x() = kyuposi[0][i].x()*cos((double)j/vx*G_DOUBLE_PI) + kyuposi[0][i].z()*sin((double)j/vx*G_DOUBLE_PI);
			kyuposi[j][i].y() = kyuposi[0][i].y();
			kyuposi[j][i].z() = -kyuposi[0][i].x()*sin((double)j/vx*G_DOUBLE_PI) + kyuposi[0][i].z()*cos((double)j/vx*G_DOUBLE_PI);	
			//}
			if(fabs(kyuposi[j][i].x()) < 1e-5)kyuposi[j][i].x()=0;
			if(fabs(kyuposi[j][i].y()) < 1e-5)kyuposi[j][i].y()=0;
			if(fabs(kyuposi[j][i].z()) < 1e-5)kyuposi[j][i].z()=0;
		}
	}
	
	
	glPushMatrix();
	
	for(int i = 0; i < vy; i++)
	{
		for(int j = 0; j < vx; j++)
		{
			//if(bWB){
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[1]);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 6.0);
			//}else{

			//}
			//bWB=!bWB;
			CRealvector v1,v2,pv;
			v1 = kyuposi[j][i] - kyuposi[j+1][i];
			v2 = kyuposi[j][i+1] - kyuposi[j+1][i];
			pv = v1^v2;
			//glEnable(GL_COLOR_MATERIAL);
			//glColor3d(1.0,0,0);
			
			glBegin(GL_TRIANGLES);

			glNormal3fv(pv);
			glVertex3d(kyuposi[j][i][0],  kyuposi[j][i][1], kyuposi[j][i][2]);
			glVertex3d(kyuposi[j+1][i][0],  kyuposi[j+1][i][1], kyuposi[j+1][i][2]);
			glVertex3d(kyuposi[j][i+1][0],  kyuposi[j][i+1][1], kyuposi[j][i+1][2]);
			glEnd();
			//glColor3d(0.0,1,0);
			
			v1 = kyuposi[j+1][i+1] - kyuposi[j+1][i];
			v2 =  kyuposi[j+1][i+1] - kyuposi[j][i+1];
			pv = v1^v2;
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,color[1]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 6.0);
			
			glBegin(GL_TRIANGLES);
			glNormal3fv(pv);
			glVertex3d(kyuposi[j+1][i][0],  kyuposi[j+1][i][1], kyuposi[j+1][i][2]);
			glVertex3d(kyuposi[j+1][i+1][0],  kyuposi[j+1][i+1][1], kyuposi[j+1][i+1][2]);
			glVertex3d(kyuposi[j][i+1][0],  kyuposi[j][i+1][1], kyuposi[j][i+1][2]);
			glEnd();
			//glDisable(GL_COLOR_MATERIAL);
		}
		//bWB=!bWB;

	}

	glPopMatrix();
}

void CGObjects::RenderingScene(void){
	if(m_SceneMode){
		renderEnv();
	}else{
		renderOnlyGround();
	}
}

GLint CGObjects::DrawBox(GLdouble d, GLdouble w, GLdouble h)
{
	GLdouble norm[3];

	glPushMatrix();
	glTranslated(-d/2.0,-w/2.0,-h/2.0);
	glBegin( GL_POLYGON );
	norm[0]=0;norm[1]=-1;norm[2]=0;
	glNormal3dv (norm);
	glVertex3d( 0 , 0 , 0 );
	glVertex3d( d , 0 , 0 );
	glVertex3d( d , 0 , h );
	glVertex3d( 0 , 0 , h );
	glEnd();
	glBegin( GL_POLYGON );
	norm[0]=0;norm[1]=-1;norm[2]=0;
	glNormal3dv (norm);
	glVertex3d( 0 , w , 0 );
	glVertex3d( d , w , 0 );
	glVertex3d( d , w , h );
	glVertex3d( 0 , w , h );
	glEnd();

	glBegin( GL_POLYGON );
	norm[0]=0;norm[1]=0;norm[2]=1;
	glNormal3dv (norm);
	glVertex3d( 0 , 0 , h );
	glVertex3d( d , 0 , h );
	glVertex3d( d , w , h );
	glVertex3d( 0 , w , h );
	glEnd();
	glBegin( GL_POLYGON );
	norm[0]=0;norm[1]=0;norm[2]=1;
	glNormal3dv (norm);
	glVertex3d( 0 , 0 , 0 );
	glVertex3d( d , 0 , 0 );
	glVertex3d( d , w , 0 );
	glVertex3d( 0 , w , 0 );
	glEnd();

	glBegin( GL_POLYGON );
	norm[0]=-1;norm[1]=0;norm[2]=0;
	glNormal3dv (norm);
	glVertex3d( 0 , 0 , 0 );
	glVertex3d( 0 , 0 , h );
	glVertex3d( 0 , w , h );
	glVertex3d( 0 , w , 0 );
	glEnd();
	glBegin( GL_POLYGON );
	norm[0]=-1;norm[1]=0;norm[2]=0;
	glNormal3dv (norm);
	glVertex3d( d , 0 , 0 );
	glVertex3d( d , 0 , h );
	glVertex3d( d , w , h );
	glVertex3d( d , w , 0 );
	glEnd();
	glPopMatrix();

	return(0) ;

}

void CGObjects::obj_zroad(void){

	GLfloat color[][4] = {
		{ 0.3, 0.3, 0.3, 1.0 },
		{ 0.8, 0.8, 0.8, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};

	glPushMatrix();
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 40; j++){
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
			glBegin(GL_QUADS);
			glNormal3d(0.0, 1.0, 0.0);

			glVertex3d(  -1.0 + i,  0.001, -20.0 + j);
			glNormal3d(0.0, 1.0, 0.0);
			glVertex3d(   0.0 + i,  0.001, -20.0 + j);
			glNormal3d(0.0, 1.0, 0.0);
			glVertex3d(   0.0 + i,  0.001, -19.0 + j);
			glNormal3d(0.0, 1.0, 0.0);
			glVertex3d(  -1.0 + i,  0.001, -19.0 + j);
			glEnd();
		}
	}


	for(int i = 0; i < 40; i += 2){
		glBegin(GL_QUADS);
		glNormal3d(0.0, 1.0, 0.0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
		glVertex3d(  -0.02,  0.002, -19.5 + i);
		glVertex3d(   0.02,  0.002, -19.5 + i);
		glVertex3d(   0.02,  0.002, -18.5 + i);
		glVertex3d(  -0.02,  0.002, -18.5 + i);	
		glEnd();

	}
	glPopMatrix();
}

void CGObjects::set_car()
{
	// まずタイヤを置く
	// 進行方向に対して
	// 左後輪、左前輪、右後輪、右前輪の順
	glPushMatrix();
	glRotated(90, 1.0f, 0.0f, 0.0f);
	glRotated(90, 0.0f, 0.0f, 1.0f);
	glTranslated(-0.1, 0.3, -0.16);
	cylinder(0.15, 0.05, 15);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1.0f, 0.0f, 0.0f);
	glRotated(90, 0.0f, 0.0f, 1.0f);
	glTranslated(-1.1, 0.3, -0.16);
	cylinder(0.15, 0.05, 15);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1.0f, 0.0f, 0.0f);
	glRotated(90, 0.0f, 0.0f, 1.0f);
	glTranslated(-0.1, -0.3, -0.16);
	cylinder(0.15, 0.05, 15);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1.0f, 0.0f, 0.0f);
	glRotated(90, 0.0f, 0.0f, 1.0f);
	glTranslated(-1.1, -0.3, -0.16);
	cylinder(0.15, 0.05, 15);
	glPopMatrix();

	// タイヤの上にはこを２つおく
	// タイヤの色
	GLfloat car_color[][4] = {
		{0.8, 0.8, 0.8, 0.8},
		{0.17, 0.24, 0.31, 1.0},
	};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, car_color[0]);
	glPushMatrix();
	glTranslated(0.0, 0.3, -0.6);
    //glScalef(1.0f, 1.0f, 1.0f);

	obj_cube(0.75, 0.2, 1.5);
    //glutSolidCube(0.5f);
    glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.5, -0.6);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, car_color[1]);
	obj_cube(0.75, 0.3, 0.6);
	glPopMatrix();
}

/////////////////ピラミッド形状を書くための関数/////////////////
// 　X-Z平面上に横幅nx,高さny，縦幅nzのピラミッドを書くための関数//
//　 色をつける機能はない 　　　　　　　　　　　　　　　　　　　　// 
////////////////////////////////////////////////////////////
void CGObjects::obj_pyramid(GLdouble nx, GLdouble ny, GLdouble nz)
{
	GLdouble vertex[][3] = {
		{ 0.0, 1.0, 0.0 },
		{ -0.5, 0.0, -0.5 },
		{ 0.5, 0.0, -0.5 },
		{ 0.5, 0.0, 0.5 },
		{ -0.5, 0.0, 0.5 },
	};
	int face[][4] = {
		{ 0, 1, 2, 0 },
		{ 0, 2, 3, 0 },
		{ 0, 3, 4, 0 },
		{ 0, 4, 1, 0 },
		{ 1, 4, 3, 2 },
	};

	GLdouble normal[][3] = {
		{ 0.0, 1.0, -1.0 },
		{ 1.0, 1.0, 0.0 },
		{ 0.0, 1.0, 1.0 },
		{ -1.0, 1.0, 0.0 },
	};

	int i, j;

	glBegin(GL_TRIANGLES);
	for(j = 0; j < 4; j++) {
		glNormal3dv(normal[j]);
		for(i = 0; i < 3; i++) {
			glVertex3d(vertex[face[j][i]][0] * nx,
				vertex[face[j][i]][1] * ny,
				vertex[face[j][i]][2] * nz);
		}
	}
	glEnd();
	glBegin(GL_QUADS);
	glNormal3d(0.0, -1.0, 0.0);
	for(i = 0; i < 4; i++) {
		glVertex3d(vertex[face[j][i]][0] * nx,
			vertex[face[j][i]][1] * ny,
			vertex[face[j][i]][2] * nz);
	}
	glEnd();
}

////////////////木っぽいオブジェクトを書くための関数////////////
// 　X-Z平面上に垂直に木っぽいオブジェクトをおっ立てる //
//　 色は適当にそれっぽいのをつけてある　　　　　　　　　　　　　　　　// 
////////////////////////////////////////////////////////////
void CGObjects::obj_tree()
{
	GLfloat color[][4] = {
		{ 0.0, 0.4, 0.1, 1.0 },
		{ 0.4, 0.2, 0.1, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};

	// 色と材質の設定
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
	// プッシュしといて、上に平行移動して上のピラミッド型部分の描画
	glPushMatrix();
	glTranslated(0.0, 0.5, 0.0);
	obj_pyramid(0.4, 1.0, 0.4);
	glPopMatrix();
	// 下の長方形部分(木の幹)の描画
	// 平行移動せんので行列のプッシュしなくていい、色と材質だけ
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
	obj_cube(0.1, 0.5, 0.1);
}

// 道路っぽいものの描画
void CGObjects::obj_road()
{
	GLfloat color[][4] = {
		{ 0.3, 0.3, 0.3, 1.0 },
		{ 0.8, 0.8, 0.8, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};

	int i, j;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);

	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 40; j++)
		{
			glVertex3d(  -1.0 + i,  0.01, -20.0 + j);
			glVertex3d(   0.0 + i,  0.01, -20.0 + j);
			glVertex3d(   0.0 + i,  0.01, -19.0 + j);
			glVertex3d(  -1.0 + i,  0.01, -19.0 + j);
		}
	}
	glEnd();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);

	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	for(i = 0; i < 40; i += 2)
	{
		glVertex3d(  -0.02,  0.02, -19.5 + i);
		glVertex3d(   0.02,  0.02, -19.5 + i);
		glVertex3d(   0.02,  0.02, -18.5 + i);
		glVertex3d(  -0.02,  0.02, -18.5 + i);
	}
	glEnd();
}

void CGObjects::subaxis(GLfloat d, GLfloat s)
{
	int i;
	double pi = 3.14, t;
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= 6; i++) {
		t = i * 2 * pi / 6;
		glNormal3f((GLfloat)cos(t), 0.0, (GLfloat)sin(t));
		glVertex3f((GLfloat)(d * cos(t)), -s, (GLfloat)(d*sin(t)));
		glVertex3f((GLfloat)(d * cos(t)), s, (GLfloat)(d * sin(t)));

	}
	glEnd();
	glTranslatef(0.0, s, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(2.0*d, 4.0*d, 5, 5);
}

// X,Y,Z軸の描画
void CGObjects::obj_axis(GLfloat d, GLfloat s)
{
	glEnable(GL_COLOR);
	glPushMatrix();
	glColor3d(1, 0, 0);
	subaxis(d, s);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glColor3d(0, 1, 0);
	subaxis(d, s);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90.0, 0.0, 0.0, 1.0);
	glColor3d(0, 0, 1);
	subaxis(d, s);
	glPopMatrix();
	glDisable(GL_COLOR);
}

// x-z平面に座標(i,j)に色(R,G,B)の立方体を配置する関数
void CGObjects::draw_cube(double i, double j, double cr, double cg, double cb)
{
	GLdouble col[] = {cr, cg, cb, 1.0};

	// 座標変換の行列を一時退避させる
	glPushMatrix();
	// 地面の上に立方体が来るようにキューブの場所を調節
	// プラス2しとかないとボードの左上からになっちゃう
	glTranslatef(i+2, 0.25, j+2);
	// キューブの色を設定
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
	glColor3dv(col);
	// 大きさ0.5のキューブを配置
	glutSolidCube(0.5);
	// 配置が終わったので行列を元に戻す
	glPopMatrix();
}

// 画像を元に立体を配置してみる関数
void CGObjects::draw_picture(struct ppmimg *img)
{
	double k = -5.0, l = -5.0;
	int i, j;

	for (j = 0, l = -5.0; j < img->iheight; j++, l += 0.5) {
		for (i = 0, k = -5.0; i < img->iwidth; i++, k += 0.5) {
			struct RGBColor rgb = getPnmPixel(img, i, j);
			// 0-1の範囲に直す
			double R = (double)rgb.R / 255.0;
			double G = (double)rgb.G / 255.0;
			double B = (double)rgb.B / 255.0;

			draw_cube(k, l, R, G, B);
		}
	}
}

// 木をn個spanずつz軸負の方向に離して設置する関数
void CGObjects::set_tree(int n, double span)
{
	glPushMatrix();
	for (int i = 0; i < n; i++)
	{
		CGObjects::obj_tree();
		glTranslated(0.0, 0.0, -span);
	}
	glPopMatrix();
}

// obj_planeを使って適当な大きさの地面を作る
void CGObjects::set_ground()
{
	// 地面の色
	GLfloat color[][4] = {
		{0.6, 0.3, 0, 1.0},		// 茶色
		{0.48, 0.55, 0.26, 1.0},	// 草色
	};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[1]);
	obj_plane(1000.0, 1000.0);
}

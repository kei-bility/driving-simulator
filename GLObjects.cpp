#include "ppmload.h"
#include "globjects.h"



GLObjects::GLObjects(void)
{
}

GLObjects::~GLObjects(void)
{
}

void GLObjects::obj_depthtest(GLdouble nx, GLdouble ny, GLdouble nz){
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
void GLObjects::obj_pyramid(GLdouble nx, GLdouble ny, GLdouble nz){
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


	int i, j;

	glBegin(GL_TRIANGLES);
	for(j = 0; j < 4; j++)	{
		glNormal3dv(normal[j]);
		for(i = 0; i < 3; i++)		{
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

void GLObjects::obj_textureTest(void){
	static bool istexloaded=false;

	glDisable(GL_COLOR);
	if(!istexloaded){
		if(image==NULL)image=(struct ppmimg*)malloc(sizeof(ppmimg));
		image->dat=NULL;
		loadppmimage("mesi1.ppm",image);
		istexloaded=true;

		/* ƒeƒNƒXƒ`ƒƒ‰æ‘œ‚ÍƒoƒCƒg’PˆÊ‚É‹l‚ßž‚Ü‚ê‚Ä‚¢‚é */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, env);

		/* ƒeƒNƒXƒ`ƒƒ‚ÌŠ„‚è“–‚Ä */
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

void GLObjects::obj_ground(){
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

void GLObjects::obj_road(){
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
			glVertex3d(  -1.0 + i,  0.001, -20.0 + j);
			glVertex3d(   0.0 + i,  0.001, -20.0 + j);
			glVertex3d(   0.0 + i,  0.001, -19.0 + j);
			glVertex3d(  -1.0 + i,  0.001, -19.0 + j);
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
		glVertex3d(  -0.02,  0.002, -19.5 + i);
		glVertex3d(   0.02,  0.002, -19.5 + i);
		glVertex3d(   0.02,  0.002, -18.5 + i);
		glVertex3d(  -0.02,  0.002, -18.5 + i);
	}
	glEnd();
}


void GLObjects::obj_stone(){
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
void GLObjects::obj_tree(){
	GLfloat color[][4] = {
		{ 0.0, 0.4, 0.1, 1.0 },
		{ 0.4, 0.2, 0.1, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
	glPushMatrix();
	glTranslated(0.0, 0.5, 0.0);
	obj_pyramid(0.4, 1.0, 0.4);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
	obj_cube(0.1, 0.5, 0.1);
}

void GLObjects::obj_cube(GLdouble nx, GLdouble ny, GLdouble nz){
	// ÄºÅÀ¥Ç¡¼¥¿
	GLdouble vertex[][3] = {
		{ -0.5, 0.0, -0.5 },
		{  0.5, 0.0, -0.5 },
		{  0.5, 1.0, -0.5 },
		{ -0.5, 1.0, -0.5 },
		{ -0.5, 0.0,  0.5 },
		{  0.5, 0.0,  0.5 },
		{  0.5, 1.0,  0.5 },
		{ -0.5, 1.0,  0.5 }
	};
	// ÌÌ¥Ç¡¼¥¿
	int face[][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};
	// ÌÌ¤ÎË¡Àþ¥Ù¥¯¥È¥E	
	GLdouble normal[][3] = {
		{  0.0,  0.0, -1.0 },
		{  1.0,  0.0,  0.0 },
		{  0.0,  0.0,  1.0 },
		{ -1.0,  0.0,  0.0 },
		{  0.0, -1.0,  0.0 },
		{  0.0,  1.0,  0.0 }
	};

	int i, j;

	glBegin(GL_QUADS);
	for(j = 0; j < 6; j++)
	{
		glNormal3dv(normal[j]);
		for(i = 0; i < 4; i++)
		{
			glVertex3d(vertex[face[j][i]][0] * nx,
				vertex[face[j][i]][1] * ny,
				vertex[face[j][i]][2] * nz);
		}
	}
	glEnd();
}

void GLObjects::obj_signal(){
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

void GLObjects::obj_house(){
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


void GLObjects::obj_building(){
	GLfloat color[][4] = {
		{ 0.5, 0.5, 0.6, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[1]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);

	obj_cube(2.0, 4.0, 1.0);
}



void GLObjects::renderEnv(void){
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
	glTranslated(-1.8, 0.0, 1.8);
	obj_house();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.2, 0.0, -2.0);
	obj_building();
	glPopMatrix();

	glPushMatrix();
	glTranslated(10.0, 0.0, 5.0);
	obj_building();
	glPopMatrix();


	glPopMatrix();
}



void GLObjects::SetGroundColor(GLfloat *col1,GLfloat *col2){
	for(int i=0;i<4;i++){
		m_groundColor1[i]=col1[i];
		m_groundColor2[i]=col2[i];
	}
}

void GLObjects::renderOnlyGround(void){
	//GLfloat color[][4] = {
	//	{ 0.42, 0.572, 0.765, 1.0 },
	//	{ 0.243, 0.271, 0.288, 1.0 },
	//	{ 0.0, 0.0, 0.0, 1.0 },

	//};
	GLfloat color[][4] = {
		{ 0.22, 0.572, 0.265, 1.0 },
		{ 0.243, 0.271, 0.288, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 },

	};
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

void GLObjects::obj_checker(void){

	GLfloat color[][4] = {
		{ 1, 0, 1, 1.0 },
		{ 0.8, 0.271, 0.288, 1.0 },
	};


	bool bWB=false;
	for(double i = 0; i < 3; i+=0.5)
	{
		for(double j = 0; j < 3; j+=0.5)
		{
			glPushMatrix();
			if(bWB){
				glColor3fv(color[0]);
				//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
				//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[0]);
				//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 6.0);
			}else{
				glColor3fv(color[1]);
				//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,color[1]);
				//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[1]);
				//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 6.0);
			}
			bWB=!bWB;
			glTranslated(i,0,j);
			DrawPlane(0.5,0.5);
			glPopMatrix();
		}
		bWB=!bWB;

	}

}

void GLObjects::RenderingScene(void){
	if(m_SceneMode){
		renderEnv();
	}else{
		renderOnlyGround();
	}
}

GLint GLObjects::DrawPlane( GLdouble w, GLdouble d)
{
	GLdouble norm[3];
	glPushMatrix();
	glBegin( GL_POLYGON );
	norm[0]=1;norm[1]=0;norm[2]=0;
	glNormal3dv (norm);
	glVertex3d( w/2.0 , 0 ,   d/2.0 );
	glVertex3d( w/2.0 , 0 ,  -d/2.0  );
	glVertex3d(-w/2.0 , 0 ,  -d/2.0  );
	glVertex3d(-w/2.0 , 0 ,   d/2.0  );
	glEnd();
	glPopMatrix();

	return(0) ;
}
GLint GLObjects::DrawBox(GLdouble d, GLdouble w, GLdouble h)
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


void GLObjects::obj_zroad(void){

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
void GLObjects::subaxis(GLfloat d, GLfloat s){
	int i;
	double pi = 3.14, t;
	glBegin(GL_QUAD_STRIP);
	for(i = 0; i<=6; i++){
		t = i*2*pi/6;
		glNormal3f( (GLfloat)cos(t),         0.0, (GLfloat)sin(t));
		glVertex3f( (GLfloat) (d * cos(t)),   -s, (GLfloat)(d * sin(t)));
		glVertex3f( (GLfloat) (d * cos(t)),    s, (GLfloat)(d * sin(t)));
	}
	glEnd();
	glTranslatef(0.0, s, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(2.0*d, 4.0*d, 5, 5);
}

void GLObjects::obj_axis(GLfloat d, GLfloat s){

	glEnable(GL_COLOR);
	glPushMatrix();	
	glColor3d(1,0,0);
	subaxis(d, s);	
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glColor3d(0,1,0);
	subaxis(d, s);	
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90.0, 0.0, 0.0, 1.0);
	glColor3d(0,0,1);
	subaxis(d, s);
	glPopMatrix();
	glDisable(GL_COLOR);
	glColor3d(1,1,1);
}

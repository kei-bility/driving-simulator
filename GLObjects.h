#include "glut.h"
class GLObjects
{
	void subaxis(GLfloat d, GLfloat s);
public:
	GLObjects(void);
	~GLObjects(void);
	GLfloat clearcol[4];
	//‚ ‚¿‚ç‚©‚ç—A“ü
	void obj_pyramid(GLdouble nx, GLdouble ny, GLdouble nz);
	void obj_ground(void);
	void obj_road(void);
	void obj_zroad(void);
	void obj_xroad(void);
	void obj_tree(void);
	void obj_signal(void);
	void obj_house(void);
	void obj_building(void);
	void obj_cube(GLdouble nx, GLdouble ny, GLdouble nz);
	void obj_stone(void);
	void roboco_load(void);
	void renderEnv(void);
	bool m_SceneMode;
	void renderOnlyGround(void);
	void RenderingScene(void);
	void obj_checker(void);
    void obj_depthtest(GLdouble nx, GLdouble ny, GLdouble nz);
	GLint DrawPlane( GLdouble w, GLdouble d );
	GLint DrawBox(GLdouble d, GLdouble w, GLdouble h);
	void obj_textureTest(void);
	void obj_axis(GLfloat d, GLfloat s);
	GLfloat m_groundColor1[4];
	GLfloat m_groundColor2[4];
	GLfloat m_swpObjColor[4];

	void SetGroundColor(GLfloat *col1,GLfloat *col2);

};

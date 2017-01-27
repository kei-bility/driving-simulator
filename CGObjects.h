#include <GLUT.H>

// É}ÉEÉXÇÃëäëŒà íuÅ®âÒì]äpÇÃä∑éZåWêî
#define SCALE (2.0 * 3.14159265358979323846)
#define M_PI 3.14159265358979323846

class CGObjects {
public:
	CGObjects();
	void obj_plane(GLdouble w, GLdouble d);
	void obj_checker(void);
	void obj_cube(GLdouble nx, GLdouble ny, GLdouble nz);
	void obj_pyramid(GLdouble nx, GLdouble ny, GLdouble nz);
	void obj_tree();
	void obj_road();
	void obj_axis(GLfloat d, GLfloat s);
	void subaxis(GLfloat d, GLfloat s);
	void draw_cube(double i, double j, double cr, double cg, double cb);
	void draw_picture(struct ppmimg *img);
	void set_tree(int n, double span);
	void cylinder(float radius, float height, int sides);
	void set_car();
	void set_ground();

	void obj_zroad(void);
	GLint DrawBox(GLdouble d, GLdouble w, GLdouble h);
	void RenderingScene(void);
	void renderKyu(double r,int vx,int vy);
	void renderOnlyGround();
	void SetGroundColor(GLfloat *col1,GLfloat *col2);
	void renderEnv(void);
	void obj_building();
	void obj_house();
	void obj_signal();
	void obj_stone();
	void obj_textureTest();
	void obj_depthtest(GLdouble nx, GLdouble ny, GLdouble nz);
	void obj_ground();

	GLfloat m_groundColor1[4];
	GLfloat m_groundColor2[4];
	GLfloat m_swpObjColor[4];
	GLfloat clearcol[4];
	static const GLint m_SceneMode = 1;

private:

};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct RGBColor{
	union {
		struct {
			unsigned char R,G,B;
		};
		unsigned char dens;
	};
};


struct ppmimg{
	int iwidth;
	int iheight;
	int depth;
	int cmode;
	//ascii color(P3):3
	//ascii gray (P2):1
	char magicnumber[10];//テキスト形式カラーとグレイスケールのみ(P3,P2)
	unsigned char* dat;
};

//int main(void);

struct ppmimg *makeimagestruct(struct ppmimg* simg);
void loadppmimage(const char *imagename,struct ppmimg* simg);
void saveppmimage(struct ppmimg* simg,char *imagename);

void setPnmPixel(struct ppmimg* simg,int x,int y,struct RGBColor _rgb);
struct RGBColor getPnmPixel(struct ppmimg* simg,int x,int y);
//struct ppmimg *cloneppmimage(struct ppmimg* simg);
struct ppmimg *cloneppmimage(struct ppmimg* simg,struct ppmimg *dimage);
struct ppmimg *createppmimage(struct ppmimg* cimage,int width,int height,int mode);
//struct ppmimg *createppmimage(int width,int height,int mode);
void deleteppmimg(struct ppmimg *img);

//void affinetransform(struct ppmimg* simg,struct ppmimg* dimg,double theta,double xtrans,double ytrans);
//void setaffineMatrix(MATRIX *m,double theta,double xtrans,double ytrans);
//int MatrixInverse(MATRIX *pM, MATRIX *pOUT);
//void MatrixIdentity(MATRIX *pOut);

void colorReverse(struct ppmimg* simg); 

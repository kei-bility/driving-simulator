#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ppmload.h"
struct RGBColor;
struct ppmimg;


struct ppmimg *makeimagestruct(struct ppmimg* simg){
	simg = (struct ppmimg*)malloc(sizeof(struct ppmimg));
	return(simg);
}

void loadppmimage(const char *imagename,struct ppmimg* simg){
	FILE *fp;
	char buff[255];
	int imgwidth,imgheight;
	int imagedepth;
	int datasize;
	int itemp[3];
	//unsigned char ctemp[3];
	//struct ppmimg * tmpimg = (struct ppmimg*)malloc(sizeof(struct ppmimg));
	//(*simg) = new struct ppmimg;
	
	if((fp=fopen(imagename,"rt"))==NULL){
        printf("image cannot read...\n");
        getchar();
		exit(1);
	}
	fgets(buff,255,fp);
	while(buff[0]=='#'){
		fgets(buff,255,fp);
	}
	if(buff[0]=='P'){
		sprintf((simg)->magicnumber,"%s",buff);
		simg->magicnumber[2] = '\0';//fgetsは
	}
	fgets(buff,255,fp);
	while(buff[0]=='#'){
		fgets(buff,255,fp);
	}
	sscanf(buff,"%d %d",&imgwidth,&imgheight);
	fgets(buff,255,fp);
	while(buff[0]=='#'){
		fgets(buff,255,fp);
	}
	sscanf(buff,"%d",&imagedepth);
	//fgets(buff,255,fp);
	//while(buff[0]=='#'){
	//	fgets(buff,255,fp);
	//}
	
	simg->iwidth=imgwidth;
	simg->iheight=imgheight;
	simg->depth=imagedepth;

	printf("%d\n",strcmp(simg->magicnumber,"P2"));
	if(strcmp(simg->magicnumber,"P3")==0){
		simg->cmode = 3;
	}else if(strcmp(simg->magicnumber,"P2")==0){
		simg->cmode = 1;
	}
	datasize = (simg->iwidth)*(simg->iheight)*(simg->cmode);
	simg->dat=(unsigned char *)malloc(sizeof(unsigned char)*datasize);
	//free((*simg)->dat);
	if(simg->cmode==3){
		for(int i=0;i<datasize;i+=simg->cmode){
		//	fscanf(fp,"%d %d %d",&((*simg)->dat[i]),&((*simg)->dat[i+1]),&((*simg)->dat[i+2]));	
			//fscanf(fp,"%d",(*simg)->dat + i);
			//fscanf(fp,"%d",(*simg)->dat + i+1);
			//fscanf(fp,"%d",(*simg)->dat + i+2);
			fscanf(fp,"%d %d %d",&(itemp[0]),&(itemp[1]),&(itemp[2]));
			for(int k=0;k<3;k++){
				simg->dat[i+k] = (unsigned char)itemp[k];
			}
			
		}
	}else{
		for(int i=0;i<datasize;i+=simg->cmode){
			//fscanf(fp,"%d",(*simg)->dat + i);	
			fscanf(fp,"%d",&(itemp[0]));
			simg->dat[i] = (unsigned char)itemp[0];
		}
	}
}
struct ppmimg *cloneppmimage(struct ppmimg* simg,struct ppmimg *dimage){
	//struct ppmimg *tempimg=(struct ppmimg *)malloc(sizeof(struct ppmimg));
	dimage->dat = (unsigned char *)malloc(sizeof(unsigned char)*simg->iwidth*simg->iheight*simg->cmode);
	dimage->cmode=simg->cmode;
	dimage->depth=simg->depth;
	dimage->iheight=simg->iheight;
	dimage->iwidth =simg->iwidth;
	strcpy(dimage->magicnumber,simg->magicnumber);
	for(int j=0;j<simg->iheight;j++){
		for(int i=0;i<simg->iwidth;i++){
			struct RGBColor trgb={0};
			setPnmPixel(dimage,i,j,trgb);
		}
	}
	return(dimage);
}

struct ppmimg *createppmimage(struct ppmimg* cimage,int width,int height,int mode){
	//struct ppmimg *tempimg=(struct ppmimg *)malloc(sizeof(struct ppmimg));
	cimage->dat = (unsigned char *)malloc(sizeof(unsigned char)*width*height*mode);
	cimage->cmode=mode;
	cimage->depth=255;
	cimage->iheight=height;
	cimage->iwidth =width;
	if(mode == 1)sprintf(cimage->magicnumber,"P2");
	else sprintf(cimage->magicnumber,"P3");
	for(int j=0;j<height;j++){
		for(int i=0;i<width;i++){
			struct RGBColor trgb;
			if(mode==1)trgb.dens=0;
			else{
				trgb.R=0;trgb.G=0;trgb.B=0;
			}
			setPnmPixel(cimage,i,j,trgb);
		}
	}
	return(cimage);
}

void saveppmimage(ppmimg *simg,char *imagename){
	FILE *ofp;
	ofp=fopen(imagename,"w");
	fprintf(ofp,"%s\n%d %d\n%d\n",simg->magicnumber,simg->iwidth,simg->iheight,simg->depth);
	for(int j=0;j<simg->iheight;j++){
		for(int i=0;i<simg->iwidth;i++){
			if(simg->cmode==3){
				struct RGBColor trgb=getPnmPixel(simg,i,j);
				fprintf(ofp,"%d %d %d",trgb.R,trgb.G,trgb.B);
				if(i!=simg->iwidth-1)fprintf(ofp," ");
			}else{
				struct RGBColor trgb=getPnmPixel(simg,i,j);
				fprintf(ofp,"%d ",trgb.dens);
			}
		}
		fprintf(ofp,"\n");
	}
	fclose(ofp);
}


void setPnmPixel(struct ppmimg* simg,int x,int y,struct RGBColor _rgb){
	if(simg->cmode==3){
		unsigned long rindex = (simg->iwidth)*y*simg->cmode+x*simg->cmode;
		unsigned long gindex = (simg->iwidth)*y*simg->cmode+x*simg->cmode+1;
		unsigned long bindex = (simg->iwidth)*y*simg->cmode+x*simg->cmode+2;
		simg->dat[rindex] = _rgb.R;
		simg->dat[gindex] = _rgb.G;
		simg->dat[bindex] = _rgb.B;
	}else{
		int gindex = (simg->iwidth)*y*simg->cmode+x*simg->cmode;
		simg->dat[gindex] = _rgb.dens;
	}
}

struct RGBColor getPnmPixel(struct ppmimg* simg,int x,int y){
	struct RGBColor colordat;
	if(simg->cmode==3){	
		unsigned long rindex = (simg->iwidth)*y*simg->cmode+x*simg->cmode;
		unsigned long gindex = (simg->iwidth)*y*simg->cmode+x*simg->cmode+1;
		unsigned long bindex = (simg->iwidth)*y*simg->cmode+x*simg->cmode+2;
		colordat.R=simg->dat[rindex];
		colordat.G=simg->dat[gindex];
		colordat.B=simg->dat[bindex];
	}else{
		int gindex = (simg->iwidth)*y*simg->cmode+x*simg->cmode;
		colordat.dens = simg->dat[gindex];
	}
	return colordat;
}

void deleteppmimg(struct ppmimg *img){
if(img->dat != NULL)free(img->dat);
	if(img != NULL)free(img);
}


struct RGBColor getColorReversePnmPixel(struct ppmimg* simg,int x,int y){
	struct RGBColor colordat = getPnmPixel(simg, x, y);
	if(simg->cmode==3){	
//ここに色反転したピクセルをとってくる処理を書く（カラーの時）
	}else{
//ここに色反転したピクセルをとってくる処理を書く（グレースケールの時）
	}
	return colordat;
}

void colorReverse(struct ppmimg* simg){
	//struct RGBColor trgb;
	for(int j=0;j<(simg->iheight);j++){
		for(int i=0;(i<simg->iwidth);i++){
			//色反転したピクセルをとってきて元のピクセルを置き換える
			//このままだと元の画像が書き換えられるのでコピーを作ってそれを書き換えたほうがいい
		}
	}
}

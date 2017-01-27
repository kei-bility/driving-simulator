
#include <stdio.h>
#include <math.h>
#include <string>
#include <glut.h>
#include "3DMATH.H"
#include "GLMetaseq.h"
#define MY_DELETE(__x__) { if (__x__) delete __x__; }
class MyRacingCar;
class PlayerCar;

/// プログラムに必要な変数を構造体として定義
struct DataSet {
	int gameTimer;			///< 全体用タイマー

	// 3D表示用のデータ
	float aspect;			///< 画面アスペクト比（縦横比）
	float cam_rotx;			///< オブジェクト回転量
	float cam_roty;
	float cam_transz;		///< カメラのZ位置

	float model_rotx;		///< モデルの回転量
	float model_roty;

	PlayerCar *player;
	CRealvector camPos;
	float cam_near, cam_far;
};

////////////////////////// class MyRacingCar //////////////////////////

/// 車のためのクラス。プレイヤー車と敵車のクラスを派生させるため
/// このクラスは基底クラスとして作成する
class MyRacingCar {
private:
	std::string name;		// 車名

protected:
	// これらのメンバを派生クラスが直接操作できるよう、protectedにする。
	float pos[3];		// 現在位置
	float dir;			// 進行方向。X+軸を0度とする右ネジ回転系。単位はラジアン
	float heading;		// 向いている方向。通常はdirと同値。
	float vel;			// 速度
	float max_vel;		// 最大車速
	float acc;			// 加速度

	int timer;			// アニメーション時などのためのタイマー

	// コンストラクタがprotectedエリアにあるため直接このクラスのインスタンスを
	// 作ることはできない。派生クラスからのみ作れる。
	MyRacingCar(std::string nn, float maxv);

public:
	MQO_MODEL model;	// 車のデータ
	virtual ~MyRacingCar();
	virtual void setpos(float xx, float yy, float zz);
	virtual void getpos(float &xx, float &yy, float &zz);
	virtual void setdir(float theta);
	virtual float getdir() { return dir; }
	virtual float adddir(float theta);
	virtual float setacc(float aa) { return acc = aa; }
	virtual float getacc() { return acc; }
	virtual float getvel() { return vel; }
	virtual void update(DataSet *data, float elapsed);

	virtual void draw();
	virtual void control(DataSet *data, int key) { /* no control */ };
};

////////////////////////// class PlayerCar //////////////////////////
/// 派生クラス「プレイヤー車」
class PlayerCar: public MyRacingCar {
public:
	PlayerCar(const char *nn, float maxv) : MyRacingCar(nn, maxv) {}
	virtual ~PlayerCar() {}
	virtual void control(DataSet *data, int key);
};

void setCamera(DataSet *data);
void updateData(DataSet *data);
void ReleaseDataSet(DataSet *data);

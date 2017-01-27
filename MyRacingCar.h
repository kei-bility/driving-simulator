
#include <stdio.h>
#include <math.h>
#include <string>
#include <glut.h>
#include "3DMATH.H"
#include "GLMetaseq.h"
#define MY_DELETE(__x__) { if (__x__) delete __x__; }
class MyRacingCar;
class PlayerCar;

/// �v���O�����ɕK�v�ȕϐ����\���̂Ƃ��Ē�`
struct DataSet {
	int gameTimer;			///< �S�̗p�^�C�}�[

	// 3D�\���p�̃f�[�^
	float aspect;			///< ��ʃA�X�y�N�g��i�c����j
	float cam_rotx;			///< �I�u�W�F�N�g��]��
	float cam_roty;
	float cam_transz;		///< �J������Z�ʒu

	float model_rotx;		///< ���f���̉�]��
	float model_roty;

	PlayerCar *player;
	CRealvector camPos;
	float cam_near, cam_far;
};

////////////////////////// class MyRacingCar //////////////////////////

/// �Ԃ̂��߂̃N���X�B�v���C���[�ԂƓG�Ԃ̃N���X��h�������邽��
/// ���̃N���X�͊��N���X�Ƃ��č쐬����
class MyRacingCar {
private:
	std::string name;		// �Ԗ�

protected:
	// �����̃����o��h���N���X�����ڑ���ł���悤�Aprotected�ɂ���B
	float pos[3];		// ���݈ʒu
	float dir;			// �i�s�����BX+����0�x�Ƃ���E�l�W��]�n�B�P�ʂ̓��W�A��
	float heading;		// �����Ă�������B�ʏ��dir�Ɠ��l�B
	float vel;			// ���x
	float max_vel;		// �ő�ԑ�
	float acc;			// �����x

	int timer;			// �A�j���[�V�������Ȃǂ̂��߂̃^�C�}�[

	// �R���X�g���N�^��protected�G���A�ɂ��邽�ߒ��ڂ��̃N���X�̃C���X�^���X��
	// ��邱�Ƃ͂ł��Ȃ��B�h���N���X����̂ݍ���B
	MyRacingCar(std::string nn, float maxv);

public:
	MQO_MODEL model;	// �Ԃ̃f�[�^
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
/// �h���N���X�u�v���C���[�ԁv
class PlayerCar: public MyRacingCar {
public:
	PlayerCar(const char *nn, float maxv) : MyRacingCar(nn, maxv) {}
	virtual ~PlayerCar() {}
	virtual void control(DataSet *data, int key);
};

void setCamera(DataSet *data);
void updateData(DataSet *data);
void ReleaseDataSet(DataSet *data);

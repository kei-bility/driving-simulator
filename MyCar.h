
#include <cstdio>
#include <cmath>
#include "3DMATH.h"
#include <glut.h>

#define D3DX_PI 3.14

//�P�ʎ��Ԃ�����̈ړ��ʐݒ�
#define	kDIV_UNIT		60
#define	kTIMER			( 1000 / kDIV_UNIT )

//���̒l�ɓx���|����ƃ��W�A���ɂȂ�
#define	kDEG_TO_RAD			( D3DX_PI * 2.0f / 360.0f)

#define	kPI_3FOURTH			( D3DX_PI * 3.0f / 4.0f )
#define	kPI_1FOURTH			( D3DX_PI / 4.0f )
#define	kPI_HALF			( D3DX_PI / 2.0f )

#define	kMOST_FAR_POINT		100000.0f				//�ł������_

#define	kACCELL_LIMIT		120.0f					//�������E�����i����ȏ�ڋ߂�����������~�߂�j
#define kAPPROACH_LIMIT		80.0f					//�ڋߌ��E�����i����ȏ�ڋ߂����猸������j
#define	kCURVE_APPROACH		100.0f

//�Ö��C�W���E�����C�W��
#define	kSTATIC_FRICTION	0.9f
#define	kDINAMIC_FRICTION	0.3f

//�d�͉����x
#define	kG					9.8f

//��R�W���i��C��R�Ƃ��]�����R�Ƃ��܂Ƃ߂āj
#define	kRESISTANCE_C		.1f

//�G���W����]�������W���i�A�N�Z���𗣂����Ƃ��p�j
#define	kSLOWDOWN_C			0.075f

//�K�[�h���[���ڐG���̌����Ɣ���
#define	kGUARDRAIL_REG		0.5f	
#define	kGUARDRAIL_REF		4.0f

//�Ԃ̐��\�l���ۂ�����
#define	kCAR_WEIGHT			5.0f
#define	kCAR_WIDTH			5.0f
#define	kCAR_LENGTH			10.0f
#define kWHEEL_UNIT			( 0.2f * kDEG_TO_RAD )
#define kWHELL_MAX			(20.0f * kDEG_TO_RAD )
#define	kSPEED_MAX			10.0f
#define	kBRAKE_UNIT			0.01f
#define	kBRAKE_MAX			0.75f
#define	kREV_MAX			1.0f
#define	kREV_IDLING			( kREV_MAX * 0.1f )
#define kREV_UNIT			( kREV_MAX / kDIV_UNIT)
#define kACCEL_UNIT			( kSPEED_MAX / kDIV_UNIT)

//�L�[�{�[�h�i�W���C�X�e�B�b�N�j�̏��
enum{
	kKEY_NONE		= 0x0000,
	kKEY_LEFT		= 0x0001,
	kKEY_RIGHT		= 0x0002,
	kKEY_UP			= 0x0004,
	kKEY_DOWN		= 0x0008,
	kKEY_SPACE		= 0x0010,
	kKEY_Z			= 0x0020,
	kKEY_X			= 0x0040,
	kKEY_C			= 0x0080,
	kKEY_ENTER		= 0x0100,
	kKEY_TAB		= 0x0200,
	kKEY_SHIFT		= 0x0400,
	kKEY_DELBS		= 0x0800,
	kKEY_ERR		= 0x8000,
	kKEY_MOVE_MASK	= 0x000f,
	kKEY_BUTTON_MASK= 0xfff0,
	kKEY_ALL_MASK	= 0xffff
};

class MyCar {
private:

public:

	double x, y;	// �Ԃ̍��W
	bool isMoving;	// �~�܂��Ă邩�ǂ���
	double accel;	// �����x
	double rotate;	// �p���x
	double vx, vy;	// ���x

	double vel;
	double veltheta;
	double yaw;
	double yawvel;
	double roll;
	double rollvel;
	double pitch;
	double pitchvel;
	double brake;
	double rev;
	double wheelangle;
	CRealvector carpos;
	bool accel_flag;
	bool right_flag;
	bool left_flag;

	MyCar();
	void init();
	void setSpeed(double x, double y);

	void drivingCar(const int key , MyCar me );
	void computeObjForce( MyCar me);

	// ���̉��̃��\�b�h�͓G�̓����̂Ƃ��Ɏg��?
	float FreeRunDistance( MyCar me );
	float FreeRollTheta( MyCar me );

	void update();

};
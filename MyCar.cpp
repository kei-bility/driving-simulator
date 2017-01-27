
#include <cmath>
#include "MyCar.h"

//CRealvector carpos(-1.0, 0.7, 3.0);
//CRealvector carpos(0.0, 0.0, 0.0);

MyCar::MyCar()
{

}

void MyCar::init()
{
	accel = 0.0;
	isMoving = false;
	vx = 0.0;
	vy = 0.0;
	x = 0.0;
	y = 0.0;

	carpos = CRealvector(-1.0, 0.7, 3.0);
	accel_flag = false;
	right_flag = false;
	left_flag = false;
	vel = 0.0;
	veltheta = 0.0;
	yaw = 0.0;
	yawvel = 0.0;
	roll = 0.0;
	rollvel = 0.0;
	pitch = 0.0;
	pitchvel = 0.0;
	brake = 0.0;
	rev = 0.0;
	wheelangle = 0.0;
}

// �ʒu�Ƃ����x�Ƃ��A�b�v�f�[�g
void MyCar::update()
{

}

//-----------------------------------------------------------------------------FreeRollTheta()
//	�����_�Ő���L�[�𗣂����Ƃ��A�ǂꂾ�����񂵂Â��邩�̒l��Ԃ�
//-----------------------------------------------------------------------------
float MyCar::FreeRollTheta( MyCar me )
{
	return(2.0f * (float)pow( me.wheelangle , 2.0f ) / kWHEEL_UNIT) - (me.wheelangle);
}

//-----------------------------------------------------------------------------FreeRunDistance()
//	�����_�ŉ����L�[�𗣂����Ƃ��A�ǂꂾ���O�i���Â��邩�̒l��Ԃ�
//-----------------------------------------------------------------------------
float MyCar::FreeRunDistance( MyCar me )
{
	return(2.0f * (float)pow( me.vel , 2.0f ) / kACCEL_UNIT) - (me.vel);
}

//-----------------------------------------------------------------------------
void MyCar::drivingCar(const int key , MyCar me )
{
	
	//======================================================================
	//�L�[�{�[�h����̓��͂ɂ��ړ�
	//����]
	if( key == GLUT_KEY_LEFT )
	{
		me.wheelangle -= kWHEEL_UNIT;
		if( me.wheelangle < -kWHELL_MAX ){ me.wheelangle = -kWHELL_MAX; }
	}
	else
	//�E��]
	if( key == GLUT_KEY_RIGHT )
	{
		me.wheelangle += kWHEEL_UNIT;
		if( me.wheelangle > kWHELL_MAX ){ me.wheelangle = kWHELL_MAX; }
	}

	//�����i�����̓G���W���̉�]���j
	if( key == GLUT_KEY_UP )
	{
		printf("up");
		accel_flag =true;
		me.rev += kREV_UNIT;
		if( me.rev > kREV_MAX ){ me.rev = kREV_MAX;}

		//myposi.z() -= 0.1;
			//eyetarget.z() -= 0.1;
			//setViewPosition(myposi, eyetarget, upView);
			accel_flag = true;
			//pro += 0.1;
			glutPostRedisplay();

	}
	else if(me.rev > 0)
	{
		//�G���W����]�����炵�����ikrev_IDLING�ŐÎ~�j
		me.rev -= me.rev * kSLOWDOWN_C;
		me.rev =  ( fabs( me.rev ) > kREV_IDLING ) ? me.rev : kREV_IDLING;
	}

	//�����i�u���[�L�𓥂ݍ��݁j
	if( key == GLUT_KEY_DOWN)
	{
		me.brake += kBRAKE_UNIT;
		if( me.brake > kBRAKE_MAX ){ me.brake = kBRAKE_MAX; }
	}
	else if( me.brake > 0.0f )
	{
		me.brake -= kBRAKE_UNIT * 2.5f;
		if( me.brake < 0.0f ){ me.brake = 0.0f;}
	}

	//�n���h���������A�����i�O�ߎ��ŐÎ~�j
	if((me.wheelangle != 0.0f)&&(!(key & (kKEY_LEFT + kKEY_RIGHT))))
	{
		me.wheelangle += ( me.wheelangle > 0 ) ? -kWHEEL_UNIT * 4.0f : kWHEEL_UNIT * 4.0f;
		me.wheelangle =  ( fabs( me.wheelangle ) > kWHEEL_UNIT * 4.0f ) ? me.wheelangle : 0.0f;
	}
}

//-----------------------------------------------------------------------------
void MyCar::computeObjForce( MyCar me)
{
	//����͑Ǌp�Ƒ��x�ɔ�Ⴕ�āA�Ԃ̒����i�z�C�[���x�[�X�j�ɔ���Ⴗ��
	me.yaw += ( me.wheelangle * me.vel ) / kCAR_LENGTH;
//	me.velTheta = me.yaw;

	//CheckFloatRange( &(me.yaw) , D3DX_PI );				//�߉�]�`�F�b�N

	me.vel += me.rev / kCAR_WEIGHT * kSTATIC_FRICTION;	//���x�ω�

	//��C��R�Ƃ��ɂ�錸��
	me.vel -= me.vel * kRESISTANCE_C/ kCAR_WEIGHT;

	//�u���[�L�Ƃ��|���܂�
	if( me.brake > 0.0f )
	{
		me.vel -= kCAR_WEIGHT * me.brake;
		if( me.vel < 0 ){ me.vel = 0;}
	}

	//�ړ����܂�
	carpos.x() -= sinf( me.yaw ) * me.vel;
	carpos.z() -= cosf( me.yaw ) * me.vel;
	printf("%f %f\n", carpos.x(), carpos.z());
}

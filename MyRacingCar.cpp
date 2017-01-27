
#include <math.h>
#include "MyRacingCar.h"

/// �R���X�g���N�^�ł͊e�����o������������
MyRacingCar::MyRacingCar(std::string nn, float maxv): name(nn)
{
	// pos�͎Ԃ̌��݈ʒu�Ƃ������������ʒu����ǂ̒��x�ړ�������������
	// ����pos�̕��������s�ړ�����
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
	dir = 0;
	heading = 0;
	vel = 0;
	acc = 0;
	max_vel = maxv;
	timer = 0;
	mqoInit();
	model = mqoCreateModel( "mqo/rancer.mqo", 0.0035 );
}

/// �f�X�g���N�^�ł�mqo�f�[�^���������
MyRacingCar::~MyRacingCar()
{
	//if (cardata) delete cardata;
	mqoDeleteModel( model );
	mqoCleanup();
}

/// mqo�f�[�^��`�悷��Bheading�����Ɍ����悤�ɂ���
void MyRacingCar::draw()
{
	glPushMatrix();
	glTranslated(0.0, 0.2, 0.5);
	glTranslated(pos[0], pos[1], pos[2]);	// �v�Z�������W�Ԃ�i�ނ悤�ɂ���
	glRotated(-90.0, 0.0, 1.0, 0.0);
	double th = (double)RadToDeg(heading);
	glRotated(-th, 0.0, 1.0, 0.0);	// �Ԏ��̂���]������
	mqoCallModel( model );
	glPopMatrix();
}

/// �ʒu���Z�b�g����B�������p
void MyRacingCar::setpos(float xx, float yy, float zz)
{
	pos[0] = xx; pos[1] = yy; pos[2] = zz;
}

/// ���݈ʒu���擾����B�e��v�Z�ɗp����
void MyRacingCar::getpos(float &xx, float &yy, float &zz)
{
	xx = pos[0]; yy = pos[1]; zz = pos[2];
}

/// ������ݒ肷��
void MyRacingCar::setdir(float theta)
{
	heading = dir = theta;
}

float MyRacingCar::adddir(float theta)
{
	dir += theta;
	return heading = dir;
}

/// �Ԃ̑��x�ƈʒu���X�V����B
void MyRacingCar::update(DataSet *data, float elapsed)
{
	//timer++;

	// ���x���v�Z���A��������
	vel += acc * elapsed;			// �����x�����x
	if (vel > max_vel) vel = max_vel;
	if (vel <= 0) {
		vel = 0;
		this->setacc(0);
	}

	// ���x���ʒu�A����������̂Ńx�N�g���Ōv�Z
	float xx, zz;
	xx = cos(dir) * vel * elapsed;
	zz = sin(dir) * vel * elapsed;

	// ���X��+=����������-�ɂ��Ȃ��ƕ����Ƌt�ɂȂ�
	// �i�s����������E���}�C�i�X������
	pos[0] -= xx;
	pos[2] -= zz;
	// �S�[���܂ł��ǂ�����猸��
	if (pos[0] < -175) {
		this->setacc(-130);
	}

	//glutPostRedisplay();
}

/// �v���C���[�Ԃ̑���
void PlayerCar::control(DataSet *data, int key)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		data->player->setacc(10.0);
		break;
	case GLUT_KEY_DOWN:
		data->player->setacc(-40);
		break;
	case GLUT_KEY_RIGHT:
		data->player->adddir(0.05f);
		break;
	case GLUT_KEY_LEFT:
		data->player->adddir(-0.05f);
		break;
	}
	update(data, 1/60.0f);
}

/*
�������牺�͎g��Ȃ�
-----------------------------------------------------------------
*/


////////////////////////// ::resetData //////////////////////////
/// �f�[�^�����Z�b�g���A������Ԃɖ߂�
void resetData(DataSet *data)
{
	data->cam_rotx = data->cam_roty = 0;
	data->cam_transz = 5;
}

////////////////////////// ::setCamera //////////////////////////
/// �J������ݒ肷��B�J�����̈ʒu�A�����̑��A�X�N���[����������B
void setCamera(DataSet *data)
{
	//HRESULT hr = D3D_OK;

	// �Ԃ̂�����΂ߏォ��̎��_�ʒu�����
	float xx, yy, zz;
	data->player->getpos(xx, yy, zz);
	float th = data->player->getdir();

	/// ���J�����̃f�B���C�������B���ꂪ�Ȃ��Ɖ�ʂ̓������}��������
	/// �R�����g���O���ĈႢ���m�F���邱��
	//float deltay = th - data->cam_roty;
	//data->cam_roty += deltay * 0.05f;		// ���t���[��5%���ړ�
	//th = data->cam_roty;
	/// �����܂ŃJ�����f�B���C�̃R�����g

	float dist = data->cam_transz;
	float cmx = xx - cos(th) * dist;
	float cmy = yy + 20;		// ���ォ�猩��
	float cmz = zz - sin(th) * dist;

	CRealvector cam(cmx, cmy, cmz);	// �J�����ʒu�͎Ԃ̌��
	CRealvector camAt(xx, yy, zz); // �J�����̒����_�͎�
	CRealvector camUp(0, 1, 0); // �J�����͐����p�� up=(0, 1, 0)
	//setViewPosition(cam, camAt, camUp);
	// �J�����ʒu��񂩂�s����쐬�BRH�͉E��n�̈Ӗ�
	//D3DXMATRIX camera;
	//D3DXMatrixLookAtRH(&camera, &cam, &camAt, &camUp);

	// �r���[�s��Ƃ��Đݒ�(�Œ�@�\�p)
	//data->dev->SetTransform(D3DTS_VIEW, &camera);

	// ���e�s���ݒ肷��B�X�N���[���ɓ��e���邽�߂ɕK�v�B����p�̓�/2
	//D3DXMATRIX proj;
	//D3DXMatrixPerspectiveFovRH(&proj, D3DX_PI/6, data->aspect, data->cam_near, data->cam_far);
	// �쐬�����s����A���e�s��ɐݒ�(�Œ�@�\�p)
	//data->dev->SetTransform(D3DTS_PROJECTION, &proj);

	//if (data->pEffect) {
		// proj, camera�s������O���Z���Ă����āA�V�F�[�_�ɓn��
		// �V�F�[�_���ŉ��Z����ƒ��_�����̉��Z����������B���O�Ɍv�Z����Έ��ōς�
		// �V�F�[�_�ɓn����钸�_���W�́AD3DTS_PROJECTION�Ȃǂ̉e�����󂯂Ă��Ȃ��̂�
		// �V�F�[�_���ō��W�ϊ�����K�v������B
		//D3DXMATRIX vp = camera * proj;
		//data->pEffect->SetMatrix(data->pViewProj, &vp);

		data->camPos.x() = cmx;
		data->camPos.y() = cmy;
		data->camPos.z() = cmz;
		//data->camPos.w = 1;
		//data->pEffect->SetVector(data->pEye, &data->camPos);
	//}
}

////////////////////////// ::updateData //////////////////////////
/// �f�[�^���A�b�v�f�[�g����B
/// \param data �f�[�^�Z�b�g
void updateData(DataSet *data) {
	data->gameTimer++;

	data->player->update(data, 1/60.0f);
}

/// DataSet���������B
void ReleaseDataSet(DataSet *data) {
	//MY_DELETE(data->courceMap);
	//MY_DELETE(data->courceGrass);
	MY_DELETE(data->player);
}



#include <math.h>
#include "MyRacingCar.h"

/// コンストラクタでは各メンバを初期化する
MyRacingCar::MyRacingCar(std::string nn, float maxv): name(nn)
{
	// posは車の現在位置というよりも初期位置からどの程度移動したかを示す
	// このposの分だけ平行移動する
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

/// デストラクタではmqoデータを解放する
MyRacingCar::~MyRacingCar()
{
	//if (cardata) delete cardata;
	mqoDeleteModel( model );
	mqoCleanup();
}

/// mqoデータを描画する。heading方向に向くようにする
void MyRacingCar::draw()
{
	glPushMatrix();
	glTranslated(0.0, 0.2, 0.5);
	glTranslated(pos[0], pos[1], pos[2]);	// 計算した座標ぶん進むようにする
	glRotated(-90.0, 0.0, 1.0, 0.0);
	double th = (double)RadToDeg(heading);
	glRotated(-th, 0.0, 1.0, 0.0);	// 車自体を回転させる
	mqoCallModel( model );
	glPopMatrix();
}

/// 位置をセットする。初期化用
void MyRacingCar::setpos(float xx, float yy, float zz)
{
	pos[0] = xx; pos[1] = yy; pos[2] = zz;
}

/// 現在位置を取得する。各種計算に用いる
void MyRacingCar::getpos(float &xx, float &yy, float &zz)
{
	xx = pos[0]; yy = pos[1]; zz = pos[2];
}

/// 方向を設定する
void MyRacingCar::setdir(float theta)
{
	heading = dir = theta;
}

float MyRacingCar::adddir(float theta)
{
	dir += theta;
	return heading = dir;
}

/// 車の速度と位置を更新する。
void MyRacingCar::update(DataSet *data, float elapsed)
{
	//timer++;

	// 速度を計算し、調整する
	vel += acc * elapsed;			// 加速度→速度
	if (vel > max_vel) vel = max_vel;
	if (vel <= 0) {
		vel = 0;
		this->setacc(0);
	}

	// 速度→位置、向きがあるのでベクトルで計算
	float xx, zz;
	xx = cos(dir) * vel * elapsed;
	zz = sin(dir) * vel * elapsed;

	// 元々は+=だったけど-にしないと方向と逆になる
	// 進行方向が上も右もマイナスだから
	pos[0] -= xx;
	pos[2] -= zz;
	// ゴールまでたどりついたら減速
	if (pos[0] < -175) {
		this->setacc(-130);
	}

	//glutPostRedisplay();
}

/// プレイヤー車の操作
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
こっから下は使わない
-----------------------------------------------------------------
*/


////////////////////////// ::resetData //////////////////////////
/// データをリセットし、初期状態に戻す
void resetData(DataSet *data)
{
	data->cam_rotx = data->cam_roty = 0;
	data->cam_transz = 5;
}

////////////////////////// ::setCamera //////////////////////////
/// カメラを設定する。カメラの位置、向きの他、スクリーン情報もある。
void setCamera(DataSet *data)
{
	//HRESULT hr = D3D_OK;

	// 車のやや後方斜め上からの視点位置を作る
	float xx, yy, zz;
	data->player->getpos(xx, yy, zz);
	float th = data->player->getdir();

	/// ↓カメラのディレイを実現。これがないと画面の動きが急激すぎる
	/// コメントを外して違いを確認すること
	//float deltay = th - data->cam_roty;
	//data->cam_roty += deltay * 0.05f;		// 毎フレーム5%ずつ移動
	//th = data->cam_roty;
	/// ここまでカメラディレイのコメント

	float dist = data->cam_transz;
	float cmx = xx - cos(th) * dist;
	float cmy = yy + 20;		// やや上から見る
	float cmz = zz - sin(th) * dist;

	CRealvector cam(cmx, cmy, cmz);	// カメラ位置は車の後ろ
	CRealvector camAt(xx, yy, zz); // カメラの注視点は車
	CRealvector camUp(0, 1, 0); // カメラは垂直姿勢 up=(0, 1, 0)
	//setViewPosition(cam, camAt, camUp);
	// カメラ位置情報から行列を作成。RHは右手系の意味
	//D3DXMATRIX camera;
	//D3DXMatrixLookAtRH(&camera, &cam, &camAt, &camUp);

	// ビュー行列として設定(固定機能用)
	//data->dev->SetTransform(D3DTS_VIEW, &camera);

	// 投影行列を設定する。スクリーンに投影するために必要。視野角はπ/2
	//D3DXMATRIX proj;
	//D3DXMatrixPerspectiveFovRH(&proj, D3DX_PI/6, data->aspect, data->cam_near, data->cam_far);
	// 作成した行列を、投影行列に設定(固定機能用)
	//data->dev->SetTransform(D3DTS_PROJECTION, &proj);

	//if (data->pEffect) {
		// proj, camera行列を事前演算しておいて、シェーダに渡す
		// シェーダ内で演算すると頂点個数分の演算が発生する。事前に計算すれば一回で済む
		// シェーダに渡される頂点座標は、D3DTS_PROJECTIONなどの影響を受けていないので
		// シェーダ内で座標変換する必要がある。
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
/// データをアップデートする。
/// \param data データセット
void updateData(DataSet *data) {
	data->gameTimer++;

	data->player->update(data, 1/60.0f);
}

/// DataSetを解放する。
void ReleaseDataSet(DataSet *data) {
	//MY_DELETE(data->courceMap);
	//MY_DELETE(data->courceGrass);
	MY_DELETE(data->player);
}


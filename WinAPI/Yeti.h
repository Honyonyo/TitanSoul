#include "Boss.h"
class Animation;
class PixelCollision;
class Snowball;
class Yeti : public Boss
{
/*
	eObjectKinds m_kinds;
	D2D1_POINT_2F m_center;	//������Ʈ �߽���
	float m_hitboxRange;	//�ǰ� ����
	D2D_POINT_2F m_hitboxCenter;	//�ǰݹ����� �߽���
	float m_attackRange;	//���ݹ���
	D2D1_POINT_2F m_attackCenter;		//���ݹ����� �߽���

	bool m_isOnHit;
	bool m_isOnAttack;

	bool m_isAlive;
*/
private:
	enum eAction
	{
		eIdle = 0,
		eReady,
		eThrow,
		eRolling,
		eRollingLoop,
		eSit,
		eDeath,

		eActionNumCount
	};

	CImage* m_sprite;
	CImage* m_shadow;
	D2D1_POINT_2F m_shadowCenter;

	Animation* m_animation;
	//[eMoveDirection][eActionNumCount]
	vector<int> m_aniIndexArr[eMoveDirectionNumCount][eActionNumCount];
	PixelCollision* m_pixelCollision;

	eAction m_action;
	queue<eAction> m_pattern;

	const float m_rollingSpeed;
	const int m_maxSnowball;
	float m_rot;
	float m_rotRad;
	bool m_isAction;
	bool m_isSleep;

	//�Ѹ����� ����
	bool m_isOnRollingLoop; 
	bool m_hitWall;
	D2D1_POINT_2F m_rollingHitWallPoint;
	D2D1_POINT_2F m_rollingDest;
	const float m_rollingDefJumpSpeed;
	float m_rollingJumpSpeed;
	float m_rollingGravity;
	float m_rollingJumpMoveX;
	float m_rollingJumpMoveY;
	int m_rollingJumpCount;

	//����캼 ���� ����
	Snowball* snowball;
/// <����>
///		1. ���������� 4ȸ
///		2. ������ -> ��帧
///			������ �� ���� �ڰ�, ���� ���� �κк��� �÷��̾��� ��ġ������ �����ϴ� ���� �޷�迡 ��帧 ������
///			Ƚ�� ���������� 3~4ȸ �󵵰� ����. 
///			������ �� �� ������ ������ ������ �������� �ٲ�
/// 
/// 	bitset<eActionTagNumCount> action;
///
/// 	�ִ� ������Ʈ�� �Ѵ�
///		������ �������� Ȯ���Ѵ�
/// 	������ �������� direction�� �ٲ۴�
/// 					���� pattern�� �����Ѵ�
///		������ ������ �ʾҴٸ�
///						���� ������Ʈ�� �Ѵ�
///						�ִϸ��̼� ������Ʈ�� �Ѵ�
///						���� READY������ ���, ������ ���̻��̿� direction����ش�
/// 
/// 	������ �����ϸ�
/// 	�ش� bitset�� on�Ѵ�.
/// 	isAction�� true�Ѵ�
///		���� ������ �Ѵ�. - �ִϸ��̼� ������ ����, fps���� 
/// 	actionAni�� �������� Ȯ���ϰ�,
/// 	�ȳ����� ���� m_action�� ���� switch�� �Լ� ȣ���Ѵ�.
///		������ �ش� ������ ���������� ǥ�����ش�.
/// 	���� ������ �����Ѵ�.
/// 	queue�� ��������� SetPattern ����
/// 
///	ActionSet func
///		: SetFrame, fps��isAct true�� action[action] = 1
///
///		If(action[actiontag])
///	{
///		if (ani get end)
///	}
/// </����>

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);

	void SetCollUpdate(int animationtFrameIdx);

	void SetPattern();
	void SetThrowSnowball();
	void SetRolling();
	void SetReady();
	void ThrowSnowball();
	void Rolling();
	void IcicleDrop();
	void Ready(int prevFrame);

	void SetAnimationFrame();
	void SetDirection();
	void SetStart();

	Yeti();
	~Yeti();
};
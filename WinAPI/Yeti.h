#include "Boss.h"
class Animation;
class PixelCollision;
class Snowball;
class Yeti : public Boss
{
/*
	eObjectKinds m_kinds;
	D2D1_POINT_2F m_center;	//오브젝트 중심점
	float m_hitboxRange;	//피격 범위
	D2D_POINT_2F m_hitboxCenter;	//피격범위의 중심점
	float m_attackRange;	//공격범위
	D2D1_POINT_2F m_attackCenter;		//공격범위의 중심점

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

	//롤링관련 변수
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

	//스노우볼 관련 변수
	Snowball* snowball;
/// <패턴>
///		1. 눈공던지기 4회
///		2. 구르기 -> 고드름
///			구르기 후 벽에 박고, 벽에 박은 부분부터 플레이어의 위치까지를 연결하는 직선 쭈루룩에 고드름 퍼퍼퍽
///			횟수 랜덤이지만 3~4회 빈도가 높음. 
///			구르기 후 몸 각도는 마무리 프레임 끝나고나서 바뀜
/// 
/// 	bitset<eActionTagNumCount> action;
///
/// 	애니 업데이트를 한다
///		동작이 끝났는지 확인한다
/// 	동작이 끝났으면 direction을 바꾼다
/// 					다음 pattern을 실행한다
///		동작이 끝나지 않았다면
///						동작 업데이트를 한다
///						애니메이션 업데이트를 한다
///						만약 READY상태인 경우, 프레임 사이사이에 direction잡아준다
/// 
/// 	동작을 시작하면
/// 	해당 bitset을 on한다.
/// 	isAction을 true한다
///		동작 세팅을 한다. - 애니메이션 프레임 세팅, fps세팅 
/// 	actionAni가 끝났는지 확인하고,
/// 	안끝나면 현재 m_action에 따라 switch로 함수 호출한다.
///		끝나면 해당 동작이 끝난것으로 표시해준다.
/// 	다음 동작을 실행한다.
/// 	queue가 비어있으면 SetPattern 실행
/// 
///	ActionSet func
///		: SetFrame, fps→isAct true→ action[action] = 1
///
///		If(action[actiontag])
///	{
///		if (ani get end)
///	}
/// </패턴>

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
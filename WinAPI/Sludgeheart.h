#pragma once
#include "Object.h"
class Sludgeheart : public Object
{
private:
	/*
		eObjectKinds m_kinds;
	D2D1_POINT_2F m_center;	//오브젝트 중심점
	float m_hitboxRange;	//피격 범위
	D2D_POINT_2F m_hitboxCenter;	//피격범위의 중심점
	float m_attackRange;	//공격범위
	D2D1_POINT_2F m_attackCenter;		//공격범위의 중심점

	eMoveDirection m_direction;

	bool m_isOnHit;
	bool m_isOnAttack;
	bool m_isDelete;

	bool m_isAlive;
*/
	CImage* m_shadow;
	CImage* m_img;
	float m_scale;

	bool m_isHeart;
	const int m_splitCountMax;
	int m_splitCount;

	bool m_isJumped;
	float m_jumpSpeed;
	float m_speed;

public:
	//그림자센터를 attackCenter에 맞추기
	//img센터를 hitCenter에 맞추기
	//점프하면 hit, attack꺼진다
	//

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);	//날 때린놈의 종류에 따라서 다른 반응 보여주기

	void SetRange();
	void SetPos();
	
	//분열 후 생성 포인트 계산기
	D2D1_POINT_2F CalculationSplitPos(D2D1_POINT_2F* origin, D2D1_POINT_2F* split);


	Sludgeheart();
	//분열체 생성자 (몇단계분열체인가, 생성 좌표)
	//처맞으면 피격시 center좌표 중심으로 특정 거리만큼 서로 반대로 떨어진 두 장소에 맞은 원본놈과 새로 생성된놈을 각각 놓는다. rnd로 줘
	//좌표 계산해서 생성자에 넣고 OBJMANAGER에 넣기
	Sludgeheart(int splitCount,D2D1_POINT_2F splitPos);
	~Sludgeheart();
};


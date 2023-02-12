#pragma once
// ============================================================
// # Utils (간소화 하고 싶거나 or 수학적 기능을 모아둔다.)
// ============================================================



// 디그리 1도의 라디안 값
#define DEG_TO_RAD		0.017453f
#define RAD_TO_DEG		57.2958f
#define PI				3.141592654f
//#define PI_2			PI *2 가안들어가는 이유 연산속도
#define PI_2			6.283185308f

#pragma region
/*

▶ 앱실론(Epsilon)
	2D -> int 많이 사용
	3D -> 소수점(F,D,D)
	- 부동 소수점은 실수를 정확히 표현할 수 없는 문제를 가지고 있다.
	ㄴ 왜냐면 수학적으로 실수는 무한한데 이 실수를 유한한 비트로 표현 하기 위해서는
	컴퓨터가 어쩔 수 없이 근사치(값) 표현해야 하기 때문

	- 이러한 구조적 문제로 발생하는게 부동소수점 반올림 오차

	- 이러한 특성때문에 필연적으로 오차 범위가 발생하는데 정확한 계산을 위해서 상수로 정의된 값을 사용한다.

	- 2D는 크게 인식을 못하는 경우가 많지는 않지만 3D 같은 경우는 결과값이 달라 질 수 있다.


*/
#pragma endregion

#define FLOAT_EPSILON	0.0001f
// 상수화 시키는 것 연산속도 올리기 위해서


// 사칙연산 제일 느린것 - 나눗셈 (계속 반복이 돌아감)
/*
▶ 캐스팅 연산자 종류

1. static_cast
플레이어 형변환시 논리적으로 가능할시 바꿔줌

2. const_cast
CONST로 선언된 변수를 가르키는 포인터 변수가 있을때,
CONST 변수는 유지한체 포인터만 자유롭게 풀어줌
원본변경X

3. dynamic_cast
상속관계에서 형변환을 프로그래메가 올바르게 하도록
도와주는 기능을 제공  BUT virtual function을 사용해야하며
 virtaul fuction 사용시 해당클래스에 v_table이 생성되고
 v_tablar에는 override된 자식클래스의 함수를 가르킬 수 있는 주소값이
 들어있는데 이 v-table에 클래스의 type_info 정보가 들어가게됨


4. reinterpret_cast
포인터 ->포인터,포인터 ->일반변수,일반변수->포인터로
주로 포인터 관련된 cast 자료형-> 다른자료형은 x
형변환시 해당자료형의 bit수에 맞게 드가는데
int -> char-> int 면 주소값다 표현x임





*/




// 실수를 정수형으로 변환(캐스팅)
#define FLOAT_TO_INT(f1)        static_cast<int>(f1+FLOAT_EPSILON)
// 스태틱캐스트와 강제형변환의 차이 ( static_cast 논리적으로 가능할시 바꿔줌, 강제형변환은 그냥 바꿔줌 (버그 잡는유무)
// 
// 
// 
//두 실수가 같은지 비교
//s fabs는 소수점 차의 절대값 반환
#define FLOAT_EQUAL(f1,f2)      (fabs(f1-f2)<= FLOAT_EPSILON)

namespace MY_UTIL
{

	float getDistance(float startX, float startY, float endX, float endY);

	float getAngle(float startX, float startY, float endX, float endY);

}
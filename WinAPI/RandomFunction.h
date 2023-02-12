#pragma once
#include "SingletonBase.h"

/*
과제1.마우스로 무작위 도형 출력하기
마우스로 클릭한 좌표에 무작위 도형을 출력
도형의 크기, 모양, 종류까지 모두 무작위

도형의 종류 : 사각형, 원, (선택) 삼각형 및 다각형
다각형 및 삼각형은 Polygon()을이용한다.

과제2. 내 얼굴 그려오기
도형!을 이용해서 그려온다.
마우스 클릭 좌표..등을 이용하면 반칙

과제3. 공 튕기기.(과제 1번과 2번 안됐으면 하지 마셈~)
큰 사각형 안에 작은 공이 있다.
작은 공은 큰 사각형 안에서 사용자 입력이 없어도 사각형 면에 튕기면서 움직인다.
큰 사각형은 키보드 입력을 받을 수 있으며 크기를 줄일 수 있다.
크기가 줄어들면 작은 공도 영향을 받는다.
마우스 왼쪽 클릭 : 공 속도 증가
마우스오른쪽클릭 : 공 속도 감소
예외 : 반전됐을 시 자연스럽게 원래대로 공을 유지한다.
힌트 : WM_TIMER / SetTimer / KillTimer

한줄조사 : GetAsyncKeyState

*/



class RandomFunction : public SingletonBase <RandomFunction>
{
public:
	RandomFunction() {
		srand(GetTickCount());
		//	난수 시드 초기화. GetTickCount():API함수로, os가 부팅된 후의 경과 시간을 체크한다.
		// 밀리세컨단위의 체크, 반환이 되기 때문에 규모가 크지 않은 난수에 좋다.
	}
	~RandomFunction(){}

	inline int getInt(int num)	{		return rand() % num;	}
	inline int getFromIntTo(int fromNum, int toNum) { return rand() % (toNum - fromNum + 1) + fromNum; }
	inline float getfloat(void) {		return (float)rand() / (float)RAND_MAX;}
	inline float getfloat(float num) { return (float)rand() / (float)RAND_MAX*num; }
	float getFromFloatTo(float fromNum, int toNum) {
		float rnd = (float)rand() / (float)RAND_MAX;
		return(rnd * (toNum = fromNum) + fromNum);
	}

	HRESULT Init(void) { return S_OK; }

private:

};
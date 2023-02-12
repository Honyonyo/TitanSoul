#pragma once
#include "stdafx.h"//이게 들어가도 되는거 맞나?
inline POINT PointMake(int x, int y) {
	POINT pt = { x, y };
	return pt;
}

//선그리기
inline void LineMake(HDC hdc, int startX, int startY, int endX, int endY) {
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

//포인트로 선그리기
inline void LineMake(HDC hdc, POINT start, POINT end) {
	MoveToEx(hdc, start.x, start.y, NULL);
	LineTo(hdc, end.x, end.y);
}


//사각형만들기. 헤더산개를 막기위해 여기서 함수로 감싸준 것
inline RECT RectMake(int x, int y, int width, int height) {
	RECT rc = { x, y, width, height };
	return rc;
}

//사각형 만들기(중심점과 길이를 이용)
inline RECT RectMakeCenter(int x, int y, int width, int height) {
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}
inline RECT RECTMakeLT(int rcleft, int rctop, int width, int height) {
	RECT rc = { rcleft, rctop, rcleft+width, rctop+height };
	return rc;
}
//사각형 만들기 (중심점포인트와 길이를 이용)
inline RECT RectMakeCenter(POINT pt, int width, int height) {
	RECT rc = { pt.x - width / 2, pt.y - height / 2, pt.x + width / 2, pt.y + height / 2 };
	return rc;
}

//사각형그리기
inline void DrawRectMake(HDC hdc, RECT rc) {
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

//사각형 그리기 (렉탱글 함수)
inline void RectangleMake(HDC hdc, int x, int y, int width, int height) {
	Rectangle(hdc, x, y, x+width, y+height);
}
//사각형그리기 (중심점과 렉탱글 함수)
inline void RectangleMakeCenter(HDC hdc, int x, int y, int width, int height) {
	Rectangle(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}
//사각형그리기 (중심점포인트와 렉탱글 함수)
inline void RectangleMakeCenter(HDC hdc, POINT pt, int width, int height) {
	Rectangle(hdc, pt.x - width / 2, pt.y - height / 2, pt.x + width / 2, pt.y + height / 2);
}


//타원 그리기 (일립스)
inline void EllipseMake(HDC hdc, int startX, int startY, int width, int height) {
	Ellipse(hdc, startX, startY, startX + width, startY + height);
}

//타원 그리기 (일립스, 중점과 반지름)
inline void EllipseMakeCenter(HDC hdc, int centerX, int centerY, int halfWidth, int halfHeight) {
	Ellipse(hdc, centerX - halfWidth, centerY - halfHeight, centerX + halfWidth, centerY + halfHeight);
}

//타원 그리기 (일립스, 중점포인트와 반지름)
inline void EllipseMakeCenter(HDC hdc, POINT center, int halfWidth, int halfHeight) {
	Ellipse(hdc, center.x - halfWidth, center.y - halfHeight, center.x + halfWidth, center.y + halfHeight);
}

//거리의 제곱
float getDistancePow(POINT pt1, POINT pt2) {
	return (pt2.x - pt1.x) * (pt2.x - pt1.x) + (pt2.y - pt1.y) * (pt2.y - pt1.y);
}

float Slop(POINT startP, POINT drct2) {
	return ((float)drct2.y - startP.y) / ((float)drct2.x - startP.x);
}

//충돌 영역 리사이징
inline RECT CollisionAreaResizing(RECT &rcDest, int width, int height)
{
	RECT rc = { rcDest.left + width / 2, rcDest.top + height / 2,
	rcDest.right - width / 2, rcDest.bottom - height / 2 };
	
	return rc;
}

// 
// # 내가 추가한 것 #
// 

//두 RECT가 겹친 부분이 있는지 판단하기 (딱 만나도 true)  아이거이미함수있음;
//inline bool RectOverlapRect(RECT rc1, RECT rc2)
//{
//	int x = (rc1.right - rc2.left) >= 0 ? (rc1.right - rc2.left) : -(rc1.right - rc2.left);
//	int y = (rc1.bottom - rc2.top) >= 0 ? (rc1.bottom - rc2.top) : -(rc1.bottom - rc2.top);
//	int width = rc1.right - rc1.left + rc2.right - rc2.left;
//	int height = rc1.bottom - rc1.top + rc2.bottom - rc2.top;
//
//	if ((x <= width) && (y <= height)) return true;
//	else return false;	
//}

//rect구조체 움직이기
inline void MoveRect(RECT rc, int moveRow,int moveColumn ) {
	rc.left += moveRow;
	rc.right+= moveRow;
	rc.top += moveColumn;
	rc.bottom += moveColumn;
}
//원본은 안움직이고 새 RECT를 반환하기
inline RECT RectMoveReturn(RECT rc, int moveRow, int moveColumn) {
	RECT tmp = rc;
	tmp.left += moveRow;
	tmp.right += moveRow;
	tmp.top += moveColumn;
	tmp.bottom += moveColumn;
	return tmp;
}


//큰 RECT 안에 작은 RECT가 있는가
inline bool RectInRect(RECT BIGrc, RECT SMALLrc) {
	if ((BIGrc.left < SMALLrc.left) && (BIGrc.right > SMALLrc.right) && (BIGrc.top < SMALLrc.top) && (BIGrc.bottom > SMALLrc.bottom))
		return true;
	else return false;
}

//1네모 안에 2네모가 포함관계인가(시작점과 길이)
inline bool RectInRect(int x, int y, int width, int height,
	int x2, int y2, int width2, int height2)
{
	if (x < x2 && (x2 + width2) < (x + width) && y < y2 && (y2 + height) < (y + height)) return true;
	else return false;
}

//포인트가 rect구조체 안에 있는지	//이미 다른 함수가 있다.. PtInRect(&rc,_ptMouse)
//inline bool PointInRect( POINT pt, RECT rc ) {
//	if ((rc.left < pt.x) && (pt.x < rc.right) && (rc.top < pt.y) && (pt.y < rc.bottom))	return true;
//	else return false;
//}

//포인트가 rectangle선 안에 있는지
inline bool PointInRect(POINT pt,int boxStartX, int boxStartY, int width, int height) {
	if ((boxStartX < pt.x) && (pt.x < boxStartX+width) && (boxStartY < pt.y) && (pt.y < boxStartX+height))	return true;
	else return false;
}

 //포인트가 시작점+길이로 만들어진 rectangle 선 안에 있는지
inline bool PointInRect(POINT pt, POINT boxStart, int width, int height) {
	if ((boxStart.x < pt.x) && (pt.x < boxStart.x + width) && (boxStart.y < pt.y) && (pt.y < boxStart.y + height))	return true;
	else return false;
}

//포인트가 원(중심점과 거리) 안에 있는지
inline bool PtInCircle(POINT pt, POINT centerCircle, int radiusCircle) {
	//피타고라스
	if (((centerCircle.x - pt.x) * (centerCircle.x - pt.x) + (centerCircle.y - pt.y, 2) * (centerCircle.y - pt.y, 2))
	> (radiusCircle * radiusCircle))
		return true;
	else return false;

	//삼각함수를 쓰려면 역탄젠트, 역코사인등등이 들어가야하는데,
	//각이 4개도출돼서 경우의 수 지정해야하므로 번거로우니 버린다.	
}
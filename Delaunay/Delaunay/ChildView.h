
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include "Vector2D.h"
#include <vector>

// CChildView 창
struct Triangle;
class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	std::vector<CVector2D> pointsList;
	std::vector<std::shared_ptr<Triangle>> ret;

// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};


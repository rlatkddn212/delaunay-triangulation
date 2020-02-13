
// ChildView.cpp: CChildView 클래스의 구현
//

#include "stdafx.h"
#include "Delaunay.h"
#include "ChildView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <iostream>
#include <vector>
#include "Vector2D.h"
#include <list>
#include <memory>
#include <math.h>
#include <stdio.h>
#include <algorithm>
using namespace std;

double TRIANGLE_SIZE = 1000.0;

struct Triangle;
struct Node
{
	Node()
	{
		triangle = nullptr;
		p0 = nullptr;
		p1 = nullptr;
		visited = false;
	}

	shared_ptr<Triangle> triangle;

	shared_ptr<Node> p0;
	shared_ptr<Node> p1;

	vector<shared_ptr<Node>> childs;
	bool visited;

	void AddChlid(shared_ptr<Node> newNode)
	{
		childs.push_back(newNode);
	}
};

struct Triangle
{
	CVector2D p0;
	CVector2D p1;
	CVector2D p2;

	weak_ptr<Node> thisNode;

	vector<shared_ptr<Triangle>> adj;

	Triangle(CVector2D _p0, CVector2D _p1, CVector2D _p2)
		: adj(3, nullptr)
	{
		p0 = _p0;
		p1 = _p1;
		p2 = _p2;
	}

	void print()
	{
		//printf("삼각형 : p1(%f, %f), p2(%f, %f), p3(%f, %f)\n",
		//	p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
	}

	bool IsOnPoint(CVector2D p)
	{
		if (p0.Equal(p) || p1.Equal(p) || p2.Equal(p))
		{
			return true;
		}

		return false;
	}

	bool IsPointInTriangle(CVector2D p)
	{
		if ((ccw(p - p0, p1 - p0) <= 0.0) &&
			(ccw(p - p1, p2 - p1) <= 0.0) &&
			(ccw(p - p2, p0 - p2) <= 0.0))
		{
			return true;
		}

		return false;
	}

	bool IsOnLine(CVector2D p)
	{
		if ((ccw(p - p0, p1 - p0) == 0.0) ||
			(ccw(p - p1, p2 - p1) == 0.0) ||
			(ccw(p - p2, p0 - p2) == 0.0))
		{
			return true;
		}

		return false;
	}

	int GetEdgeWithAdj(shared_ptr<Triangle> triangle)
	{
		for (size_t i = 0; i < adj.size(); ++i)
		{
			if (triangle == adj[i])
			{
				return i;
			}
		}

		return -1;
	}

	int GetEdge(CVector2D p)
	{
		if (ccw(p - p0, p1 - p0) == 0.0)
		{
			return 0;
		}
		else if (ccw(p - p1, p2 - p1) == 0.0)
		{
			return 1;
		}
		else if (ccw(p - p2, p0 - p2) == 0.0)
		{
			return 2;
		}

		return -1;
	}

	int GetSideEdge(CVector2D p)
	{
		if (p.Equal(p0))
		{
			if (adj[1] &&
				(adj[1]->p0.Equal(p) ||
					adj[1]->p1.Equal(p) ||
					adj[1]->p2.Equal(p)))
			{
				printf("Edge 오류\n");
			}

			return 1;
		}
		else if (p.Equal(p1))
		{
			if (adj[2] &&
				(adj[2]->p0.Equal(p) ||
					adj[2]->p1.Equal(p) ||
					adj[2]->p2.Equal(p)))
			{
				printf("Edge 오류\n");
			}

			return 2;
		}
		else if (p.Equal(p2))
		{
			if (adj[0] &&
				(adj[0]->p0.Equal(p) ||
					adj[0]->p1.Equal(p) ||
					adj[0]->p2.Equal(p)))
			{
				printf("Edge 오류\n");
			}

			return 0;
		}

		return -1;
	}

	CVector2D GetSidePoint(int edge)
	{
		switch (edge)
		{
		case 0:
			if (0 != GetSideEdge(p2))
			{
				printf("Point 오류\n");
			}

			return p2;
			break;
		case 1:
			if (1 != GetSideEdge(p0))
			{
				printf("Point 오류\n");
			}
			return p0;
			break;
		case 2:
			if (2 != GetSideEdge(p1))
			{
				printf("Point 오류\n");
			}
			return p1;
			break;

		default:
			break;
		}
	}

	pair<CVector2D, CVector2D> GetEdgePoint(int edge)
	{
		switch (edge)
		{
		case 0:
			return make_pair(p0, p1);
			break;
		case 1:
			return make_pair(p1, p2);
			break;
		case 2:
			return make_pair(p2, p0);
			break;

		default:
			break;
		}
	}

	vector<shared_ptr<Triangle>> SplitTriangle(CVector2D p)
	{
		vector<shared_ptr<Triangle>> ret;

		int edge = GetEdge(p);

		switch (edge)
		{
		case 0:
			ret.push_back(make_shared<Triangle>(p0, p, p2));
			ret.push_back(make_shared<Triangle>(p, p1, p2));
			ret[0]->adj[1] = ret[1];
			ret[0]->adj[2] = adj[2];

			for (int i = 0; i < 3; ++i)
			{
				if (adj[2]->adj[i].get() == this)
				{
					adj[2]->adj[i] = ret[0];
					break;
				}
			}

			ret[1]->adj[1] = adj[1];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[1]->adj[i].get() == this)
				{
					adj[1]->adj[i] = ret[1];
					break;
				}
			}
			ret[1]->adj[2] = ret[0];

			break;
		case 1:
			ret.push_back(make_shared<Triangle>(p1, p, p0));
			ret.push_back(make_shared<Triangle>(p, p2, p0));
			ret[0]->adj[1] = ret[1];
			ret[0]->adj[2] = adj[0];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[0]->adj[i].get() == this)
				{
					adj[0]->adj[i] = ret[0];
					break;
				}
			}

			ret[1]->adj[1] = adj[2];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[2]->adj[i].get() == this)
				{
					adj[2]->adj[i] = ret[1];
					break;
				}
			}
			ret[1]->adj[2] = ret[0];

			break;
		case 2:
			ret.push_back(make_shared<Triangle>(p2, p, p1));
			ret.push_back(make_shared<Triangle>(p, p0, p1));
			ret[0]->adj[1] = ret[1];
			ret[0]->adj[2] = adj[1];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[1]->adj[i].get() == this)
				{
					adj[1]->adj[i] = ret[0];
					break;
				}
			}
			ret[1]->adj[1] = adj[0];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[0]->adj[i].get() == this)
				{
					adj[0]->adj[i] = ret[1];
					break;
				}
			}
			ret[1]->adj[2] = ret[0];

			break;
		default:
			break;
		}

		// DAG
		shared_ptr<Node> parentNode = thisNode.lock();

		shared_ptr<Node> newNode0 = make_shared<Node>();
		shared_ptr<Node> newNode1 = make_shared<Node>();

		newNode0->triangle = ret[0];
		newNode1->triangle = ret[1];

		ret[0]->thisNode = newNode0;
		ret[1]->thisNode = newNode1;

		// 분리된 삼각형을 DAG에 연결한다.
		parentNode->AddChlid(newNode0);
		parentNode->AddChlid(newNode1);

		return ret;
	}
};

/**
 * 들로네 삼각형을 생성하기 위해 커다란 삼각형을 만든다.
 *
 * @param points 점들의 집함
 * @return The large triangle
 */
shared_ptr<Triangle> GetLargeTriangle(const vector<CVector2D>& points)
{
	double minX = points[0].x;
	double maxX = points[0].x;

	double minY = points[0].y;
	double maxY = points[0].y;

	for (size_t i = 0; i < points.size(); ++i)
	{
		minX = min(points[i].x, minX);
		maxX = max(points[i].x, maxX);
		minY = min(points[i].y, minY);
		maxY = max(points[i].y, maxY);
	}

	return make_shared<Triangle>(CVector2D((minX + maxX) / 2.0, maxY + (maxY - minY) * 100.0 + TRIANGLE_SIZE),
		CVector2D(minX - (maxX - minX) * 100.0 - TRIANGLE_SIZE, minY - (maxY - minY) * 100.0 - TRIANGLE_SIZE),
		CVector2D(maxX + (maxX - minX) * 100.0 + TRIANGLE_SIZE, minY - (maxY - minY) * 100.0 - TRIANGLE_SIZE));
}

class TriangleDAG
{
public:
	TriangleDAG(shared_ptr<Triangle> tri)
	{
		root = make_shared<Node>();
		root->triangle = tri;
		tri->thisNode = root;
	}

	~TriangleDAG()
	{

	}

	shared_ptr<Node> GetNode(shared_ptr<Node> n, const CVector2D& p)
	{
		if (n->childs.empty()) return n;

		for (size_t i = 0; i < n->childs.size(); ++i)
		{
			if (n->childs[i]->triangle->IsPointInTriangle(p))
			{
				shared_ptr<Node> node = GetNode(n->childs[i], p);
				if (node != nullptr)
				{
					return node;
				}
			}
		}

		return nullptr;
	}

	shared_ptr<Node> GetNode(const CVector2D& p)
	{
		return GetNode(root, p);
	}

	void GenTriangle(shared_ptr<Node> n)
	{
		if (n->childs.empty())
		{
			ret.push_back(n->triangle);

			return;
		}

		for (size_t i = 0; i < n->childs.size(); ++i)
		{
			if (!n->childs[i]->visited)
			{
				n->childs[i]->visited = true;
				GenTriangle(n->childs[i]);
			}
		}
	}

	vector<shared_ptr<Triangle>> GenTriangle()
	{
		ret.clear();
		GenTriangle(root);

		return ret;
	}

private:
	shared_ptr<Node> root;
	vector<shared_ptr<Triangle>> ret;
};

bool IsLegal(CVector2D point, shared_ptr<Triangle> triangle)
{
	CVector2D a = triangle->p0 - triangle->p2;
	CVector2D b = triangle->p1 - triangle->p2;

	double v = ccw(a, b);

	double adx = triangle->p0.x - point.x, ady = triangle->p0.y - point.y,
		bdx = triangle->p1.x - point.x, bdy = triangle->p1.y - point.y,
		cdx = triangle->p2.x - point.x, cdy = triangle->p2.y - point.y,
		bdxcdy = bdx * cdy, cdxbdy = cdx * bdy,
		cdxady = cdx * ady, adxcdy = adx * cdy,
		adxbdy = adx * bdy, bdxady = bdx * ady,
		alift = adx * adx + ady * ady,
		blift = bdx * bdx + bdy * bdy,
		clift = cdx * cdx + cdy * cdy;

	double det = alift * (bdxcdy - cdxbdy)
		+ blift * (cdxady - adxcdy)
		+ clift * (adxbdy - bdxady);

	if (v > 0) return det >= 0;
	else return det <= 0;
}

void checkAdj(shared_ptr<Triangle> t)
{
#ifdef _DEBUG
	for (int i = 0; i < 3; ++i)
	{
		if (t->adj[i])
		{
			bool isFind = false;
			for (int j = 0; j < 3; ++j)
			{
				if (t->adj[i]->adj[j] == t)
				{
					isFind = true;
				}
			}

			if (!isFind)
			{
				printf("삼각형 인접 오류\n");
				int a = 0;
			}
		}
	}

	t->GetSideEdge(t->p0);
	t->GetSideEdge(t->p1);
	t->GetSideEdge(t->p2);

	t->GetSidePoint(0);
	t->GetSidePoint(1);
	t->GetSidePoint(2);
#endif _DEBUG
}

vector<shared_ptr<Triangle>> Flip(shared_ptr<Triangle> t0, int e0, shared_ptr<Triangle> t1, int e1)
{
	CVector2D a = t0->GetSidePoint(e0);
	CVector2D b = t1->GetSidePoint(e1);

	pair<CVector2D, CVector2D> p = t0->GetEdgePoint(e0);

	if (ccw(b - a, p.first - a) <= 0.0)
	{
		CVector2D temp = p.first;
		p.first = p.second;
		p.second = temp;
	}

	vector<shared_ptr<Triangle>> ret;
	ret.push_back(make_shared<Triangle>(a, b, p.first));
	ret.push_back(make_shared<Triangle>(b, a, p.second));

	// ret[0] 링크
	ret[0]->adj[0] = ret[1];
	int t1p2Edge = t1->GetSideEdge(p.second);
	if (t1p2Edge != -1)
	{
		ret[0]->adj[1] = t1->adj[t1p2Edge];
		shared_ptr<Triangle> adj = t1->adj[t1p2Edge];
		if (adj)
		{
			adj->adj[adj->GetEdgeWithAdj(t1)] = ret[0];
		}
	}

	int t0p2Edge = t0->GetSideEdge(p.second);
	if (t0p2Edge != -1)
	{
		ret[0]->adj[2] = t0->adj[t0p2Edge];
		shared_ptr<Triangle> adj = t0->adj[t0p2Edge];
		if (adj)
		{
			adj->adj[adj->GetEdgeWithAdj(t0)] = ret[0];
		}
	}

	// ret[1] 링크
	ret[1]->adj[0] = ret[0];
	int t0p1Edge = t0->GetSideEdge(p.first);
	if (t0p1Edge != -1)
	{
		ret[1]->adj[1] = t0->adj[t0p1Edge];
		shared_ptr<Triangle> adj = t0->adj[t0p1Edge];
		if (adj)
		{
			adj->adj[adj->GetEdgeWithAdj(t0)] = ret[1];
		}
	}

	int t1p1Edge = t1->GetSideEdge(p.first);
	if (t1p1Edge != -1)
	{
		ret[1]->adj[2] = t1->adj[t1p1Edge];
		shared_ptr<Triangle> adj = t1->adj[t1p1Edge];
		if (adj)
		{
			adj->adj[adj->GetEdgeWithAdj(t1)] = ret[1];
		}
	}

	// DAG
	shared_ptr<Node> parentNode0 = t0->thisNode.lock();
	shared_ptr<Node> parentNode1 = t1->thisNode.lock();

	shared_ptr<Node> newNode0 = make_shared<Node>();
	shared_ptr<Node> newNode1 = make_shared<Node>();

	newNode0->triangle = ret[0];
	newNode1->triangle = ret[1];

	ret[0]->thisNode = newNode0;
	ret[1]->thisNode = newNode1;

	ret[0]->print();
	ret[1]->print();

	checkAdj(ret[0]);
	checkAdj(ret[1]);

	// 분리된 삼각형을 DAG에 연결한다.
	parentNode0->AddChlid(newNode0);
	parentNode0->AddChlid(newNode1);
	parentNode1->AddChlid(newNode0);
	parentNode1->AddChlid(newNode1);

	return ret;
}

void LegalizeEdge(CVector2D point, shared_ptr<Triangle> triangle)
{
	int edgeNum = triangle->GetSideEdge(point);
	if (edgeNum == -1)
	{
		return;
	}

	// 인접한 삼각형에서 겹쳐진 edge를 가져온다.
	shared_ptr<Triangle> adjTriangle = triangle->adj[edgeNum];
	if (adjTriangle == nullptr)
	{
		return;
	}

	int adjNum = adjTriangle->GetEdgeWithAdj(triangle);
	if (adjNum == -1)
	{
		return;
	}

	CVector2D p = adjTriangle->GetSidePoint(adjNum);
	if (IsLegal(p, triangle))
	{
		// 플립
		vector<shared_ptr<Triangle>> newTriangle = Flip(triangle, edgeNum, adjTriangle, adjNum);
		LegalizeEdge(point, newTriangle[0]);
		LegalizeEdge(point, newTriangle[1]);
	}
}

vector<shared_ptr<Triangle>> DelaunayTriangulate(const vector<CVector2D>& points)
{
	// 점들을 분석하여 커다란 삼각형을 만든다.
	shared_ptr<Triangle> largeTriangle = GetLargeTriangle(points);
	TriangleDAG dag(largeTriangle);

	for (size_t i = 0; i < points.size(); ++i)
	{
		// node 위치를 가져와야함
		shared_ptr<Node> node = dag.GetNode(points[i]);
		shared_ptr<Triangle> triangle = node->triangle;

		// 삼각형 안에 있을 경우
		if (!triangle->IsOnLine(points[i]))
		{
			// 삼각형을 분할한다.
			shared_ptr<Triangle> t0 = make_shared<Triangle>(points[i], triangle->p0, triangle->p1);
			shared_ptr<Triangle> t1 = make_shared<Triangle>(points[i], triangle->p1, triangle->p2);
			shared_ptr<Triangle> t2 = make_shared<Triangle>(points[i], triangle->p2, triangle->p0);

			// 인접 삼각형 설정 (넣는 순서에 주의)
			t0->adj[0] = t2;
			t0->adj[1] = triangle->adj[0];
			t0->adj[2] = t1;
			shared_ptr<Triangle> adj0 = triangle->adj[0];
			if (adj0)
			{
				adj0->adj[adj0->GetEdgeWithAdj(triangle)] = t0;
			}

			t1->adj[0] = t0;
			t1->adj[1] = triangle->adj[1];
			t1->adj[2] = t2;
			shared_ptr<Triangle> adj1 = triangle->adj[1];
			if (adj1)
			{
				adj1->adj[adj1->GetEdgeWithAdj(triangle)] = t1;
			}

			t2->adj[0] = t1;
			t2->adj[1] = triangle->adj[2];
			t2->adj[2] = t0;
			shared_ptr<Triangle> adj2 = triangle->adj[2];
			if (adj2)
			{
				adj2->adj[adj2->GetEdgeWithAdj(triangle)] = t2;
			}

			// 삼각형을 DAG에 연결
			shared_ptr<Node> parentNode = triangle->thisNode.lock();

			shared_ptr<Node> newNode0 = make_shared<Node>();
			shared_ptr<Node> newNode1 = make_shared<Node>();
			shared_ptr<Node> newNode2 = make_shared<Node>();

			newNode0->triangle = t0;
			newNode1->triangle = t1;
			newNode2->triangle = t2;

			t0->thisNode = newNode0;
			t1->thisNode = newNode1;
			t2->thisNode = newNode2;

			t0->print();
			t1->print();
			t2->print();

			checkAdj(t0);
			checkAdj(t1);
			checkAdj(t2);

			// 분리된 삼각형을 DAG에 연결한다.
			parentNode->AddChlid(newNode0);
			parentNode->AddChlid(newNode1);
			parentNode->AddChlid(newNode2);

			// 모든 Edge를 legal하게 만든다.
			LegalizeEdge(points[i], t0);
			LegalizeEdge(points[i], t1);
			LegalizeEdge(points[i], t2);
		}
		// edge에 겹칠 경우
		else
		{
			// 겹쳐진 edge 가져온다.
			int edgeNum = triangle->GetEdge(points[i]);
			// 인접한 삼각형에서 겹쳐진 edge를 가져온다.
			shared_ptr<Triangle> adjTriangle = triangle->adj[edgeNum];

			// 삼각형 생성
			vector<shared_ptr<Triangle>> t0 = triangle->SplitTriangle(points[i]);
			vector<shared_ptr<Triangle>> t1 = adjTriangle->SplitTriangle(points[i]);

			if ((t0[0]->p0.Equal(t1[0]->p0) && t0[0]->p1.Equal(t1[0]->p1)) ||
				(t0[0]->p0.Equal(t1[0]->p1) && t0[0]->p1.Equal(t1[0]->p0)))
			{
				t0[0]->adj[0] = t1[0];
				t0[1]->adj[0] = t1[1];

				t1[0]->adj[0] = t0[0];
				t1[1]->adj[0] = t0[1];
			}
			else
			{
				t0[0]->adj[0] = t1[1];
				t0[1]->adj[0] = t1[0];

				t1[0]->adj[0] = t0[1];
				t1[1]->adj[0] = t0[0];
			}

			t0[0]->print();
			t0[1]->print();
			t1[0]->print();
			t1[1]->print();

			checkAdj(t0[0]);
			checkAdj(t0[1]);
			checkAdj(t1[0]);
			checkAdj(t1[1]);

			LegalizeEdge(points[i], t0[0]);
			LegalizeEdge(points[i], t0[1]);
			LegalizeEdge(points[i], t1[0]);
			LegalizeEdge(points[i], t1[1]);
		}
	}

	vector<shared_ptr<Triangle>> ret = dag.GenTriangle();
	vector<shared_ptr<Triangle>> res;
	for (size_t i = 0; i < ret.size(); ++i)
	{
		if (largeTriangle->IsOnPoint(ret[i]->p0) ||
			largeTriangle->IsOnPoint(ret[i]->p1) ||
			largeTriangle->IsOnPoint(ret[i]->p2))
		{
			continue;
		}

		res.push_back(ret[i]);
	}

	return res;
}

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CVector2D newPos = CVector2D(point.x, point.y);
	
	for (int i = 0; i < pointsList.size(); ++i)
	{
		if (pointsList[i].Equal(newPos))
			return;
	}
		
	pointsList.push_back(newPos);
	ret = DelaunayTriangulate(pointsList);
	Invalidate();
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	pointsList.clear();
	Invalidate();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
	
	for (int i = 0; i < pointsList.size(); i++)
	{
		dc.SetPixel(pointsList[i].x, pointsList[i].y, RGB(255, 0, 0));
	}

	CPen pen(PS_SOLID, 1, RGB(0, 255, 0));
	dc.SelectObject(&pen);
	for (int i = 0; i < ret.size(); ++i)
	{
		dc.MoveTo(ret[i]->p0.x, ret[i]->p0.y);
		dc.LineTo(ret[i]->p1.x, ret[i]->p1.y);
		dc.LineTo(ret[i]->p2.x, ret[i]->p2.y);
		dc.LineTo(ret[i]->p0.x, ret[i]->p0.y);
	}
}


#include "cgepublicmath.h"

PublicMath::PublicMath()
{

}

PublicMath::~PublicMath()
{

}


//若_x1<=_x0<=_x2 or _x2<=_x0<=_x1, 返回 true, 否则返回 false
bool PublicMath::between(double _x0, double _x1, double _x2)
{
	double temp1,temp2;
	temp1 = _x0 - _x1;
	temp2 = _x0 - _x2;

  	if ((temp1 < EPSILON && temp2 > -EPSILON) || (temp2 < EPSILON && temp1 > -EPSILON))
	{
		return true;
	}
	else 
	{
		return false;
	}
}


//获得两点之间的距离
double PublicMath::getDistance(PlanePoint _p1,PlanePoint _p2)
{
	double temp1,temp2;
	temp1 = (_p1.x - _p2.x) * (_p1.x - _p2.x);
	temp2 = (_p1.y - _p2.y) * (_p1.y - _p2.y);
	return(sqrt (temp1 + temp2));
}

//计算比值(a-b)/(c-b)
double PublicMath::delta(double _a, double _b, double _c)
{
	double temp;

	if (_b == _c)
	{
		temp = 1.0;
	}
	else
	{
		temp = (_a - _b) / (_c - _b);
	}

	return temp;
}

//判断两线段之间的相交关系
//若两线段重叠,则返回的是某一线段的端点坐标
bool PublicMath::lineSegmRelation(PlanePoint _p1, PlanePoint _p2,PlanePoint _p3, PlanePoint _p4, PlanePoint &_intersectPoint)
{
	if ((fabs(_p1.x - _p2.x) < EPSILON) && (fabs(_p3.x - _p4.x) < EPSILON))
	 {
		 return false;
	 }
	 //如果直线段_p1_p2垂直与y轴
	 else if ((fabs (_p1.x - _p2.x) < EPSILON))
	 {
		 if (between(_p1.x, _p3.x, _p4.x))
		 {
			 double k = (_p4.y - _p3.y) / (_p4.x - _p3.x);
			 _intersectPoint.x = _p1.x;
			 _intersectPoint.y = k * (_intersectPoint.x - _p3.x) + _p3.y;

			 if (between(_intersectPoint.y, _p1.y, _p2.y))
			 {
				 return true;
			 }
			 else
			 {
				 return false;
			 }
		 }
		 else 
		 {
			 return false;
		 }
	 }
	 //如果直线段_p3_p4垂直与y轴
	 else if ((fabs(_p3.x - _p4.x) < EPSILON))
	 {
		 if (between(_p3.x, _p1.x, _p2.x))
		 {
			 double k = (_p2.y - _p1.y) / (_p2.x - _p1.x);
			 _intersectPoint.x = _p3.x;
			 _intersectPoint.y = k * (_intersectPoint.x - _p2.x) + _p2.y;

			 if (between(_intersectPoint.y, _p3.y, _p4.y))
			 {
				 return true;
			 }
			 else
			 {
				 return false;
			 }
		 }
		 else 
		 {
			 return false;
		 }
	 }
	 else
     {
		 double k1 = (_p2.y - _p1.y) / (_p2.x - _p1.x); 
	     double k2 = (_p4.y - _p3.y) / (_p4.x - _p3.x);

		 //两线段平行
    	 if (fabs(k1 - k2) < EPSILON)
		 {
			 _intersectPoint.x = _p3.x; 
			 _intersectPoint.y = k1 * (_intersectPoint.x - _p1.x) + _p1.y;

			 if (fabs(_intersectPoint.y - _p3.y) < EPSILON)
			 {
				 //两线段在同一直线上
				 if (between(_p3.x, _p1.x, _p2.x))
				 {
					 //两线段部分重叠,_p3在_p1_p2内
					 _intersectPoint.x = _p3.x; 
					 _intersectPoint.y = _p3.y; 
					 return true;
				 }
				 else if (between(_p4.x, _p1.x, _p2.x))
				 {
					 //两线段部分重叠,_p4在_p1_p2内
					 _intersectPoint.x = _p4.x; 
					 _intersectPoint.y = _p4.y;
					 return true;
				 }
				 else if (between(_p3.x, _p1.x, _p2.x) && between(_p4.x, _p1.x, _p2.x))
				 {
					 //_p1_p2包含了_p3_p4
					 _intersectPoint.x = _p4.x; 
					 _intersectPoint.y = _p4.y;
					 return true;
				 }
				 else
				 {
					 return false;
				 }
			 }
			 else
			 {
				 return false;
			 }
		 }
	     else 
		 {
	    	 _intersectPoint.x = ((_p3.y - _p1.y) - (k2 * _p3.x - k1 * _p1.x)) / (k1-k2);
	    	 _intersectPoint.y = k1 * (_intersectPoint.x - _p1.x) + _p1.y;
		 }

		 if (between(_intersectPoint.x, _p1.x, _p2.x) && between(_intersectPoint.x, _p3.x, _p4.x))
		 {
			 return true;
		 }
    	 else 
		 {
    		 return false;
		 }
	 }
}

// 判断线段与多边形的相交关系,返回交点数
int PublicMath::linePolyRelation(PlanePoint _p1, PlanePoint _p2, BrokenLine _polygon, PlanePoint &_intersectsectPoint)
{
	int i;

	double tempDistance = getDistance(_p1, _p2);
	if (tempDistance < EPSILON)
	{
		return 0;
	}

	//多边形为空直接返回
	int polygonPointNum = _polygon.size();
	if (polygonPointNum <= 0)
	{
		return 0;
	}

	//移除相邻的相同点以
	std::vector<SpacePoint> tempPointVec;
	tempPointVec.push_back(_polygon[0]);
	PlanePoint tempPoint1, tempPoint2;

	for (i = 1; i < polygonPointNum; i++)
	{
		tempPoint1.x = _polygon[i].x;
		tempPoint1.y = _polygon[i].y;
		tempPoint2.x = tempPointVec.back().x;
		tempPoint2.y = tempPointVec.back().y;
		double distance  = getDistance(tempPoint1,tempPoint2);

		if (distance > EPSILON)
		{
			tempPointVec.push_back(_polygon[i]);
		}
	}

	//移除与_p1_p2重叠的线段
	_polygon.clear();
	_polygon = tempPointVec;
	tempPointVec.clear();
	tempPointVec.push_back(_polygon[0]);
	polygonPointNum = _polygon.size();

	for (i = 1; i < polygonPointNum; i++)
	{
		if (fabs(tempPointVec.back().y - _p1.y) < EPSILON && fabs(_polygon[i].y - _p1.y) < EPSILON)
		{
			if (i == polygonPointNum - 1)
			{
				//终点和起始点连线与_p1_p2重合
				tempPointVec.pop_back();
			}
			else
			{
				continue;
			}
		}
		else
		{
			tempPointVec.push_back(_polygon[i]);
		}
	}

	_polygon.clear();
	_polygon = tempPointVec;
	polygonPointNum = _polygon.size();

	if (polygonPointNum < 2)
	{
		return 0;
	}

	//判断起始结束点是否一致
	tempPoint1.x = _polygon.front().x;
	tempPoint1.y = _polygon.front().y;
	tempPoint2.x = _polygon.back().x;
	tempPoint2.y = _polygon.back().y;
	double distance = getDistance(tempPoint1, tempPoint2);

	if (distance > EPSILON)
	{
		_polygon.push_back(_polygon.front());
	}

	int intersectPointNum = 0; //交点个数
	std::vector<PlanePoint> intersectPointVec; //与断层多边形会有多个交点

	int preFlag = -1; //上一次相交的断层线标号
	PlanePoint p3, p4, temp;

	//获得所有交点
	for (i=0; i < (polygonPointNum - 1); i++)
	{
		p3.x = _polygon[i].x;
		p3.y = _polygon[i].y;
		p4.x = _polygon[i+1].x;
		p4.y = _polygon[i+1].y;

		if (lineSegmRelation(_p1, _p2, p3, p4, temp))
		{
			intersectPointVec.push_back(temp);

			//判断是否交在连续的两条线段的端点
			if (preFlag < 0)
			{
				intersectPointNum++;
				preFlag = i;
			}
			else
			{
				//判断两点是否连续
				int indexdiff = i - preFlag;
				if (indexdiff == 1 || indexdiff == polygonPointNum - 1)
				{
					PlanePoint tp1, tp2, tp3, tp4, tp5;
					tp1.x = _polygon[i].x;
					tp1.y = _polygon[i].y;
					tp3.x = _polygon[i+1].x;
					tp3.y = _polygon[i+1].y;
					tp4.x = _polygon[i-1].x;
					tp4.y = _polygon[i-1].y;

					tp2 = _p1;
					tp5 = _p2;
					
					if (unitLineIntersect(tp1,tp2,tp3,tp4) || unitLineIntersect(tp1,tp5,tp3,tp4))
					{
						continue;
					}
					else
					{
						intersectPointNum++;
					}
				}
				else
				{
					intersectPointNum++;
				}

				preFlag = i;
			}
		}
	}

	//获得与距离p1近的交点
	double minDistance;
	int vectorSize = intersectPointVec.size();

	for (i = 0; i < vectorSize; i++)
	{
		temp = intersectPointVec[i];
		tempDistance = getDistance(_p1, temp);

		if (i==0)
		{
			minDistance = tempDistance;
			_intersectsectPoint = temp;
		}
		else if (tempDistance < minDistance)
		{
			minDistance = tempDistance;
			_intersectsectPoint = temp;
		}
	}

	return intersectPointNum;
}

//判断线段与多个多边形的相交关系
//相交返回true,否则返回false
bool PublicMath::lineSegmFaulRelation(PlanePoint _p1, PlanePoint _p2, Faultages _faultages)
{
	int i;
	int faultageNum = _faultages.size();

	for (i = 0; i < faultageNum; i++)
	{
		int flag = 0;
		PlanePoint tempPoint;

		flag = linePolyRelation(_p1, _p2, _faultages[i].polygon, tempPoint);

		if (flag > 0)
		{
			return true;
		}
	}

	return false;
}

// 判断归一化线段关系
bool PublicMath::unitLineIntersect(PlanePoint _p1, PlanePoint _p2, PlanePoint _p3, PlanePoint _p4)
{
	PlanePoint tp1,tp2,tp3,tp4;
	tp1 = _p1;
	tp2 = _p2;
	tp3 = _p3;
	tp4 = _p4;

	//对线段长度进行归一化
	double distance = getDistance(tp1, tp2);
	if (distance< EPSILON)
	{
		return true;
	}
	else
	{
		tp2.x = (tp2.x - tp1.x) / distance;
		tp2.y = (tp2.y - tp1.y) / distance;
	}
	
	distance = getDistance(tp1, tp3);
	if (distance < EPSILON)
	{
		return true;
	}
	else
	{
		tp3.x = (tp3.x - tp1.x) / distance;
		tp3.y = (tp3.y - tp1.y) / distance;
	}

	distance = getDistance(tp1, tp4);
	if (distance < EPSILON)
	{
		return true;
	}
	else
	{
		tp4.x = (tp4.x - tp1.x) / distance;
		tp4.y = (tp4.y - tp1.y) / distance;
	}

	tp1.x = 0.0;
	tp1.y = 0.0;
	
	PlanePoint temp;
	if (lineSegmRelation(tp1, tp2, tp3, tp4, temp))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//判断点是否在多边形上
bool PublicMath::pointOnPolygon(PlanePoint _p, BrokenLine _polygon)
{
	int i;
	//移除相邻的相同点
	int polygonPointNum = _polygon.size();

	if (polygonPointNum <= 0)
	{
		return false;
	}

	std::vector<SpacePoint> tempPointVec;
	tempPointVec.push_back(_polygon[0]);
	PlanePoint tempPoint1, tempPoint2;

	for (i = 1; i < polygonPointNum; i++)
	{
		tempPoint1.x = _polygon[i].x;
		tempPoint1.y = _polygon[i].y;
		tempPoint2.x = tempPointVec.back().x;
		tempPoint2.y = tempPointVec.back().y;

		double distance  = getDistance(tempPoint1,tempPoint2);
		if (distance > EPSILON)
		{
			tempPointVec.push_back(_polygon[i]);
		}
	}

	//判断起始结束点是否一致
	tempPoint1.x = tempPointVec.front().x;
	tempPoint1.y = tempPointVec.front().y;
	tempPoint2.x = tempPointVec.back().x;
	tempPoint2.y = tempPointVec.back().y;
	double distance = getDistance(tempPoint1, tempPoint2);

	if (distance > EPSILON)
	{
		tempPointVec.push_back(tempPointVec.front());
	}

	_polygon.clear();
	_polygon = tempPointVec;
	polygonPointNum = _polygon.size();

	if (polygonPointNum < 2)
	{
		return false;
	}

	//判断点是否在多边形上,若是则认为多边形内部
	for (i = 0; i < polygonPointNum - 1; i++)
	{
		tempPoint1.x = _polygon[i].x;
		tempPoint1.y = _polygon[i].y;
		tempPoint2.x = _polygon[i+1].x;
		tempPoint2.y = _polygon[i+1].y;

		if (pointOnLineSegm(_p, tempPoint1, tempPoint2))
		{
			return true;
		}
	}

	return false;
}

//判断点"_p"是否在线段"_p1_p2"上
bool PublicMath::pointOnLineSegm(PlanePoint _p, PlanePoint _p1, PlanePoint _p2)
{
	//若"_p1_p2"与y轴垂直
	if (fabs(_p1.x - _p2.x) < EPSILON)
	{
		if (fabs(_p.x - _p2.x) < EPSILON && between(_p.y, _p1.y, _p2.y))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		//计算斜率
		double k = (_p1.y - _p2.y) / (_p1.x - _p2.x);

		if (fabs(k) < EPSILON)
		{
			if (fabs(_p.y - _p1.y) < EPSILON && between(_p.x, _p1.x, _p2.x))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//判断_p是否在_p1_p2对应的直线上
		double x, y;
		x = _p.x;
		y = k * (x - _p1.x) + _p1.y;

		if (fabs(y - _p.y) < EPSILON && between(_p.y, _p1.y, _p2.y))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

// 判断点与多边形的关系(多边形边上的点认为在多边形内部)
// 返回true:包含;false:不包含
bool PublicMath::pointPolyRelation(PlanePoint _p, BrokenLine _polygon)
{

	//判断是否在多边形边上
	if (pointOnPolygon(_p, _polygon))
	{
		return true;
	}

	//_p与assistantPoint是一条向右的射线
	PlanePoint assistantPoint;
	assistantPoint.x = INFINITY;
	assistantPoint.y = _p.y;

	int intersectPointNum = 0;
	PlanePoint tempPoint;
	intersectPointNum = linePolyRelation(_p, assistantPoint, _polygon, tempPoint);

	if (intersectPointNum % 2 == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 判断点与多个多边形的关系 
// 返回true:至少被其中一个多边形包含;false:不包含
bool PublicMath::PointFaulRelation(PlanePoint _p, Faultages _faultages)
{
	int i;
	int faultageNum = _faultages.size();

	for (i = 0; i < faultageNum; i++)
	{
		bool flag = false;
		PlanePoint tempPoint;

		flag = pointPolyRelation(_p, _faultages[i].polygon);

		if (flag)
		{
			return true;
		}
	}

	return false;
}

// 判断平面上一个点是否落在由三个点构造的外接圆内
bool PublicMath::circumCircle(SpacePoint _px, SpacePoint _p1, SpacePoint _p2, SpacePoint _p3, SpacePoint &_pc, double &_r)
{
	double m1, m2, mx1, mx2, my1, my2;
	double dx, dy, rsqr, drsqr;
	
	if (fabs(_p1.y - _p2.y) < EPSILON && fabs(_p2.y - _p3.y) < EPSILON)
	{
		return (false);
	}
	
	if (fabs(_p2.y - _p1.y) < EPSILON)
	{
		m2 = -(_p3.x - _p2.x) /(_p3.y - _p2.y);
		mx2 = (_p2.x + _p3.x) / 2.0;
		my2 = (_p2.y + _p3.y) / 2.0;
		_pc.x = (_p2.x + _p1.x) / 2.0;
		_pc.y = m2 * (_pc.x - mx2) + my2;
	}
	else if (fabs(_p3.y - _p2.y) < EPSILON)
	{
		m1 = -(_p2.x - _p1.x) /(_p2.y - _p1.y);
		mx1 = (_p1.x + _p2.x) / 2.0;
		my1 = (_p1.y + _p2.y) / 2.0;
		_pc.x = (_p3.x + _p2.x) / 2.0;
		_pc.y = m1 * (_pc.x - mx1) + my1;
	}
	else	
	{
		m1 = -(_p2.x - _p1.x) / (_p2.y - _p1.y);
		m2 = -(_p3.x - _p2.x) / (_p3.y - _p2.y);
		mx1 = (_p1.x + _p2.x) / 2.0;
		mx2 = (_p2.x + _p3.x) / 2.0;
		my1 = (_p1.y + _p2.y) / 2.0;
		my2 = (_p2.y + _p3.y) / 2.0;
		_pc.x = (m1 * mx1 - m2 * mx2 + my2 - my1) /(m1 - m2);
		_pc.y = m1 *(_pc.x - mx1) + my1;
	}
	
	dx = _p2.x - _pc.x;
	dy = _p2.y - _pc.y;
	rsqr = dx * dx + dy * dy;
	_r = sqrt(rsqr);
	
	dx = _px.x - _pc.x;
	dy = _px.y - _pc.y;
	drsqr = dx * dx + dy * dy;
	
	return ((drsqr <= rsqr) ? true : false);
}
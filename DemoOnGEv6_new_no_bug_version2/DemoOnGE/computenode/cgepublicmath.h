#ifndef TRIANGLE_PUBLICMATH_H
#define TRIANGLE_PUBLICMATH_H

#include "cgedataformatdefine.h"

#include <cmath>

// This class is implement for some geometry operation
class PublicMath
{

public:

	PublicMath();
	~PublicMath();

	bool between(double _x0, double _x1, double _x2);
	double getDistance(PlanePoint _p1,PlanePoint _p2);
	bool lineSegmRelation(PlanePoint _p1, PlanePoint _p2,PlanePoint _p3, PlanePoint _p4, PlanePoint &_intersectPoint);
	int linePolyRelation(PlanePoint _p1, PlanePoint _p2, BrokenLine _polygon, PlanePoint &_intersectsectPoint);
	bool pointPolyRelation(PlanePoint _p, BrokenLine _polygon);
	bool pointOnPolygon(PlanePoint _p, BrokenLine _polygon);
	bool pointOnLineSegm(PlanePoint _p, PlanePoint _p1, PlanePoint _p2);
	bool circumCircle(SpacePoint _px, SpacePoint _p1, SpacePoint _p2, SpacePoint _p3, SpacePoint &_pc, double &_r);
	bool PointFaulRelation(PlanePoint _p, Faultages _faultages);
	bool lineSegmFaulRelation(PlanePoint _p1, PlanePoint _p2, Faultages _faultages);
	double delta(double _a, double _b, double _c);

private:

	bool unitLineIntersect(PlanePoint _p1, PlanePoint _p2, PlanePoint _p3, PlanePoint _p4);
};

#endif
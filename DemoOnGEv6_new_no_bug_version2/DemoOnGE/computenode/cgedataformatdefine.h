#ifndef CGEDATAFORMATDEFINE_H
#define CGEDATAFORMATDEFINE_H

#include <vector>
#include <qstring>
using namespace std;
#define EPSILON		1.0E-6	   //minimun distance of two value
#define INFINITY    1.0E10     //maximun value

//define 3d point
class SpacePoint
{
public:
	float x;            //x coordinate
	float y;            //y coordinate
	int index;           //index of the point at input data. the index should be constant both in triangulating and contour tracing
	
	QString continent;
	QString country;
	QString city;
	///int faultageFlag;    //if faultage point 1, else if non-faultage point 0
	///int layerFlag;       //if the first layer 0, else if non-first layer 1
	///double z;            //z coordinate

	bool operator < (const SpacePoint &p) const {return x < p.x;}
};

//define 2d point
typedef struct PlanePoint
{
	double x;            //x coordinate
	double y;            //y coordinate
	int layerFlag;       //if the first layer 0, else if non-first layer 1
}PlanePoint;

//define line segment, p1,p2 is index of corresponding points
typedef struct LineSegment
{
	int p1;
	int p2;
}LineSegment;

//define triangle, p[3] are index of three points, and neighbor[3] are tree neighbor triangle
//neighbor[0] is the neighbor triangle with p[0],p[1]
//neighbor[1] is the neighbor triangle with p[1],p[2]
//neighbor[2] is the neighbor triangle with p[0],p[2]
typedef struct Triangle
{
public:
	int p[3];
	int neighbor[3];
}Triangle;

//define broken line, polygon is also broken line with same start point and end point
typedef std::vector<SpacePoint> BrokenLine;

//define faultage
typedef struct Faultage
{
	int faultageType;		 //if forward faultage point 1, else reverse faultage -1
	QString faultageName;    //faultage name
	BrokenLine lower;		 //the highter part
	BrokenLine upper;		 //the lower part
	BrokenLine polygon;		 //polygon form

}Faultage;

//define set of faultages, using vector to represent
typedef std::vector<LineSegment> Edges;

//define set of faultages, using vector to represent
typedef std::vector<Faultage> Faultages;

//define set of space points, using vector to represent
typedef std::vector<SpacePoint> Points;

//define set of triangles, using vector to represent
typedef std::vector<Triangle> Triangles;

//define set of PlanPoint, using vector to represent
typedef std::vector<PlanePoint> XYPoints;

//define single contour with elevation value "value"
typedef struct Contour
{
	double value;
	std::vector<XYPoints> contourLines;
}Contour;

//next define the input data structure

//define block data
typedef struct BlockPoints
{
	Points points;
	BrokenLine externalEdge;                  //external edge
	std::vector<BrokenLine> internalEdges;    //internal edges
}BlockPoints;

//define the input data for trigulation
typedef struct InputData
{
	Points allPoints;
	BlockPoints externalPoints;                //external points, externalPoints.externalEdge = NULL
	std::vector<BlockPoints> internalPoints;   //internal points
}InputData;

//define treenode
typedef struct TreeNode
{
	int index;
	int degree;
	QString IP;
	///严格的一棵树只能有1个父节点
	TreeNode* fathernode;
	///对于不严格的，可以有很多前序节点
	vector<TreeNode*> prenodesvec;
	vector<TreeNode*> nextnodesvec;
}TreeNode;
#endif
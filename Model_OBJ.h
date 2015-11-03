#ifndef MODEL_OBJ_H
#define MODEL_OBJ_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <assert.h>
#include <math.h>
#include <algorithm>
#include <vector>

struct Point3d
{
public:
    float X;
    float Y;
    float Z;


    Point3d() 
    {
        X = 0;
        Y = 0;
        Z = 0;
    }
    Point3d(const Point3d& p)
    {
        X=p.X;
        Y=p.Y;
        Z=p.Z;
    }
    Point3d(float x, float y, float z) 
    {
        X = x;
        Y = y;
        Z = z;
    }
};

struct Vector
{
public:
    float X;
    float Y;
    float Z;
    Vector() 
    {
        X = 0;
        Y = 0;
        Z = 0;
    }

    Vector(const Vector& v)
    {
        X=v.X;
        Y=v.Y;
        Z=v.Z;
    }

    Vector(float x, float y, float z) 
    {
        X = x;
        Y = y;
        Z = z;
    }
};

struct Polygon_Point
{
    float Index0_;//顶点号
    float Index1_;//纹理号
    float Index2_;//法向号
    Polygon_Point()
    {
        Index0_=0;
        Index1_=0;
        Index2_=0;
    }

    Polygon_Point(float x, float y, float z) 
    {
        Index0_ = x;
        Index1_ = y;
        Index2_ = z;
    }
};


class Model_OBJ
{
public:
	std::vector<Point3d> OBJ_V_;//点坐标集合
	std::vector<Vector> OBJ_VT_;//纹理坐标集合
	std::vector<Vector> OBJ_VN_;//法向量坐标集合
	std::vector<std::vector<Polygon_Point> > OBJ_Faces_;//面的集合
    bool flag_vt_;
    bool flag_vn_; 

    Model_OBJ()
    {
        flag_vt_=false;
        flag_vn_=false;
    }
    


    void LoadOBJ(const char* filename);//读入后序号以从0开始，而不是1
    void OutputOBJ(const char* filename);

    virtual ~Model_OBJ()
    {
    }
};










#endif
#ifndef MESH_H
#define MESH_H
#include "Model_OBJ.h"




struct Triangle
{
public :
    int P0Index;
    int P1Index;
    int P2Index;

    Triangle(int p0index, int p1index, int p2index)
    {
        this->P0Index=p0index;
        this->P1Index=p1index;
        this->P2Index=p2index;
    }
    Triangle()
    {
        P0Index=-1;
        P1Index=-1;
        P2Index=-1;
    }
};



class Mesh:public Model_OBJ
{
public:
    std::vector<Point3d> Vertices;//存放点的集合
    std::vector<Triangle> Faces;//存放三角形的集合 也是第一个点是0开始的
    std::vector<Vector> FaceNormals;//存放面法向的集合，与三角形集合一一对应
    std::vector<Vector> VertexNormals;//存放点法向的集合，与点集一一对应
    std::vector<Vector> VertexTexture;
    std::vector<std::vector<int>*> AdjacentFacesPerVertex;//存放点的邻接面的集合，与点集一一对应
    
    std::vector<std::vector<int>*> AdjacentVerticesPerVertex;//存放点的邻接点的集合，与点集一一对应

    void MeshLoadOBJ(const char* filename)
    {
        LoadOBJ(filename);//载入文件
        /////////////////////////////////


        for(int i=0;i<OBJ_V_.size();++i)
        {
            Vertices.push_back(OBJ_V_[i]);//添加点集

        }

        //printf("OBJ_Faces_[i].size:%d\n",OBJ_Faces_[0].size());
        for(int i=0;i<OBJ_Faces_.size();++i)
        {
            Triangle T;
            T.P0Index=OBJ_Faces_[i][0].Index0_;
            T.P1Index=OBJ_Faces_[i][1].Index0_;
            T.P2Index=OBJ_Faces_[i][2].Index0_;
            Faces.push_back(T);
        }


        if(flag_vn_)
        {
            for(int i=0;i<OBJ_VN_.size();++i)
            {
                VertexNormals.push_back(OBJ_VN_[i]);//添加法向量集合

            }
        }

        if(flag_vt_)
        {
            for(int i=0;i<OBJ_VT_.size();++i)
            {
                VertexTexture.push_back(OBJ_VT_[i]);//添加纹理坐标集合

            }
        }

    }

    int AddVertex(Point3d& toAdd)
    {
        int index = Vertices.size();
        Vertices.push_back(toAdd);
        return index;
    }
    

    int AddFace(Triangle& tri)
    {
        int index = Faces.size();
        Faces.push_back(tri);
        return index;
    }
    void CaculateFaceNormals();
    
    void CaculateAdjacentFacesPerVertex();

    void CaculateVertexNormals();

    void CaculateAdjacentVerticesPerVertex();

   

};

Vector CaculateTriangleNormal(Point3d& p0, Point3d& p1, Point3d& p2);

#endif
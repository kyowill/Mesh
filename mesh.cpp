#include "mesh.h"

Vector CaculateTriangleNormal(Point3d& p0, Point3d& p1, Point3d& p2)
{
    Vector Normal;
    float v1x = p1.X - p0.X;
    float v1y = p1.Y - p0.Y;
    float v1z = p1.Z - p0.Z;
    float v2x = p2.X - p1.X;
    float v2y = p2.Y - p1.Y;
    float v2z = p2.Z - p1.Z;
    Normal.X= v1y * v2z - v1z * v2y;
    Normal.Y = v1z * v2x - v1x * v2z;
    Normal.Z = v1x * v2y - v1y * v2x;
    float len = (float)sqrt(Normal.X * Normal.X + Normal.Y * Normal.Y + Normal.Z * Normal.Z);
    if (len == 0)
    {
        //throw Exception();
    }
    else
    {
        Normal.X /= len;
        Normal.Y /= len;
        Normal.Z /= len;
    }
    return Normal;
}



void Mesh::CaculateFaceNormals()
{
    FaceNormals.reserve(Faces.size());
    for(size_t i=0;i<Faces.size();i++)
    {
        Point3d& p0=Vertices[Faces[i].P0Index];
        Point3d& p1=Vertices[Faces[i].P1Index];
        Point3d& p2=Vertices[Faces[i].P2Index];
        Vector v=CaculateTriangleNormal(p0,p1,p2);
        FaceNormals.push_back(v);
    }//对每一个三角形计算面法向并存至FaceNormals
}//计算面发向

void Mesh::CaculateAdjacentFacesPerVertex()
{
    AdjacentFacesPerVertex.reserve(Vertices.size());
    for (size_t i = 0; i <Vertices.size(); ++i)
    {
        std::vector<int>* list=new std::vector<int>();
        list->reserve(4);//预先假设每个点大概有4个邻接面
        AdjacentFacesPerVertex.push_back(list);
    }//首先分配好存储空间
    for (size_t i = 0; i < Faces.size(); ++i)
    {
        Triangle& t = Faces[i];
        std::vector<int> *t0list= AdjacentFacesPerVertex[t.P0Index];
        std::vector<int> *t1list= AdjacentFacesPerVertex[t.P1Index];
        std::vector<int> *t2list= AdjacentFacesPerVertex[t.P2Index];
        t0list->push_back(i);
        t1list->push_back(i);
        t2list->push_back(i);
    }//遍历三角形集合，使用三角形信息补充邻接面表
}//计算邻接面


void Mesh::CaculateVertexNormals()
{
    if(OBJ_VN_.size()!=0)
        return;
    if(FaceNormals.size()==0)
        CaculateFaceNormals();//计算点法向前需计算完面法向
    if(AdjacentFacesPerVertex.size()==0)
        CaculateAdjacentFacesPerVertex();//计算点法向前也必须计算完邻接面
    //printf("FaceNormals.size():%d\n",FaceNormals.size());
    //printf("AdjacentFacesPerVertex.size():%d\n",AdjacentFacesPerVertex.size());

    VertexNormals.reserve(Vertices.size());
    for (size_t i = 0; i < Vertices.size(); i++)
    {
        float sumx = 0;
        float sumy = 0;
        float sumz = 0;
        std::vector<int>& tlist = *(AdjacentFacesPerVertex[i]);
        if(tlist.size()!=0)
        {
            for (size_t j = 0; j < tlist.size(); j++)
            {
                sumx += FaceNormals[tlist[j]].X;
                sumy += FaceNormals[tlist[j]].Y;
                sumz += FaceNormals[tlist[j]].Z;
            }
            VertexNormals.push_back(Vector(sumx / tlist.size(), sumy / tlist.size(), sumz /tlist.size()));//求邻接面发向均值
        }
        else
        {
            VertexNormals.push_back(Vector(0,0,0));//若无邻接面，则为孤立点，使用默认值
        }
    }
}//计算点法向

void Mesh::CaculateAdjacentVerticesPerVertex()
{
    AdjacentVerticesPerVertex.reserve(Vertices.size());
    for (size_t i = 0; i < Vertices.size(); i++)
    {
        std::vector<int>* list=new std::vector<int>();
        list->reserve(4);//预先假设每个点大概有4个邻接点
        AdjacentVerticesPerVertex.push_back(list);
    }//首先分配好存储空间
    for (size_t i = 0; i < Faces.size(); i++)
    {
        Triangle &t = Faces[i];
        std::vector<int> *p0list= AdjacentVerticesPerVertex[t.P0Index];
        std::vector<int> *p1list= AdjacentVerticesPerVertex[t.P1Index];
        std::vector<int> *p2list= AdjacentVerticesPerVertex[t.P2Index];

        if (std::find(p0list->begin(), p0list->end(), t.P1Index)==p0list->end())
            p0list->push_back(t.P1Index);
        if (std::find(p0list->begin(), p0list->end(), t.P2Index)==p0list->end())
            p0list->push_back(t.P2Index);

        if (std::find(p1list->begin(), p1list->end(), t.P0Index)==p1list->end())
            p1list->push_back(t.P0Index);
        if (std::find(p1list->begin(), p1list->end(), t.P2Index)==p1list->end())
            p1list->push_back(t.P2Index);

        if (std::find(p2list->begin(), p2list->end(), t.P0Index)==p2list->end())
            p2list->push_back(t.P0Index);
        if (std::find(p2list->begin(), p2list->end(), t.P1Index)==p2list->end())
            p2list->push_back(t.P1Index);
    }//遍历三角形集合，使用三角形信息补充邻接面表
}//计算邻接点
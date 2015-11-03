#include "LaplacianSmooth.h"
void LaplacianSmooth(Mesh& mesh,int time)
{
    if(mesh.AdjacentVerticesPerVertex.size()==0)
        mesh.CaculateAdjacentVerticesPerVertex();//平滑前需要计算邻接点
    Point3d* tempPos=new Point3d[mesh.Vertices.size()];//辅助空间存储每次平滑后的点将要挪到的位置
    for(int k=0;k<time;k++)
    {
        for (size_t i = 0; i < mesh.Vertices.size(); i++)
        {
            float xav = 0;
            float yav = 0;
            float zav = 0;
            std::vector<int>& adjlist =*(mesh.AdjacentVerticesPerVertex[i]);
            size_t adjcount=adjlist.size();
            if(adjcount==0)
                continue;
            for (size_t j = 0; j < adjcount; j++)
            {
                xav += mesh.Vertices[adjlist[j]].X;
                yav += mesh.Vertices[adjlist[j]].Y;
                zav += mesh.Vertices[adjlist[j]].Z;
            }
            xav /= adjcount;
            yav /= adjcount;
            zav /= adjcount;
            tempPos[i].X = xav;
            tempPos[i].Y = yav;
            tempPos[i].Z = zav;
        }//对所有点计算邻接点平均位置并存到辅助空间
        for (size_t i = 0; i < mesh.Vertices.size(); i++)
        {
            mesh.Vertices[i].X = tempPos[i].X;
            mesh.Vertices[i].Y = tempPos[i].Y;
            mesh.Vertices[i].Z = tempPos[i].Z;
        }//将计算出的新点位置覆盖原来点的位置
    }//每次循环意味着一次平滑
    delete[] tempPos;
}
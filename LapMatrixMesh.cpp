#include "LapMatrixMesh.h"


void CalculateLapMatrix(Mesh& mesh,std::vector<std::vector<int>*>& lapmatrix)
{
	if(mesh.AdjacentVerticesPerVertex.empty())
		mesh.CaculateAdjacentVerticesPerVertex();
	///////////////下面是Lap矩阵的转置//////////////////////
	for(size_t i=0;i<mesh.AdjacentVerticesPerVertex.size();++i)
	{
		std::vector<int>* p=new std::vector<int>();
		std::vector<int>* tp=mesh.AdjacentVerticesPerVertex[i];
		for(size_t j=0;j<mesh.Vertices.size();++j)
		{
			if(std::find(tp->begin(),tp->end(),j)==tp->end())
			{
				if(i==j)
				{	
					p->push_back(tp->size());
				}else
				{
					p->push_back(0);
				}
			}else
			{
				if(i==j)
				{
					p->push_back(tp->size()-1);
				}else
				{
					p->push_back(-1);
				}
			}
		}

		lapmatrix.push_back(p);

	}
	///////////////下面是Lap矩阵//////////////////////

	for(int i=0;i<lapmatrix.size();++i)
	{
		for(int j=0;j<i;++j)
		{
			int temp=(*lapmatrix[i])[j];
			(*lapmatrix[i])[j]=(*lapmatrix[j])[i];
			(*lapmatrix[j])[i]=temp;

		}
	}


}
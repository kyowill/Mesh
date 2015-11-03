#include "DLRS.h"
#include "mesh_op.h"
void CalculateDLRS(Mesh& old_mesh,float k)
{
	std::vector<std::vector<int>*> lapmatrix;
	std::vector<std::vector<int>*> lapmatrix_;
	std::vector<std::vector<int>*> M;
	
	CalculateLapMatrix(old_mesh,lapmatrix);
	CalculateMatrixTranspose(lapmatrix,lapmatrix_);
	CalculateMatrixMultiply(lapmatrix_,lapmatrix,M);
	
	for(int i=0;i<M.size();++i)
	{
		for(int j=0;j<(*M[0]).size();++j)
		{
			(*M[i])[j]=k*(*M[i])[j];
			if(i==j)
			{
				(*M[i])[j]=(*M[i])[j]+1;
			}
		}
	}
	//std::vector<Point3d> Vertices
	//void CalculateMatrixSolution(const std::vector<std::vector<float>*>& A,
							 //const std::vector<std::vector<float>*>& B,
							 //std::vector<std::vector<float>*>& X);
	/////////////////////求B矩阵//////////////////////////
	std::vector<std::vector<float>*> P;
	for(int i=0;i<old_mesh.Vertices.size();++i)
	{
		std::vector<float>* p=new std::vector<float>();
		for(int j=0;j<3;++j)
		{
			if(j==0)
			{
				p->push_back(old_mesh.Vertices[i].X);
			}else if(j==1)
			{
				p->push_back(old_mesh.Vertices[i].Y);
			}else if(j==2)
			{
				p->push_back(old_mesh.Vertices[i].Z);
			}
		}
		P.push_back(p);
	}//printf("B.size=%d\n",(*P[0]).size());
	////////////////////求A矩阵///////////////////////////////////////
	
	
	std::vector<std::vector<float>*> A;

	for(int i=0;i<M.size();++i)
	{
		std::vector<float>* p=new std::vector<float>();
		for(int j=0;j<(*M[0]).size();++j)
		{
			p->push_back((*M[i])[j]);
		}
		A.push_back(p);
	}


	std::vector<std::vector<float>*> _X;
	CalculateMatrixSolution(A,P,_X);//if(X[0])
	// printf("%d\n",X.size());
	// for(int i=0;i<X.size();++i)
	// {
	// 	for(int j=0;j<(*X[0]).size();++j)
	// 	{
	// 		printf("%f ",(*X[i])[j]);
	// 	}
	// 	printf("\n");
	// }
	
	printf("正在计算...\n");
	//////////////////////////改变mesh里Vertices成员////////////////////////
	for(int i=0;i<old_mesh.Vertices.size();++i)
	{
		
        old_mesh.Vertices[i].X=(*_X[i])[0];
        old_mesh.Vertices[i].Y=(*_X[i])[1];
        old_mesh.Vertices[i].Z=(*_X[i])[2];

	}
	// for(int i=0;i<old_mesh.Faces.size();++i)
 //    {
 //        Triangle t(old_mesh.Faces[i].P0Index,old_mesh.Faces[i].P1Index,old_mesh.Faces[i].P2Index);
 //        new_mesh.AddFace(t);
 //    }

}
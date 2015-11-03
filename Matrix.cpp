#include "Matrix.h"

static int CalculatePerElement(const std::vector<std::vector<int>*>& A,
							   int m,
							   const std::vector<std::vector<int>*>& B,
							   int n)
{
	int sum=0;
	for(int i=0;i<B.size();++i)
	{
		sum+=(*A[m])[i]*(*B[i])[n];
	}
	return sum;
}



void CalculateMatrixMultiply(const std::vector<std::vector<int>*>& A,
							 const std::vector<std::vector<int>*>& B,
							 std::vector<std::vector<int>*>& C)
{
	if(A.empty()||B.empty())
	{
		return;
	}
	if((*A[0]).size()!=B.size())
	{
		return;
	}
	////////////////////////计算A*B=C矩阵///////////////////////////////
	for(int i=0;i<A.size();++i)
	{
		std::vector<int>* p=new std::vector<int>();
		for(int j=0;j<(*B[0]).size();++j)
		{
			p->push_back(CalculatePerElement(A,i,B,j));
		}
		C.push_back(p);
	}
	
}
///////////////////////////////////计算A+B=C/////////////////////////////////
void CalculateMatrixPlus(const std::vector<std::vector<int>*>& A,
						 const std::vector<std::vector<int>*>& B,
				         std::vector<std::vector<int>*>& C)
{
	if(A.empty()||B.empty())
	{
		return;
	}
	if(!(A.size()==B.size()&&(*A[0]).size()==(*B[0]).size()))
	{
		return;
	}

	for(int i=0;i<A.size();++i)
	{
		std::vector<int>* p=new std::vector<int>();
		for(int j=0;j<(*A[0]).size();++j)
		{
			p->push_back((*A[i])[j]+(*B[i])[j]);
		}
		C.push_back(p);
	}
}
////////////////////////////////////////////计算A的转置_A///////////////////////
void CalculateMatrixTranspose(const std::vector<std::vector<int>*>& A,
							  std::vector<std::vector<int>*>& _A)
{
	if(A.empty())
	{
		return;
	}
	for(int i=0;i<(*A[0]).size();++i)
	{
		std::vector<int>* p=new std::vector<int>();
		for(int j=0;j<A.size();++j)
		{
			p->push_back((*A[j])[i]);
		}
		_A.push_back(p);
	}
}

/////////////////////////////////求解AX=B//////////////////////////
static void CalculateMatrixSolution_L(const std::vector<std::vector<float>*>& A,
								 const std::vector<float>* B,
								 std::vector<float>* &X)
{
	double SUM, DET = 1.0;  
	int  T,J,P,K,C;
	int N=(*B).size();
	std::vector<int>* Row=new std::vector<int>(N,0);//int    Row[Limit];
	std::vector<float>* Y=new std::vector<float>(N,0);//int    Y[Limit];
	X=new std::vector<float>(N,0);
	/* Initialize the pointer vector */

    for ( J = 1; J<=N ; J++) (*Row)[J-1] = J - 1;
        /* Start LU factorization */

    for (P = 1; P <= N - 1; P++)
    {

	    /* Find pivot element */  

	    for (K = P + 1; K <= N; K++)
	    {
	        if ( fabs((*A[(*Row)[K-1]])[P-1]) > fabs((*A[(*Row)[P-1]])[P-1]) )
	        {
	        /* Switch the index for the p-1 th pivot row if necessary */ 
	            T        = (*Row)[P-1];
	            (*Row)[P-1] = (*Row)[K-1];
	            (*Row)[K-1] = T;
	            DET      = - DET; 
	        }
	    } /* End of simulated row interchange */

	        if ((*A[(*Row)[P-1]])[P-1] == 0)
	        {   
	            printf("The matrix is SINGULAR !\n");
	            printf("Cannot use algorithm --> exit\n");
	            exit(1);
	        }

	     /* Multiply the diagonal elements */
	     
	    DET = DET * (*A[(*Row)[P-1]])[P-1];

	     /* Form multiplier */

	    for (K = P + 1; K <= N; K++)
	    { 
	        (*A[(*Row)[K-1]])[P-1]= (*A[(*Row)[K-1]])[P-1] / (*A[(*Row)[P-1]])[P-1];
	      
	        /* Eliminate X_(p-1) */

	        for (C = P + 1; C <= N + 1; C++)
	        {
	            (*A[(*Row)[K-1]])[C-1] -= (*A[(*Row)[K-1]])[P-1] * (*A[(*Row)[P-1]])[C-1];
	        }
	    } 

    } /* End of  L*U  factorization routine */
	DET = DET * (*A[(*Row)[N-1]])[N-1];
	(*Y)[0] = (*B)[(*Row)[0]]; 
	for ( K = 2; K <= N; K++)
    {
        SUM =0;
        for ( C = 1; C <= K -1; C++) SUM += (*A[(*Row)[K-1]])[C-1] * (*Y)[C-1];
        (*Y)[K-1] = (*B)[(*Row)[K-1]] - SUM;
    }
    /* Start the back substitution */
   
    (*X)[N-1] = (*Y)[N-1] / (*A[(*Row)[N-1]])[N-1];

    for (K = N - 1; K >= 1; K--)
    {
        SUM = 0;
        for (C = K + 1; C <= N; C++)
        {
            SUM += (*A[(*Row)[K-1]])[C-1] * (*X)[C-1];   
        }

        (*X)[K-1] = ( (*Y)[K-1] - SUM) / (*A[(*Row)[K-1]])[K-1];

    }   /* End of back substitution */



}
void CalculateMatrixSolution(const std::vector<std::vector<float>*>& A,
							 const std::vector<std::vector<float>*>& B,
							 std::vector<std::vector<float>*>& X)
{
	for(int i=0;i<B.size();++i)
	{
		std::vector<float>* X_;
		CalculateMatrixSolution_L(A,B[i],X_);
		X.push_back(X_);
	}
}
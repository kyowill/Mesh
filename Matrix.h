#ifndef MATRIX_H
#define MAYRIX_H
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void CalculateMatrixMultiply(const std::vector<std::vector<int>*>& A,
							 const std::vector<std::vector<int>*>& B,
							 std::vector<std::vector<int>*>& C);//计算A*B乘积矩阵
void CalculateMatrixPlus(const std::vector<std::vector<int>*>& A,
				         const std::vector<std::vector<int>*>& B,
						 std::vector<std::vector<int>*>& C);//计算A+B和矩阵
void CalculateMatrixTranspose(const std::vector<std::vector<int>*>& A,
							  std::vector<std::vector<int>*>& _A);//计算A的转置 


void CalculateMatrixSolution(const std::vector<std::vector<float>*>& A,
							 const std::vector<std::vector<float>*>& B,
							 std::vector<std::vector<float>*>& X);//求解AX=B




#endif
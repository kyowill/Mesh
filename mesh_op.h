#ifndef MESH_OP_H
#define MESH_OP_H
#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <assert.h>



void ReadFile(Mesh& mesh,const char* fileName);

void Output(Mesh& mesh,const char* filename);


#endif
#include "mesh_op.h"



void ReadFile(Mesh& mesh,const char* fileName)
{

    FILE * nfile = fopen(fileName,"rb");
    assert(nfile);
    float v1=0,v2=0,v3=0;
    
    int i1=0,i2=0,i3=0;
    char buf[1024];

    while(memset(buf,0,1024),fgets(buf,1024,nfile))
    {
        if(!(buf[0]=='v'||buf[0]=='f'))
        {
            continue;
        }
        
        if(buf[0]=='v'&&buf[1]==' ')
        {
            sscanf(buf,"v %f %f %f\n",&v1,&v2,&v3);
            Point3d p3d(v1,v2,v3);
            mesh.AddVertex(p3d);
            

        }else if(buf[0]=='f'&&buf[1]==' ')
        {
            sscanf(buf,"f %d %d %d\n",&i1,&i2,&i3);
            Triangle t(i1-1,i2-1,i3-1);//注意obj文件中第一个点用的是1表示不是0
            mesh.AddFace(t);
        }else
        {   continue;}
    }
    
    fclose(nfile);
}

void Output(Mesh& mesh,const char* filename)
{
    FILE * nfile = fopen(filename,"wb");
    assert(nfile);

    fprintf(nfile,"#element vertex %d\n",mesh.Vertices.size());

    fprintf(nfile,"#element face %d\n",mesh.Faces.size());

    
    for(int i=0;i<mesh.Vertices.size();i++)
    {
        fprintf(nfile,"v %.6f %.6f %.6f\n",mesh.Vertices[i].X,mesh.Vertices[i].Y,mesh.Vertices[i].Z);
    }
    for(int i=0;i<mesh.Faces.size();i++)
    {
        fprintf(nfile,"f %d %d %d\n",mesh.Faces[i].P0Index,mesh.Faces[i].P1Index,mesh.Faces[i].P2Index);
    }
    fclose(nfile);
}
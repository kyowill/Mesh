#include "Model_OBJ.h"
/////////////////////读取obj文件///////////////////
void Model_OBJ::LoadOBJ(const char* filename)
{
	FILE * nfile = fopen(filename,"rb");
    assert(nfile);
    float v1=0,v2=0,v3=0;
    
    int i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0,i9=0;
    char buf[1024];

    bool flag_vt=false;
    bool flag_vn=false;

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
            OBJ_V_.push_back(p3d);
            

        }else if(buf[0]=='v'&&buf[1]=='t')
        {
        	flag_vt=true;
            flag_vt_=true;
        	sscanf(buf,"vt %f %f %f\n",&v1,&v2,&v3);
        	Vector vt(v1,v2,v3);
        	OBJ_VT_.push_back(vt);

        }else if(buf[0]=='v'&&buf[1]=='n')
        {
        	flag_vn=true;
            flag_vn_=true;
        	sscanf(buf,"vn %f %f %f\n",&v1,&v2,&v3);
        	Vector vn(v1,v2,v3);
        	OBJ_VN_.push_back(vn);

        }else if(buf[0]=='f'&&buf[1]==' ')
        {
            if(flag_vt==true||flag_vn==true)
            {
            	if(flag_vt==true&&flag_vn==true)
            	{
            		sscanf(buf,"f %d/%d/%d %d/%d/%d %d/%d/%d\n",
            			&i1,&i2,&i3,&i4,&i5,&i6,&i7,&i8,&i9);
            		Polygon_Point t1(i1-1,i2-1,i3-1);
            		Polygon_Point t2(i4-1,i5-1,i6-1);
            		Polygon_Point t3(i7-1,i8-1,i9-1);
            		std::vector<Polygon_Point> vec;
		            vec.push_back(t1);
		            vec.push_back(t2);
		            vec.push_back(t3);
		            OBJ_Faces_.push_back(vec);

            	}else if(flag_vt==false&&flag_vn==true)
            	{
            		sscanf(buf,"f %d//%d %d//%d %d//%d\n",
            			&i1,&i2,&i3,&i4,&i5,&i6);
            		Polygon_Point t1(i1-1,-1,i2-1);
            		Polygon_Point t2(i3-1,-1,i4-1);
            		Polygon_Point t3(i5-1,-1,i6-1);
            		std::vector<Polygon_Point> vec;
		            vec.push_back(t1);
		            vec.push_back(t2);
		            vec.push_back(t3);
		            OBJ_Faces_.push_back(vec);
            	}else if(flag_vt==true&&flag_vn==false)
            	{
            		
                    sscanf(buf,"f %d/%d %d/%d %d/%d\n",
                        &i1,&i2,&i3,&i4,&i5,&i6);
                    Polygon_Point t1(i1-1,i2-1,-1);
                    Polygon_Point t2(i3-1,i4-1,-1);
                    Polygon_Point t3(i5-1,i6-1,-1);
                    std::vector<Polygon_Point> vec;
                    vec.push_back(t1);
                    vec.push_back(t2);
                    vec.push_back(t3);
                    OBJ_Faces_.push_back(vec);


            	}else
                {
                    //perror("flag_vt==true&&flag_vn==false no oppose");
                    exit(1);
                }
            }else{
	            sscanf(buf,"f %d %d %d\n",&i1,&i2,&i3);
	            Polygon_Point t1(i1-1,-1,-1);//注意obj文件中第一个点用的是1表示不是0
	            Polygon_Point t2(i2-1,-1,-1);
	            Polygon_Point t3(i3-1,-1,-1);
	            std::vector<Polygon_Point> vec;
	            vec.push_back(t1);
	            vec.push_back(t2);
	            vec.push_back(t3);
	            OBJ_Faces_.push_back(vec);
        	}
        }else
        {   continue;}
    }
    
    fclose(nfile);
}


void Model_OBJ::OutputOBJ(const char* filename)
{
	printf("OutputOBJ no write\n");
}
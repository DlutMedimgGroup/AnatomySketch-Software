#pragma once

//---------------------- 常量 -------------------------
#define CV_PI 3.1415926

//视图标号
#define ViewLabel_Sagittal			0		//视图标号，矢状面，Sagittal
#define ViewLabel_Coronal			1		//视图标号，冠状面，Coronal
#define ViewLabel_Axial				2		//视图标号，横截面，Axial	
#define ViewLabel_3D				3		//视图标号，三维视图

//---------------------- 结构体 -------------------------
typedef struct structPoint {
	double dX;
	double dY;
	double dZ;
}structPoint;

typedef struct int_two
{
	int int1;
	int int2;
}int_two;



//todo: use XML-format VTK file for parallel I/O for using multiple cores
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
using namespace std;

bool isinset(double cx, double cy, double x, double y, int iters = 80)
{
	for (int iter = 0; iter < iters; iter++)
	{
		if (2 < x || -2 > x || 2 < y || -2 > y) return false;
		double x2 = x * x - y * y  + cx;
		y = 2 * x * y + cy;
		x = x2;
	}
	return true;
}

double* rotate(double x, double y, double z, double w, int plane_p, int plane_q, double angle)
{
	static double result[4];
	double vector[4] = { x, y, z, w };
	double matrix[4][4];
	for (int p = 0; p < 4; p++) for (int q = 0; q < 4; q++) matrix[p][q] = (p == q) ? 1:0;
	matrix[plane_p][plane_p] = cos(angle);
	matrix[plane_q][plane_q] = cos(angle);
	matrix[plane_p][plane_q] = sin(angle);
	matrix[plane_q][plane_p] = -sin(angle);
	for (int r_i = 0; r_i < 4; r_i++)
	{
		double r = 0;
		for (int q = 0; q < 4; q++) r += vector[q] * matrix[r_i][q];
		result[r_i] = r;
	}
	return result;
}

int main(int argc, char* argv[])
{
	const int res = 1000;
	char buffer[res];
	int plane_p[6];
	int plane_q[6];
	double angle[6];
	ofstream rawfile("julia.raw", ios::out | ios::binary);
	ofstream txtfile("julia.txt", ios::out);
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0, M_PI*2);
	int index = 0;
	char planes[5] = "xyzw";
	for (int p = 0; p < 4; p++) 
	{
		for(int q=p+1; q<4; q++ )
		{ 
			plane_p[index] = p;
			plane_q[index] = q;
			angle[index] = distribution(generator);
			txtfile << planes[p] << planes[q] << " = " << angle[index] << endl;
			index++;
		}
	}
	txtfile.close();
	double pa = -2;
	double i = 4.0 / res;
	for (int a = 0; a < res; a++)
	{
		double pb = -2;
		for (int b = 0; b < res; b++)
		{
			double pc = -2;
			for (int c = 0; c< res; c++)
			{
				double rotated_x = pa;
				double rotated_y = pb;
				double rotated_z = pc;
				double rotated_w = 0; //is this right?
				for (int index = 0; index < 6; index++)
				{
					double* result = rotate(rotated_x, rotated_y, rotated_z, rotated_w, plane_p[index], plane_q[index], angle[index]);
					rotated_x = result[0];
					rotated_y = result[1];
					rotated_z = result[2];
					rotated_w = result[3];
				}
				//cout << rotated_x << " " << rotated_y << " " << rotated_z << " " << rotated_w << endl;
				buffer[c] = isinset(rotated_x, rotated_y, rotated_z, rotated_w) ? 255 : 0;
				pc += i;
			}
			pb += i;
			rawfile.write(buffer, res);
		}
		cout << a<< endl;
		pa += i;
	}
	rawfile.close();
}
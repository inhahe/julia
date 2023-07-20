#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
using namespace std;

bool isinset(float cx, float cy, float x, float y, int iters = 80)
{
	for (int iter = 0; iter < iters; iter++)
	{
		if (2 < x || -2 > x || 2 < y || -2 > y) return false;
		float x2 = x * x - y * y  + cx;
		y = 2 * x * y + cy;
		x = x2;
	}
	return true;
}

float* rotate(float x, float y, float z, float w, int plane_p, int plane_q, float angle)
{
	float result[4];
	float vector[4] = { x, y, z, w };
	float matrix[4][4];
	for (int p = 0; p < 4; p++) for (int q = 0; q < 4; q++) matrix[p][q] = 0;
	for (int pq = 0; pq < 4; pq++) matrix[pq][pq] = 1;
	matrix[plane_p][plane_p] = cos(angle);
	matrix[plane_q][plane_q] = cos(angle);
	matrix[plane_p][plane_q] = sin(angle);
	matrix[plane_q][plane_p] = -sin(angle);
	for (int r_i = 0; r_i < 4; r_i++)
	{
		float r = 0;
		for (int q = 0; q < 4; q++) r += vector[r_i] * matrix[r_i][q];
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
	float angle[6];
	ofstream rawfile("julia.raw", ios::out | ios::binary);
	ofstream txtfile("julia.txt", ios::out);
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0, M_PI*2);
	int index = 0;
	char* planes = { "x", "y", "z", "w" };
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
	float pa = -2;
	float i = 4.0 / res;
	for (int a = 0; a < res; a++)
	{
		float pb = -2;
		for (int b = 0; b < res; b++)
		{
			float pc = -2;
			float start_x = pa;
			float start_y = pb;
			float start_z = pc;
			float start_w = -2;
			for (int index = 0; index < 6; index++)
			{
				float* result = rotate(start_x, start_y, start_z, start_w, plane_p[index], plane_q[index], angle[index]);
				start_x = result[0];
				start_y = result[1];
				start_z = result[2];
				start_w = result[3];
			}
			float* result = rotate(start_x, start_y, start_z, start_w, plane_p[index], plane_q[index], angle[index]+M_PI);
			float end_x = result[0];
			float end_y = result[1];
			float end_z = result[2];
			float end_w = result[3];
			float i_x = (end_x - start_x) / res;
			float i_y = (end_y - start_y) / res;
			float i_z = (end_z - start_z) / res;
			float i_w = (end_w - start_w) / res;
			for (int c = 0; c< res; c++)
			
			{
				buffer[c] = isinset(px, py, 0, pcx) ? 255 : 0;
				pc += i;
			}
			pb += i;
			rawfile.write(buffer, res);
		}
		cout << a<< endl;
		pa += i;
	}
	rawfile.close();

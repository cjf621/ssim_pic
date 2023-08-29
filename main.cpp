#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
double getSimilarity(int** hash_source, int** hash_temp, int x, int y, int w, int h) {
	int len = w * h;
	double* a = new double[len];
	double* b = new double[len];
	for (int i = 0; i < len; i++) {
		a[i] = (double)hash_source[x + i % w][y + i / w];
		b[i] = (double)hash_source[i % w][i / w];
	}
	//二维转一维，本demo二维数组列在前（x坐标）行在后（y坐标）
	double average_a = 0, average_b = 0, average_ab = 0;
	for (int i = 0; i < len; i++) {
		average_a += a[i];
		average_b += b[i];
		average_ab += a[i] * b[i];
	}
	average_a /= len;
	average_b /= len;
	average_ab /= len;
	double variance_a = 0, variance_b = 0;
	for (int i = 0; i < len; i++) {
		variance_a += (a[i] - average_a) * (a[i] - average_a);
		variance_b += (b[i] - average_b) * (b[i] - average_b);
	}
	variance_a /= len;
	variance_b /= len;
	double cov = average_ab - average_a * average_b;
	double SSIM = (2 * average_a * average_b + 0.0256) * (2 * cov + 0.2304) / (average_a * average_a + average_b * average_b + 0.0256) / (variance_a + variance_b + 0.2304);
    //https://baike.baidu.com/item/SSIM/2091025
	//c1=(k1*L)^2，c2=(k2*L)^2，L是像素值动态范围（本demo按16级灰度值计算，故而L=16），k1=0.01，k2=0.03
	delete[] a;
	delete[] b;
	return SSIM;
}
int main() {
	srand((unsigned)time(NULL));
	int** hash_source = new int*[192];
	for (int i = 0; i < 192; i++) {
		hash_source[i] = new int[112];
	}
	int** hash_temp = new int* [10];
	for (int i = 0; i < 10; i++) {
		hash_temp[i] = new int[10];
	}
	//960*560与50*50按1:5比例缩放
	for (int i = 0; i < 192; i++) {
		for (int j = 0; j < 112; j++) {
			hash_source[i][j] = rand() % 16 + 0;
		}
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			hash_temp[i][j] = rand() % 16 + 0;
		}
	}
 auto start = std::chrono::high_resolution_clock::now();
	double result = 0;
	for (int c = 0; c < 100; c++) {
		for (int i = 0; i < 182; i++) {
			for (int j = 0; j < 102; j++) {
				result = getSimilarity(hash_source, hash_temp, i, j, 10, 10);
			}
		}
	}
auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "100次匹配用时" << elapsed.count() << "毫秒" << std::endl;
	for (int i = 0; i < 192; i++) {
		delete hash_source[i];
	}
	delete[] hash_source;
	for (int i = 0; i < 10; i++) {
		delete hash_temp[i];
	}
	delete[] hash_temp;
	return 0;
}
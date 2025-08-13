#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
using namespace std;
using Complex = complex<double>;
const double PI = acos(-1);


void FFT(vector<Complex>& x) {
	int len = x.size();
	if (len == 1) return;

	vector<Complex> odds(len / 2), even(len / 2);

	for (int i = 0; i < len/2; i++) {
		even[i] = x[2 * i];
		odds[i] = x[2 * i + 1];
	}

	FFT(even);
	FFT(odds);

	//double ang = 2 * PI / len * (1);
	//Complex omega(1);
	//Complex omega_n(cos(ang), sin(ang));

	for (int k = 0; k < len / 2; k++) {
		Complex omega = exp(Complex(0, 2 * PI * k / len));
		x[k] = even[k] + omega * odds[k];
		x[k + len / 2] = even[k] - omega * odds[k];
		//omega *= omega_n;
	}

	return;
}

void IFFT(vector<Complex>& y) {
	int len = y.size();
	if (len == 1) return;

	vector<Complex> odds(len / 2), even(len / 2);

	for (int i = 0; i < len / 2; i++) {
		even[i] = y[2 * i];
		odds[i] = y[2 * i + 1];
	}

	IFFT(even);
	IFFT(odds);

	//double ang = 2 * PI / len * (1);
	//Complex omega(1);
	//Complex omega_n(cos(ang), sin(ang));

	for (int k = 0; k < len / 2; k++) {
		Complex omega = exp(Complex(0, -2 * PI * k / len));
		y[k] = (even[k] + omega * odds[k]);
		y[k + len / 2] = (even[k] - omega * odds[k]);
		y[k] /= 2;
		y[k + len / 2] /= 2;
		//omega *= omega_n;
	}

	return;
}

int main() {
	int n, m;
	cin >> n >> m;
	int length = 1;

	while (length < n + m + 1) {
		length *= 2;
	}

	vector<Complex> a(length), b(length);

	for (int i = 0; i <= n; i++) {
		int tmp = 0;
		cin >> tmp;
		a[i] = tmp;
	}
	for (int i = 0; i <= m; i++) {
		int tmp = 0;
		cin >> tmp;
		b[i] = tmp;
	}


	FFT(a);
	FFT(b);

	for (int j = 0; j < length; j++) {
		a[j] *= b[j];
	}

	IFFT(a);


	for (int i = 0; i <= m + n; i++) {
		cout << (int)round(a[i].real()) << " ";
	}

	return 0;
}
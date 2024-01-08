//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#ifndef FASTFOURIERTRANSFORMH
#define FASTFOURIERTRANSFORMH

#include <complex>
#include <cmath>

using namespace std;

class FastFourierTransform
{
private:

	unsigned int N1,N2,N3;
	unsigned int M1,M2,M3;
	unsigned int K1,K2,K3;

	double F1,F2,F3;

	unsigned int *D1, *D2, *D3;

	complex<double> *w1, *w2, *w3;
	complex<double> *v1, *v2, *v3;

	complex<double> **z1a;
	complex<double> *y1;

	complex<double> ***z2a, ***u2a;
	complex<double> **z2b, **u2b;
	complex<double> **y2;

	complex<double> ****z3a, ****u3a, ****g3a;
	complex<double> ***z3b, ***u3b, ***g3b;
	complex<double> ***y3;

	double *r1;
	double **r2;
	double ***r3;

	void initFFT(unsigned int n1, unsigned int n2, unsigned int n3, unsigned int d);

	void inline fft(complex<double> *x, complex<double> **z, unsigned int N, unsigned int M, unsigned int K, complex<double> *w, unsigned int *D);
	void inline ifft(complex<double> *x, complex<double> **z, unsigned int N, unsigned int M, unsigned int K, complex<double> *v, unsigned int *D, double F);

public:

	FastFourierTransform(unsigned int n1);
	FastFourierTransform(unsigned int n1, unsigned int n2);
	FastFourierTransform(unsigned int n1, unsigned int n2, unsigned int n3);
	
	~FastFourierTransform();

	complex<double> inline *fft1(double *x);		//complex-conjugate results!
	complex<double> inline **fft2(double **x);	//complex-conjugate results!
	complex<double> inline ***fft3(double ***x);

	double inline *ifft1(complex<double> *x);	
	double inline **ifft2(complex<double> **x);
	double inline ***ifft3(complex<double> ***x);

};

void inline FastFourierTransform::fft(complex<double> *x, complex<double> **z, unsigned int N, unsigned int M, unsigned int K, complex<double> *w, unsigned int *D)
{
	for(unsigned int i=0;i<K;i++) {
		unsigned int mask = 0xffffffff<<i;
		for(unsigned int j=0;j<M;j++) {
			z[i+1][j<<1] = z[i][j] + z[i][j+M];
			z[i+1][(j<<1)+1] = w[j&mask]*(z[i][j] - z[i][j+M]);
		}
	}

	for(unsigned int i=0;i<N;i++) {
		unsigned int j = D[i];
		x[i] = z[K][j];
	}
}

void inline FastFourierTransform::ifft(complex<double> *x, complex<double> **z, unsigned int N, unsigned int M, unsigned int K, complex<double> *v, unsigned int *D, double F)
{
	for(unsigned int i=0;i<K;i++) {
		unsigned int mask = 0xffffffff<<i;
		for(unsigned int j=0;j<M;j++) {
			z[i+1][j<<1] = z[i][j] + z[i][j+M];
			z[i+1][(j<<1)+1] = v[j&mask]*(z[i][j] - z[i][j+M]);
		}
	}

	for(unsigned int i=0;i<N;i++) {
		unsigned int j = D[i];
		x[i] = z[K][j]*F;
	}
}

complex<double> inline *FastFourierTransform::fft1(double *x)
{
	for(unsigned int i=0;i<N1;i++) {
		z1a[0][i] = complex<double>(x[i],0);
	}
	fft(y1,z1a,N1,M1,K1,w1,D1);

	return(y1);
}

double inline *FastFourierTransform::ifft1(complex<double> *x)
{
	for(unsigned int i=0;i<N1;i++) {
		z1a[0][i] = x[i];
	}
	ifft(y1,z1a,N1,M1,K1,v1,D1,F1);

	for(unsigned int i=0;i<N1;i++) {
		r1[i] = y1[i].real();
	}
	return(r1);
}

complex<double> inline **FastFourierTransform::fft2(double **x)
{
	#pragma omp parallel for
	for(int i=0;i<(int)N1;i++) {
		for(unsigned int j=0;j<N2;j++) {
			z2a[i][0][j] = complex<double>(x[i][j],0);
		}
		fft(z2b[i],z2a[i],N2,M2,K2,w2,D2);
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N2;i++) {
		for(unsigned int j=0;j<N1;j++) {
			u2a[i][0][j] = z2b[j][i];
		}
		fft(u2b[i],u2a[i],N1,M1,K1,w1,D1);
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N1;i++) {
		for(unsigned int j=0;j<N2;j++) {
			y2[i][j] = u2b[j][i];
		}
	}

	return(y2);
}

double inline **FastFourierTransform::ifft2(complex<double> **x)
{
	#pragma omp parallel for
	for(int i=0;i<(int)N1;i++) {
		for(unsigned int j=0;j<N2;j++) {
			z2a[i][0][j] = x[i][j];
		}
		ifft(z2b[i],z2a[i],N2,M2,K2,v2,D2,F2);
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N2;i++) {
		for(unsigned int j=0;j<N1;j++) {
			u2a[i][0][j] = z2b[j][i];
		}
		ifft(u2b[i],u2a[i],N1,M1,K1,v1,D1,F1);
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N1;i++) {
		for(unsigned int j=0;j<N2;j++) {
			r2[i][j] = u2b[j][i].real();
		}
	}
	return(r2);
}

complex<double> inline ***FastFourierTransform::fft3(double ***x)
{
	#pragma omp parallel for
	for(int i=0;i<(int)N1;i++) {
		for(unsigned int j=0;j<N2;j++) {
			for(unsigned int k=0;k<N3;k++) {
				z3a[i][j][0][k] = complex<double>(x[i][j][k],0);
			}
			fft(z3b[i][j],z3a[i][j],N3,M3,K3,w3,D3);
		}
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N2;i++) {
		for(unsigned int j=0;j<N3;j++) {
			for(unsigned int k=0;k<N1;k++) {
				u3a[i][j][0][k] = z3b[k][i][j];
			}
			fft(u3b[i][j],u3a[i][j],N1,M1,K1,w1,D1);
		}
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N3;i++) {
		for(unsigned int j=0;j<N1;j++) {
			for(unsigned int k=0;k<N2;k++) {
				g3a[i][j][0][k] = u3b[k][i][j];
			}
			fft(g3b[i][j],g3a[i][j],N2,M2,K2,w2,D2);
		}
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N1;i++) {
		for(unsigned int j=0;j<N2;j++) {
			for(unsigned int k=0;k<N3;k++) {
				y3[i][j][k] = g3b[k][i][j];
			}
		}
	}

	return(y3);
}

double inline ***FastFourierTransform::ifft3(complex<double> ***x)
{
	#pragma omp parallel for
	for(int i=0;i<(int)N1;i++) {
		for(unsigned int j=0;j<N2;j++) {
			for(unsigned int k=0;k<N3;k++) {
				z3a[i][j][0][k] = x[i][j][k];
			}
			ifft(z3b[i][j],z3a[i][j],N3,M3,K3,v3,D3,F3);
		}
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N2;i++) {
		for(unsigned int j=0;j<N3;j++) {
			for(unsigned int k=0;k<N1;k++) {
				u3a[i][j][0][k] = z3b[k][i][j];
			}
			ifft(u3b[i][j],u3a[i][j],N1,M1,K1,v1,D1,F1);
		}
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N3;i++) {
		for(unsigned int j=0;j<N1;j++) {
			for(unsigned int k=0;k<N2;k++) {
				g3a[i][j][0][k] = u3b[k][i][j];
			}
			ifft(g3b[i][j],g3a[i][j],N2,M2,K2,v2,D2,F2);
		}
	}

	#pragma omp parallel for
	for(int i=0;i<(int)N1;i++) {
		for(unsigned int j=0;j<N2;j++) {
			for(unsigned int k=0;k<N3;k++) {
				r3[i][j][k] = g3b[k][i][j].real();
			}
		}
	}

	return(r3);
}

#endif

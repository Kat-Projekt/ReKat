//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#include "FastFourierTransform.h"

const double pi = 3.14159265358979323846f;

FastFourierTransform::FastFourierTransform(unsigned int n1)
{
	initFFT(n1,0,0,1);
}

FastFourierTransform::FastFourierTransform(unsigned int n1, unsigned int n2)
{
	initFFT(n1,n2,0,2);
}

FastFourierTransform::FastFourierTransform(unsigned int n1, unsigned int n2, unsigned int n3)
{
	initFFT(n1,n2,n3,3);
}

void FastFourierTransform::initFFT(unsigned int n1, unsigned int n2, unsigned int n3, unsigned int d)
{
	if(d>=1) {	//1D FFT
		K1 = (unsigned int)log2f((double)n1);
		N1 = 2<<(K1-1);
		M1 = N1>>1;

		F1 = (double)1/N1;
		w1 = new complex<double>[M1];
		v1 = new complex<double>[M1];

		#pragma omp parallel for
		for(int i=0;i<(int)M1;i++) {
			double real = cosf(2*pi*i/N1);
			double imag = sinf(2*pi*i/N1)*(-1);
			w1[i] = complex<double>(real,imag);
			v1[i] = complex<double>(real,-imag);
		}

		//Decimation lookup table
		D1 = new unsigned int[N1];
		for(unsigned int i=0;i<N1;i++) {
			unsigned int k=0;
			for(unsigned int j=0;j<K1;j++) {
				k = k<<1;
				k += i>>j & 1;
			}
			D1[i] = k;
		}
	}
	else {
		//error condition!
		return;
	}
	
	if(d>=2) {	//2D FFT
		K2 = (unsigned int)log2f((double)n2);
		N2 = 2<<(K2-1);
		M2 = N2>>1;

		F2 = (double)1/N2;
		w2 = new complex<double>[M2];
		v2 = new complex<double>[M2];

		#pragma omp parallel for
		for(int i=0;i<(int)M2;i++) {
			double real = cosf(2*pi*i/N2);
			double imag = sinf(2*pi*i/N2)*(-1);
			w2[i] = complex<double>(real,imag);
			v2[i] = complex<double>(real,-imag);
		}

		//Decimation lookup table
		D2 = new unsigned int[N2];
		for(unsigned int i=0;i<N2;i++) {
			unsigned int k=0;
			for(unsigned int j=0;j<K2;j++) {
				k = k<<1;
				k += i>>j & 1;
			}
			D2[i] = k;
		}
	}
	else {
		K2 = 0;
		N2 = 0;
		M2 = 0;
		F2 = 0;

		w2 = NULL;
		v2 = NULL;
		D2 = NULL;
	}

	if(d>=3) {	//3D FFT
		K3 = (unsigned int)log2f((double)n3);
		N3 = 2<<(K3-1);
		M3 = N3>>1;

		F3 = (double)1/N3;
		w3 = new complex<double>[M3];
		v3 = new complex<double>[M3];

		#pragma omp parallel for
		for(int i=0;i<(int)M3;i++) {
			double real = cosf(2*pi*i/N3);
			double imag = sinf(2*pi*i/N3)*(-1);
			w3[i] = complex<double>(real,imag);
			v3[i] = complex<double>(real,-imag);
		}

		//Decimation lookup table
		D3 = new unsigned int[N3];
		for(unsigned int i=0;i<N3;i++) {
			unsigned int k=0;
			for(unsigned int j=0;j<K3;j++) {
				k = k<<1;
				k += i>>j & 1;
			}
			D3[i] = k;
		}
	}
	else {
		K3 = 0;
		N3 = 0;
		M3 = 0;
		F3 = 0;

		w3 = NULL;
		v3 = NULL;
		D3 = NULL;
	}

	y1 = NULL; r1 = NULL; z1a = NULL; 
	y2 = NULL; r2 = NULL; z2a = NULL; z2b = NULL; u2a = NULL; u2b = NULL; 
	y3 = NULL; r3 = NULL; z3a = NULL; z3b = NULL; u3a = NULL; u3b = NULL; g3a = NULL; g3b = NULL; 

	switch(d) {
		case 1: {	//1D FFT
			z1a = new complex<double>*[K1+1];
			y1 = new complex<double>[N1];
			r1 = new double[N1];
			for(unsigned int i=0;i<K1+1;i++) {
				z1a[i] = new complex<double>[N1];
			}
		} break;

		case 2: {	//2D FFT
			z2a = new complex<double>**[N1];
			z2b = new complex<double>*[N1];
			y2 = new complex<double>*[N1];
			r2 = new double*[N1];
			for(unsigned int i=0;i<N1;i++) {
				z2a[i] = new complex<double>*[K2+1];
				z2b[i] = new complex<double>[N2];
				y2[i] = new complex<double>[N2];
				r2[i] = new double[N2];
				for(unsigned int j=0;j<K2+1;j++) {
					z2a[i][j] = new complex<double>[N2];
				}
				
			}
			u2a = new complex<double>**[N2];
			u2b = new complex<double>*[N2];
			for(unsigned int i=0;i<N2;i++) {
				u2a[i] = new complex<double>*[K1+1];
				u2b[i] = new complex<double>[N1];
				for(unsigned int j=0;j<K1+1;j++) {
					u2a[i][j] = new complex<double>[N1];
				}
			}
		} break;
		
		case 3: {	//3D FFT
			z3a = new complex<double>***[N1];
			z3b = new complex<double>**[N1];
			y3 = new complex<double>**[N1];
			r3 = new double**[N1];
			for(unsigned int i=0;i<N1;i++) {
				z3a[i] = new complex<double>**[N2];
				z3b[i] = new complex<double>*[N2];
				y3[i] = new complex<double>*[N2];
				r3[i] = new double*[N2];
				for(unsigned int j=0;j<N2;j++) {
					z3a[i][j] = new complex<double>*[K3+1];
					z3b[i][j] = new complex<double>[N3];
					y3[i][j] = new complex<double>[N3];
					r3[i][j] = new double[N3];
					for(unsigned int k=0;k<K3+1;k++) {
						z3a[i][j][k] = new complex<double>[N3];
					}
				}
			}

			u3a = new complex<double>***[N2];
			u3b = new complex<double>**[N2];
			for(unsigned int i=0;i<N2;i++) {
				u3a[i] = new complex<double>**[N3];
				u3b[i] = new complex<double>*[N3];
				for(unsigned int j=0;j<N3;j++) {
					u3a[i][j] = new complex<double>*[K1+1];
					u3b[i][j] = new complex<double>[N1];
					for(unsigned int k=0;k<K1+1;k++) {
						u3a[i][j][k] = new complex<double>[N1];
					}
				}
			}
			
			g3a = new complex<double>***[N3];
			g3b = new complex<double>**[N3];
			for(unsigned int i=0;i<N3;i++) {
				g3a[i] = new complex<double>**[N1];
				g3b[i] = new complex<double>*[N1];
				for(unsigned int j=0;j<N1;j++) {
					g3a[i][j] = new complex<double>*[K2+1];
					g3b[i][j] = new complex<double>[N2];
					for(unsigned int k=0;k<K2+1;k++) {
						g3a[i][j][k] = new complex<double>[N2];
					}
				}
			}

		} break;
	}
}

FastFourierTransform::~FastFourierTransform()
{
	delete[] w1;
	delete[] v1;
	delete[] D1;

	if(w2!=NULL) {
		delete[] w2;
		delete[] v2;
		delete[] D2;
	}

	if(w3!=NULL) {
		delete[] w3;
		delete[] v3;
		delete[] D3;
	}

	if(y1!=NULL) {
		for(unsigned int i=0;i<K1+1;i++) {
			delete[] z1a[i];
		}

		delete[] z1a;
		delete[] y1;
		delete[] r1;
	}

	if(y2!=NULL) {
		for(unsigned int i=0;i<N1;i++) {
			for(unsigned int j=0;j<K2+1;j++) {
				delete[] z2a[i][j];
			}
			delete[] z2a[i];
			delete[] z2b[i];
			delete[] y2[i]; 
			delete[] r2[i];
		}
		delete[] z2a;
		delete[] z2b;
		delete[] y2; 
		delete[] r2; 

		for(unsigned int i=0;i<N2;i++) {
			for(unsigned int j=0;j<K1+1;j++) {
				delete[] u2a[i][j];
			}
			delete[] u2a[i];
			delete[] u2b[i];
		}
		delete[] u2a;
		delete[] u2b;
	}

	if(y3!=NULL) {
		for(unsigned int i=0;i<N1;i++) {
			for(unsigned int j=0;j<N2;j++) {
				for(unsigned int k=0;k<K3+1;k++) {
					delete[] z3a[i][j][k];
				}
				delete[] z3a[i][j];
				delete[] z3b[i][j];
				delete[] y3[i][j]; 
				delete[] r3[i][j]; 
			}
			delete[] z3a[i];
			delete[] z3b[i];
			delete[] y3[i];
			delete[] r3[i];
		}
		delete[] z3a;
		delete[] z3b;
		delete[] y3; 
		delete[] r3; 

		for(unsigned int i=0;i<N2;i++) {
			for(unsigned int j=0;j<N3;j++) {
				for(unsigned int k=0;k<K1+1;k++) {
					delete[] u3a[i][j][k];
				}
				delete[] u3a[i][j];
				delete[] u3b[i][j];
			}
			delete[] u3a[i];
			delete[] u3b[i];
		}
		delete[] u3a;
		delete[] u3b;

		for(unsigned int i=0;i<N3;i++) {
			for(unsigned int j=0;j<N1;j++) {
				for(unsigned int k=0;k<K2+1;k++) {
					delete[] g3a[i][j][k];
				}
				delete[] g3a[i][j];
				delete[] g3b[i][j];
			}
			delete[] g3a[i];
			delete[] g3b[i];
		}
		delete[] g3a;
		delete[] g3b;
	}	
}




// fft.cp - a Fast Fourier Transformation function

// Transcribed from the FORTRAN as presented in "FFT as Nested
// Multiplication with a Twist", by Carl de Boor in SIAM Sci. Stat. Comput.
// Vol 1 No 1, March 1980

#include <complex.h>

void fftstp(complex *, int, int, int, complex*);

const NEXTMX = 12;
int prime[NEXTMX] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 31, 37 };

complex *fft(complex *z1, complex *z2, int n, int inzee)
{
/*
	Construct the discrete Fourier transform of z1 (or z2) in the
	Cooley-Tukey way, but with a twist.

	z1[before], z2[before].
	inzee == 1 means input in z1; inzee==2 means input in z2.
*/
	int before = n;
	int after = 1;
	int next = 0;
	int now;

	do {
		int np = prime[next];
		if ( ( before/np)*np < before) {
			if (++next < NEXTMX) continue;
			now = before;
			before = 1;
		}
		else {
			now = np;
			before /= np;
		}

		if (inzee == 1)
			fftstp(z1, after, now, before, z2);
		else
			fftstp(z2, after, now, before, z1);
		inzee = 3 - inzee;
		after *= now;
	} while ( 1 < before );

	return (inzee == 1) ? z1 : z2;
}

void fftstp (complex *zin, int after, int now, int before, cpmplex *zout)
{
/*
	zin(after,before,now)
	zout(after,now,before)

	there are ample scope for optimization
*/

	double angle = PI2/(now*after);
	complex omega = complex ( cos(angle), -sin(angle));
	complex arg = 1;

	for (register int j = 0; j < now; j++
	{
	    for (register int ia = 0; ia < after; ia++)
	    {
		for (register int ib = 0; ib < before; ib++)
		{
		    // value = zin (ia, ib, now)

		    complex value =
				zin[ia + ib*after + (now-1)*before*after];

		    for (register int in = now-2; 0 <= zin; in--)
		    {
			value *= arg;
			value += zin[ia + ib*after + in*before*after];
		    }

		    zout[ia + j * after + ib*now*after] = value;
		}
		arg *= omega;
	    }
	}
}

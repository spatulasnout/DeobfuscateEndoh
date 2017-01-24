//
// Original: http://www.ioccc.org/2012/endoh1/hint.html
//
// Initial deobfuscation by: https://github.com/litonico/DeobfuscateEndoh
//
// Continued deobfuscation by: https://github.com/spatulasnout/DeobfuscateEndoh
//	- Converted to C++ because my C compiler has no "double complex" support.
//	- Separated out init / run methods.
//	- Deobfuscated remaining tricky one-liners.
//	- Converted to Particle struct.
//	- Converted to 2D vector instead of std::complex.
//

#define USE_COMPLEX_ABS_PRECISION	0	// see Vector2D::length for explanation

#include <exception>
#if USE_COMPLEX_ABS_PRECISION
#include <complex>
#endif
#include <memory>
#include <chrono>
#include <thread>
#include <stdio.h>

#ifdef _WIN32
#include "ansicolor-w32.h"
#endif

#if USE_COMPLEX_ABS_PRECISION
using std::complex;
#endif
using std::unique_ptr;

#define Gravity		1.0f
#define Pressure	4.0f
#define Velocity	8.0f

template <typename T>
struct Vector2D {
	typedef T scalar_type;
	typedef Vector2D<T> this_type;

	union {
		scalar_type		v[2];
		struct {
			scalar_type	x;
			scalar_type	y;
		};
	};

	Vector2D () : x(0), y(0) {}
	Vector2D (scalar_type x_, scalar_type y_) : x(x_), y(y_) {}

	inline void set (scalar_type x_, scalar_type y_) { x=x_; y=y_; }
	inline void zero () { x = 0; y = 0; }

	inline scalar_type	operator[] (int idx) const { return v[idx]; }
	inline scalar_type & operator[] (int idx) { return v[idx]; }

	inline this_type & operator= (const this_type &other)
	{
		x = other.x;
		y = other.y;
		return (*this);
	}

	inline bool operator== (const this_type &other) const
	{
		return x == other.x  &&  y == other.y;
	}

	inline bool operator!= (const this_type &other) const
	{
		return ! ((*this) == other);
	}

	inline this_type & operator+=(const this_type &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline this_type & operator-=(const this_type &other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline this_type & operator*=(const scalar_type factor)
	{
		x *= factor;
		y *= factor;
		return *this;
	}

	inline this_type & operator/=(const scalar_type factor)
	{
		x /= factor;
		y /= factor;
		return *this;
	}

	inline this_type operator+(const this_type &other) const
	{
		this_type tmp(*this);
		return tmp += other;
	}

	inline this_type operator-(const this_type &other) const
	{
		this_type tmp(*this);
		return tmp -= other;
	}

	inline this_type operator*(const scalar_type factor) const
	{
		this_type tmp(*this);
		return tmp *= factor;
	}

	inline this_type operator/(const scalar_type factor) const
	{
		this_type tmp(*this);
		return tmp /= factor;
	}

	inline scalar_type length () const
	{
		// NOTE: abs(complex&) (at least the MSVC++ implementation) goes
		// through some lengths to preserve precision.  In order to achieve
		// identical behavior to the original, we need to use abs(complex&)
		// here to compute length.
		//
		// However, using the simpler std::sqrt-based length forumla runs
		// significantly faster--speeds up framerate by over 2x--and produces
		// almost the same output (very slight differences over time.)
#if USE_COMPLEX_ABS_PRECISION
		complex<scalar_type> cv(x, y);
		return std::abs(cv);
#else
		return std::sqrt(x*x + y*y);
#endif
	}
};

template <typename T>
inline Vector2D<T> operator*(const T factor, const Vector2D<T> &rhs)
{
	return rhs * factor;
}

typedef float real_type;
typedef Vector2D<real_type> vec2_type;

struct Particle {
	vec2_type	position;
	vec2_type	force;
	vec2_type	velocity;
	real_type	density;
	int			wall_flag;
};

class EndohDeobfuscate {
public:
	static const size_t SIMBUF_LEN = 19538;
	static const size_t SCRBUF_LEN = 6856;
	static const int SCR_WID = 80;
	static const int SCR_HGT = 24;

	void init (FILE *in = stdin)
	{
		vec2_type w;

		r = a;
		for (int ch;  (ch = getc(in)) != EOF; ) {
			if (ch > '\n') {

				if (ch > ' ') {
					Particle *s = (r + 1);
					r->position = w;
					s->position = vec2_type(w.x, w.y + 1.0f);
					r->wall_flag = s->wall_flag = (ch == '#');
					r += 2;
				}


				w.x += 1.0f;
			}
			else {
				w = vec2_type(0, ((int)w.y) + 2);
			}
		}
	}

	void run ()
	{
		memset(b, 0, SCRBUF_LEN);

		clearscreen();

		int dbg__frame = 0;
		int dbg__sim_cells_per_frame = 0;
		int dbg__sim_ops_per_frame = 0;
		int dbg__render_ops_per_frame = 0;

		for (;;) {
			Particle *p, *q;
			vec2_type d;
			real_type w;
			int x, y;
			char *t;

			++dbg__frame;

			cursor_home();

			puts(b);

			printf("\nframe          : %08d\nsim cells/frame: %08d\nsim ops/frame  : %08d\nvis ops/frame  : %08d\n",
					dbg__frame, dbg__sim_cells_per_frame, dbg__sim_ops_per_frame, dbg__render_ops_per_frame);

			dbg__sim_cells_per_frame = 0;
			dbg__sim_ops_per_frame = 0;
			dbg__render_ops_per_frame = 0;

			for (p = a; p < r; ++p) {
				p->density = p->wall_flag * 9.0f;
				for (q = a; q < r; ++q) {
					w = (d = p->position - q->position).length() / 2 - 1;
					if (0 < (x = (1.0f - w))) {
						p->density += w * w;
						++dbg__sim_ops_per_frame;
					}
					++dbg__sim_cells_per_frame;
				}
			}

			for (p = a; p < r; ++p) {
				p->force = vec2_type(0, Gravity);
				for (q = a; q < r; ++q) {
					w = (d = p->position - q->position).length() / 2 - 1;
					if (0 < (x = (1.0f - w))) {
						p->force += (w * ((d * (3.0f - p->density - q->density) * Pressure) + (p->velocity * Velocity) - (q->velocity * Velocity))) / p->density;
						++dbg__sim_ops_per_frame;
					}
					++dbg__sim_cells_per_frame;
				}
			}

			for (x = 0; 2002 > x; ++x) {
				b[x] = 0;
				++dbg__render_ops_per_frame;
			}

			// compute marching square edges, and sim cell position update
			for (p = a; p < r; ++p) {
				t = b + (x = (p->position.x)) + SCR_WID * (y = (p->position.y / 2.0f));
				p->position += p->velocity += (p->force / 10.0f) * (float)(!(p->wall_flag));
				if (0 <= x && x < (SCR_WID - 1) && 0 <= y && y < (SCR_HGT - 1)) {
					t[0] |= 8;
					t[1] |= 4;
					t += SCR_WID;
					t[0] |= 2;
					t[1] = 1;
				}
				++dbg__render_ops_per_frame;
			}

			// render marching square edge data to ascii
			for (x = 0; 2002 > x; ++x) {
				bool want_newline = (x > 0) && (((x + 1) % SCR_WID) == 0);
				if (want_newline) {
					b[x] = '\n';
				}
				else {
					char edge_bits = b[x];
					b[x] = " '`-.|//,\\|\\_\\/#"[edge_bits];
				}
				++dbg__render_ops_per_frame;
			}

			// std::this_thread::sleep_for(std::chrono::milliseconds(12));
		} 

	}

protected:
	void clearscreen ()
	{
		puts("\x1b[2J");
	}

	void cursor_home ()
	{
		puts("\x1b[1;1H");
	}

	Particle			a[SIMBUF_LEN];
	Particle			*r;
	char				b[SCRBUF_LEN];
};


// Expects a text document providing initial state on stdin.
// For example, feed it the original "endoh1.c" source code
// which looks like the word "Fluid".

int main (int argc, char **argv)
{
	try {
		unique_ptr<EndohDeobfuscate> endoh(new EndohDeobfuscate());

		endoh->init();
		endoh->run();
	}
	catch (std::exception &ex) {
		fprintf(stderr, "FATAL: exception in outer scope: %s", ex.what());
	}
	catch (...) {
		fprintf(stderr, "FATAL: unknown C++ exception in outer scope");
	}

	return 0;
}



#if 0

// ****************************************************************************
// The original endoh1.c:

#  include<stdio.h>//  .IOCCC                                         Fluid-  #
#  include <unistd.h>  //2012                                         _Sim!_  #
#  include<complex.h>  //||||                     ,____.              IOCCC-  #
#  define              h for(                     x=011;              2012/*  #
#  */-1>x              ++;)b[                     x]//-'              winner  #
#  define              f(p,e)                                         for(/*  #
#  */p=a;              e,p<r;                                        p+=5)//  #
#  define              z(e,i)                                        f(p,p/*  #
## */[i]=e)f(q,w=cabs  (d=*p-  *q)/2-     1)if(0  <(x=1-      w))p[i]+=w*/// ##
   double complex a [  97687]  ,*p,*q     ,*r=a,  w=0,d;    int x,y;char b/* ##
## */[6856]="\x1b[2J"  "\x1b"  "[1;1H     ", *o=  b, *t;   int main   (){/** ##
## */for(              ;0<(x=  getc (     stdin)  );)w=x  >10?32<     x?4[/* ##
## */*r++              =w,r]=  w+1,*r     =r[5]=  x==35,  r+=9:0      ,w-I/* ##
## */:(x=              w+2);;  for(;;     puts(o  ),o=b+  4){z(p      [1]*/* ##
## */9,2)              w;z(G,  3)(d*(     3-p[2]  -q[2])  *P+p[4      ]*V-/* ##
## */q[4]              *V)/p[  2];h=0     ;f(p,(  t=b+10  +(x=*p      *I)+/* ##
## */80*(              y=*p/2  ),*p+=p    [4]+=p  [3]/10  *!p[1])     )x=0/* ##
## */ <=x              &&x<79   &&0<=y&&y<23?1[1  [*t|=8   ,t]|=4,t+=80]=1/* ##
## */, *t              |=2:0;    h=" '`-.|//,\\"  "|\\_"    "\\/\x23\n"[x/** ##
## */%80-              9?x[b]      :16];;usleep(  12321)      ;}return 0;}/* ##
####                                                                       ####
###############################################################################
**###########################################################################*/

// ****************************************************************************

// gcc -DG=1 -DP=4 -DV=8 -D_BSD_SOURCE -std=c99 -Wall -W -Wextra -pedantic -O2 -o endoh1.exe endoh1.c
// gcc -DGravity=1 -DPressure=4 -DVelocity=8 -D_BSD_SOURCE -std=c99 -Wall -W -Wextra -pedantic -O2 -o endoh1_deobfuscate.exe endoh1_deobfuscate.c

#endif // 0


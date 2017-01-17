
#include <exception>
#include <complex>
#include <memory>
#include <chrono>
#include <thread>
#include <stdio.h>

#ifdef _WIN32
#include "ansicolor-w32.h"
#endif


using std::complex;
using std::unique_ptr;

#define Gravity		1.0f
#define Pressure	4.0f
#define Velocity	8.0f

typedef complex<float> complex_type;

struct Particle {
	complex_type	position;
	complex_type	wall_flag;
	complex_type	pressure;
	complex_type	density;
	complex_type	force;
	complex_type	velocity;
};

class EndohDeobfuscate {
public:
	static const size_t SIMBUF_LEN = 97687;
	static const size_t SCRBUF_LEN = 6856;

	void init (FILE *in = stdin)
	{
		using namespace std::complex_literals;

		const complex_type I(1i);
		complex_type w;

		r = a;
		for (int ch;  (ch = getc(in)) != EOF; ) {
//			w = (x > 10 ? (32 < x ? 4[*r++ = w, r] = w + 1, *r = r[5] = x == 35, r += 9 : 0, w - _Complex_I) : (x = w + 2));

			if (ch > '\n') {

				if (ch > ' ') {
					r[0] = w;
					r[5] = (w + 1.0f);
					r[1] = r[6] = (ch == '#');
					r += 10;
				}

				w = w - I;
			}
			else {
				w = complex_type(((int)w.real()) + 2, 0);
			}
		}
	}

	void run ()
	{
		using namespace std::complex_literals;

		const complex_type I(1i);

		memset(b, 0, SCRBUF_LEN);

		clearscreen();

		int dbg__sim_ops_per_frame = 0;
		int dbg__render_ops_per_frame = 0;

		for (;;) {
			complex_type *p, *q, w, d;
			int x, y;
			char *t;

			cursor_home();

			char *o = b + (4 + 6);
			puts(o);

			printf("\nsim ops/frame: %08d\nvis ops/frame: %08d\n", dbg__sim_ops_per_frame, dbg__render_ops_per_frame);

			dbg__sim_ops_per_frame = 0;
			dbg__render_ops_per_frame = 0;

			for (p = a; p[2] = p[1] * 9.0f, p < r; p += 5) {
				for (q = a; w = abs(d = *p - *q) / 2 - 1, q < r; q += 5) {
					if (0 < (x = (1.0f - w).real())) {
						p[2] += w * w;
						++dbg__sim_ops_per_frame;
					}
				}
			}

			for (p = a; p[3] = Gravity, p < r; p += 5) {
				for (q = a; w = abs(d = *p - *q) / 2 - 1, q < r; q += 5) {
					if (0 < (x = (1.0f - w).real())) {
						p[3] += w * (d * (3.0f - p[2] - q[2]) * Pressure + p[4] * Velocity - q[4] * Velocity) / p[2];
						++dbg__sim_ops_per_frame;
					}
				}
			}

			for (x = 9; 2011 > x++;) {
				b[x] = 0;
				++dbg__render_ops_per_frame;
			}

			// marching squares
			for (p = a; (t = b + 10 + (x = (*p * I).real()) + 80 * (y = (*p / 2.0f).real()), *p += p[4] += (p[3] / 10.0f) * (float)(!(p[1].real())) ), p < r; p += 5) {
				x = 0 <= x && x < 79 && 0 <= y && y < 23 ? 1[1[*t |= 8, t] |= 4, t += 80] = 1, *t |= 2 : 0;
				++dbg__render_ops_per_frame;
			}

			for (x = 9; 2011 > x++;) {
				b[x] = " '`-.|//,\\|\\_\\/#\n"[x % 80 - 9 ? x[b] : 16];
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

	complex_type		a[SIMBUF_LEN];
	complex_type		*r;
	char				b[SCRBUF_LEN];
};


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

// Re-assignment:
// G -> Gravity
// P -> Pressure
// V -> Velocity
// These are read in from command line args

// Endoh:
// "Though some assignments may look meaningless, it is actually meaningful;
// it extracts “integer part of real part” from a complex value by assigning
// (and casting) it to an integer-type variable."

// Essential info: in C, a = b EVALUATES to 0, and performs an assignment
// as a 'side effect.' Assignment in the initialization of a for loop will
// make the compiler complain; when he uses this, Mr. Endoh wraps the
// expressions in parens to silence that.


// Use of the comma operator in C:
// For example, in the statement
// *r++ = w, r
//
// The contents of the address pointed to by r is set to w (a complex double).
// The assignment produces a return value of 0, which the comma promptly discards.
// r is returned (a pointer)
// The contents of r are incremented (postfix operator)
//
// Mr. Endoh uses the comma extensively to perform assignment inline.

#include <stdio.h>//  .IOCCC                                         Fluid-  #
#include <unistd.h>  //2012                                         _Sim!_  #
#include <complex.h>  //||||                     ,____.              IOCCC-  #

double complex	a[97687],
// Endoh: "Every five entries of double complex a[] contain information
// of one particle: position, wall-flag, density, force, and velocity in turn."
       *p, *q, *r = a, w = 0, d;

int	x, y;
char b[6856] = "\x1b[2J" //clears the screen (ANSI escape code). '\x1b' is equivalent to ESC.
               "\x1b[1;1H     "; // Moves the cursor to line 1, char 1
                                 //TODO: unclear if whitespace important

char *o = b, *t;

// The character # represents “wall particle” (a particle with fixed position),
// and any other non-space characters represent free particles.

int main(){

    // Endoh: "This program reads a text from standard input, and uses it as 
    // an initial configuration of the particles."
	for (; 0 < (x = getc(stdin));){ // While the end of stdin is not reached
         w = (x > 10 ? (32 < x ? 4[*r++ = w, r] = w + 1, *r = r[5] = x == 35, r += 9 : 0, w - _Complex_I) : (x = w + 2)); // why the heck were there two semicolons

        // Breaking it down!
        //
        // 4[*r++ = w, r] = w + 1,
        //
        // 4[x] is a weird but valid array access: it's the same as x[4] 
        // for any array x.  The expression x[y] is defined to be *((x)+(y)), 
        // so x and y can be swapped (Kernigan and Ritchie).
        //
        // Again, the comma makes this into a multi-statement instruction
        // *r++ = w; // -> returns 0, which is disregarded
        // r[4] = w+1;
        //
        // Note the order of application in `*r++'! To be a little more clear:
        //
        // *(r++) = w;
        // r[4] = w+1;
        //
        //
        // Okay, next bit
        // *r = r[5] = x == 35,
        //
        // Assignment is right-associative, so let's put some parens in
        //
        // (*r = (r[5] = (x == 35))),
        //  
        // Doesn't really look much better, does it? But we can break it down.
        // x == 35 is a boolean: x is the current character, and 35 is the 
        // ASCII code for '#' (the 'hash' symbol), which Mr. Endoh uses to mark
        // a wall in the simulation.
        //
        // Let's rewrite this as an if statement:
        //
        // *r is r[0]; given the context, that's a little clearer
        //
        // if (x == 35) // if x (the current character) is '#'
        //      // This sets the 'wall flag' for unmoveable particles.
        //      r[0] = r[5] = 1;
        // else 
        //      r[0] = r[5] = 0;
        //
        //
        // More verbose, but more comprehensible!
        //
        // r += 9;
        //

        /*
        if (x > 10){
            //  
            w = (32 < x ? 4[*r++ = w, r] = w + 1, *r = r[5] = x == 35, r += 9 : 0, (w - _Complex_I));
            
            if (x > 32){
                w = 4[*r++ = w, r] = w + 1, *r = r[5] = x == 35, r += 9;
            }
            else{
                w = 0, w - _Complex_I;
            }
            //
        }
        else {
            x = w + 2;
            w = x;
        }
        
        */
 
    }

	for (;; puts(o), o = b + 4) { // The SPH algorithm
        // while(true){ 
        // puts(o) // clears the screen, returns 0 (probably- need to check the value of o)
        // the comma discards the return value
        // o = b + 4 // o points to the 4th elem of b

		for (p = a; p[2] = p[1] * 9, p < r; p += 5){
			for (q = a; w = cabs(d = *p - *q) / 2 - 1, q < r; q += 5){
				if (0 < (x = 1 - w)){
					p[2] += w * w;
                }
            }
        }

        // Iterate over the positions(?) of particles
		for (p = a; p[3] = Gravity, p < r; p += 5){
			for (q = a; w = cabs(d = *p - *q) / 2 - 1, q < r; q += 5){
				if (0 < (x = 1 - w)){
					p[3] += w * (d * (3 - p[2] - q[2]) * Pressure + p[4] * Velocity - q[4] * Velocity) / p[2];
                }
            }
        }

		for (x = 011; 2012 - 1 > x++;){ // 011 is ASCII horizontal tab in octal; important?
			b[x] = 0;
        }

		for (p = a; (t = b + 10 + (x = *p * _Complex_I) + 80 * (y = *p / 2), *p += p[4] += p[3] / 10 * !p[1]), p < r; p += 5) {
			x = 0 <= x && x < 79 && 0 <= y && y < 23 ? 1[1[*t |= 8, t] |= 4, t += 80] = 1, *t |= 2 : 0;
        }

		for (x = 011; 2012 - 1 > x++;){ // Render the particles as marching squares
			b[x] = " '`-.|//,\\" "|\\_" "\\/\x23\n"[x % 80 - 9 ? x[b] : 16];; // \x23 is the # (hash) symbol in hex ASCII
            // Maybe uses \x23 instead of # because # is the walls?
            // Uses array offsets to get the appropriate set of chars for the 
            // Marching squares algorithm
            // 0 =  (space)
            // 1 = '
            // 2 = `
            // 3 = - 
            // 4 = 
        }

		usleep(12321);
	} 
    
    return 0;
}

// ****************************************************************************

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

double complex a [  97687]  ,*p,*q     ,*r=a,  w=0,d;
int x,y;
char b[6856]="\x1b[2J"  "\x1b"  "[1;1H     ", *o=  b, *t;

int main ()
{
	for(; 0<( x = getc (stdin)  );)
		w=x  >10?32<     x?4[
		*r++              =w,r]=  w+1,*r     =r[5]=  x==35,  r+=9:0      ,w-I
			:(x=              w+2);;
			for(;;     puts(o  ),o=b+  4){
			for( p=a; p [2]=p [1]* 9,p<r; p+=5)
			for( q=a; w=cabs (d=*p- *q)/2- 1,q<r; q+=5)
			if(0 <(x=1- w))p[2]+=w*              w;
			for( p=a; p [3]=G,p<r; p+=5)
			for( q=a; w=cabs (d=*p- *q)/2- 1,q<r; q+=5)
			if(0 <(x=1- w))p[3]+=w*(d*(     3-p[2]  -q[2])  *P+p[4      ]*V-
				q[4]              *V)/p[  2];
			for( x=011; 2012 -1>x ++;)b[ x]=0     ;
			for( p=a; ( t=b+10 +(x=*p *I)+ 80*( y=*p/2 ),*p+=p [4]+=p [3]/10 *!p[1]),p<r; p+=5)x=0
				<=x              &&x<79   &&0<=y&&y<23?1[1  [*t|=8   ,t]|=4,t+=80]=1
				, *t              |=2:0;
				for( x=011; 2012 -1>x ++;)b[ x]=" '`-.|//,\\"  "|\\_"    "\\/\x23\n"[x
				%80-              9?x[b]      :16];;usleep(  12321)      ;
			}
	return 0;
}

// ****************************************************************************

double complex a [  97687], *p, *q, *r = a,  w = 0, d;
int x, y;
char b[6856] = "\x1b[2J"  "\x1b"  "[1;1H     ", *o =  b, *t;

int main ()
{
	for (; 0 < ( x = getc (stdin)  );)
		w = x  > 10 ? 32 <     x ? 4[		*r++              = w, r] =  w + 1, *r     = r[5] =  x == 35,  r += 9 : 0, w - I
		: (x =              w + 2);;
	for (;;     puts(o  ), o = b +  4) {
		for ( p = a; p [2] = p [1] * 9, p < r; p += 5)
			for ( q = a; w = cabs (d = *p - *q) / 2 - 1, q < r; q += 5)
				if (0 < (x = 1 - w))p[2] += w *              w;
		for ( p = a; p [3] = G, p < r; p += 5)
			for ( q = a; w = cabs (d = *p - *q) / 2 - 1, q < r; q += 5)
				if (0 < (x = 1 - w))
					p[3] += w * (d * (     3 - p[2]  - q[2])  * P + p[4      ] * V -		q[4]              * V) / p[  2];
		for ( x = 011; 2012 - 1 > x ++;)b[ x] = 0     ;
		for ( p = a; ( t = b + 10 + (x = *p * I) + 80 * ( y = *p / 2 ), *p += p [4] += p [3] / 10 * !p[1]), p < r; p += 5)
			x = 0	<= x              && x < 79   && 0 <= y && y < 23 ? 1[1  [*t |= 8, t] |= 4, t += 80] = 1
			, *t              |= 2 : 0;
		for ( x = 011; 2012 - 1 > x ++;)
			b[ x] = " '`-.|//,\\"  "|\\_"    "\\/\x23\n"[x % 80 -              9 ? x[b]      : 16];;
		usleep(  12321)      ;
	}
	return 0;
}

// gcc -DG=1 -DP=4 -DV=8 -D_BSD_SOURCE -std=c99 -Wall -W -Wextra -pedantic endoh1.c
// gcc -DGravity=1 -DPressure=4 -DVelocity=8 -D_BSD_SOURCE -std=c99 -Wall -W -Wextra -pedantic -o endoh1_deobfuscate.exe endoh1_deobfuscate.c

#endif // 0


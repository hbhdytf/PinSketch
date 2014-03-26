
INTRODUCTION

The files pinsketch.h, sketch.cpp, differ.cpp, io.cpp and bch.cpp contain
an implementation of PinSketch, the BCH-based secure sketch from "Fuzzy
Extractors: How to Generate Strong Keys from Biometrics and Other Noisy
Data" by Dodis, Ostrovsky, Reyzin and Smith (SIAM Journal on Computing,
volume 38, number 1, pages 97-139, 2008; on-line at
http://eprint.iacr.org/2003/235 and http://arxiv.org/abs/cs/0602007).  The
implementation is by Kevin Harmon and Leonid Reyzin (reyzin@cs.bu.edu).  It
uses Victor Shoup's NTL (A Library for doing Number Theory), which must be
installed before it will compile; see http://www.shoup.net.

Given an input set A of any number of (nonzero) m-bit strings and a
parameter t, the program "sketch" will produce as output a sketch of A of
size tm bits.  Then, if the size of the symmetric difference between sets A
and B is at most t, the program "differ" will find the symmetric difference
between A and B given only B and the sketch of A (thus, in particular,
recovering elements of A that are not in B without seeing A).  In fact,
"differ" can work given just the sketches of A and B (even if the sketches
were computed with different values of t, as long as the number of
differences is at most the smaller of the two).

These programs provide an efficient way to find differences between two
sets without having to communicate the sets.  They also allow for
information-reconciliation with minimum information leakage.  For more
details and applications, see the aforementioned "Fuzzy Extractors" paper.

See also companion implementation of Improved Juels-Sudan (IJS) Secure
Sketch, another secure sketch from the same paper, at
http://www.cs.bu.edu/~reyzin/code/fuzzy.html. Note that the user interface
for the two implementations is slightly different (in particular, because
IJS, unlike this implementation, cannot produce symmetric differences from
only two sketch files), that PinSketch is more efficient, and that PinSketch,
unlike IJS, does not require the two sets of be of the same size.

The mathematical meat of the implementation is in bch.cpp, which is based
on "Syndrome Encoding and Decoding of BCH Codes in Sublinear Time"
(excerpted from the aforementioned "Fuzzy Extractors" paper).  The code in
bch.cpp is of greatest general interest; the rest of the code is specific
to the formats chosen by this implementation.

WARNINGS AND LIMITATIONS

The programs will not behave correctly if any of the following occurs.

1) There are duplicate elements in A or duplicate elements in B (i.e., if A
or B are multisets).

2) A or B contains a string of all zeroes.

3) t is greater than or equal to 2^{m-1}

4) The input or sketch files do not follow the prescribed format.

5) The size of the symmetric difference between the two sets is greater
than t (then "differ" will output an error message unless there is a set C
that has the same sketch as A and whose symmetric difference with B has no
more than t elements, in which case C will be output).

Note that cases 1, 2, 3, and 4 can be taken care of at the input stage (but
currently are not).  Case 5 is impossible to fix completely, although there
are techniques that will reduce the likelihood of an incorrect output at
the expense of a larger sketch.


USAGE DETAILS

To compile, make sure you have NTL (A Library for doing Number Theory)
installed (see http://www.shoup.net) and issue command 

make

If the compilation fails, see the comments in the file called Makefile.
Once everything compiles, invoking

./sketch A.set

will produce A.ss (note that file must end in .set).  The format of A.set
is

t=<integer>
m=<integer>
[<int1> <int2> ... ]

where <intj> is a nonzero m-bit integer written in decimal. 

For example,

t=2
m=10
[2 347 532 87 876 39]

(Note that the choice to have inputs be decimal integers was arbitrary, and
the program can be easily modified to accept other inputs.  Also note that
no spaces around the equal signs are allowed; otherwise, spaces and
linebreaks can be added anywhere.)

The sketch file A.ss will contain information on the input parameters and
generating polynomial of GF(2^m), followed by the tm bits of the sketch in
human-readable form as t elements of GF(2^m).  Of course, in a system where
the input parameters are fixed, only the tm bits are needed.

Invoking

./differ A.ss B.set

will output the symmetric difference between A and B if it has at most t
elements.  More generally, if a set C exists such that the symmetric
difference between B and C is of size at most t and the sketch of C is
equal to the sketch in A.ss, then the symmetric difference between B and C
will be output (note that such a C is unique if it exists).  If such a C
does not exist, then an error message will be printed.

Note that A and B are sets, not sequences: the order of elements doesn't
matter, and, therefore, the order of elements in the symmetric difference
is arbitrary.  The format of B.set is the same as of A.set; however, its t
and m values will be ignored.

Invoking

./differ A.ss B.ss

(assuming B.ss was computed using sketch B.set) will also output the
symmetric difference between A and B (with the same generalization).  If A
and B were computed with respect to different m (or the same m but
different generating polynomials, which should not happen the way sketch is
currently coded), an error message will be output.  If A.ss was computed
with respect to t1 and B.ss was computed with respect to t2, then the
difference must have no more than min(t1, t2) elements; else an error
message will be output, or possibly an incorrect answer.

Sample files X.set, Y.set X.ss, Y.ss are included with the implementation.

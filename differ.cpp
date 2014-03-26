// differ.cpp
//
// C++ code by Kevin Harmon and Leonid Reyzin
//
// Finds the symmetric difference of two sets given their 
// PinSketch BCH-based secure sketches,
// or given the sketch of the first set
// and the entire second set.
//
// See pinsketch.txt for detailed documentation
// This code and explanatory notes
// are hosted at http://www.cs.bu.edu/~reyzin/code/fuzzy.html
//

#include "pinsketch.h"

int main(int argc, char** argv)
{
	int len1, len2; // lengths of argv[1], argv[2], if they exist

	long d; // minimum distance of the code; 
                // can handle set difference up to t=(d-1)/2 elements
		// sketches are (d-1)/2 elements long

	long d1, d2;

        long m; // elements of the set and of the sketch are m-bit values

	GF2X irrP; // irreducible polynomial generating GF(2^m)

	vec_GF2E ss1; // sketch of the first set
	vec_GF2E ss2; // sketch of the second set
	vec_GF2E ss; // sketch of the set difference
	bool success;


	if (argc != 3 || 
	    (len1=strlen(argv[1])) < 4 || (len2=strlen(argv[2])) < 4 ||
	    strcmp(&argv[1][len1-3], ".ss") ||
	    (strcmp(&argv[2][len2-3], ".ss") && strcmp (&argv[2][len2-4], ".set")))
	{	
		cerr << "Usage:\ndiffer A.ss B.ss\nOR\ndiffer A.ss B.set\n";
		if (argc == 3)
			cerr << "(first file must be named `*.ss`, second can be either `*.ss' or `*.set')" << endl;
		return -1;
	}
		
	bool mode; // 0 if comparing .ss with .ss; 1 if comparing .ss with .set
	mode = (strcmp (&argv[2][len2-4], ".set")==0);

	ifstream infile1(argv[1]);
	ifstream infile2(argv[2]);
        if (!infile1 || !infile2) {
           cerr << "Could not open files!" << endl;
           if (infile1) infile1.close();
           if (infile2) infile2.close();
           return -1;
        }

	// fix field and error tolerance
	ReadSSParams(m, d1, irrP, infile1);
	if (!mode) { // the second input is .ss, i.e., another secure sketch
        	long m2;
		GF2X irrP2;
		ReadSSParams(m2, d2, irrP2, infile2);
		// We cannot work with the two sketches unless
		// they were computed over the same field
		// If error-tolerance is not the same, that's ok, we will work with the smaller
		// of the two
		if (m != m2 || irrP != irrP2)
		{
		  cerr << "Incompatible parameters!" << endl;
		  infile1.close();
		  infile2.close();
		  return -1;
		}
	}

	// Initialize the field
	GF2E::init(irrP);

	// read in first syndrome
	ReadSS(ss1, infile1, d1);
	infile1.close();

	if (!mode) { // the second input is .ss, i.e., another secure sketch
		// read in second syndrome
	        ReadSS(ss2, infile2, d2);
		// chop the syndromes to the smaller of the two
		d = min(d1, d2);
		ss1.SetLength((d-1)/2);
		ss2.SetLength((d-1)/2);
	}

	else { // the second input is a .set file, i.e., a set
	      // params are ignored in .set case and are taken from the .ss file
		long m2;
		ReadSetParams(m2, d2, infile2); 
		d = d1;
		// compute second syndrome
		vec_GF2E set;
		ReadSet(set, infile2, m);
		BCHSyndromeCompute(ss2, set, d);
	}
	infile2.close();

        // Compute the syndrome of the set difference
	// By linearity, it is the same as the difference of the syndromes
	// of the two sets
	ss = ss1 - ss2;

	vec_GF2E setDifference;
	// Find the set difference from its syndrome
	// This is the most difficult and interesting part
	success = BCHSyndromeDecode(setDifference, ss, d);

	if (success == false) { 
		cerr << "Set difference too large, greater than " 
		        << (d-1)/2 << " elements.\n"
		        << "Unable to compute from the given sketches.\n";
		return -1;
	}

	OutputSetDifference(cout, setDifference);

	return 0;
}

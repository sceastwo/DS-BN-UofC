
#ifndef __FACTORS_H__
#define __FACTORS_H__

#include "Variables.h"
#include <iostream>
#include <fstream>

class Factor_Template;
class Tabular_Factor;
class Tabular_DS_Factor;

class Factor_Template
{
protected:
	int num_of_vars;
	string* factor_var_names;
	int* var_domain_sizes;
	// The index increment necessary to advance the array index by one step according to the variable under consideration.
	long int* var_index_increments; // The latter variables cycle the fastest. 
	long int num_of_vals; // The total number of assignments to the set of variables.

public:
	Factor_Template();
	virtual void reset();
	Factor_Template(int, string*); //Array is copied.
	Factor_Template(istream&);
	Factor_Template(Factor_Template&); //Copy Constructor
	virtual Factor_Template& operator=(Factor_Template&); //Overloaded Assignment Operator
	virtual ~Factor_Template();
	//virtual void consolidate(); //Prune excess space from the arrays

	void reorder_vars(string*); // Reorder the variables to the given order.
	void extract_vars(int&, string*&); // Copy the variable list from the factor into the provided parameters.
	bool has_var(string);
};

class Tabular_Factor : public Factor_Template
{
	float* factor_vals; // The latter variables cycle the fastest.

public:
	Tabular_Factor();
	void reset();
	Tabular_Factor(int, string*); // Creates the identity (all 1s) factor.
	Tabular_Factor(int, string*, float*);
	Tabular_Factor(istream&);
	Tabular_Factor(Tabular_Factor&); //Copy Constructor
	Tabular_Factor& operator=(Tabular_Factor&); //Overloaded Assignment Operator
	~Tabular_Factor();

	void read_vals(float*); // Copy the values from the provided array into the factor.
	void print(ostream&);
	void extract_vals(long int&, float*&); // Copy the value list from the factor into the provided parameters.
	static Tabular_Factor& multiply(int, Tabular_Factor**);
	void marginalize(int, string*);
	void condition(int, string*, int*);
	void normalize();
};

class Tabular_DS_Factor : public Factor_Template
{
public:
	typedef bool* focal_set;

private:
	int num_of_focal_elements;
	focal_set* focal_elements; //focal_elements is an array of bool arrays that denote each focal element. 1 denotes membership, and 0 does not. The latter variables cycle the fastest. 
	float* focal_element_weights;

public:
	Tabular_DS_Factor();
	void reset();
	Tabular_DS_Factor(int, string*); // Creates a vacuous DS factor.
	Tabular_DS_Factor(int, string*, int, focal_set*, float*); 
	Tabular_DS_Factor(istream&);
	Tabular_DS_Factor(Tabular_DS_Factor&); //Copy Constructor
	Tabular_DS_Factor& operator=(Tabular_DS_Factor&); //Overloaded Assignment Operator
	~Tabular_DS_Factor();

private:
	static bool is_empty(long int, focal_set);
	static bool set_equal(long int, focal_set, focal_set);

public:
	void print(ostream&);
	void extract_focal_elements(long int&, int&, focal_set*&, float*&);
	void extend(int, string*);
	static Tabular_DS_Factor& multiply(int, Tabular_DS_Factor**);
	void marginalize(int,string*);
	void condition(int,string*,int*);
	void normalize();

	void balloon_extend(int,string*,int*);
};


#endif



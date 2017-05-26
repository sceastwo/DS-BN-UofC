
#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#include <cstdlib>
#include <string>
using namespace std;

class Variable_List; // Only 1 global instance.

extern Variable_List the_vars;

typedef int index_type; 
#define NOT_FOUND -1

class Variable_List
{
	struct Variable
	{
		string the_name;
		int the_domain_size;

		Variable();	//Default constructor 
		Variable(string, int); //Initialization constructor
		Variable(Variable&); //Copy constructor
	};
	index_type the_list_size;
#define LIST_SIZE_INCREMENT 50
	Variable** the_var_array; // Pointer to an array of pointers to Variable structs.

public:
	Variable_List(); //Default constructor
	Variable_List(Variable_List&); //Copy constructor 
	Variable_List& operator=(Variable_List&); //Overloaded assignment operator 
	~Variable_List(); //Default destructor

	index_type get_var_index(string);
	string get_var_name(index_type);
	int get_domain_size(string); 
	int get_domain_size(index_type); 
	index_type add_variable(string, int);
	//void delete_variable(string);
	//void delete_variable(index_type);
};


#endif


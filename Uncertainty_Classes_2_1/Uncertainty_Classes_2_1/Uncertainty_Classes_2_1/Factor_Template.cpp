
#include "Variables.h"
#include "Factors.h"

Factor_Template::Factor_Template()
{
	num_of_vars = 0;
	factor_var_names = new string[0];
	var_domain_sizes = new int[0];
	var_index_increments = new long int[0];
	num_of_vals = 1;
}

void Factor_Template::reset()
{
	delete[] factor_var_names;
	delete[] var_domain_sizes;
	delete[] var_index_increments;

	num_of_vars = 0;
	factor_var_names = new string[0];
	var_domain_sizes = new int[0];
	var_index_increments = new long int[0];
	num_of_vals = 1;
}

Factor_Template::Factor_Template(int new_num_of_vars, string* new_factor_var_names)
{
	num_of_vars = new_num_of_vars;
	if (num_of_vars < 0)
	{
		throw "A factor cannot have a negative number of variables.";
	}
	factor_var_names = new string[num_of_vars];
	var_domain_sizes = new int[num_of_vars];
	var_index_increments = new long int[num_of_vars];
	num_of_vals = 1;
	for (int i = 0; i < num_of_vars; i++)
	{
		factor_var_names[i] = new_factor_var_names[i];
		var_domain_sizes[i] = the_vars.get_domain_size(factor_var_names[i]);
		var_index_increments[i] = 1;
		for (int j = 0; j < i; j++)
		{
			var_index_increments[j] *= var_domain_sizes[i];
		}
		num_of_vals *= var_domain_sizes[i];
	}
}

Factor_Template::Factor_Template(istream& in)
{
	// Initialization to ensure compatibility with "reset".
	factor_var_names = new string[0];
	var_domain_sizes = new int[0];
	var_index_increments = new long int[0];

	in >> num_of_vars;
	if (in.fail()) // Error checking:
	{
		reset();
		throw "Factor_Template: Variable name read error.";
	}
	if (num_of_vars < 0) // Error checking:
	{
		reset();
		throw "A factor cannot have a negative number of variables.";
	}

	// Reverse the initialization:
	delete[] factor_var_names;
	delete[] var_domain_sizes;
	delete[] var_index_increments;

	factor_var_names = new string[num_of_vars];
	var_domain_sizes = new int[num_of_vars];
	var_index_increments = new long int[num_of_vars];

	num_of_vals = 1;
	for (int i = 0; i < num_of_vars; i++)
	{
		in >> factor_var_names[i];
		if (in.fail()) // Error checking:
		{
			reset();
			throw "File read error.";
		}
		try
		{
			var_domain_sizes[i] = the_vars.get_domain_size(factor_var_names[i]);
		}
		catch (char* error)
		{
			reset();
			throw error;
		}
		var_index_increments[i] = 1;
		for (int j = 0; j < i; j++)
		{
			var_index_increments[j] *= var_domain_sizes[i];
		}
		num_of_vals *= var_domain_sizes[i];
	}
}

Factor_Template::Factor_Template(Factor_Template& existing_factor)
{
	num_of_vars = existing_factor.num_of_vars;
	factor_var_names = new string[num_of_vars];
	var_domain_sizes = new int[num_of_vars];
	var_index_increments = new long int[num_of_vars];

	num_of_vals = 1;
	for (int i = 0; i < num_of_vars; i++)
	{
		factor_var_names[i] = existing_factor.factor_var_names[i];
		var_domain_sizes[i] = the_vars.get_domain_size(factor_var_names[i]);
		var_index_increments[i] = 1;
		for (int j = 0; j < i; j++)
		{
			var_index_increments[j] *= var_domain_sizes[i];
		}
		num_of_vals *= var_domain_sizes[i];
	}
}

Factor_Template& Factor_Template::operator=(Factor_Template& existing_factor)
{
	delete[] factor_var_names;
	delete[] var_domain_sizes;
	delete[] var_index_increments;

	num_of_vars = existing_factor.num_of_vars;
	factor_var_names = new string[num_of_vars];
	var_domain_sizes = new int[num_of_vars];
	var_index_increments = new long int[num_of_vars];
	
	num_of_vals = 1;
	for (int i = 0; i < num_of_vars; i++)
	{
		factor_var_names[i] = existing_factor.factor_var_names[i];
		var_domain_sizes[i] = the_vars.get_domain_size(factor_var_names[i]);
		var_index_increments[i] = 1;
		for (int j = 0; j < i; j++)
		{
			var_index_increments[j] *= var_domain_sizes[i];
		}
		num_of_vals *= var_domain_sizes[i];
	}

	return existing_factor;
}

Factor_Template::~Factor_Template()
{
	delete[] factor_var_names;
	delete[] var_domain_sizes;
	delete[] var_index_increments;
}

void reorder_vars(string*)
{

}

void Factor_Template::extract_vars(int& int_out, string*& array_out)
{
	int_out = num_of_vars;
	array_out = new string[num_of_vars];
	for (long int i = 0; i < num_of_vars; i++)
	{
		array_out[i] = factor_var_names[i];
	}
}

bool Factor_Template::has_var(string sought_var)
{
	for (int i = 0; i < num_of_vars; i++)
	{
		if (factor_var_names[i] == sought_var)
		{
			return true;
		}
	}
	return false;
}



#include "Variables.h"
#include "Factors.h"


void Tabular_DS_Factor::balloon_extend(int num_of_new_vars, string* new_vars, int* new_var_vals)
{
	// Ensure that none of the new variables are already present in the current factor, and ensure that the provided values are within the correct range.
	for (int i = 0; i < num_of_new_vars; i++)
	{
		bool found_flag = false;
		for (int j = 0; j < num_of_vars; j++)
		{
			if (new_vars[i] == factor_var_names[j])
			{
				found_flag = true;
				break;
			}
		}
		if (found_flag)
		{
			throw "Tabular_DS_Factor::balloon_extend: variable " + new_vars[i] + " already exists.";
		}
		if ((new_var_vals[i] < 0) || (the_vars.get_domain_size(new_vars[i]) <= new_var_vals[i]))
		{
			throw "Tabular_DS_Factor::balloon_extend: variable " + new_vars[i] + " cannot attain the value " + to_string(new_var_vals[i]);
		}
	}

	int new_num_of_vars = num_of_vars + num_of_new_vars;
	string* new_factor_var_names = new string[new_num_of_vars];
	int* new_var_domain_sizes = new int[new_num_of_vars];
	long int* new_var_index_increments = new long int[new_num_of_vars];
	long int new_num_of_vals = num_of_vals;
	for (int i = 0; i < num_of_vars; i++) // Old variables
	{
		new_factor_var_names[i] = factor_var_names[i];
		new_var_domain_sizes[i] = var_domain_sizes[i];
		new_var_index_increments[i] = var_index_increments[i];
	}
	for (int i = 0; i < num_of_new_vars; i++) // New variables
	{
		new_factor_var_names[num_of_vars+i] = new_vars[i];
		new_var_domain_sizes[num_of_vars+i] = the_vars.get_domain_size(new_vars[i]);
		new_var_index_increments[num_of_vars+i] = 1;
		for (int j = 0; j < num_of_vars+i; j++)
		{
			new_var_index_increments[j] *= new_var_domain_sizes[num_of_vars+i];
		}
		new_num_of_vals *= new_var_domain_sizes[num_of_vars+i];
	}

	// Generate the new focal elements:
	focal_set* new_focal_elements = new focal_set[num_of_focal_elements];
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		new_focal_elements[i] = new bool[new_num_of_vals];
	}

	int* new_curr_var_assignment = new int[new_num_of_vars];
	for (int i = 0; i < new_num_of_vars; i++)
	{
		new_curr_var_assignment[i] = 0;
	}
	long int curr_index = 0;
	long int new_curr_index = 0;
	bool termination_flag = false;
	while (!termination_flag)
	{
		// Test if the current assignment matches the provided assignment to the new variables:
		bool match_flag = true;
		for (int i = 0; i < num_of_new_vars; i++)
		{
			if (new_curr_var_assignment[num_of_vars+i] != new_var_vals[i])
			{
				match_flag = false;
				break;
			}
		}

		for (int i = 0; i < num_of_focal_elements; i++)
		{
			if (match_flag)
			{
				new_focal_elements[i][new_curr_index] = focal_elements[i][curr_index];
			}
			else
			{
				new_focal_elements[i][new_curr_index] = true;
			}
		}

		int i = new_num_of_vars - 1;
		bool carry_flag = true;
		while (carry_flag && i >= 0)
		{
			if (new_curr_var_assignment[i] < new_var_domain_sizes[i]-1)
			{
				new_curr_var_assignment[i]++;
				if (i < num_of_vars)
				{
					curr_index += var_index_increments[i];
				}
				new_curr_index += new_var_index_increments[i];
				carry_flag = false;
			}
			else
			{
				new_curr_var_assignment[i] = 0;
				if (i < num_of_vars)
				{
					curr_index -= (new_var_domain_sizes[i]-1)*var_index_increments[i];
				}
				new_curr_index -= (new_var_domain_sizes[i]-1)*new_var_index_increments[i];
			}
			i--;
		}

		if (carry_flag)
		{
			termination_flag = true;
		}
	}

	//Update all parameters:
	delete[] factor_var_names;
	delete[] var_domain_sizes;
	delete[] var_index_increments;
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		delete[] focal_elements[i];
	}
	delete[] focal_elements;
	
	num_of_vars = new_num_of_vars;
	factor_var_names = new_factor_var_names;
	var_domain_sizes = new_var_domain_sizes;
	var_index_increments = new_var_index_increments;
	num_of_vals = new_num_of_vals;
	// The number of focal elements remains unchanged.
	focal_elements = new_focal_elements;
	// The focal element weights do not need to be updated.

	// Garbage collection:
	delete[] new_curr_var_assignment;

}











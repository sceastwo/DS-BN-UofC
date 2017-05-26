
#include "Variables.h"
#include "Factors.h"


Tabular_Factor::Tabular_Factor() : Factor_Template()
{
	factor_vals = new float[1];
	factor_vals[0] = 1.0;
}

void Tabular_Factor::reset()
{
	delete[] factor_vals;

	factor_vals = new float[1];
	factor_vals[0] = 1.0;

	Factor_Template::reset();
}

Tabular_Factor::Tabular_Factor(int new_num_of_vars, string* new_factor_var_names) : Factor_Template(new_num_of_vars, new_factor_var_names)
{
	factor_vals = new float[num_of_vals];
	for (long int i = 0; i < num_of_vals; i++)
	{
		factor_vals[i] = 1.0;
	}
}

Tabular_Factor::Tabular_Factor(int new_num_of_vars, string* new_factor_var_names, float* new_vals) : Factor_Template(new_num_of_vars, new_factor_var_names)
{
	factor_vals = new float[num_of_vals];
	for (long int i = 0; i < num_of_vals; i++)
	{
		factor_vals[i] = new_vals[i];
	}
}

Tabular_Factor::Tabular_Factor(istream& in) : Factor_Template(in)
{
	factor_vals = new float[num_of_vals];
	for (long int i = 0; i < num_of_vals; i++)
	{
		in >> factor_vals[i];
		if (in.fail()) // Error checking:
		{
			reset();
			throw "Tabular_Factor: Value read error.";
		}
	}
}

Tabular_Factor::Tabular_Factor(Tabular_Factor& existing_factor) : Factor_Template(existing_factor)
{
	factor_vals = new float[num_of_vals];
	for (long int i = 0; i < num_of_vals; i++)
	{
		factor_vals[i] = existing_factor.factor_vals[i];
	}
}

Tabular_Factor& Tabular_Factor::operator=(Tabular_Factor& existing_factor)
{
	Factor_Template::operator=(existing_factor);

	delete[] factor_vals;
	factor_vals = new float[num_of_vals];
	for (long int i = 0; i < num_of_vals; i++)
	{
		factor_vals[i] = existing_factor.factor_vals[i];
	}
	return existing_factor;
}

Tabular_Factor::~Tabular_Factor()
{
	delete[] factor_vals;
}

void Tabular_Factor::read_vals(float* new_vals)
{
	delete[] factor_vals;

	for (int i = 0; i < num_of_vals; i++)
	{
		factor_vals[i] = new_vals[i];
	}
}

void Tabular_Factor::print(ostream& out)
{
	out << num_of_vars << '\n';
	for (int i = 0; i < num_of_vars; i++)
	{
		out << factor_var_names[i] << ' ';
	}
	out << '\n';
	for (long int i = 0; i < num_of_vals; i++)
	{
		out << factor_vals[i] << ' ';
	}
	out << "\n\n";
}


void Tabular_Factor::extract_vals(long int& long_int_out, float*& array_out)
{
	long_int_out = num_of_vals;
	array_out = new float[num_of_vals];
	for (long int i = 0; i < num_of_vals; i++)
	{
		array_out[i] = factor_vals[i];
	}
}


Tabular_Factor& Tabular_Factor::multiply(int num_of_factors, Tabular_Factor** factor_array)
{
	/*if (num_of_factors < 1) // Check for an empty product.
	{
		return *(new Tabular_Factor());
	}*/

	int prod_num_of_variables = 0;
	int max_num_of_variables = 0;
	for (int i = 0; i < num_of_factors; i++)
	{
		max_num_of_variables += factor_array[i]->num_of_vars;
	}
	string* prod_variables = new string[max_num_of_variables];
	for (int i = 0; i < num_of_factors; i++)
	{
		for (int j = 0; j < factor_array[i]->num_of_vars; j++)
		{
			bool included_flag = false;
			for (int k = 0; k < prod_num_of_variables; k++)
			{
				if (factor_array[i]->factor_var_names[j] == prod_variables[k])
				{
					included_flag = true;
					break;
				}
			}
			if (!included_flag)
			{
				prod_variables[prod_num_of_variables] = factor_array[i]->factor_var_names[j];
				prod_num_of_variables++;
			}
		}
	}

	Tabular_Factor* factor_product = new Tabular_Factor(prod_num_of_variables, prod_variables); // Create new factor.
	
	/*if (factor_product->num_of_vars == 0) // Special case where the product factor has no variables (all the multiplicands are scalars). 
	{
		// Form the product of all scalars.
		factor_product->factor_vals[0] = 1.0;
		for (int i = 0; i < num_of_factors; i++)
		{
			factor_product->factor_vals[0] *= factor_array[i]->factor_vals[0];
		}
		return *factor_product;
	}*/

	//Fill the new factor with the correct entries:
	//	Generate the correct increments:
	long int** factor_index_increments = new long int*[num_of_factors]; 
	for (int i = 0; i < num_of_factors; i++) // For each multiplicand factor...
	{
		factor_index_increments[i] = new long int[factor_product->num_of_vars];
		for (int j = 0; j < factor_product->num_of_vars; j++) // For each variable in the product factor...
		{
			factor_index_increments[i][j] = 0; // Find the index increment for the ith factor that corresponds to the jth variable in the product factor.
			bool found_flag = false;
			for (int k = 0; k < factor_array[i]->num_of_vars; k++)
			{
				if (factor_array[i]->factor_var_names[k] == factor_product->factor_var_names[j])
				{
					found_flag = true;
					factor_index_increments[i][j] = factor_array[i]->var_index_increments[k];
					break;
				}
			}
		}
	}

	//	Sweep the array:

	int* curr_var_assignment = new int[factor_product->num_of_vars]; //Assignment to variables in the product factor.
	for (int i = 0; i < factor_product->num_of_vars; i++)
	{
		curr_var_assignment[i] = 0;
	}

	long int curr_prod_index = 0; //Current index in the product factor.
	long int* curr_indices = new long int[num_of_factors]; //Index corresponding to each multiplicand factor.
	for (int i = 0; i < num_of_factors; i++)
	{
		curr_indices[i] = 0;
	}

	bool termination_flag = false;
	while (!termination_flag)
	{
		//Compute the current product factor value:
		factor_product->factor_vals[curr_prod_index] = 1.0;
		for (int i = 0; i < num_of_factors; i++)
		{
			factor_product->factor_vals[curr_prod_index] *= factor_array[i]->factor_vals[curr_indices[i]];
		}
		//Move to the next array entry:
		bool carry_flag = true;
		int i = factor_product->num_of_vars - 1;
		while (carry_flag && i >= 0)
		{
			if (curr_var_assignment[i] < factor_product->var_domain_sizes[i] - 1)
			{
				curr_var_assignment[i]++;
				curr_prod_index += factor_product->var_index_increments[i];
				for (int j = 0; j < num_of_factors; j++)
				{
					curr_indices[j] += factor_index_increments[j][i];
				}
				carry_flag = false;
			}
			else
			{
				curr_var_assignment[i] = 0;
				curr_prod_index -= (factor_product->var_domain_sizes[i]-1)*factor_product->var_index_increments[i];
				for (int j = 0; j < num_of_factors; j++)
				{
					curr_indices[j] -= (factor_product->var_domain_sizes[i]-1)*factor_index_increments[j][i];
				}
			}
			i--;
		}
		if (carry_flag)
		{
			termination_flag = true;
		}
	}

	// Garbage collection:
	delete[] prod_variables;
	for (int i = 0; i < num_of_factors; i++)
	{
		delete[] factor_index_increments[i];
	}
	delete[] factor_index_increments;
	delete[] curr_var_assignment;
	delete[] curr_indices;

	return *factor_product;
}


void Tabular_Factor::marginalize(int num_of_elim_vars, string* elim_var_names)
{
	if (num_of_elim_vars <= 0)
	{
		return;
	}

	//Ensure that all elimination variables are present in the current factor:
	for (int i = 0; i < num_of_elim_vars; i++)
	{
		bool found_flag = false;
		for (int j = 0; j < num_of_vars; j++)
		{
			if (factor_var_names[j] == elim_var_names[i])
			{
				found_flag = true;
				break;
			}
		}
		if (!found_flag)
		{
			throw "Tabular_Factor::marginalize: variable " + elim_var_names[i] + " does not exist.";
		}
	}

	//Generate the new set of parameters:
	int new_num_of_vars = num_of_vars - num_of_elim_vars;
	string* new_factor_var_names = new string[new_num_of_vars];
	int* new_var_domain_sizes = new int[new_num_of_vars];
	long int* new_var_index_increments = new long int[new_num_of_vars];
	long int new_num_of_vals = 1;

	bool* inclusion_array = new bool[num_of_vars];
	int* old_to_new_index_map = new int[num_of_vars];
	int var_counter = 0; // Variables added so far.
	for (int i = 0; i < num_of_vars; i++)
	{
		bool found_flag = false;
		for (int j = 0; j < num_of_elim_vars; j++)
		{
			if (factor_var_names[i] == elim_var_names[j])
			{
				found_flag = true;

				inclusion_array[i] = false;

				break;
			}
		}
		if (found_flag)
		{
			continue;
		}

		new_factor_var_names[var_counter] = factor_var_names[i];
		new_var_domain_sizes[var_counter] = var_domain_sizes[i];
		new_var_index_increments[var_counter] = 1;
		for (int j = 0; j < var_counter; j++)
		{
			new_var_index_increments[j] *= new_var_domain_sizes[var_counter];
		}
		new_num_of_vals *= new_var_domain_sizes[var_counter];

		inclusion_array[i] = true;
		old_to_new_index_map[i] = var_counter;
		var_counter++;
	}

	// Initialize the new values array:
	float* new_factor_vals = new float[new_num_of_vals];
	for (long int i = 0; i < new_num_of_vals; i++)
	{
		new_factor_vals[i] = 0.0;
	}

	//Fill the factor with the correct entries:
	//	Sweep the array:
	int* curr_var_assignment = new int[num_of_vars]; //Current assignment to all existing variables.
	for (int i = 0; i < num_of_vars; i++)
	{
		curr_var_assignment[i] = 0;
	}
	long int curr_index = 0;
	long int new_curr_index = 0;
	bool termination_flag = false;
	while (!termination_flag)
	{
		// Add to the value in the new array:
		new_factor_vals[new_curr_index] += factor_vals[curr_index];
		// Move to the next entry:
		bool carry_flag = true;
		int i = num_of_vars - 1;
		while (carry_flag && i >= 0)
		{
			if (curr_var_assignment[i] < var_domain_sizes[i] - 1)
			{
				curr_var_assignment[i]++;
				curr_index += var_index_increments[i];
				if (inclusion_array[i])
				{
					new_curr_index += new_var_index_increments[old_to_new_index_map[i]];
				}
				carry_flag = false;
			}
			else
			{
				curr_var_assignment[i] = 0;
				curr_index -= (var_domain_sizes[i]-1)*var_index_increments[i];
				if (inclusion_array[i])
				{
					new_curr_index -= (new_var_domain_sizes[old_to_new_index_map[i]]-1)*new_var_index_increments[old_to_new_index_map[i]];
				}
			}
			i--;
		}
		if (carry_flag)
		{
			termination_flag = true;
		}
	}

	// Replace all existing parameters:
	delete[] factor_var_names;
	delete[] var_domain_sizes;
	delete[] var_index_increments;
	delete[] factor_vals;

	num_of_vars = new_num_of_vars;
	factor_var_names = new_factor_var_names;
	var_domain_sizes = new_var_domain_sizes;
	var_index_increments = new_var_index_increments;
	num_of_vals = new_num_of_vals;
	factor_vals = new_factor_vals;

	// Garbage collection:
	delete[] inclusion_array;
	delete[] old_to_new_index_map;
	delete[] curr_var_assignment;
	
}


void Tabular_Factor::condition(int num_of_cond_vars, string* cond_var_names, int* cond_vals)
{
	if (num_of_cond_vars <= 0)
	{
		return;
	}

	// Check that the values assigned to each conditioned variable are within the appropriate range:
	for (int i = 0; i < num_of_cond_vars; i++)
	{
		if ((cond_vals[i] < 0) || (the_vars.get_domain_size(cond_var_names[i]) <= cond_vals[i]))
		{
			throw "Tabular_Factor::condition: the value assigned to " + cond_var_names[i] + " is invalid.";
		}
	}
	
	// Find the remaining variables:
	int bucket_size = 0;
	string* new_name_bucket = new string[num_of_vars];
	bool* inclusion_array = new bool[num_of_vars]; // Does the variable remain after conditioning?
	int* old_to_new_index_map = new int[num_of_vars];
	int* old_to_cond_index_map = new int[num_of_vars];
	for (int i = 0; i < num_of_vars; i++)
	{
		bool found_flag = false;
		for (int j = 0; j < num_of_cond_vars; j++)
		{
			if (factor_var_names[i] == cond_var_names[j])
			{
				found_flag = true;

				inclusion_array[i] = false;
				old_to_cond_index_map[i] = j;

				break;
			}
		}
		if (!found_flag)
		{
			inclusion_array[i] = true;
			old_to_new_index_map[i] = bucket_size;

			new_name_bucket[bucket_size] = factor_var_names[i];
			bucket_size++;
		}
	}

	// Generate the new set of parameters:
	int new_num_of_vars = bucket_size;
	string* new_factor_var_names = new string[bucket_size];
	int* new_var_domain_sizes = new int[bucket_size];
	long int* new_var_index_increments = new long int[bucket_size];
	long int new_num_of_vals = 1;
	for (int i = 0; i < bucket_size; i++)
	{
		new_factor_var_names[i] = new_name_bucket[i];
		new_var_domain_sizes[i] = the_vars.get_domain_size(new_name_bucket[i]);
		new_var_index_increments[i] = 1;
		for (int j = 0; j < i; j++)
		{
			new_var_index_increments[j] *= new_var_domain_sizes[i];
		}
		new_num_of_vals *= new_var_domain_sizes[i];
	}
	float* new_factor_vals = new float[new_num_of_vals];


	//Fill the factor with the correct entries:
	//  Initialize the assignment and index counters:
	int* curr_var_assignment = new int[num_of_vars]; //Current assignment to the variables.
	for (int i = 0; i < num_of_vars; i++)
	{
		if (inclusion_array[i])
		{
			curr_var_assignment[i] = 0;
		}
		else
		{
			curr_var_assignment[i] = cond_vals[old_to_cond_index_map[i]];
		}
	}
	long int curr_index = 0; // Index in the existing array.
	for (int i = 0; i < num_of_vars; i++)
	{
		if (!inclusion_array[i])
		{
			curr_index += cond_vals[old_to_cond_index_map[i]]*var_index_increments[i];
		}
	}
	long int new_curr_index = 0; // Index in the new array.

	//	Sweep the relevant parts of the array:
	bool termination_flag = false;
	while (!termination_flag)
	{
		new_factor_vals[new_curr_index] = factor_vals[curr_index];
		// Move to the next entry:
		bool carry_flag = true;
		int i = num_of_vars - 1;
		while (carry_flag && i >= 0)
		{
			if (!inclusion_array[i]) // Skip conditioned variables.
			{
				i--;
				continue;
			}
			if (curr_var_assignment[i] < var_domain_sizes[i] - 1)
			{
				curr_var_assignment[i]++;
				curr_index += var_index_increments[i];
				new_curr_index += new_var_index_increments[old_to_new_index_map[i]];
				carry_flag = false;
			}
			else
			{
				curr_var_assignment[i] = 0;
				curr_index -= (var_domain_sizes[i]-1)*var_index_increments[i];
				new_curr_index -= (new_var_domain_sizes[old_to_new_index_map[i]]-1)*new_var_index_increments[old_to_new_index_map[i]];
			}
			i--;
		}
		if (carry_flag)
		{
			termination_flag = true;
		}
	}


	// Replace all existing parameters:
	delete[] factor_var_names;
	delete[] var_domain_sizes;
	delete[] var_index_increments;
	delete[] factor_vals;

	num_of_vars = new_num_of_vars;
	factor_var_names = new_factor_var_names;
	var_domain_sizes = new_var_domain_sizes;
	var_index_increments = new_var_index_increments;
	num_of_vals = new_num_of_vals;
	factor_vals = new_factor_vals;

	// Garbage collection:
	delete[] new_name_bucket;
	delete[] inclusion_array;
	delete[] old_to_new_index_map;
	delete[] old_to_cond_index_map;
	delete[] curr_var_assignment;
	
}


void Tabular_Factor::normalize()
{
	float total = 0.0;
	for (long int i = 0; i < num_of_vals; i++)
	{
		total += factor_vals[i];
	}
	for (long int i = 0; i < num_of_vals; i++)
	{
		factor_vals[i] /= total;
	}
}

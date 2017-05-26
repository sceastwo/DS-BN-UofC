
#include "Variables.h"
#include "Factors.h"

Tabular_DS_Factor::Tabular_DS_Factor() : Factor_Template()
{
	num_of_focal_elements = 1;
	focal_elements = new focal_set[1];
	focal_elements[0] = new bool[1];
	focal_elements[0][0] = true;
	focal_element_weights = new float[1];
	focal_element_weights[0] = 1.0;
}

void Tabular_DS_Factor::reset()
{
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		delete[] focal_elements[i];
	}
	delete[] focal_elements;
	delete[] focal_element_weights;

	Factor_Template::reset();
	num_of_focal_elements = 1;
	focal_elements = new focal_set[1];
	focal_elements[0] = new bool[1];
	focal_elements[0][0] = true;
	focal_element_weights = new float[1];
	focal_element_weights[0] = 1.0;
}

Tabular_DS_Factor::Tabular_DS_Factor(int new_num_of_vars, string* new_factor_var_names) : Factor_Template(new_num_of_vars, new_factor_var_names)
{
	num_of_focal_elements = 1;
	focal_elements = new focal_set[1];
	focal_elements[0] = new bool[num_of_vals];
	for (long int i = 0; i < num_of_vals; i++)
	{
		focal_elements[0][i] = true;
	}
	focal_element_weights = new float[1];	
	focal_element_weights[0] = 1.0;
}


Tabular_DS_Factor::Tabular_DS_Factor(int new_num_of_vars, string* new_factor_var_names, 
									 int new_num_of_focal_elements, focal_set* new_focal_elements, float* new_focal_element_weights) : Factor_Template(new_num_of_vars, new_factor_var_names)
{
	num_of_focal_elements = new_num_of_focal_elements;
	if (num_of_focal_elements < 0)
	{
		throw "There cannot be a negative number of focal elements.";
	}
	focal_elements = new focal_set[num_of_focal_elements];
	focal_element_weights = new float[num_of_focal_elements];
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		focal_elements[i] = new bool[num_of_vals];
		for (long int j = 0; j < num_of_vals; j++)
		{
			focal_elements[i][j] = new_focal_elements[i][j];
		}
		if (is_empty(num_of_vals, focal_elements[i]))
		{
			cerr << "warning: focal elements cannot be empty.";
		}
		focal_element_weights[i] = new_focal_element_weights[i];
	}
}


Tabular_DS_Factor::Tabular_DS_Factor(istream& in) : Factor_Template(in)
{
	// Initialization to ensure compatibility with "reset".
	num_of_focal_elements = 0;
	focal_elements = new focal_set[0];
	focal_element_weights = new float[0];

	in >> num_of_focal_elements;
	if (in.fail()) // Error checking:
	{
		reset();
		throw "File read error.";
	}
	if (num_of_focal_elements < 0) // Error checking:
	{
		reset();
		throw "There cannot be a negative number of focal elements.";
	}
	delete[] focal_elements;
	delete[] focal_element_weights;
	focal_elements = new focal_set[num_of_focal_elements];
	focal_element_weights = new float[num_of_focal_elements];
	for (int i = 0; i < num_of_focal_elements; i++) // Initialization to ensure compatibility with reset.
	{
		focal_elements[i] = new bool[0]; 
	}
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		in >> focal_element_weights[i];
		if (in.fail()) // Error checking:
		{
			reset();
			throw "Tabular_DS_Factor: Focal element weight read error.";
		}
		delete[] focal_elements[i];
		focal_elements[i] = new bool[num_of_vals];
		for (long int j = 0; j < num_of_vals; j++)
		{
			in >> focal_elements[i][j];
			if (in.fail()) // Error checking:
			{
				reset();
				throw "Tabular_DS_Factor: Focal element inclusion flag read error.";
			}
		}
		if (is_empty(num_of_vals, focal_elements[i]))
		{
			cerr << "warning: focal elements cannot be empty.";
		}
	}
}


Tabular_DS_Factor::Tabular_DS_Factor(Tabular_DS_Factor& existing_factor) : Factor_Template(existing_factor)
{
	num_of_focal_elements = existing_factor.num_of_focal_elements;
	focal_elements = new focal_set[num_of_focal_elements];
	focal_element_weights = new float[num_of_focal_elements];
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		focal_elements[i] = new bool[num_of_vals];
		for (long int j = 0; j < num_of_vals; j++)
		{
			focal_elements[i][j] = existing_factor.focal_elements[i][j];
		}
		focal_element_weights[i] = existing_factor.focal_element_weights[i];
	}
}


Tabular_DS_Factor& Tabular_DS_Factor::operator=(Tabular_DS_Factor& existing_factor)
{
	Factor_Template::operator=(existing_factor);

	for (int i = 0; i < num_of_focal_elements; i++)
	{
		delete[] focal_elements[i];
	}
	delete[] focal_elements;
	delete[] focal_element_weights;
	num_of_focal_elements = existing_factor.num_of_focal_elements;
	focal_elements = new focal_set[num_of_focal_elements];
	focal_element_weights = new float[num_of_focal_elements];
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		focal_elements[i] = new bool[num_of_vals];
		for (long int j = 0; j < num_of_vals; j++)
		{
			focal_elements[i][j] = existing_factor.focal_elements[i][j];
		}
		focal_element_weights[i] = existing_factor.focal_element_weights[i];
	}

	return existing_factor;
}


Tabular_DS_Factor::~Tabular_DS_Factor()
{
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		delete[] focal_elements[i];
	}
	delete[] focal_elements;
	delete[] focal_element_weights;
}


bool Tabular_DS_Factor::is_empty(long int domain_size, Tabular_DS_Factor::focal_set set)
{
	for(long int i = 0; i < domain_size; i++)
	{
		if (set[i])
		{
			return false;
		}
	}
	return true;
}


bool Tabular_DS_Factor::set_equal(long int domain_size, Tabular_DS_Factor::focal_set set1, Tabular_DS_Factor::focal_set set2)
{
	for (long int i = 0; i < domain_size; i++)
	{
		if (set1[i] != set2[i])
		{
			return false;
		}
	}
	return true;
}


void Tabular_DS_Factor::print(ostream& out)
{
	out << num_of_vars << '\n';
	for (int i = 0; i < num_of_vars; i++)
	{
		out << factor_var_names[i] << ' ';
	}
	out << '\n' << num_of_focal_elements << '\n';
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		out << focal_element_weights[i] << ' ';
		for (int j = 0; j < num_of_vals; j++)
		{
			out << focal_elements[i][j] << ' ';
		}
		out << '\n';
	}
	out << "\n\n";
}


void Tabular_DS_Factor::extract_focal_elements(long int& domain_size_out, int& num_of_focal_elements_out, 
											   focal_set*& focal_elements_out, float*& focal_element_weights_out)
{
	domain_size_out = num_of_vals;
	num_of_focal_elements_out = num_of_focal_elements;
	focal_elements_out = new focal_set[num_of_focal_elements];
	focal_element_weights_out = new float[num_of_focal_elements];
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		focal_elements_out[i] = new bool[num_of_vals];
		for (long int j = 0; j < num_of_vals; j++)
		{
			focal_elements_out[i][j] = focal_elements[i][j];
		}
		focal_element_weights_out[i] = focal_element_weights[i];
	}
}


void Tabular_DS_Factor::extend(int num_of_new_vars, string* new_vars)
{
	if (num_of_new_vars <= 0)
	{
		return;
	}

	// Compute the unique new variables:
	int bucket_size = 0;
	string* new_name_bucket = new string[num_of_new_vars];
	for (int i = 0; i < num_of_new_vars; i++)
	{
		bool found_flag = false;
		for (int j = 0; j < num_of_vars; j++) 
		{
			if (new_vars[i] == factor_var_names[j]) // A new variable cannot be already included in the factor.
			{
				found_flag = true;
				break;
			}
		}
		if (!found_flag)
		{
			for (int j = 0; j < bucket_size; j++)
			{
				if (new_vars[i] == new_name_bucket[j]) // A new variable cannot be added twice by the input list.
				{
					found_flag = true;
					break;
				}
			}
			if (!found_flag)
			{
				new_name_bucket[bucket_size] = new_vars[i];
				bucket_size++;
			}
		}
	}
	// Generate the new set of parameters:
	int new_num_of_vars = num_of_vars + bucket_size;
	string* new_factor_var_names = new string[new_num_of_vars];
	int* new_var_domain_sizes = new int[new_num_of_vars];
	long int* new_var_index_increments = new long int[new_num_of_vars];
	long int new_num_of_vals = num_of_vals;
	for (int i = 0; i < num_of_vars; i++) // Old varaibles
	{
		new_factor_var_names[i] = factor_var_names[i];
		new_var_domain_sizes[i] = var_domain_sizes[i];
		new_var_index_increments[i] = var_index_increments[i];
	}
	for (int i = 0; i < bucket_size; i++) // New variables
	{
		new_factor_var_names[num_of_vars+i] = new_name_bucket[i];
		new_var_domain_sizes[num_of_vars+i] = the_vars.get_domain_size(new_name_bucket[i]);
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
		for (int i = 0; i < num_of_focal_elements; i++)
		{
			new_focal_elements[i][new_curr_index] = focal_elements[i][curr_index];
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
	delete[] new_name_bucket; 
	delete[] new_curr_var_assignment;
}


Tabular_DS_Factor& Tabular_DS_Factor::multiply(int num_of_factors, Tabular_DS_Factor** factor_array)
{
	/*if (num_of_factors < 1) // Check for an empty product.
	{
		return *(new Tabular_DS_Factor());
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

	Tabular_DS_Factor* factor_product = new Tabular_DS_Factor(prod_num_of_variables, prod_variables);
	

	/*if (factor_product->num_of_vars == 0) // Special case where the product factor has no variables (all the multiplicands are scalars). 
	{
		// Form the product of all scalars.
		factor_product->focal_element_weights[0] = 1.0;
		for (int i = 0; i < num_of_factors; i++)
		{
			factor_product->focal_element_weights[0] *= factor_array[i]->focal_element_weights[0];
		}
		return *factor_product;
	}*/

	//Generate the correct corresponding increments for the multiplicand factors.
	long int** factor_index_increments = new long int*[num_of_factors];
	for (int i = 0; i < num_of_factors; i++)
	{
		factor_index_increments[i] = new long int[factor_product->num_of_vars];
		for (int j = 0; j < factor_product->num_of_vars; j++)
		{
			factor_index_increments[i][j] = 0;
			for (int k = 0; k < factor_array[i]->num_of_vars; k++)
			{
				if (factor_array[i]->factor_var_names[k] == factor_product->factor_var_names[j])
				{
					factor_index_increments[i][j] = factor_array[i]->var_index_increments[k];
					break;
				}
			}
		}
	}

	// Generate the focal elements:
	int max_num_of_focal_elements = 1;
	for (int i = 0; i < num_of_factors; i++)
	{
		max_num_of_focal_elements *= factor_array[i]->num_of_focal_elements;
	}
	if (max_num_of_focal_elements == 0) // Special case where one of the factors has no focal elements: (the 0 factor)
	{
		delete[] factor_product->focal_elements[0];
		delete[] factor_product->focal_elements;
		delete[] factor_product->focal_element_weights;

		factor_product->num_of_focal_elements = 0;
		factor_product->focal_elements = new focal_set[0];
		factor_product->focal_element_weights = new float[0];

		return *factor_product;
	}

	int bucket_size = 0; // Collect the focal elements and their weights.
	focal_set* prod_focal_elements_bucket = new focal_set[max_num_of_focal_elements];
	float* prod_focal_element_weights_bucket = new float[max_num_of_focal_elements];

	int* focal_element_indices = new int[num_of_factors];
	for (int i = 0; i < num_of_factors; i++)
	{
		focal_element_indices[i] = 0;
	}
	focal_set intersect_set = new bool[factor_product->num_of_vals];
	int* curr_var_assignment = new int[factor_product->num_of_vars];
	long int curr_prod_index; // Array index in the intersection set.
	long int* curr_indices = new long int[num_of_factors]; // Array index in each focal element.
	bool termination_flag = false;
	while (!termination_flag)
	{
		for (int i = 0; i < factor_product->num_of_vars; i++)
		{
			curr_var_assignment[i] = 0;
		}

		curr_prod_index = 0;
		for (int i = 0; i < num_of_factors; i++)
		{
			curr_indices[i] = 0;
		}

		bool termination_flag_2 = false;
		while (!termination_flag_2)
		{
			intersect_set[curr_prod_index] = true;
			for (int i = 0; i < num_of_factors; i++)
			{
				if (!factor_array[i]->focal_elements[focal_element_indices[i]][curr_indices[i]])
				{
					intersect_set[curr_prod_index] = false;
					break;
				}
			}

			//Move to the next assignment:
			bool carry_flag_2 = true;
			int i = factor_product->num_of_vars - 1;
			while (carry_flag_2 && i >= 0)
			{
				if (curr_var_assignment[i] < factor_product->var_domain_sizes[i] - 1)
				{
					curr_var_assignment[i]++;
					curr_prod_index += factor_product->var_index_increments[i];
					for (int j = 0; j < num_of_factors; j++)
					{
						curr_indices[j] += factor_index_increments[j][i];
					}
					carry_flag_2 = false;
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
			if (carry_flag_2)
			{
				termination_flag_2 = true;
			}
		}

		float product_of_weights = 1.0;
		for (int i = 0; i < num_of_factors; i++)
		{
			product_of_weights *= factor_array[i]->focal_element_weights[focal_element_indices[i]];
		}
		if (!is_empty(factor_product->num_of_vals, intersect_set)) // Only consider an intersection if it is nonempty.
		{
			//Search for the set among the existing focal elements
			bool found_flag = false;
			int found_index = 0;
			for (int i = 0; i < bucket_size; i++)
			{
				if (set_equal(factor_product->num_of_vals, intersect_set, prod_focal_elements_bucket[i]))
				{
					found_flag = true;
					found_index = i;
					break;
				}
			}
			if (!found_flag)
			{
				// The focal element does not already exist.
				prod_focal_elements_bucket[bucket_size] = new bool[factor_product->num_of_vals];
				for (long int i = 0; i < factor_product->num_of_vals; i++) // Copy "intersect_set"
				{
					prod_focal_elements_bucket[bucket_size][i] = intersect_set[i];
				}
				prod_focal_element_weights_bucket[bucket_size] = product_of_weights;
				bucket_size++;
			}
			else
			{
				// The focal element already exists.
				prod_focal_element_weights_bucket[found_index] += product_of_weights;
			}
		}

		// Consider the next choice of focal elements.
		bool carry_flag = true;
		int i = num_of_factors - 1;
		while (carry_flag && i >= 0)
		{
			if (focal_element_indices[i] < factor_array[i]->num_of_focal_elements - 1)
			{
				focal_element_indices[i]++;
				carry_flag = false;
			}
			else
			{
				focal_element_indices[i] = 0;
			}
			i--;
		}
		if (carry_flag)
		{
			termination_flag = true;
		}
	}

	// Insert the new data into the factor_product:
	delete[] factor_product->focal_elements[0];
	delete[] factor_product->focal_elements;
	delete[] factor_product->focal_element_weights;

	factor_product->num_of_focal_elements = bucket_size;
	factor_product->focal_elements = new focal_set[bucket_size];
	factor_product->focal_element_weights = new float[bucket_size];
	for (int i = 0; i < bucket_size; i++)
	{
		factor_product->focal_elements[i] = prod_focal_elements_bucket[i]; // The array for each focal element is not oversized and does not need to be copied and deleted.
		factor_product->focal_element_weights[i] = prod_focal_element_weights_bucket[i];
	}

	// Garbage collection:
	delete[] prod_variables;

	for (int i = 0; i < num_of_factors; i++)
	{
		delete[] factor_index_increments[i];
	}
	delete[] factor_index_increments;

	delete[] focal_element_indices;
	delete[] intersect_set;
	delete[] curr_var_assignment;
	delete[] curr_indices;

	delete[] prod_focal_elements_bucket;
	delete[] prod_focal_element_weights_bucket;

	return *factor_product;
}


void Tabular_DS_Factor::marginalize(int num_of_elim_vars, string* elim_var_names)
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
			throw "Tabular_DS_Factor::marginalize: variable " + elim_var_names[i] + " does not exist.";
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

	//Generate the new focal elements:
	int bucket_size = 0;
	focal_set* new_focal_elements_bucket = new focal_set[num_of_focal_elements];
	float* new_focal_element_weights_bucket = new float[num_of_focal_elements];
	focal_set projected_set = new bool[new_num_of_vals];
	int* curr_var_assignment = new int[num_of_vars]; // The assignment to all existing variables.
	int curr_index; // Index that uses the existing variables
	int new_curr_index; // Index that uses the remaining variables
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		for (int j = 0; j < new_num_of_vals; j++) // Ready the projected set.
		{
			projected_set[j] = false;
		}
		
		for (int j = 0; j < num_of_vars; j++)
		{
			curr_var_assignment[j] = 0;
		}
		curr_index = 0;
		new_curr_index = 0;
		bool termination_flag = false;
		while (!termination_flag)
		{
			if (focal_elements[i][curr_index])
			{
				projected_set[new_curr_index] = true;
			}

			// Move to the next assignment:
			bool carry_flag = true;
			int j = num_of_vars - 1;
			while (carry_flag && j >= 0)
			{
				if (curr_var_assignment[j] < var_domain_sizes[j] - 1)
				{
					curr_var_assignment[j]++;
					curr_index += var_index_increments[j];
					if (inclusion_array[j])
					{
						new_curr_index += new_var_index_increments[old_to_new_index_map[j]];
					}
					carry_flag = false;
				}
				else
				{
					curr_var_assignment[j] = 0;
					curr_index -= (var_domain_sizes[j]-1)*var_index_increments[j];
					if (inclusion_array[j])
					{
						new_curr_index -= (var_domain_sizes[j]-1)*new_var_index_increments[old_to_new_index_map[j]];
					}
				}
				j--;
			}
			if (carry_flag)
			{
				termination_flag = true;
			}
		}

		//Search for the set among the existing focal elements
		bool found_flag = false;
		int found_index;
		for (int j = 0; j < bucket_size; j++)
		{
			if (set_equal(new_num_of_vals, projected_set, new_focal_elements_bucket[j]))
			{
				found_flag = true;
				found_index = j;
				break;
			}
		}
		if (!found_flag)
		{
			// The focal element does not already exist.
			new_focal_elements_bucket[bucket_size] = new bool[new_num_of_vals];
			for (long int j = 0; j < new_num_of_vals; j++) // Copy projected_set
			{
				new_focal_elements_bucket[bucket_size][j] = projected_set[j];
			}
			new_focal_element_weights_bucket[bucket_size] = focal_element_weights[i];
			bucket_size++;
		}
		else
		{
			// The focal element already exists.
			new_focal_element_weights_bucket[found_index] += focal_element_weights[i];
		}
	}

	// Replace all existing parameters:
	delete[] factor_var_names;
	delete[] var_domain_sizes;
	delete[] var_index_increments;
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		delete[] focal_elements[i];
	}
	delete[] focal_elements;
	delete[] focal_element_weights;	

	num_of_vars = new_num_of_vars;
	factor_var_names = new_factor_var_names;
	var_domain_sizes = new_var_domain_sizes;
	var_index_increments = new_var_index_increments;
	num_of_vals = new_num_of_vals;
	num_of_focal_elements = bucket_size;
	focal_elements = new focal_set[bucket_size];
	focal_element_weights = new float[bucket_size];
	for (int i = 0; i < bucket_size; i++)
	{
		focal_elements[i] = new_focal_elements_bucket[i];
		focal_element_weights[i] = new_focal_element_weights_bucket[i];
	}

	// Garbage collection:	
	delete[] inclusion_array;
	delete[] old_to_new_index_map;
	delete[] curr_var_assignment;
	delete[] new_focal_elements_bucket;
	delete[] new_focal_element_weights_bucket;
}


void Tabular_DS_Factor::condition(int num_of_cond_vars, string* cond_var_names, int* cond_vals)
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
	
	//Generate the new focal elements:
	bucket_size = 0;
	focal_set* new_focal_elements_bucket = new focal_set[num_of_focal_elements];
	float* new_focal_element_weights_bucket = new float[num_of_focal_elements];
	focal_set slice_set = new bool[new_num_of_vals];
	int* curr_var_assignment = new int[num_of_vars]; // The assignment to all existing variables.
	int curr_index; // Index that uses the existing variables
	int new_curr_index; // Index that uses the remaining variables
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		// Condition the initial assignment:
		for (int j = 0; j < num_of_vars; j++)
		{
			if (inclusion_array[j])
			{
				curr_var_assignment[j] = 0;
			}
			else
			{
				curr_var_assignment[j] = cond_vals[old_to_cond_index_map[j]];
			}
		}
		curr_index = 0;
		for (int j = 0; j < num_of_vars; j++)
		{
			if (!inclusion_array[j])
			{
				curr_index += cond_vals[old_to_cond_index_map[j]]*var_index_increments[j];
			}
		}
		new_curr_index = 0;

		bool termination_flag = false;
		while (!termination_flag)
		{
			slice_set[new_curr_index] = focal_elements[i][curr_index];

			// Move to the next entry:
			bool carry_flag = true;
			int j = num_of_vars - 1;
			while (carry_flag && j >= 0)
			{
				if (!inclusion_array[j]) // Skip conditioned variables.
				{
					j--;
					continue;
				}
				if (curr_var_assignment[j] < var_domain_sizes[j] - 1)
				{
					curr_var_assignment[j]++;
					curr_index += var_index_increments[j];
					if (inclusion_array[j])
					{
						new_curr_index += new_var_index_increments[old_to_new_index_map[j]];
					}
					carry_flag = false;
				}
				else
				{
					curr_var_assignment[j] = 0;
					curr_index -= (var_domain_sizes[j]-1)*var_index_increments[j];
					if (inclusion_array[j])
					{
						new_curr_index -= (var_domain_sizes[j]-1)*new_var_index_increments[old_to_new_index_map[j]];
					}
				}
				j--;
			}
			if (carry_flag)
			{
				termination_flag = true;
			}
		}
		
		if (!is_empty(new_num_of_vals,slice_set)) // Only consider a slice if it is nonempty.
		{
			//Search for the set among the existing focal elements
			bool found_flag = false;
			int found_index;
			for (int j = 0; j < bucket_size; j++)
			{
				if (set_equal(new_num_of_vals, slice_set, new_focal_elements_bucket[j]))
				{
					found_flag = true;
					found_index = j;
					break;
				}
			}
			if (!found_flag)
			{
				// The focal element does not already exist.
				new_focal_elements_bucket[bucket_size] = new bool[new_num_of_vals];
				for (long int j = 0; j < new_num_of_vals; j++) // Copy slice_set
				{
					new_focal_elements_bucket[bucket_size][j] = slice_set[j];
				}
				new_focal_element_weights_bucket[bucket_size] = focal_element_weights[i];
				bucket_size++;
			}
			else
			{
				// The focal element already exists.
				new_focal_element_weights_bucket[found_index] += focal_element_weights[i];
			}
		}
	}

	// Replace all existing parameters:
	delete[] factor_var_names;
	delete[] var_domain_sizes;	
	delete[] var_index_increments;	
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		delete[] focal_elements[i];
	}
	delete[] focal_elements;
	delete[] focal_element_weights;		

	num_of_vars = new_num_of_vars;
	factor_var_names = new_factor_var_names;
	var_domain_sizes = new_var_domain_sizes;
	var_index_increments = new_var_index_increments;
	num_of_vals = new_num_of_vals;
	num_of_focal_elements = bucket_size;
	focal_elements = new focal_set[bucket_size];
	focal_element_weights = new float[bucket_size];
	for (int i = 0; i < bucket_size; i++)
	{
		focal_elements[i] = new_focal_elements_bucket[i];
		focal_element_weights[i] = new_focal_element_weights_bucket[i];
	}

	// Garbage collection:
	delete[] new_name_bucket;
	delete[] inclusion_array;
	delete[] old_to_new_index_map;
	delete[] old_to_cond_index_map;
	delete[] curr_var_assignment;
	delete[] new_focal_elements_bucket;
	delete[] new_focal_element_weights_bucket;
}


void Tabular_DS_Factor::normalize()
{
	float total = 0.0;
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		total += focal_element_weights[i];
	}
	for (int i = 0; i < num_of_focal_elements; i++)
	{
		focal_element_weights[i] /= total;
	}
}


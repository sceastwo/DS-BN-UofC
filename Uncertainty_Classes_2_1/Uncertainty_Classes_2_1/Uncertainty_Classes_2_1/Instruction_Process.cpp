
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

#include "Variables.h"
#include "Factors.h"


template <class F>
class Factor_List
{
	int list_length;
	int array_length;
	string* factor_identifiers;
	F** factor_array;

public:

	Factor_List()
	{
		list_length = 0;
		array_length = 10;
		factor_identifiers = new string[array_length];
		factor_array = new F*[array_length];
		for (int i = 0; i < array_length; i++)
		{
			factor_identifiers[i] = "";
			factor_array[i] = NULL;
		}
	}

	
	~Factor_List() // Factors are NOT destroyed along with the list.
	{
		delete[] factor_array;
	}
	

	// Only a reference gets returned.
	F* get_factor(string identifier) 
	{
		for (int i = 0; i < list_length; i++)
		{
			if (factor_identifiers[i] == identifier)
			{
				return factor_array[i];
			}
		}
		throw "Factor does not exist.";
	}

	// The factor is NOT copied, the pointer is assigned directly.
	// Existing factors are NOT deleted iff the "delete_and_replace_flag" is not set.
	void set_factor(string identifier, F* new_factor, bool delete_and_replace_flag) 
	{
		if (identifier.empty())
		{
			throw "An empty identifier is not allowed.";
		}
		for (int i = 0; i < list_length; i++)
		{
			if (factor_identifiers[i] == identifier)
			{
				if (factor_array[i] != NULL && delete_and_replace_flag)
				{
					delete factor_array[i];
				}
				factor_array[i] = new_factor;
				return;
			}
		}
		if (list_length < array_length) // Check for space remaining in the array.
		{
			factor_identifiers[list_length] = identifier;
			factor_array[list_length] = new_factor;
			list_length++;
		}
		else // Extend the array if needed.
		{
			array_length += 10;
			string* new_factor_identifiers = new string[array_length];
			F** new_factor_array = new F*[array_length];
			for (int i = 0; i < list_length; i++)
			{
				new_factor_identifiers[i] = factor_identifiers[i];
				new_factor_array[i] = factor_array[i];
			}
			new_factor_identifiers[list_length] = identifier;
			new_factor_array[list_length] = new_factor;
			list_length++;
			for (int i = list_length; i < array_length; i++)
			{
				new_factor_identifiers[i] = "";
				new_factor_array[i] = NULL;
			}
			delete[] factor_identifiers;
			factor_identifiers = new_factor_identifiers;
			delete[] factor_array;
			factor_array = new_factor_array;
		}
	}
};



void instruction_process()
{
	int app_number;
	cout << "Enter Experiment #:"; 
	cin >> app_number;

	// Read variables:
	ifstream var_in(to_string(app_number) + "_Variables_file.txt", ios_base::in);
	if (var_in.fail())
	{
		throw "Variable file not found.";
	}
	int num_of_vars;
	var_in >> num_of_vars;
	if (var_in.fail() || num_of_vars < 0)
	{
		throw "Invalid number of variables.";
	}
	cout << "Variables:\n";
	for (int i = 0; i < num_of_vars; i++)
	{
		string var_name;
		int domain_size;
		var_in >> var_name >> domain_size;
		if (var_in.fail())
		{
			throw "Invalid syntax.";
		}
		if (domain_size < 1)
		{
			throw "Invalid domain size.";
		}
		the_vars.add_variable(var_name,domain_size);
		cout << var_name << ' ' << domain_size << '\n';
	}
	var_in.close();
	cout << "\n\n";


	
	// Read tabular factors:
	ifstream factor_in(to_string(app_number) + "_Factors_file.txt");
	if (factor_in.fail())
	{
		throw "Factor file not found.";
	}
	int num_of_factors;
	factor_in >> num_of_factors;
	if (factor_in.fail() || num_of_factors < 0)
	{
		throw "Invalid number of factors.";
	}

	Factor_List<Tabular_Factor> f_list;
	cout << "Factors:\n";
	for (int i = 0; i < num_of_factors; i++)
	{
		f_list.set_factor("F" + to_string(i+1), new Tabular_Factor(factor_in), false);
		f_list.get_factor("F" + to_string(i+1))->print(cout); // Print the factor as a preview.
	}
	factor_in.close();
	cout << "\n\n";

	
	
	// Read Tabular DS Factors
	ifstream ds_factor_in(to_string(app_number) + "_DS_Factors_file.txt");
	if (ds_factor_in.fail())
	{
		throw "DS Factor file not found.";
	}
	int num_of_ds_factors;
	ds_factor_in >> num_of_ds_factors;
	if (factor_in.fail() || num_of_ds_factors < 0)
	{
		throw "Invalid number of DS factors.";
	}

	Factor_List<Tabular_DS_Factor> ds_f_list;
	cout << "DS Factors:\n";
	for (int i = 0; i < num_of_ds_factors; i++)
	{
		ds_f_list.set_factor("F" + to_string(i+1), new Tabular_DS_Factor(ds_factor_in), false);
		ds_f_list.get_factor("F" + to_string(i+1))->print(cout); // Print the factor as a preview.
	}
	ds_factor_in.close();

	cout << "\n\n************************************\n\n";


	//Open file for output:
	ofstream sim_out(to_string(app_number) + "_Output_file.txt");
	if (sim_out.fail())
	{
		throw "Problem creating the output file.";
	}


	//Process instructions
	ifstream instructions_in(to_string(app_number) + "_Instructions_file.txt");
	if (instructions_in.fail())
	{
		throw "Instruction file not found.";
	}
	enum instruction_type {Factor_Type, DSF_Type} ins_type;
	enum instruction {assign, multiply, marginalize, condition, normalize, print, balloon} ins;
	enum instruction_state {Type, Command, Arg1, Arg2, Arg3, Arg4} ins_state = Type;
	Tabular_Factor* factor_buffer_1;
	Tabular_Factor** factor_buffer_list = NULL;
	Tabular_DS_Factor* ds_factor_buffer_1;
	Tabular_DS_Factor** ds_factor_buffer_list = NULL;
	string string_buffer_1;
	string string_buffer_2;
	string* string_buffer_list = NULL;
	int int_buffer_1;
	int* int_buffer_list = NULL;
	int counter;
	bool comment_flag = false;

	string token;
	instructions_in >> token;
	while (!instructions_in.fail())
	{
		// Comment handling:
		if (token == "/*")
		{
			comment_flag = true;
		}
		else if (token == "*/")
		{
			comment_flag = false;
			instructions_in >> token;
			continue;
		}
		if (comment_flag)
		{
			instructions_in >> token;
			continue;
		}

		// Instruction handling:
		switch (ins_state)
		{
		case Type:
			if (token == "%Tabular_Factor")
			{
				ins_type = Factor_Type;
			}
			else if (token == "%Tabular_DS_Factor")
			{
				ins_type = DSF_Type;
			}
			else
			{
				throw "Invalid instruction type.";
			}
			ins_state = Command;
			break;
		case Command:
			if (token == "#assign")
			{
				ins = assign;
			}
			else if (token == "#multiply")
			{
				ins = multiply;
			}
			else if (token == "#marginalize")
			{
				ins = marginalize;
			}
			else if (token == "#condition")
			{
				ins = condition;
			}
			else if (token == "#normalize")
			{
				ins = normalize;
			}
			else if (token == "#print")
			{
				ins = print;
			}
			else if (token == "#balloon")
			{
				if (ins_type != DSF_Type)
				{
					throw "#balloon can only be used with Dempster-Shafer factors.";
				}
				ins = balloon;
			}
			else
			{
				throw "Invalid command.";
			}
			ins_state = Arg1;
			break;
		default:
			// Command argument processing:
			switch (ins)
			{
			case assign:
				switch (ins_state)
				{
				case Arg1:
					string_buffer_1 = token;
					ins_state = Arg2;
					break;
				case Arg2:
					switch (ins_type)
					{
					case Factor_Type:
						factor_buffer_1 = f_list.get_factor(token);
						f_list.set_factor(string_buffer_1, new Tabular_Factor(*factor_buffer_1), true);
						break;
					case DSF_Type:
						ds_factor_buffer_1 = ds_f_list.get_factor(token);
						ds_f_list.set_factor(string_buffer_1, new Tabular_DS_Factor(*ds_factor_buffer_1), true);
						break;
					}
					ins_state = Type;
					break;
				}
				break;

			case multiply:
				switch (ins_state)
				{
				case Arg1:
					string_buffer_1 = token;
					ins_state = Arg2;
					break;
				case Arg2:
					int_buffer_1 = stoi(token);
					if (int_buffer_1 < 0)
					{
						throw "#multiply: The number of multiplicands cannot be negative.";
					}
					if (int_buffer_1 == 0) // Write the identity factor if the number of multiplicands is 0. 
					{
						switch (ins_type)
						{
						case Factor_Type:
							f_list.set_factor(string_buffer_1, new Tabular_Factor(), true);
							break;
						case DSF_Type:
							ds_f_list.set_factor(string_buffer_1, new Tabular_DS_Factor(), true);
							break;
						}
						ins_state = Type;
						break;
					}
					switch (ins_type)
					{
					case Factor_Type:
						factor_buffer_list = new Tabular_Factor*[int_buffer_1];
						break;
					case DSF_Type:
						ds_factor_buffer_list = new Tabular_DS_Factor*[int_buffer_1];
						break;
					}
					counter = 0;
					ins_state = Arg3;
					break;
				case Arg3:
					switch (ins_type)
					{
					case Factor_Type:
						factor_buffer_list[counter] = f_list.get_factor(token);
						break;
					case DSF_Type:
						ds_factor_buffer_list[counter] = ds_f_list.get_factor(token);
						break;
					}
					counter++;
					if (counter >= int_buffer_1)
					{
						switch (ins_type)
						{
						case Factor_Type:
							f_list.set_factor(string_buffer_1, &Tabular_Factor::multiply(int_buffer_1,factor_buffer_list), true);
							delete[] factor_buffer_list;
							break;
						case DSF_Type:
							ds_f_list.set_factor(string_buffer_1, &Tabular_DS_Factor::multiply(int_buffer_1,ds_factor_buffer_list), true);
							delete[] ds_factor_buffer_list;
							break;
						}
						ins_state = Type;
					}
					break;
				}
				break;

			case marginalize:
				switch (ins_state)
				{
				case Arg1:
					string_buffer_1 = token;
					ins_state = Arg2;
					break;
				case Arg2:
					int_buffer_1 = stoi(token);
					if (int_buffer_1 < 0)
					{
						throw "#marginalize: The number of variables cannot be negative.";
					}
					if (int_buffer_1 == 0) // Do nothing if there are no variables to be marginalized.
					{
						ins_state = Type;
						break;
					}
					string_buffer_list = new string[int_buffer_1];	
					counter = 0;
					ins_state = Arg3;
					break;
				case Arg3:
					string_buffer_list[counter] = token;
					counter++;
					if (counter >= int_buffer_1)
					{
						switch (ins_type)
						{
						case Factor_Type:
							f_list.get_factor(string_buffer_1)->marginalize(int_buffer_1,string_buffer_list);
							break;
						case DSF_Type:
							ds_f_list.get_factor(string_buffer_1)->marginalize(int_buffer_1,string_buffer_list);
							break;
						}
						delete[] string_buffer_list;
						ins_state = Type;
					}
					break;
				}
				break;

			case condition:
				switch (ins_state)
				{
				case Arg1:
					string_buffer_1 = token;
					ins_state = Arg2;
					break;
				case Arg2:
					int_buffer_1 = stoi(token);
					if (int_buffer_1 < 0)
					{
						throw "#condition: The number of variables cannot be negative.";
					}
					if (int_buffer_1 == 0) // Do nothing if there are no variables to be conditioned.
					{
						ins_state = Type;
						break;
					}
					string_buffer_list = new string[int_buffer_1];	
					int_buffer_list = new int[int_buffer_1];
					counter = 0;
					ins_state = Arg3;
					break;
				case Arg3:
					string_buffer_list[counter] = token;
					ins_state = Arg4;
					break;
				case Arg4:
					int_buffer_list[counter] = stoi(token);
					ins_state = Arg3;
					counter++;
					if (counter >= int_buffer_1)
					{
						switch (ins_type)
						{
						case Factor_Type:
							f_list.get_factor(string_buffer_1)->condition(int_buffer_1,string_buffer_list,int_buffer_list);
							break;
						case DSF_Type:
							ds_f_list.get_factor(string_buffer_1)->condition(int_buffer_1,string_buffer_list,int_buffer_list);
							break;
						}
						delete[] string_buffer_list;
						delete[] int_buffer_list;
						ins_state = Type;
					}
					break;
				}
				break;

			case normalize:
				string_buffer_1 = token;
				switch (ins_type)
				{
				case Factor_Type:
					f_list.get_factor(string_buffer_1)->normalize();
					break;
				case DSF_Type:
					ds_f_list.get_factor(string_buffer_1)->normalize();
					break;
				}
				ins_state = Type;
				break;

			case print:
				string_buffer_1 = token;
				switch (ins_type)
				{
				case Factor_Type:
					f_list.get_factor(string_buffer_1)->print(cout);
					f_list.get_factor(string_buffer_1)->print(sim_out);
					break;
				case DSF_Type:
					ds_f_list.get_factor(string_buffer_1)->print(cout);
					ds_f_list.get_factor(string_buffer_1)->print(sim_out);
					break;
				}
				cout << "\n";
				sim_out << "\n";
				ins_state = Type;
				break;

			case balloon:
				switch (ins_state)
				{
				case Arg1:
					string_buffer_1 = token;
					ins_state = Arg2;
					break;
				case Arg2:
					int_buffer_1 = stoi(token);
					if (int_buffer_1 < 0)
					{
						throw "#condition: The number of variables cannot be negative.";
					}
					if (int_buffer_1 == 0) // Do nothing if there are no variables to be expanded into.
					{
						ins_state = Type;
						break;
					}
					string_buffer_list = new string[int_buffer_1];	
					int_buffer_list = new int[int_buffer_1];
					counter = 0;
					ins_state = Arg3;
					break;
				case Arg3:
					string_buffer_list[counter] = token;
					ins_state = Arg4;
					break;
				case Arg4:
					int_buffer_list[counter] = stoi(token);
					ins_state = Arg3;
					counter++;
					if (counter >= int_buffer_1)
					{
						ds_f_list.get_factor(string_buffer_1)->balloon_extend(int_buffer_1,string_buffer_list,int_buffer_list);
						delete[] string_buffer_list;
						delete[] int_buffer_list;
						ins_state = Type;
					}
					break;
				}
				break;
			}
			break;
		}
		instructions_in >> token;
	}
	
	//Close the output file:
	sim_out.close();

}

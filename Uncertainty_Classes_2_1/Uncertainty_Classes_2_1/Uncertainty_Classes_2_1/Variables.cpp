
#include "Variables.h"

Variable_List the_vars;


Variable_List::Variable::Variable()
{
	the_name = "";
	the_domain_size = 1;
}

Variable_List::Variable::Variable(string new_name, int new_size)
{
	the_name = new_name;
	the_domain_size = new_size;
}

Variable_List::Variable::Variable(Variable& existing_var)
{
	the_name = existing_var.the_name;
	the_domain_size = existing_var.the_domain_size;
}


Variable_List::Variable_List()
{
	the_list_size = LIST_SIZE_INCREMENT;
	the_var_array = new Variable*[the_list_size];
	for (index_type i = 0; i < the_list_size; i++)
	{
		the_var_array[i] = NULL;
	}
}

Variable_List::Variable_List(Variable_List& existing_list)
{
	the_list_size = existing_list.the_list_size;
	the_var_array = new Variable*[the_list_size];
	for (index_type i = 0; i < the_list_size; i++)
	{
		if (existing_list.the_var_array[i] != NULL)
		{
			the_var_array[i] = new Variable(*(existing_list.the_var_array[i]));
		}
		else
		{
			the_var_array[i] = NULL;
		}
	}
}

Variable_List& Variable_List::operator=(Variable_List& existing_list)
{
	for (index_type i = 0; i < the_list_size; i++)
	{
		if (the_var_array[i] != NULL)
		{
			delete the_var_array[i];
		}
	}
	delete[] the_var_array;
	
	the_list_size = existing_list.the_list_size;
	the_var_array = new Variable*[the_list_size];
	for (index_type i = 0; i < the_list_size; i++)
	{
		if (existing_list.the_var_array[i] != NULL)
		{
			the_var_array[i] = new Variable(*(existing_list.the_var_array[i]));
		}
		else
		{
			the_var_array[i] = NULL;
		}
	}
	return existing_list;
}

Variable_List::~Variable_List()
{
	for (index_type i = 0; i < the_list_size; i++)
	{
		if (the_var_array[i] != NULL)
		{
			delete the_var_array[i];
		}
	}
	delete[] the_var_array;
}

index_type Variable_List::get_var_index(string name)
{
	for(index_type i = 0; i < the_list_size; i++)
	{
		if (the_var_array[i] == NULL)
		{
			continue;
		}
		else if (the_var_array[i]->the_name == name)
		{
			return i;
		}
	}
	throw "Variable name not found.";
}

string Variable_List::get_var_name(index_type index)
{
	if (index < 0 || index >= the_list_size)
	{
		throw "Index out of range.";
	}
	else if (the_var_array[index] == NULL)
	{
		throw "No variable found.";
	}
	else
	{
		return the_var_array[index]->the_name;
	}
}

int Variable_List::get_domain_size(string name)
{
	for(index_type i = 0; i < the_list_size; i++)
	{
		if (the_var_array[i] == NULL)
		{
			continue;
		}
		else if (the_var_array[i]->the_name == name)
		{
			return the_var_array[i]->the_domain_size;
		}
	}
	throw "Variable name not found.";
}

int Variable_List::get_domain_size(index_type index)
{
	if (index < 0 || index >= the_list_size)
	{
		throw "Index out of range.";
	}
	else if (the_var_array[index] == NULL)
	{
		throw "No variable found.";
	}
	else
	{
		return the_var_array[index]->the_domain_size;
	}
}

index_type Variable_List::add_variable(string name, int domain_size)
{
	if (domain_size <= 0)
	{
		throw "Domain size must be at least 1.";
	}
	if (name == "")
	{
		throw "Empty name not allowed.";
	}
	for(index_type i = 0; i < the_list_size; i++)
	{
		if (the_var_array[i] == NULL)
		{
			continue;
		}
		else if (the_var_array[i]->the_name == name)
		{
			throw "Variable already exists.";
		}
	}
	for(index_type i = 0; i < the_list_size; i++)
	{
		if (the_var_array[i] == NULL)
		{
			the_var_array[i] = new Variable(name, domain_size);
			return i;
		}
	}
	index_type new_list_size = the_list_size + LIST_SIZE_INCREMENT;
	Variable** new_var_array = new Variable*[new_list_size];
	for(index_type i = 0; i < the_list_size; i++)
	{
		new_var_array[i] = the_var_array[i];
	}
	new_var_array[the_list_size] = new Variable(name, domain_size);
	index_type the_index = the_list_size;
	for (index_type i = the_list_size+1; i < new_list_size; i++)
	{
		new_var_array[i] = NULL;
	}
	delete[] the_var_array;
	the_var_array = new_var_array;
	the_list_size = new_list_size;
	return the_index;
}

/*void Variable_List::delete_variable(string name)
{
	for(index_type i = 0; i < the_list_size; i++)
	{
		if (the_var_array[i] == NULL)
		{
			continue;
		}
		else if (the_var_array[i]->the_name == name)
		{
			delete the_var_array[i];
			the_var_array[i] = NULL;
			return;
		}
	}
	throw "Variable name not found.";
}*/

/*void Variable_List::delete_variable(index_type index)
{
	if (index < 0 || index >= the_list_size)
	{
		return;
	}
	else if (the_var_array[index] == NULL)
	{
		return;
	}
	else
	{
		delete the_var_array[index];
		the_var_array[index] = NULL;
		return;
	}
}*/





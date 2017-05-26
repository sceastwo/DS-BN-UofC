
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

#include "Variables.h"
#include "Factors.h"

void instruction_process();

//#define __TESTING_TABULAR_DS_FACTOR__
//#define __GENERAL_TESTING__

/*class general_testing_class
{
public:
	general_testing_class()
	{
		cout << "Something\n";
	}
	general_testing_class(char* message)
	{
		cout << message;
	}
};*/

// Template testing:
/*template <class T>
class mypair {
    T* a, b;
  public:
    mypair (T* first, T* second)
      {a=first; b=second;}
    T* getmax ();
};*/

int main()
{
#ifdef __TESTING_VARIABLE_LIST__
	for (int i = 0; i < 100; i++)
	{
		the_vars.add_variable("A_" + to_string(i), i+1);
		cout << the_vars.get_domain_size("A_" + to_string(i)) << '\n';
	}
	try
	{
		the_vars.add_variable("B", -1);
	}
	catch (char* s)
	{
		cerr << s << '\n';
	}

#elif defined __TESTING_TABULAR_FACTOR__ 
	

	ifstream var_in("Tabular_Factor_Test_Variables_file.txt");
	int num_of_vars;
	var_in >> num_of_vars;
	for (int i = 0; i < num_of_vars; i++)
	{
		string var_name;
		int domain_size;
		var_in >> var_name >> domain_size;
		the_vars.add_variable(var_name,domain_size);
		cout << var_name << ' ' << domain_size << '\n';
	}
	var_in.close();
	cout << "\n\n";

	ifstream factor_in("Tabular_Factor_Test_Factors_file.txt");
	int num_of_factors;
	factor_in >> num_of_factors;
	Tabular_Factor** factor_array = new Tabular_Factor*[num_of_factors];
	for (int i = 0; i < num_of_factors; i++)
	{
		factor_array[i] = new Tabular_Factor(factor_in);
		factor_array[i]->print(cout);
	}
	factor_in.close();

	cout << "\n***************************************\n";

	/*Tabular_Factor f1(*(factor_array[0]));
	Tabular_Factor f2;
	f2 = f1;
	f1.print(cout);
	f2.print(cout);*/

	/*Tabular_Factor* fa[3];
	fa[0] = factor_array[0];
	fa[1] = factor_array[1];
	fa[2] = factor_array[2];
	Tabular_Factor& f3 = Tabular_Factor::multiply(0,fa);
	f3.print(cout);*/

	/*Tabular_Factor f4(*(factor_array[2]));
	string marg_var_array[3];
	marg_var_array[0] = "B";
	marg_var_array[1] = "A";
	marg_var_array[2] = "D";
	f4.marginalize(0,marg_var_array);
	//f4.normalize();
	f4.print(cout);*/

	Tabular_Factor f5(*(factor_array[2]));
	string cond_var_array[4];
	int cond_val_array[4];
	cond_var_array[0] = "B";
	cond_val_array[0] = 2;
	cond_var_array[1] = "E";
	cond_val_array[1] = 0;
	cond_var_array[2] = "D";
	cond_val_array[2] = 0;
	cond_var_array[3] = "A";
	cond_val_array[3] = 1;
	f5.condition(0,cond_var_array,cond_val_array);
	f5.print(cout);

	/*Tabular_Factor f6(*(factor_array[3]));
	f6.normalize();
	f6.print(cout);*/

#elif defined __TESTING_TABULAR_DS_FACTOR__

	ifstream var_in("Tabular_DS_Factor_Test_Variables_file.txt");
	int num_of_vars;
	var_in >> num_of_vars;
	for (int i = 0; i < num_of_vars; i++)
	{
		string var_name;
		int domain_size;
		var_in >> var_name >> domain_size;
		the_vars.add_variable(var_name,domain_size);
		cout << var_name << ' ' << domain_size << '\n';
	}
	var_in.close();

	cout << "\n\n\n";

	ifstream factor_in("Tabular_DS_Factor_Test_Factors_file.txt");
	int num_of_factors;
	factor_in >> num_of_factors;
	Tabular_DS_Factor** factor_array = new Tabular_DS_Factor*[num_of_factors];
	for (int i = 0; i < num_of_factors; i++)
	{
		factor_array[i] = new Tabular_DS_Factor(factor_in);
		factor_array[i]->print(cout);
	}
	factor_in.close();

	cout << "\n***************************************\n";

	/*Tabular_DS_Factor f1(*(factor_array[0]));
	Tabular_DS_Factor f2;
	f2 = f1;
	f1.print(cout);
	f2.print(cout);*/

	/*Tabular_DS_Factor f3(*(factor_array[2]));
	f3.print(cout);
	string new_var_names[10];
	new_var_names[0] = "A";
	new_var_names[1] = "B";
	f3.extend(2,new_var_names);
	f3.print(cout);*/

	/*Tabular_DS_Factor* fa[3];
	fa[0] = factor_array[0];
	fa[1] = factor_array[1];
	fa[2] = factor_array[3];
	Tabular_DS_Factor& f4 = Tabular_DS_Factor::multiply(3,fa);
	f4.print(cout);*/

	/*try
	{
		Tabular_DS_Factor f5(*(factor_array[3]));
		f5.print(cout);
		string marg_var_array[3];
		marg_var_array[0] = "B";
		marg_var_array[1] = "A";
		marg_var_array[2] = "D";
		f5.marginalize(2, marg_var_array);
		f5.print(cout);
		//f5.normalize();
		//f5.print(cout);
	}
	catch(char* err)
	{
		cerr << err << '\n';
	}
	catch(string err)
	{
		cerr << err << '\n';
	}*/

	/*try
	{
		Tabular_DS_Factor f6(*(factor_array[3]));
		f6.print(cout);
		string cond_var_array[4];
		int cond_val_array[4];
		cond_var_array[0] = "C";
		cond_val_array[0] = 4;
		cond_var_array[1] = "B";
		cond_val_array[1] = 1;
		cond_var_array[2] = "A";
		cond_val_array[2] = 1;
		cond_var_array[3] = "A";
		cond_val_array[3] = 1;
		f6.condition(3,cond_var_array,cond_val_array);
		f6.print(cout);
	}
	catch(char* err)
	{
		cerr << err << '\n';
	}
	catch(string err)
	{
		cerr << err << '\n';
	}*/

	/*string* s_arr = NULL;
	bool** b_arr_arr = new bool*[1];
	b_arr_arr[0] = new bool[1];
	b_arr_arr[0][0] = true;
	float* f_arr = new float[1];
	f_arr[0] = 1.0;
	Tabular_DS_Factor f6(0,NULL,1,b_arr_arr,f_arr);
	f6.print(cout);
	delete[] b_arr_arr[0];
	delete[] b_arr_arr;
	delete[] f_arr;*/

	try
	{
		Tabular_DS_Factor f7(*(factor_array[3]));
		f7.print(cout);
		string balloon_var_array[3];
		int val_array[3];
		balloon_var_array[0] = "E";
		val_array[0] = 0;
		balloon_var_array[1] = "F";
		val_array[1] = 1;
		balloon_var_array[2] = "A";
		val_array[2] = 1;
		f7.balloon_extend(2,balloon_var_array,val_array);
		f7.print(cout);
	}
	catch(char* err)
	{
		cerr << err << '\n';
	}
	catch(string err)
	{
		cerr << err << '\n';
	}

#elif defined __GENERAL_TESTING__

	/*int* a = new int[0];
	cout << a << '\n';
	delete[] a;*/

	cout << "Nothing\n";
	general_testing_class gt("Something\n");

#else
	try
	{
		instruction_process();
	}
	catch(char* error)
	{
		cerr << error << '\n';
		system("PAUSE");
		return EXIT_FAILURE;
	}
	catch(string error)
	{
		cerr << error << '\n';
		system("PAUSE");
		return EXIT_FAILURE;
	}

#endif
	system("PAUSE");
	return EXIT_SUCCESS;
}


#include <iostream>
#include <numeric>
#include <chrono>
#include <cstring>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <bitset>
#include <random>
#include <algorithm>
#include <functional>
#include <sstream>
#include <iomanip>

typedef unsigned char byte;

#include "gen_mkw.h"
#include "dis_query_process.h"
#include "dis_query_generation.h"

int main()
{
	std::string raw_db_file = "../../databases/db6k.dat";
	
	std::string meta_db_file = "../../databases/meta_db6k.dat";

	std:: vector<unsigned int> frequency;
	
	std::vector<std::string> db_kw;
	std::vector<unsigned int> bin_boundaries;
	std::vector<std::set<unsigned int>> raw_db;

	std::vector<std::string> db_mkw;
	std::vector<std::string> db_mkw_hash;
	
	std::vector<std::string>c_mkw;
	std::vector<unsigned int> ra;
	std::vector<unsigned int> s_vec;
	std::vector<unsigned int> bin;
	std::vector<vector<float>> size1;
	std::vector<std::set<unsigned int>> meta_db;
	std::vector<vector<std::string>> mkws_hash;
	std:: vector<vector<unsigned int>> bin_vector;
	std:: vector<unsigned int> freq_vec;

	vector<float> f;
	vector<string> fin;
	vector<set<string>> ham;
	stringstream ss;
	
	vector<string> result;

	int bucket_size = 5;
	int count = 0;

	auto start1 = high_resolution_clock::now();

	/** Reading data from Enron Inverted Index file **/
	cout<<"[*] Reading Enron Inverted Index\n\n";

	DB_ReadFromFileToDatabase(raw_db_file, db_kw, raw_db);

	cout<<"[*] Rawdb generated\n\n";

	// cout<<">>  Rawdb size = "<<raw_db.size()<<"\n";

	for(auto v:raw_db){
		auto row_size = v.size();
		count += row_size;
		// cout<<">>  Rawdb size = "<<v.size()<<"\n";
		for(auto vv:v){
			// cout<<vv<<", ";
		}
		// std::cout << std::endl;
	}

	// for(int i =0; i <=raw_db.size(); i++)
	// {
	// 	count += raw_db[i].size();

	// }
	cout<< ">>  Total unique (keyword-id) pairs = "<< count <<"\n";
	
	/** MetaDB generation function  **/
	cout << "[*] Generating Metakeywords --\n";
	DB_GenMKW(meta_db, db_mkw,bin_boundaries, raw_db,bucket_size,db_mkw_hash,frequency);
	
	cout<<"[*] Metakeywords and MetaDB generated\n";

	auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);
	cout << "[*] Time taken for metadb generation: "<< duration1.count() << " microseconds" << endl;

	// DB_WriteFromDatabaseToFile(meta_db_file, db_mkw_hash, meta_db, KW_TYPE::HEX);

	// vector<string> hw_vec;
	// vector<vector<string>> hw_mkw;

	
	/**    Generating Query vectors of different hamming weights   **/   
	// for(int i=2;i<6;i+=1)
	// {
	// 	set<string> p;
	// 	for(int counter=0;counter<10;counter++)
	// 	{
	// 		ss.clear();
	// 		p.insert(gen(i,N_KW));

	// 	}
	// 	ham.push_back(p);
	// }
	
	Query_processing(ham);

	/**  Query processing  **/
	auto start2 = high_resolution_clock::now();
	
	
	for (int i=0;i<ham.size();i++)
	{
		bin_vector.clear();
		int c = 0;
		f.clear();
	 	for(auto x:ham.at(i))
	   	  { 
			c++;
		  	//hw_vec.push_back(x);
		  	mkws_hash.clear();
		  	s_vec.clear(); 

		  	DB_ConvertQueryToMetaKeyword(mkws_hash, bin_boundaries, x, bucket_size, bin_vector, s_vec, db_mkw_hash, meta_db, frequency, freq_vec);

			ra.clear();
			resultact(x,raw_db,ra);

			//double press=(double)ra.size()/(1.0*s_vec.size());
			double prec = precision(ra, s_vec);

			cout<<"\n>>  Result_Actual = "<< ra.size();
			cout<<"\n>>  Result_Superset = "<<s_vec.size();
			cout<<"\n>>  Precision = "<<prec<<"\n";
			
		}
	//	hw_mkw.push_back(hw_vec);
	//	hw_vec.clear();
	
	}

	auto stop2 = high_resolution_clock::now();
	auto duration2 = duration_cast<microseconds>(stop2 - start2);
        

/*	cout<<"Writing to file started\n";
        std::ofstream outputfile;
        outputfile.open(test_vector,std::ios_base::out);
        for (int i=0;i<hw_mkw.size();i++)
        {
               // outputfile<<"\n.............HW="<<(i+2)<<".........................\n";
               // outputfile<<"\nR_Act"<<"\t"<<"R_Sup"<<"\t"<<"Precision\n\n";

                int p=0;
                for(auto yy:hw_mkw[i])
                {
                        //p++;
                        //c = c+yy;     
                        outputfile << yy << ",";
                        //if(p%2==0)
                       

                }
		outputfile<<"\n";
               // outputfile<<avg[i]<<"\n";


        }
        outputfile.close();
*/


	cout << "[*] Time taken for processing query: "<< duration2.count() << " microseconds\n" << endl;


	  
	meta_db.clear();
	raw_db.clear();
	mkws_hash.clear();
	size1.clear();


	return 0;
}


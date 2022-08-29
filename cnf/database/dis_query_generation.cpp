#include "dis_query_generation.h"

// int main()
int Query_processing(std::vector<std::set<std::string>> &ham)
{
	std::string raw_db_file = "../../databases/db6k.dat";
	
	// std::string meta_db_file = "test_MDB.csv";

	std:: vector<unsigned int> frequency;
	std:: vector<unsigned int> freq_vec;
	std:: vector<vector<unsigned int>> freq_vector;

	std:: vector<vector<unsigned int>> query_vector;
	std::vector<unsigned int> query_file;
	
	std::vector<std::string> db_kw;
	std::vector<std::set<unsigned int>> raw_db;

	std::vector<unsigned int> bin_boundaries;
	std:: vector<vector<unsigned int>> bin_vector;
	int bucket_size = 10;

	std::vector<std::string> db_mkw;
	std::vector<std::string> db_mkw_hash;
	
	std::vector<unsigned int> ra;
	std::vector<unsigned int> s_vec;
	
	std::vector<std::set<unsigned int>> meta_db;
	std::vector<vector<std::string>> mkws_hash;
	
	// vector<set<string>> ham;
	stringstream ss;

	// int count = 0;

	auto start1 = high_resolution_clock::now();

	/** Reading data from Enron Inverted Index file **/
	cout<<"[*] Reading Enron Inverted Index\n\n";

	DB_ReadFromFileToDatabase(raw_db_file, db_kw, raw_db);

	cout<<"[*] Rawdb generated\n\n";
	/*cout<<">>  Rawdb size = "<<raw_db.size()<<"\n";
	for(int i =0; i <=raw_db.size(); i++)
	{
		count += raw_db[i].size();
	}
	cout<< ">>  Total unique (keyword-id) pairs = "<< count<<"\n"; */
	

	/** MetaDB generation function  **/
	cout << "[*] Generating Metakeywords --\n";
	DB_GenMKW(meta_db, db_mkw,bin_boundaries, raw_db,bucket_size,db_mkw_hash,frequency);
	cout<<"[*] Metakeywords and MetaDB generated\n";


	auto stop1 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(stop1 - start1);
	cout << "[*] Time taken for metadb generation: "<< duration1.count() << " microseconds" << endl;

	// DB_WriteFromDatabaseToFile(meta_db_file, db_mkw_hash, meta_db, KW_TYPE::HEX);

	
	/**    Generating Query of different hamming weights   **/ 
	vector<string> hw_vec;
	vector<vector<string>> hw_mkw;
	vector <int> ham_wt_vec;  
	string input;
	int count_queries;
	int num;
	char c;

	cout << "Enter different hamming weights = ";
	getline(cin, input);
	istringstream is(input);	
    
	while(true)
	{
		is>>num;
		ham_wt_vec.push_back(num);
		if(!is.eof()) is>>c;
		else break;
    	}
    
	cout << "Number of queries to generate per hamming weight = " ;
	cin >> count_queries;

	for(int i=ham_wt_vec[0]; i<(ham_wt_vec[0]+ham_wt_vec.size()); i++)			//Hamming weight
	{
		set<string> p;
		for(int counter=0; counter<count_queries; counter++) 				//Number of Queries
		{
			ss.clear();
			p.insert(gen(i,N_KW));

		}
		ham.push_back(p);
	}
	
	/**  Query processing  **/
	auto start2 = high_resolution_clock::now();
	
	for (int i=0;i<ham.size();i++)
	{
		freq_vector.clear();
		hw_mkw.clear();
		query_vector.clear();
		bin_vector.clear();

	 	for(auto x:ham.at(i))
	   	  { 
			for(int j=0;j<x.size();j++)
			{
				if(x[j]=='1')
				{

					query_file.push_back(j);
				}
			}
		  	
		  	mkws_hash.clear();
		  	s_vec.clear(); 
			// hw_vec.push_back("0000");

			// int DB_ConvertQueryToMetaKeyword(std::vector<vector<std::string>> &mkws_hash, std::vector<unsigned int> &bin_boundaries,std::string query, int bin_stride,
			//  vector<unsigned int> &bin,std::vector<unsigned int>& s_vec, vector<std::string> & db_mkw_hash,vector<std::set<unsigned int>> &meta_db,std::vector<unsigned int> &frequency);


		  	DB_ConvertQueryToMetaKeyword(mkws_hash, bin_boundaries, x, bucket_size, bin_vector, s_vec, db_mkw_hash, meta_db, frequency, freq_vec);

			for(int i=0; i<mkws_hash.size(); i++)
			{
				for(auto y:mkws_hash[i]){
					hw_vec.push_back(y);
				}
			}

			ra.clear();
			resultact(x,raw_db,ra);

           
			// double press=(double)ra.size()/(1.0*s_vec.size());
            		double prec = precision(ra, s_vec);

			// cout<<"\n>>  Result_Actual = "<< ra.size();
			// cout<<"\n>>  Result_Superset = "<<s_vec.size();
			// cout<<"\n>>  Precision = "<<prec<<"\n";

	
			hw_mkw.push_back(hw_vec);
			hw_vec.clear();

			freq_vector.push_back(freq_vec);
        		freq_vec.clear();
			
			query_vector.push_back(query_file);
			query_file.clear();
			
		}
	
		/*
		cout << "\n--------------------------------------------------------------- Freq_vector_HW[" << i+2 << "]------------------------------------------------------------------------\n\n";
		for(auto i=0; i<freq_vector.size(); i++){
		    for(auto y: freq_vector[i]){
		        cout << y << "\t";
		    }
		    cout << "\n";           
		}

		
		cout << "\n\n--------------------------------------------------------------- Test_vector_HW[" << i+2 << "]-----------------------------------------------------------------------\n\n";
		for(auto i=0; i<hw_mkw.size(); i++){
		    for(auto y: hw_mkw[i]){
		        cout << y << "\t";
		    }
		    cout << "\n";           
		}


		cout << "\n\n--------------------------------------------------------------- Bin_vector_HW[" << i+2 << "] -----------------------------------------------------------------------\n\n";
		for(auto i=0; i<bin_vector.size(); i++){
		    for(auto y: bin_vector[i]){
		        cout << y << "\t";
		    }
		    cout << "\n";           
		}

		*/

		auto stop2 = high_resolution_clock::now();
		auto duration2 = duration_cast<microseconds>(stop2 - start2);
        


		/** 	Writing to File     **/
		std::string testvector = "HW"+to_string(ham_wt_vec[0]+i)+"_testvector.csv";	
		std::string testvector_1 = "HW"+to_string(ham_wt_vec[0]+i)+"_testvector.csv";
		std::ofstream outputfile1(testvector_1);
		// outputfile1.open(testvector,std::ios_base::out);
		// outputfile1.open(testvector_1,std::ios_base::out);
		for (int i=0;i<hw_mkw.size();i++)
		{
			int p=0;
			for(auto yy:hw_mkw[i])
			{
				p++;
				outputfile1 << yy << ",";
				if(p%(hw_mkw[i].size())==0)
				outputfile1 <<"\n";        
			}
		}
		outputfile1.close();

	
		
		std::string binvector = "HW"+to_string(ham_wt_vec[0]+i)+"_binvector.csv";	
		std::ofstream outputfile2(binvector);
		// outputfile2.open(binvector,std::ios_base::out);
		for (int i=0;i<bin_vector.size();i++)
		{
			int p=0;
			for(auto yy:bin_vector[i])
			{
				p++;
				outputfile2 << yy << ",";
				if(p%(bin_vector[i].size())==0)
				outputfile2 <<"\n";
			}
		}
		outputfile2.close();



		std::string freqvector = "HW"+to_string(ham_wt_vec[0]+i)+"_freqvector.csv";	
		std::ofstream outputfile3(freqvector);
		// outputfile3.open(freqvector,std::ios_base::out);
		for (int i=0;i<freq_vector.size();i++)
		{
			int p=0;
			for(auto yy:freq_vector[i])
			{
				p++;
				outputfile3 << yy << ",";
				if(p%(freq_vector[i].size())==0)
				outputfile3 <<"\n";
			}
		}
		outputfile3.close();



		std::string queryvector = "HW"+to_string(ham_wt_vec[0]+i)+"_queryvector.csv";	
		std::string queryvector_hex = "HW"+to_string(ham_wt_vec[0]+i)+"_hex_queryvector.csv";	
		std::ofstream outputfile4(queryvector_hex);
		// outputfile4.open(queryvector,std::ios_base::out);
		// outputfile4.open(queryvector_hex,std::ios_base::out);
		for (int i=0;i<query_vector.size();i++)
		{
			int p=0;
			for(auto yy:query_vector[i])
			{
				p++;
				// outputfile4 << yy << ",";					//Output in DEC

				stringstream st;						//Output in HEX
				st<< std:: hex << yy;
				outputfile4 <<string(8-st.str().size(),'0')+ st.str()<<",";

				if(p%(query_vector[i].size())==0)
				outputfile4 <<"\n";
			}
		}
		outputfile4.close();
		


		// cout << "[*] Time taken for processing query: "<< duration2.count() << " microseconds\n" << endl;

	}
	  
	meta_db.clear();
	raw_db.clear();
	mkws_hash.clear();


	return 0;
}


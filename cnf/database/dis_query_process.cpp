#include "dis_query_process.h"

  
int resultact(string query,std::vector<std::set<unsigned int>> &raw_db,vector<unsigned int> &ra)
{
 
	for(int i=0;i<query.size();i++)
	{
	 	
	 	if(query[i]=='1')
	 	{ 
		 	vector<unsigned int> m(raw_db[i].begin(), raw_db[i].end());	
			std::vector<unsigned int> v(N_ID);
			std::sort(m.begin(), m.end());
			std::sort(ra.begin(), ra.end());
			std::vector<unsigned int>::iterator it,st;
			it = std::set_union(m.begin(), m.end(), ra.begin(),ra.end(), v.begin());
			ra.clear();
			
			for (st = v.begin(); st != it; ++st)
			{
				ra.push_back(*st);
			}
		}							
	 }
	 
	return 0;
} 
   
   
vector <unsigned int> getIntersection(vector < vector <unsigned int> > &sets)
{
    vector < unsigned int> result;  // To store the reaultant set
    int smallSetInd = 0;  // Initialize index of smallest set
    int minSize = sets[0].size(); // Initialize size of smallest set
  
    // sort all the sets, and also find the smallest set
    for (int i = 1 ; i < sets.size() ; i++)
    {
        // sort this set
        sort(sets[i].begin(), sets[i].end());
  
        // update minSize, if needed
        if (minSize > sets[i].size())
        {
            minSize = sets[i].size();
            smallSetInd = i;
        }
    }
  
    map<unsigned int, unsigned int> elementsMap;
  
    // Add all the elements of smallest set to a map, if already present,
    // update the frequency
    for (int i = 0; i < sets[smallSetInd].size(); i++)
    {
        if (elementsMap.find( sets[smallSetInd][i] ) == elementsMap.end())
            elementsMap[ sets[smallSetInd][i] ] = 1;
        else
            elementsMap[ sets[smallSetInd][i] ]++;
    }
  
    // iterate through the map elements to see if they are present in
    // remaining sets
    map<unsigned int, unsigned int>::iterator it;
    for (it = elementsMap.begin(); it != elementsMap.end(); ++it)
    {
        int elem = it->first;
        int freq = it->second;
  
        bool bFound = true;
  
        // Iterate through all sets
        for (int j = 0 ; j < sets.size() ; j++)
        {
            // If this set is not the smallest set, then do binary search in it
            if (j != smallSetInd)
            {
                // If the element is found in this set, then find its frequency
                if (binary_search( sets[j].begin(), sets[j].end(), elem ))
                {
                   unsigned int lInd = lower_bound(sets[j].begin(), sets[j].end(), elem)
                                                            - sets[j].begin();
                   unsigned int rInd = upper_bound(sets[j].begin(), sets[j].end(), elem)
                                                            - sets[j].begin();
  
                   // Update the minimum frequency, if needed
                   if ((rInd - lInd) < freq)
                       freq = rInd - lInd;
                }
                // If the element is not present in any set, then no need 
                // to proceed for this element.
                else
                {
                    bFound = false;
                    break;
                }
            }
        }
  
        // If element was found in all sets, then add it to result 'freq' times
        if (bFound)
        {
            for (int k = 0; k < freq; k++)
                result.push_back(elem);
        }
    }
    return result;
} 


 
vector<string> DB_find_MKW(string q,int n, int prev, int bin_stride)
{
	std::string all_one(bin_stride,'1');
	std::string all_zero(bin_stride,'0');
	std::string all_one1((n-prev),'1');
	std::string all_zero1((n-prev),'0');
	string s;
	vector<string> mkw;
	unsigned int start_i = 0;
		if((n-prev)%bin_stride!=0){
			for(unsigned int i=0;i<(n-prev);i++){
				s.clear();
				if(q[i]=='1'){
					s += std::string(start_i,'1');
					s += std::string(i-start_i,'0');
					s += std::string((n-prev)-i,'1');
					if(s.compare(all_zero1)!=0 && s.compare(all_one1)!=0)
					mkw.push_back(s);
					start_i = i+1;
				}
			}
			s.clear();
			s += std::string(start_i,'1');
			//s += std::string(start_i,'1');
			s += std::string((n-prev)-start_i,'0');
			if(s.compare(all_zero1)!=0 && s.compare(all_one1)!=0){
				mkw.push_back(s);
			}
		}

		else{
			for(unsigned int i=0;i<bin_stride;i++){
			s.clear();
			if(q[i]=='1'){
				s += std::string(start_i,'1');
				s += std::string(i-start_i,'0');
				s += std::string(bin_stride-i,'1');
				if(s.compare(all_zero)!=0 && s.compare(all_one)!=0)
					mkw.push_back(s);
				start_i = i+1;
			}
		}
	
		s.clear();
		s += std::string(start_i,'1');
		s += std::string(bin_stride-start_i,'0');
		if(s.compare(all_zero)!=0 && s.compare(all_one)!=0){
				mkw.push_back(s);
		}

		}
	
	/*for(auto xt: mkw)
	cout<<xt;
	cout<<bin_stride;*/

	return mkw;

}

/*
 vector <unsigned int> set_of_s_intersection(vector<std::string>& mkw_bin,vector<std::string> & db_mkw,vector<std::set<unsigned int>> &meta_db,std:: vector<unsigned int> &frequency)
{
	vector<vector<unsigned int>> r;
	vector<unsigned int> hh;
  	unsigned int index;
  	

 	for(size_t z=0;z<mkw_bin.size();z++)
 	{ 	
 		hh.clear();
 		auto p=find(db_mkw.begin(),db_mkw.end(),mkw_bin[z]);
 		if (p != db_mkw.end())
 		{
			index = p - db_mkw.begin();
        }
        else{
        	cout << "[!} Could not find meta-keyword! " << mkw_bin[z] << endl;
		}	
        	
		std::copy(meta_db[index].begin(),meta_db[index].end(),back_inserter(hh));
		r.push_back(hh);
			
    }
    	 	 
    return getIntersection(r);

}


int DB_ConvertQueryToMetaKeyword(std::vector<vector<std::string>> &mkws_hash, std::vector<unsigned int> &bin_boundaries,std::string query, int bin_stride, vector<unsigned int> &bin,std::vector<unsigned int>& s_vec, vector<std::string> & db_mkw_hash,vector<std::set<unsigned int>> &meta_db,std::vector<unsigned int> &frequency)
{
	std::string all_one(bin_stride,'1');
	std::string all_zero(bin_stride,'0');
	std::string s;
	std::string ss;
	vector<vector <unsigned int>> bin_uni;
	int prev=0;
	string q;
	
	bin.clear();
	
	vector<string> mkw_bin;
	vector<string>	mkws_hash_bin;
	for(auto n:bin_boundaries)
	{
		mkw_bin.clear();
		
		int flag=0;
		q.clear();
		for(unsigned int ii = prev;ii<n;++ii)
		{
			q+=query[ii];
			if(query[ii]=='1')
			flag=1;
		}
		if (flag==1)
		{
			vector<string> mkws_bin_i = DB_find_MKW(q,n,prev,bin_stride);
			for(int y=0;y<mkws_bin_i.size();y++)
			{
				string gg; 
				gg+=string(prev,'0');
				gg+= mkws_bin_i[y];
				mkw_bin.push_back(gg);
			}
			
			convert_mkws_digest(mkw_bin,mkws_hash_bin);
			mkws_hash.push_back(mkws_hash_bin);
			bin.push_back(mkws_hash_bin.size());
			vector <unsigned int> bin_intersection =set_of_s_intersection(mkws_hash_bin,db_mkw_hash,meta_db,frequency);
			mkws_hash_bin.clear();
			bin_uni.push_back(bin_intersection);
		}
		
		prev=n;
	}
	

	set<unsigned int> sr;
	for (int i = 0; i < bin_uni.size(); i++)
	{
		for (int j = 0; j < bin_uni[i].size(); j++)
		{
		    sr.insert(bin_uni[i][j]);
		}   
	}

	for (auto it = sr.begin(); it !=sr.end(); ++it)
	{
	   s_vec.push_back(*it);
	}
    
	return 0;
} */

vector <unsigned int> set_of_s_intersection(vector<std::string>& mkw_bin,vector<std::string> & db_mkw,vector<std::set<unsigned int>> &meta_db,std:: vector<unsigned int> &frequency, std::vector<unsigned int> &freq_vec)
{
	vector<vector<unsigned int>> r;
	vector<unsigned int> hh;
  	unsigned int index;
  	

 	for(size_t z=0;z<mkw_bin.size();z++)
 	{ 	
 		hh.clear();
 		auto p=find(db_mkw.begin(),db_mkw.end(),mkw_bin[z]);
 		if (p != db_mkw.end())
 		{
			index = p - db_mkw.begin();
			freq_vec.push_back(frequency[index]);
			// cout<<frequency[index]<<"\t";
        }
        else{
        	cout << "[!] Could not find meta-keyword! " << mkw_bin[z] << endl;
		}	
        	
		std::copy(meta_db[index].begin(),meta_db[index].end(),back_inserter(hh));
		r.push_back(hh);
		// cout << "\n";	
    }
	// cout << "\n";
    	 	 
    return getIntersection(r);

}

int DB_ConvertQueryToMetaKeyword(std::vector<vector<std::string>> &mkws_hash, std::vector<unsigned int> &bin_boundaries,std::string query, int bin_stride, vector<vector<unsigned int>> &bin,std::vector<unsigned int>& s_vec, vector<std::string> & db_mkw_hash,vector<std::set<unsigned int>> &meta_db,std::vector<unsigned int> &frequency, std::vector<unsigned int> &freq_vec)
{
	std::string all_one(bin_stride,'1');
	std::string all_zero(bin_stride,'0');
	std::string s;
	std::string ss;
	vector<vector <unsigned int>> bin_uni;
	int prev=0;
	string q;
	
	vector<unsigned int> bin_temp;
	
	vector<string> mkw_bin;
	vector<string>	mkws_hash_bin;
	for(auto n:bin_boundaries)
	{
		mkw_bin.clear();
		int flag=0;
		q.clear();
		for(unsigned int ii = prev;ii<n;++ii)
		{
			q+=query[ii];
			if(query[ii]=='1')
			flag=1;
		}
		if (flag==1)
		{
			vector<string> mkws_bin_i = DB_find_MKW(q,n,prev,bin_stride);
			// cout<< (prev) << "\t";
			// cout << mkws_bin_i.size() << "\t";
			for(int y=0;y<mkws_bin_i.size(); y++)
			{
				string gg; 
				gg+=string(prev,'0');
				gg+= mkws_bin_i[y];
				mkw_bin.push_back(gg);
				bin_temp.push_back(n/bin_stride);
			}
			
			convert_mkws_digest(mkw_bin,mkws_hash_bin);
			mkws_hash.push_back(mkws_hash_bin);
			// bin.push_back(mkws_hash_bin.size());

			// for(auto y:bin)
			// 	cout << y << "\t";
			// cout << prev << "\t";
			vector <unsigned int> bin_intersection =set_of_s_intersection(mkws_hash_bin,db_mkw_hash,meta_db,frequency, freq_vec);
			mkws_hash_bin.clear();
			bin_uni.push_back(bin_intersection);
			// cout << prev << "\t";
		}
		// cout << prev << "\t";
		prev=n;
		// cout << prev << "\t";
		// cout << "\n";
	}
	bin.push_back(bin_temp);

	set<unsigned int> sr;
	for (int i = 0; i < bin_uni.size(); i++)
	{
		for (int j = 0; j < bin_uni[i].size(); j++)
		{
		    sr.insert(bin_uni[i][j]);
		}   
	}

	for (auto it = sr.begin(); it !=sr.end(); ++it)
	{
	   s_vec.push_back(*it);
	}
    
	return 0;
}


int DB_WritetoHamFile(std::string ham_file, std::vector<std::set<string>> ham)
{
	 std::stringstream ss;
	 std::ofstream outputfile(ham_file);
	//  outputfile.open(ham_file,std::ios_base::out);

	 for (int i=0;i<ham.size();i++)
	 {
		 outputfile<< i+2<<',';
		 for(auto x:ham[i])
		 	outputfile << x<<',';
		 outputfile<<"\n";
	 }

	    
	 outputfile.close();
	 return 0;
}


string gen(int ham,int  bits)
{
	stringstream ss;
	string s;
	unsigned seed =rand();
	s += std::string(ham,'1');
	s += std::string(bits-ham,'0');
	shuffle(s.begin(), s.end(), default_random_engine(seed));
	ss.clear();
	ss.str(std::string());
	
	//ss << std::setw(bits/1309) <<std::setfill('0')<<std::hex << std::uppercase << stoll(s,NULL,2);
	
    return s;
    
}



float average(std::vector<float> & v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return std::accumulate(v.begin(), v.end(),0.0) / count;
}


double precision(std::vector<unsigned int> ra, std::vector<unsigned int> rs)
{
    double press = (double)ra.size()/(1.0*rs.size());
	
    return press;
}


std::string string_to_hex(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}


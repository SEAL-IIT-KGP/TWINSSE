#include "utils.h"

std::string hexStr(::byte *data, int len)
{
	std::stringstream ss;
	ss << std::hex;

	for (int i(0); i < len; ++i)
		ss << std::setw(2) << std::setfill('0') << (int)data[i];

	return ss.str();
}

int convert_mkws_digest(std::vector<std::string> &db_mkw, vector<string> &db_mkw_hash)
{
	for (int i = 0; i < db_mkw.size(); i++)
	{
		::byte const *pbData = (::byte *)db_mkw[i].data();
		unsigned int nDataLen = db_mkw[i].length();
		::byte abDigest[SHA256::DIGESTSIZE];

		SHA256().CalculateDigest(abDigest, pbData, nDataLen);

		// return //((char*)abDigest);  -- BAD!!!
		db_mkw_hash.push_back(hexStr(abDigest, SHA256::DIGESTSIZE));
		// cout<<xx;
	}
	return 0;
}

int DB_WriteFromDatabaseToFile(std::string db_file, std::vector<std::string> db_kw, std::vector<std::set<unsigned int>> db, KW_TYPE dtype = KW_TYPE::HEX)
{
    std::stringstream ss;
    std::ofstream outputfile;
    outputfile.open(db_file,std::ios_base::out);

    unsigned int kw = 0;
    int count =0;
	
	if(dtype==KW_TYPE::HEX){
        for(auto kw_id: db){
           outputfile << db_kw[kw] << ',';
            for (auto p : kw_id){
		        count++;
                ss.clear();
                ss.str(std::string());
                ss << std::setw(ID_HEX_LEN) << std::setfill('0') << std::hex << std::uppercase << p;
                outputfile << ss.str() << ',';
            }
            outputfile << std::endl;
            kw++;
        }
    }

    else if(dtype==KW_TYPE::DEC)
    {
		int i = 1;
        for(auto kw_id: db)
        {
			int count1 = 0;
			
			ss.clear();
            ss.str(std::string());
            ss << std::setw(ID_HEX_LEN) << std::setfill('0') << std::hex << std::uppercase << kw;
            //outputfile << db_kw[kw] << ',';
			outputfile << ss.str() << ',';
            for (auto p : kw_id)
            {
				count++;
				count1++;
               // outputfile << p << ',';
            }
            outputfile << count1 << ',' << std::endl;
            kw++;
			
        }
		i++;

    }
 
    outputfile.close();

    std::cout<<"\n>>  Total number of unique (mkw-id) pairs = "<< count << std::endl;


    return 0;
}


int DB_ReadFromFileToDatabase(std::string db_file, std::vector<std::string> &db_kw, std::vector<std::set<unsigned int>> &db)
{
	std::ifstream inputfile;
	// std::ifstream inputfile(db_file);
	inputfile.open(db_file,std::ios_base::in);
	std::stringstream ss;
	std::string s;
	std::string k;
	int c = 0;
	std::string fline;
	std::vector<std::string> rawdata;
	

	while(std::getline(inputfile,fline))
	{
		rawdata.push_back(fline);
		fline.clear();
	}

	inputfile.close();
	for(auto s : rawdata)
	{
		c += 1;
		ss.clear();
		ss << s;
		std::set<unsigned int> kw_id_set;
		//cout<<s<<"\n";
		std::getline(ss,k,',');//Skip the keyword

		db_kw.push_back(s);
		
		// kw_count.emplace_back(k,c);

		while(!ss.eof())
		{ 
			while(std::getline(ss,s,','))
			{
				if(!s.empty())
				{
			  		kw_id_set.insert(std::stoll(s));
				}
			}
		}
		
		db.push_back(kw_id_set);
		//  cout<<db[0];
	}

	return 0;
}

int DB_GenMKW(std::vector<std::set<unsigned int>> &meta_db, std::vector<std::string> &db_mkw, std::vector<unsigned int> &bin_boundaries, std::vector<std::set<unsigned int>> raw_db, unsigned int bin_stride, std::vector<std::string> &db_mkw_hash,std::vector<unsigned int> &frequency)
{	
	int N_BINS=(N_KW%bin_stride==0)?(N_KW/bin_stride):(N_KW/bin_stride+1);
	string all_zero(bin_stride,'0');
	string all_one(bin_stride,'1');
	bitset<N_KW> nn(all_one);
	string s;
	string ss;
	std::set<std::string> meta_keywords_str;
	std::vector<std::bitset<N_KW>> meta_keywords;
	std::set<unsigned int> meta_keywords_ids;
	std::set<unsigned int> c_meta_keywords_ids;
	db_mkw.clear();
	
	std::vector<unsigned int> x;
	if(N_KW%N_BINS==0)
	{
		for(unsigned int i=1;i<=N_BINS;++i)
		{
			bin_boundaries.push_back(i*bin_stride);
		}
		
	}
	else
	{
		for(unsigned int i=1;i<N_BINS;++i)
		{
			bin_boundaries.push_back(i*bin_stride);
		}

		bin_boundaries.push_back(N_KW);
	}
	
	int pr=0;
	unsigned int prev=0;
	
	
	for(auto n: bin_boundaries)
	{
		string p;
		if((n-prev)% bin_stride != 0)
		{
			string all_zero1((n-prev),'0');
			string all_one1((n-prev), '1');
			for(unsigned int i = prev;i<n;++i)
			{
				for(unsigned int j=i+1;j<=n;++j)
				{
					s.clear();
					s+=string(prev,'0');
					s += std::string(i-prev,'1');
					s += std::string(j-i,'0');
					s += std::string(n-j,'1');
					ss = s.substr(s.length() - n + prev);
					if(ss.compare(all_zero1)!=0 && ss.compare(all_one1)!=0)
					{
						db_mkw.push_back(s);
					}

				}
			}
		}

		else
		{
			for(unsigned int i = prev;i<n;++i)
			{
				for(unsigned int j=i+1;j<=n;++j)
				{
					s.clear();
					s+=string(prev,'0');
					s += std::string(i-prev,'1');
					s += std::string(j-i,'0');
					s += std::string(n-j,'1');
					ss = s.substr(s.length() - bin_stride);
					if(ss.compare(all_zero)!=0 && ss.compare(all_one)!=0)
					{ 
						db_mkw.push_back(s);
					}
					
				}	
			}
		}
		prev=n;   
	}

	int j=0;
	meta_db.clear();
	for(auto m:db_mkw)
	{
		meta_keywords_ids.clear();
		for(unsigned int i=0;i<m.size();++i)
		{
			if(m[i]=='1')
			{
				meta_keywords_ids.insert(raw_db[i].begin(), raw_db[i].end());
			}
		}
		
		meta_db.push_back(meta_keywords_ids);
		frequency.push_back(meta_keywords_ids.size());
		j++;
	}
	
	cout<< "\n>>  MetaDB Size (#MKW) = "<< meta_db.size() << "\n";
	
	int sum = 0;
	for(int i=0; i<frequency.size();i++)
	{
		sum+= frequency[i];
	}
	cout<< ">>  No. of unique (mkw-id) pairs = "<< sum <<"\n";

	convert_mkws_digest(db_mkw,db_mkw_hash);

	// cout<<"Max mkw frequency:"<< *max_element(frequency.begin(), frequency.end());
	// cout<<"\n";
	// for(auto i:frequency)
	// 	cout<<i<<"\n";


	return 0;

}


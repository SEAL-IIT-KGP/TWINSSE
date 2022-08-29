import sys


byte = 194
keywords = [6043, 22087, 44000, 133958]
meta_keywords = [32400, 118800, 262872, 723365]         	     
mkw_id_pair = [1049941, 5220439, 15954070, 78805463]              

Server_Storage = []
Client_Storage = []

#Need to check the calculation once more

def ClientStorage(meta_keywords, keywords):
    client_storage = ((meta_keywords + keywords) * sys.getsizeof(int())/2**20)      #in MB
    return client_storage


def ServerStorage(mkw_id_pair, byte):
    server_storage = (((mkw_id_pair * byte)/2**20))                                #in MB
    return server_storage



for i in mkw_id_pair:
    Server_Storage.append(ServerStorage(i, byte))

for k in range(0, len(keywords)):
    Client_Storage.append(ClientStorage(meta_keywords[k], keywords[k]))



print("\nServer storage (MB): \n")
for i in Server_Storage:
    print(i)


print("\nClient storage (MB): \n")
for j in Client_Storage:
    print(j)




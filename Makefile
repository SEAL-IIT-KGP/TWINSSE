all:
	@echo "\nBuilding precision...\n"
	$(MAKE) -C ./precision all
	
	@echo "\nBilding conjuntive...\n"
	$(MAKE) -C ./conjunctive all
	
	@echo "\nBilding disjuntive...\n"
	$(MAKE) -C ./disjunctive all
	
	@echo "\nBilding DNF...\n"
	$(MAKE) -C ./dnf all
	
	@echo "\nBilding CNF...\n"
	$(MAKE) -C ./cnf all

blake_lib:
	@echo "\nCompling Blake3 lib in conjuntive...\n"
	$(MAKE) -C ./conjunctive/blake3/ lib
	
	@echo "\nCompling Blake3 lib in disjuntive...\n"
	$(MAKE) -C ./disjunctive/blake3/ lib
	
	@echo "\nCompling Blake3 lib in DNF...\n"
	$(MAKE) -C ./dnf/blake3/ lib
	
	@echo "\nCompling Blake3 lib in CNF...\n"
	$(MAKE) -C ./cnf/blake3/ lib

.PHONEY: clean clean_all

clean:
	@echo "\nCleaning precision...\n"
	$(MAKE) -C ./precision clean

	@echo "\nCleaning conjunctive...\n"
	$(MAKE) -C ./conjunctive clean
	
	@echo "\nCleaning disjunctive...\n"
	$(MAKE) -C ./disjunctive clean
	
	@echo "\nCleaning DNF...\n"
	$(MAKE) -C ./dnf clean
	
	@echo "\nCleaning CNF...\n"
	$(MAKE) -C ./cnf clean

clean_all:
	@echo "\nCleaning all precision...\n"
	$(MAKE) -C ./precision clean_all
	
	@echo "\nCleaning all conjunctive...\n"
	$(MAKE) -C ./conjunctive clean_all
	
	@echo "\nCleaning all disjunctive...\n"
	$(MAKE) -C ./disjunctive clean_all
	
	@echo "\nCleaning all DNF...\n"
	$(MAKE) -C ./dnf clean_all
	
	@echo "\nCleaning all CNF...\n"
	$(MAKE) -C ./cnf clean_all
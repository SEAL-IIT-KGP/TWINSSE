all:
	@echo "\nBuilding precision...\n"
	$(MAKE) -C ./precision all
	
	@echo "\nBuilding conjunctive...\n"
	$(MAKE) -C ./conjunctive all
	
	@echo "\nBuilding disjunctive...\n"
	$(MAKE) -C ./disjunctive all
	
	@echo "\nBuilding DNF...\n"
	$(MAKE) -C ./dnf all
	
	@echo "\nBuilding CNF...\n"
	$(MAKE) -C ./cnf all

blake_lib:
	@echo "\nCompiling Blake3 lib in conjunctive...\n"
	$(MAKE) -C ./conjunctive/blake3/ lib
	
	@echo "\nCompiling Blake3 lib in disjunctive...\n"
	$(MAKE) -C ./disjunctive/blake3/ lib
	
	@echo "\nCompiling Blake3 lib in DNF...\n"
	$(MAKE) -C ./dnf/blake3/ lib
	
	@echo "\nCompiling Blake3 lib in CNF...\n"
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

	@echo "\nCleaning Redis...\n"
	@redis-cli flushall
	@redis-cli save
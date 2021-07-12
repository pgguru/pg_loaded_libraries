EXTENSION = pg_loaded_libraries
MODULE_big = pg_loaded_libraries
DATA = pg_loaded_libraries--0.0.1.sql
OBJS = pg_loaded_libraries.o 
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

# pg_loaded_libraries

This extension returns the list of loaded libraries in the given backend in a SQL-consumable format.

## Usage

```console
$ git clone git@github.com:pgguru/pg_loaded_libraries.git
$ cd pg_loaded_libraries
$ make PG_CONFIG=path/to/pg_config && make install PG_CONFIG=path/to/pg_config
$ psql -c 'CREATE EXTENSION pg_loaded_libraries'
```

This will create the extension which exposes a single SQL function, `pg_loaded_libraries`.

Simply execute this function in a given backend to expose the shared libraries which this backend has currently installed:

```
postgres=# select * from pg_loaded_libraries();
                        pg_loaded_libraries
-----------------------------------------------------------------
 /Users/pgguru/pg/local/14/lib/postgresql/pg_loaded_libraries.so
(1 row)
```


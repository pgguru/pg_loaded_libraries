#include "postgres.h"
#include "fmgr.h"
#include "funcapi.h"
#include "miscadmin.h"

PG_MODULE_MAGIC;

void _PG_init(void);
void _PG_fini(void);

Datum pg_loaded_libraries(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(pg_loaded_libraries);

void _PG_init(void)
{
	/* ... C code here at time of extension loading ... */
}

void _PG_fini(void)
{
	/* ... C code here at time of extension unloading ... */
}

Datum pg_loaded_libraries(PG_FUNCTION_ARGS)
{
	ReturnSetInfo * rsinfo = (ReturnSetInfo *)fcinfo->resultinfo;
	bool		randomAccess;
	TupleDesc	tupdesc;
	Tuplestorestate *tupstore;
	AttInMetadata *attinmeta;
	MemoryContext oldcontext;
	Size datsize;

	/* check to see if caller supports us returning a tuplestore */
	if (rsinfo == NULL || !IsA(rsinfo, ReturnSetInfo))
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("set-valued function called in context that cannot accept a set")));
	if (!(rsinfo->allowedModes & SFRM_Materialize))
		ereport(ERROR,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("materialize mode required, but it is not allowed in this context")));

	/* The tupdesc and tuplestore must be created in ecxt_per_query_memory */
	oldcontext = MemoryContextSwitchTo(rsinfo->econtext->ecxt_per_query_memory);

	tupdesc = CreateTemplateTupleDesc(1);
	TupleDescInitEntry(tupdesc, (AttrNumber) 1, "libname",
					   TEXTOID, -1, 0);
	attinmeta = TupleDescGetAttInMetadata(tupdesc);

	randomAccess = (rsinfo->allowedModes & SFRM_Materialize_Random) != 0;
	tupstore = tuplestore_begin_heap(randomAccess, false, work_mem);
	rsinfo->returnMode = SFRM_Materialize;
	rsinfo->setResult = tupstore;
	rsinfo->setDesc = tupdesc;

	MemoryContextSwitchTo(oldcontext);

	datsize = EstimateLibraryStateSpace();

	if (datsize) {
		char *space;
		HeapTuple tuple;
        
		space = palloc(datsize);
		SerializeLibraryState(datsize, space);

		while (*space) {
			tuple = BuildTupleFromCStrings(attinmeta, &space);
			tuplestore_puttuple(tupstore, tuple);
			space += strlen(space);
		}
	}
	PG_RETURN_NULL();
}

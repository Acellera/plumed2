#include "Plumed.h"
#include "tcl.h"		/* needed by aceplug :( */
#include "aceplug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#define ERROR(a) { printf("%s\n", (a)); exit(1); }

struct p {
	plumed plumed;
	float  *pos;
	float  *frc;
	float box[3][3];
	float vir[3][3];
};

#define P  ( ((struct p*) (s->privdata))->plumed ) 
#define Ppos ( ((struct p*) (s->privdata))->pos ) 
#define Pfrc ( ((struct p*) (s->privdata))->frc ) 
#define Pbox ( ((struct p*) (s->privdata))->box ) 
#define Pvir ( ((struct p*) (s->privdata))->vir ) 

aceplug_err_t aceplug_init( aceplug_sim_t *s, int argc, char **argkey, char **argval ) {
	int   i;
	float f;

	printf("# ACEMD plugin versions: %d / %d\n", s->version, s->plugin_version());

	/* pluginarg named "input" and "log" are the only configurable
	 * parameters. We assume strings from acemd are
	 * null-terminated. */
	char plumed_dat[PATH_MAX]="plumed.dat";
	char plumed_log[PATH_MAX]="plumed.log";

	for(int c=0; c<argc; c++) {
	    if(strncmp(argkey[c],"input",6)==0) 
		strncpy(plumed_dat,argval[c],PATH_MAX);
	    if(strncmp(argkey[c],"log",4)==0)
		strncpy(plumed_log,argval[c],PATH_MAX);
	}
	
	s->privdata = (struct p*) malloc( sizeof( struct p ) );
	memset( s->privdata, 0 , sizeof( struct p ) );
	Ppos = (float*) malloc( sizeof(float) * s->natoms * 3 );
	Pfrc = (float*) malloc( sizeof(float) * s->natoms * 3 );
	
	P = plumed_create();

	int plumed_api_version=0;
	plumed_cmd( P, "getApiVersion",&plumed_api_version);
	printf("# PLUMED2 Api Version: %d\n",plumed_api_version);

	i=4;
	plumed_cmd( P, "setRealPrecision", &i );

	/* Toni: 4.184 is for thermochemical calories.
	 * MD_KCAL_TO_KJ=4.1868 is for international calories. Going
	 * with the value used in most MD engines.  */
	f=4.184;
	plumed_cmd( P, "setMDEnergyUnits", &f );

	f=0.1;
	plumed_cmd( P, "setMDLengthUnits", &f );

	f= 0.001;
	plumed_cmd( P, "setMDTimeUnits", &f );

	plumed_cmd( P, "setPlumedDat", plumed_dat );
	plumed_cmd( P, "setNatoms"   , &(s->natoms) );
	plumed_cmd( P, "setMDEngine" , "acemd" );
	plumed_cmd( P, "setLogFile"  , plumed_log );
	plumed_cmd( P, "setTimestep" , &(s->timestep_fs) );
	plumed_cmd( P, "init"        , NULL );
	memset( Pbox   , 0, sizeof(float) * 9 );
	memset( Pvir, 0, sizeof(float) * 9 );
	Pbox[0][0]    = s->box.x;
	Pbox[1][1]    = s->box.y;
	Pbox[2][2]    = s->box.z;

	printf("# PLUMED2 input file: %s\n", plumed_dat);
	printf("# PLUMED2 log file: %s\n", plumed_log);

	return ACEPLUG_OK;
}

aceplug_err_t aceplug_calcforces( aceplug_sim_t *s ) {
	int i=0;

	if( s->plugin_load_positions() ) { ERROR( "plugin_load_positions") ; }
	if( s->plugin_load_forces() ) { ERROR( "plugin_load_forces") ; }

	for( i=0; i< s->natoms; i++ ) {
		Ppos[ i*3 + 0 ] = s->pos[i].x;
		Ppos[ i*3 + 1 ] = s->pos[i].y;
		Ppos[ i*3 + 2 ] = s->pos[i].z;

		Pfrc[ i*3 + 0 ] = s->frc[i].x;
		Pfrc[ i*3 + 1 ] = s->frc[i].y;
		Pfrc[ i*3 + 2 ] = s->frc[i].z;
	}

	Pbox[0][0]    = s->box.x;
	Pbox[1][1]    = s->box.y;
	Pbox[2][2]    = s->box.z;

	plumed_cmd( P, "setStep", &(s->step) );
	plumed_cmd( P, "setMasses"   , (void*)(s->mass) );
	plumed_cmd( P, "setCharges"  , (void*)(s->charge) );
	plumed_cmd( P, "setForces"   , Pfrc );  
	plumed_cmd( P, "setPositions", Ppos );  
	plumed_cmd( P, "setBox"   , Pbox );
	plumed_cmd( P, "setVirial", Pvir );
	plumed_cmd( P, "calc"   , NULL );

	for( i=0; i < s->natoms; i++ ) {
		s->frc[i].x = Pfrc[ i*3 + 0 ];
		s->frc[i].y = Pfrc[ i*3 + 1 ];
		s->frc[i].z = Pfrc[ i*3 + 2 ];
	}

	if( s->plugin_update_forces() ) { ERROR( "plugin_update_forces") ; }
	return ACEPLUG_OK;
}

aceplug_err_t aceplug_endstep( aceplug_sim_t *s ) {
	return ACEPLUG_OK;
}

aceplug_err_t aceplug_terminate( aceplug_sim_t *s ) {
	plumed_finalize( P );
	return ACEPLUG_OK;
}


# RCS:         $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/scripts/config-template.pl,v 1.1 2002/02/18 21:16:42 tmwong Exp $
# Description: Template file for fscachesim-run.pl configuration file.
# Author:      T.M. Wong <tmwong+@cs.cmu.edu>

# Fully-qualified fscachesim binary name.

$config_bin = "/path/fscachesim";

# Path to trace files, for each trace set.

%config_trPathTable) =
  (
   "db2", "/path/db2",
   "httpd", "/path/httpd",
   "openmail", "/path/openmail"
  );

# Path into which to dump result files. Results file names have the form:
#
# $config_resultsPath/traceSet-DEMOTETYPE}-{ARRAYTYPE}-{CLIENTSIZE}-{ARRAYSIZE}
#
# where
#
# DEMOTETYPE : NONE, DEMOTE
# ARRAYTYPE : LRU, MRULRU, NSEGEXP, NSEGUNI, RSEGEXP, RSEGUNI
# CLIENTSIZE : (in MB)
# ARRAYSIZE : (in MB)

my ($config_resultPath) = "/path/results";

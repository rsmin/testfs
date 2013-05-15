#!/usr/bin/perl -w
#
# RCS:         $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/scripts/run.pl,v 1.3 2002/03/14 22:28:22 tmwong Exp $
# Description: Wrapper script for fscachesim
# Author:      T.M. Wong <tmwong+@cs.cmu.edu>

# Usage:
#
# fscachesim-run.pl TRACESET CLIENTSIZE ARRAYSIZE ARRAYTYPES...
#
# where:
#
# TRACESET:   One of the trace sets from the [Wong2002]. Valid sets:
#             db2 httpd openmail
# CLIENTSIZE: The size of the simulated client caches, in MB.
# ARRAYSIZE:  The size of the simulated array cache, in MB.
# ARRAYTYPES: A space-separated list of arraytypes to simulate. The script will
#             run fscachesim and create a separate result file for each array
#             type. Valid types:
#             LRU MRULRU NSEGEXP NSEGUNI RSEGEXP RSEGUNI
#
# Warnings:
#
# The script does not any input parameter validation.
#
# The script always simulates demoting clients for all array types except LRU.

use strict;

use File::Basename;

# Assume that we will run the wrapper script from the fscachesim source
# directory 90% of the time.

push(@INC, "./scripts");

require "config.pl";

use vars qw($config_bin %config_trPathTable $config_resultPath);

my ($progName) = basename($0);

my (%blockSizeTable) =
  (
   "db2", "",
   "db2single", "",
   "httpd", "",
   "httpdsingle", "",
   "httpdtest", "",
   "openmail", "",
   "randompanth", "",
   "tpch", "-b 16384",
   "zipfpanth", ""
  );

my (%demoteTable) =
  (
   "LRU", "",
   "MRULRU", "-d",
   "NSEGEXP", "-d",
   "NSEGUNI", "-d",
   "RSEGEXP", "-d",
   "RSEGUNI", "-d"
  );

my (%warmupTimeTable) =
  (
   "db2", "-w 1800",
   "db2single", "-w 1800",
   "httpd", "-w 3600",
   "httpdsingle", "-w 3600",
   "httpdtest", "",
   "openmail", "-w 600",
   "randompanth", "-c 32768",
   "tpch", "-w 600",
   "zipfpanth", "-c 49152"
  );

my (%trTypeTable) =
  (
   "db2", "-m",
   "db2single", "",
   "httpd", "-m",
   "httpdsingle", "",
   "httpdtest", "-m",
   "openmail", "",
   "randompanth", "",
   "tpch", "",
   "zipfpanth", ""
  );

my (%trFilesTable) =
  (
   "db2", "db2.server.0.trace db2.server.1.trace db2.server.2.trace db2.server.3.trace db2.server.4.trace db2.server.5.trace db2.server.6.trace db2.server.7.trace",
   "db2single", "db2.server.single",
   "httpd", "httpd.server.1.trace httpd.server.2.trace httpd.server.3.trace httpd.server.4.trace httpd.server.5.trace httpd.server.6.trace httpd.server.7.trace",
   "httpdsingle", "httpd.server.single",
   "httpdtest", "httpd.server.1.trace",
   "openmail", "i3125om1.fscachesim i3125om2.fscachesim i3125om3.fscachesim i3125om4.fscachesim i3125om5.fscachesim i3125om6.fscachesim",
   "randompanth", "random-panth-11",
   "tpch", "tput0.trace.fscachesim",
   "zipfpanth", "zipf-panth-11"
  );

sub usage {
  print "Usage: $progName TRACESET CLIENTSIZE ARRAYSIZE ARRAYTYPES...\n";
}

sub trFilesGet{
  my ($trSet, $trPath) = @_;
  my ($trFiles) = "";
  my ($i);

  foreach $i (split(/\s/,$trFilesTable{$trSet})) {
    $trFiles .= "$trPath/$i ";
  }

  return ($trFiles);
}

sub runSim {
  my ($trSet, $clientSize, $arraySize, @arrayTypes) = @_;

  my ($blockSize) = $blockSizeTable{$trSet};

  my ($trFiles) = trFilesGet($trSet, $config_trPathTable{$trSet});
  my ($trType) = $trTypeTable{$trSet};

  my ($warmupTime) = $warmupTimeTable{$trSet};

  my ($arrayType);
  foreach $arrayType (@arrayTypes) {
    my ($demoteFlag) = $demoteTable{$arrayType};

    my ($cmdline) =
      "$config_bin " .
      "$blockSize " .
      "$demoteFlag " .
      "$trType " .
      "-o $config_resultPath/$trSet " .
      "$warmupTime " .
      "$arrayType " .
      "$clientSize $arraySize " .
      "$trFiles";

    print "$cmdline\n";
    `$cmdline`;
  }
}

# Validate some of the command line args.

if ($#ARGV < 3) {
  usage();
  exit;
}

runSim(@ARGV);

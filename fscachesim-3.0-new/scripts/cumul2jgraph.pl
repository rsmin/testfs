#!/usr/bin/perl -w
#
# RCS:         $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/scripts/cumul2jgraph.pl,v 1.5 2002/03/14 22:28:22 tmwong Exp $
# Description: Convert a raw LRU cumulative file to a .jgraph file.
# Author:      T.M. Wong <tmwong+@cs.cmu.edu>

use strict;
use vars qw($glblProgname);

use File::Basename;
use Getopt::Std;

$glblProgname = basename($0);

sub usage {
  print "Usage: $glblProgname workload LRU-cumul_filename\n";
  exit(0);
}

if ($#ARGV < 1) {
  usage();
}

open(CUMULFILE, "<$ARGV[1]")
  or die("%glblProgname: $1");

print <<EOF;
newgraph
  legend defaults font Helvetica
  title y 1.1 font Helvetica : Cumulative hit rate vs. cache size - $ARGV[0]

xaxis
  hash_labels font Helvetica
  label font Helvetica-Oblique : Cache size (MB)
  min 0

yaxis
  hash_labels font Helvetica
  label font Helvetica-Oblique : Cumulative hit rate
  min 0
  max 1.0

(* the cumulative curve *)

newcurve
  linetype solid
  marktype none
  pts
0 0
EOF

my ($cacheSizePrev) = 0;
my ($cumulHitRatePrev) = 0.0;
while (<CUMULFILE>) {
  my ($cacheSize, $cumulHitRate) = split(/\s/, $_);

  if ($cumulHitRate != $cumulHitRatePrev) {
    print "$cacheSizePrev $cumulHitRatePrev\n";

    $cacheSizePrev = $cacheSize;
    $cumulHitRatePrev = $cumulHitRate;
  }
}

close(CUMULFILE)
  or die("%glblProgname: $1");

#!/usr/bin/tclsh

proc arrayPuts name {
    upvar $name a
    foreach element [lsort [array names a]] {
	puts "$element = $a($element)";
    }
}

proc typeGet {stanza} {
    set name [lindex $stanza 0]

    regexp -nocase {(^[a-z0-9]+)[\.]?} $name match type

    return $type
}

proc nameGet {stanza} {
    set name [lindex $stanza 0]

    regexp -nocase {^[a-z0-9]+[\.](.*)} $name match subName

    return $subName
}

proc latencyGet {stats} {
    upvar $stats a

    foreach client [lsort [array names a]] {
	lappend latencies [format "%.2f" [lindex $a($client) 9]]
    }

    return $latencies
}

proc latencyZeroGet {stats} {
    upvar $stats a

    foreach client [lsort [array names a]] {
	lappend latencies [format "%.2f" [lindex $a($client) 10]]
    }

    return $latencies
}

proc clientHitRateGet {stats} {
    upvar $stats a

    foreach client [lsort [array names a]] {
	lappend rates [expr round([lindex $a($client) 6] * 100)]
    }

    return $rates
}

proc arrayHitRateGet {stats} {
    upvar $stats a

    foreach client [lsort [array names a]] {
	lappend rates [expr round([lindex $a($client) 7] * 100)]
    }

    return $rates
}

proc speedupGet {base exp} {
    upvar $base b
    upvar $exp e

    foreach cl [lsort [array names b]] {
	lappend speedups \
	    [format "%.2f" [expr [lindex $b($cl) 9] / [lindex $e($cl) 9]]]
    }

    lappend speedups [format "%.2f" [meanGeo $speedups]]

    return $speedups
}

proc speedupZeroGet {base exp} {
    upvar $base b
    upvar $exp e

    foreach cl [lsort [array names b]] {
	lappend speedups \
	    [format "%.2f" [expr [lindex $b($cl) 9] / [lindex $e($cl) 10]]]
    }

    lappend speedups [format "%.2f" [meanGeo $speedups]]

    return $speedups
}

proc basename {name} {
    set path [split $name "/"]

    return [lindex $path [expr [llength $path] - 1]]
}

proc meanGeo {values} {
    set mean 1

    foreach i $values {
	set mean [expr $mean * $i]
    }

    return [expr pow($mean,[expr 1 / double([llength $values])])]
}

proc perOrigParse {stanza results} {
    upvar $results perOrig

    for {set i 1} {$i < [llength $stanza]} {incr i 1} {
	regexp {(.*)\=(.*)} [lindex $stanza $i] match name val
	set perOrig($name) $val
    }
}

proc StoreCacheParse {stanza readHits readMisses demoteMisses} {
    upvar $readHits rh
    upvar $readMisses rm
    upvar $demoteMisses dm

    perOrigParse [lindex $stanza 2] rh
    perOrigParse [lindex $stanza 3] rm
    perOrigParse [lindex $stanza 6] dm
}

proc StoreCacheSegParse {stanza readHits readMisses demoteMisses} {
    upvar $readHits rh
    upvar $readMisses rm
    upvar $demoteMisses dm

    StoreCacheParse [lindex $stanza 5] rh rm dm
}

proc StoreCacheSimpleParse {stanza readHits readMisses demoteMisses} {
    upvar $readHits rh
    upvar $readMisses rm
    upvar $demoteMisses dm

    StoreCacheParse [lindex $stanza 4] rh rm dm
}

proc resultsParse {resultsFilename resultsParsed} {
    upvar $resultsParsed stats

    set resultsFileH [open $resultsFilename r]

    set resultsText {};

    while {[gets $resultsFileH line] >= 0} {
	lappend resultsText $line
    }

    close $resultsFileH

    eval set resultsRaw {[join $resultsText " "]}

    set resultsArray [lindex $resultsRaw 0]

    if {[string compare "StoreCacheSeg" [typeGet $resultsArray]] == 0} {
	StoreCacheSegParse $resultsArray \
	    readHitsArray readMissesArray demoteMissesArray
    } elseif {[string compare "StoreCacheSimple" [typeGet $resultsArray]] == 0} {
	StoreCacheSimpleParse $resultsArray \
	    readHitsArray readMissesArray demoteMissesArray
    } else {
	error "Unrecognized StoreCache subclass"
    }

    set nameClients {};
    for {set i 1} {$i < [llength $resultsRaw]} {incr i 1} {
	set name [nameGet [lindex $resultsRaw $i]]

	lappend nameClients $name
	StoreCacheSimpleParse [lindex $resultsRaw $i] \
	    readHitsClients readMissesClients demoteMissesClients
    }

    foreach i $nameClients {
	set rhc 0
	catch {
	    set rhc $readHitsClients($i)
	}
	set rmc $readMissesClients($i)

	set rha 0
	catch {
	    set rha $readHitsArray($i)
	}

	set rma $readMissesArray($i)

	set dma 0
	catch {
	    set dma $demoteMissesArray($i)
	}

	set total [expr $rhc + $rmc]

	set rhrc [expr double($rhc) / $total]
	set rhra [expr double($rha) / $total]
	set dmra [expr double($dma) / $total]

	# Change latencies here.

	set lat [expr 0.2 * ($rhra + $dmra) + 5.2 * (1 - $rhrc - $rhra)]
	set latzero [expr 0.2 * $rhra + 5.2 * (1 - $rhrc - $rhra)]

	set rhrc [format "%.2f" $rhrc]
	set rhra [format "%.2f" $rhra]
	set dmra [format "%.2f" $dmra]

	set stats($i) \
	    [list \
		 $rhc \
		 $rmc \
		 $rha \
		 $rma \
		 $dma \
		 $total \
		 $rhrc \
		 $rhra \
		 $dmra \
		 $lat \
		 $latzero]
    }
}

if {$argc < 1} {
    error "Usage: fscachesim-tabulate.tcl baselineFilename expFilenames..."
}

set baseFilename [lindex $argv 0]

resultsParse $baseFilename baseStats

puts [basename $baseFilename]
puts -nonewline "Cli. hit rate\t"
puts [join [clientHitRateGet baseStats] "\t"]
puts -nonewline "Arr. hit rate\t"
puts [join [arrayHitRateGet baseStats] "\t"]
puts -nonewline "Mean lat.\t"
puts [join [latencyGet baseStats] "\t"]

foreach i [lrange $argv 1 end] {
    resultsParse $i expStats

    puts [basename $i]
    puts -nonewline "Arr. hit rate\t"
    puts [join [arrayHitRateGet expStats] "%\t"]
    puts -nonewline "Mean lat. (ms)\t"
    puts [join [latencyGet expStats] "\t"]
    puts -nonewline "Speedup  \t"
    puts [join [speedupGet baseStats expStats] "\t"]
    puts -nonewline "Mean lat0. (ms)\t"
    puts [join [latencyZeroGet expStats] "\t"]
    puts -nonewline "Speedup0\t"
    puts [join [speedupZeroGet baseStats expStats] "\t"]
}

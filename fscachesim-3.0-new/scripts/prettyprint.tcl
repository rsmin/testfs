#!/usr/bin/tclsh

proc stanzaPrint {stanza indentLevel} {
    set indent ""
    for {set i 0} {$i < $indentLevel} {incr i 1} {
	set indent [format "\t%s" $indent]
    }

    # If the first element of the list is followed by an '=', the input
    # stanza is an attribute.

    set name [lindex $stanza 0]
    if {[regexp -nocase {.*=} $name]} {
	puts "$indent\{$name\}"
    } else {
	puts "$indent\{$name"
	for {set i 1} {$i < [llength $stanza]} {incr i 1} {
	    stanzaPrint [lindex $stanza $i] [expr $indentLevel + 1]
	}
	puts "$indent\}"
    }
}

if {$argc != 1} {
    error "Usage: fscachesim-tabulate.tcl fileName"
}

set resultFileH [open [lindex $argv 0] r]

set resultText {};

while {[gets $resultFileH line] >= 0} {
    lappend resultText $line
}

close $resultFileH

eval set resultList {[join $resultText " "]}

# For each client, extract a tuple of the form:
#
# {name cl-hits cl-misses arr-read-hits arr-read-misses arr-demote-misses}

for {set i 0} {$i < [llength $resultList]} {incr i 1} {
    stanzaPrint [lindex $resultList $i] 0
}

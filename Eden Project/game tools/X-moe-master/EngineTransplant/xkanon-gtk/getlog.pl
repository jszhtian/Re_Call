#!/usr/bin/perl
#
# cvs のログから修正日、修正のあったファイル、コメント / 修正日を
# コメントが unique になるように切り出す

open(IN, "cvs log @ARGV|") || die("Cannot execute cvs\n");

sub cmp_date($$) {
	my (@date1) = split(/[ :;\/]/,$a->{"date"});
	my (@date2) = split(/[ :;\/]/,$b->{"date"});

	my ($i);
	for ($i=0; $i<6; $i++) {
		if ($date1[$i] > $date2[$i]) {return 1;}
		elsif ($date1[$i] < $date2[$i]) {return -1;}
	}
	return 0;
}

while(<IN>) {
	chomp;
	if (/^Working file: (.*)$/) {
		$file = $1;
		next;
	}
	if (/^revision (.*)$/) {
		$revision = $1;
		next;
	}
	if (/^date: (.*)$/) {
		$state = 1;
		$date = $1;
		next;
	}
	if (/^---/ || /^===/) {
		$state = 0;
		if ($revision eq "") { next; }
		my (%data);
		%data = (
			"line" => $line,
			"revision" => $revision,
			"file" => $file,
			"date" => $date
		);
		push @all_data, \%data;
		$line = ""; $revision=""; $date="";
	}
	if (/^Initial revision/) { next;}
	if (/^branches/) { next;}
	if (/^$/) { next; }
	if ($state) {
		$line .= $_; $line .= "\n";
	}
}

@sorted_data = sort cmp_date @all_data;
$prev_f = "";
foreach $i (@sorted_data) {
	if ($prev_f ne $i->{"line"} && $i->{"line"} ne "") {
		print "-------------------------\n";
		print $prev_f;
		print "=========================\n\n\n";
		$prev_f = $i->{"line"};
	}
	printf "%25s %10s date %s\n",$i->{"file"},$i->{"revision"},$i->{"date"};
}
print "-------------------------\n";
print $prev_f;
print "=========================\n\n\n";
$prev_f = $i->{"line"};

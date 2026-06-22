#!/usr/bin/perl -w

# created based on tzdump.pl

use strict;

sub timeoffset
{
    my $value = shift;
    my $seconds = $value % 60;
    my $minutes = ($value / 60) % 60;
    my $hours = $value / 3600;
    sprintf "%+03d:%02d:%02d",$hours,$minutes,$seconds;
}

# mtwtfss mtwtfss 

sub timerule
{
    my ($time,$isgmt,$offset) = @_;

    # print "isgmt=$isgmt time=$time offset=$offset\n";

    $time += $offset if(!$isgmt);
    
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$foo) = gmtime($time);
    ++$mon;
    my $nweek = int(($mday-1)/7) + 1;
    $nweek = -1 if($nweek >= 4);
    "$nweek;$wday;$mon";
}

sub dsttime
{
    my ($time,$isstd,$offset1,$offset2) = @_;
    # print "isstd=$isstd time=$time offset1=$offset1 offset2=$offset2\n";

    $time += ($isstd ? $offset1 : $offset2);

    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$foo) = gmtime($time);
    sprintf "%+03d:%02d:%02d",$hour,$min,$sec;
}

sub tzinfo
{
    my $dir = "/usr/share/zoneinfo";
    my $zone = shift;

    if (not open (FILE, "$dir/$zone"))
    {
	warn "open $dir/$zone: $!\n";
	return;
    }

    my $year = 1900 + (gmtime(time))[5];
    
    my $tzfile = {};
    my $data;

    sysread (FILE, $data, 44) or die;

    my ($header, $reserved, $ttisgmtcnt, $ttisstdcnt, $leapcnt, $timecnt,
	$typecnt, $charcnt) = unpack "a4 a16 N N N N N N", $data;

    if ($header ne "TZif")
    {
	warn "$dir/$zone: Doesn't look like tzfile data\n";
	close FILE;
	return;
    }

    # print "ttisgmtcnt=$ttisgmtcnt, ttisstdcnt=$ttisstdcnt, leapcnt=$leapcnt, timecnt=$timecnt, typecnt=$typecnt, charcnt=$charcnt\n";

    my $tzdata;
    my $index = 0;

    sysread (FILE, $tzdata, 5 * $timecnt + 6 * $typecnt + $charcnt +
	     8 * $leapcnt + $ttisstdcnt + $ttisgmtcnt);

    my $tzchunk = sub
    {
	my $howbig = shift;
	my $chunk = substr ($tzdata, $index, $howbig);
	$index += $howbig;
	$chunk;
    };

    my %ttimes;
    my @ttimes;
    my @ttindexes;

    my @dates;  # settings for current year
    my @indexes;

    my $lastindex = -1;
    
    if ($timecnt)
    {
	$data = &$tzchunk (4 * $timecnt);
	@ttimes = unpack ("N" x $timecnt, $data);

	$data = &$tzchunk ($timecnt);
	@ttindexes = unpack "C" x $timecnt, $data;

	for (my $i=0; $i<=$#ttimes; $i++)
	{
	    my $ruleyear = 1900 + (gmtime($ttimes[$i]))[5];

	    $lastindex = $ttindexes[$i];
	    
	    if ($ruleyear == $year)
	    {
		push @dates, $ttimes[$i];
		push @indexes, $ttindexes[$i];
	    }
	}
    }

    my @ttinfo;

    foreach (0 .. $typecnt - 1)
    {
	$data = &$tzchunk (6);
	my ($gmtoff, $isdst, $abbrind) = unpack ("N c C", $data);
	$gmtoff = unpack ("l", pack ("L", $gmtoff));
	push (@ttinfo, { gmtoff => $gmtoff, isdst => $isdst, abbrind => $abbrind });
	# print "ttinfo[$_]: gmtoff=$gmtoff isdst=$isdst abbrind=$abbrind\n";
    }

 
    my $tznames;

    if ($charcnt)
    {
	$tznames = &$tzchunk ($charcnt) or die;
	# print "abbrevs: ", join (", ", split /\000/, $tznames), "\n";
    }

    my @leapsecs;

    if ($leapcnt)
    {
	$data = &$tzchunk (8 * $leapcnt);
	# print "leapsecs: ", join (", ", unpack ("NN" x $leapcnt, $data)), "\n";
    }

    my @stddata;
    if ($ttisstdcnt)
    {
	$data = &$tzchunk ($ttisstdcnt);
	@stddata = unpack ("C" x $ttisstdcnt, $data);
	# print "isstd: ", join (", ", @stddata), "\n";
    }

    my @gmtdata;
    if ($ttisgmtcnt)
    {
	$data = &$tzchunk ($ttisgmtcnt);
	@gmtdata = unpack ("C" x $ttisgmtcnt, $data);
	# print "isgmt: ", join(", ", @gmtdata), "\n";
    }

    close FILE;

    # Print Boost date_time_zoneinfo line

    # for(my $i=0; $i<=$#dates; $i++)
    # {
    #  print scalar gmtime ($dates[$i]), " => ", $indexes[$i], " ", $ttimes[$i], "\n";
    # }

    if ($#dates == 1)
    {
	# ID, STD ABBR, STD NAME, DST ABBR, DST NAME, GMT offset, DST adjustment, Rule1, Time1, Rule2, Time2
	# "Europe/Helsinki","EET","EET","EEST","EEST","+02:00:00","+01:00:00","-1;0;3","+03:00:00","-1;0;10","+04:00:00"
	# Rule = week,day,month
	# week=1...5 or -1 if last week. We interpret week 4/5 to always imply last week
	# day=0...6, 0=Sun
	
	my $isdst1 = $ttinfo[$indexes[0]]{isdst};
	my $isdst2 = $ttinfo[$indexes[1]]{isdst};

	my $abbrind1 = $ttinfo[$indexes[0]]{abbrind};
	my $abbrind2 = $ttinfo[$indexes[1]]{abbrind};

	my $isgmt1 = $gmtdata[$indexes[0]];
	my $isgmt2 = $gmtdata[$indexes[1]];

	my $isstd1 = $stddata[$indexes[0]];
	my $isstd2 = $stddata[$indexes[1]];
	
	my $zname1 = substr($tznames,$abbrind1,index($tznames,"\000",$abbrind1)-$abbrind1);
	my $zname2 = substr($tznames,$abbrind2,index($tznames,"\000",$abbrind2)-$abbrind2);

	if($isdst1)
	{
	    my $tmp = $zname1;
	    $zname1 = $zname2;
	    $zname2 = $tmp;
	}
	
	my $offset1 = $ttinfo[$indexes[0]]{gmtoff};
	my $offset2 = $ttinfo[$indexes[1]]{gmtoff};

	my $gmtoffset = ($isdst1 ? timeoffset($offset2) : timeoffset($offset1));
	my $dstadjustment = timeoffset($isdst1 ? $offset1-$offset2 : $offset2-$offset1);

	my $dststartrule = timerule($isdst1 ? $dates[0] : $dates[1], $isgmt1, $offset1);
	my $dststarttime = ($isdst1 ? dsttime($dates[1],$isstd2,$offset2,$offset1) : dsttime($dates[0],$isstd1,$offset1,$offset2));
	
	my $dstendrule = timerule($isdst1 ? $dates[1] : $dates[0], $isgmt2, $offset2);
	my $dstendtime = ($isdst1 ? dsttime($dates[0],$isstd1,$offset1,$offset2) : dsttime($dates[1],$isstd2,$offset2,$offset1));
	
	print "\"$zone\",\"$zname1\",\"$zname1\",\"$zname2\",\"$zname2\",\"$gmtoffset\",\"$dstadjustment\",\"$dststartrule\",\"$dststarttime\",\"$dstendrule\",\"$dstendtime\"\n";
	
    }
    else
    {
	# Use the last row if everything fails (Asia/Calcutta, Europe/Moscow)

	if($lastindex < 0)
	{
	    $lastindex = $#ttinfo;
	}
	
	# ID, STD ABBR, STD NAME, DST ABBR, DST NAME, GMT offset, DST adjustment, Rule1, Time1, Rule2, Time2
	# "Europe/Moscow","MSK","MSK","","","+03:00:00","+00:00:00","","","","+00:00:00"

	my $isdst = $ttinfo[$lastindex]{isdst};
	my $abbrind = $ttinfo[$lastindex]{abbrind};
	my $isgmt = $gmtdata[$lastindex];
	my $isstd = $stddata[$lastindex];
	
	my $zname = substr($tznames,$abbrind,index($tznames,"\000",$abbrind)-$abbrind);

	my $offset = $ttinfo[$lastindex]{gmtoff};
	my $gmtoffset = timeoffset($offset);
	my $dstadjustment = "+00:00:00";

	my $dststartrule = "";
	my $dststarttime = "";
	
	my $dstendrule = "";
	my $dstendtime = "+00:00:00"; # no idea why the Boost.Date_time does this

	# print "$gmtoffset $offset\n";
	
	print "\"$zone\",\"$zname\",\"$zname\",\"\",\"\",\"$gmtoffset\",\"$dstadjustment\",\"$dststartrule\",\"$dststarttime\",\"$dstendrule\",\"$dstendtime\"\n";
	
    }
}


foreach my $filename (@ARGV)
{
    tzinfo ($filename);
}

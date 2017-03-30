#Copyright (C)2001-2003 Altera Corporation
#Any megafunction design, and related net list (encrypted or decrypted),
#support information, device programming or simulation file, and any other
#associated documentation or information provided by Altera or a partner
#under Altera's Megafunction Partnership Program may be used only to
#program PLD devices (but not masked PLD devices) from Altera.  Any other
#use of such megafunction design, net list, support information, device
#programming or simulation file, or any other related documentation or
#information is prohibited for any other purpose, including, but not
#limited to modification, reverse engineering, de-compiling, or use with
#any other silicon devices, unless such use is explicitly licensed under
#a separate agreement with Altera or a megafunction partner.  Title to
#the intellectual property, including patents, copyrights, trademarks,
#trade secrets, or maskworks, embodied in any such megafunction design,
#net list, support information, device programming or simulation file, or
#any other related documentation or information provided by Altera or a
#megafunction partner, remains with Altera, the megafunction partner, or
#their respective licensors.  No other licenses, including any licenses
#needed under any third party's intellectual property, are provided herein.
#Copying or modifying any file, or portion thereof, to which this notice
#is attached violates this copyright.


































@prog = split (/\/|\\/,$0) ; $pname = $prog[$#prog] ;

$SIG{'INT'} = 'quit';		# run &quit on ^C

&usage if @ARGV < 3;		# die without 3 file args





die if (&args_test(@ARGV) && sleep 5);











$char_stream = "";

select (STDIN); $| = 1;		# make STDIN unbufferred
select (STDOUT);$| = 1;




print "Always wait for the '+' prompt...\n";

while (1) {			# spin until interrupt
    &wait_mutex ($ARGV[0], $sim); # spin until mutex contains '0'
    print "+";			# prompt!
    $char_stream = <STDIN>;	# HACK ALERT!!! Verify this behavior with perl






    my $newline      = "\n";
    my $cr           = "\n";
    my $double_quote = "\"";
    


    $char_stream =~ s/\\n\\r/\n/sg;
    
    $char_stream     =~ s/\\n/$newline/sg;
    $char_stream     =~ s/\\r/$cr/sg;
    $char_stream     =~ s/\\\"/$double_quote/sg;

    my $crlf = "\n\r";
    $char_stream =~ s/\n/$crlf/smg;

    
    open (STREAM,">$ARGV[1]") || die "$pname: $ARGV[1]: $!\n";
    

    my $addr = 0;
    foreach my $char (split (//, $char_stream)) {
	printf STREAM "\@%X\n", $addr++;
	printf STREAM "%X\n", ord($char);
    }

    printf STREAM "\@%X\n", $addr;
    printf STREAM "%X\n", 0;
    
    close (STREAM);


    open (LOG, ">>$ARGV[2]") || die "$pname: $ARGV[2]: $!\n";
    select (LOG); $| = 1;	# make LOG unbufferred
    select (STDOUT);		# reset to default output stream for print
    print LOG "$char_stream";
    close (LOG);

    open (MUTEX, ">$ARGV[0]") || die "$pname: $ARGV[0]: $!\n";
    printf MUTEX "%X\n", $addr;
    close (MUTEX);
} # while TRUE




sub usage {
    die "usage: $pname mutex.dat stream.dat logfile\n";
} # usage

sub quit {			# interrupt handler for exit
    local ($sig) = @_;
    print STDERR "Caught SIG$sig; Closing files and exiting...\n";
    close (MUTEX);
    close(STREAM);
    close   (LOG);
    sleep 1;
    exit;
} # quit
    
sub args_test {			# return 0 for success; return 1 for fail
    local (@files) = @_;
    local (@codes) = (" "," "," "," ");
    local ($die)   = 0;



    if (defined (open (MUTEX, $files[0]))) { 
	close(MUTEX);		# success
    } else {
	$codes[0] = "$!\n";		# remember fail reason
    }
    if (defined (open (STREAM,$files[1]))) {
	close(STREAM);
    } else {
	$codes[1] = "$!\n";
    }
    if (defined (open (LOG,">$files[2]"))) {

	close(LOG);
    } else {
	$codes[2] = "$!\n";
    }				# leave logfile open

    for ($i = 0; $i < 3; $i++) {
	if ($codes[$i] ne " ") {
	    print STDERR "$pname: Cannot open '$files[$i]': $codes[$i]";
	    $die = 1;		# remember any failure
	}
    }

    return $die;		# return any failure status.

} # args_test

sub wait_mutex {		# spin until mutex contains '0'
    local ($mutex,$sim) = @_;	# filenames passed in
    local ($bytes) = 0;		# init scalar number to be read from mutex file

    do {
	open (MUTEX,$mutex);
	while (<MUTEX>) {
	    $bytes = $_;
	    chomp ($bytes);

	    if ($bytes =~ /^@/) { # skip @ddress spec, if present.

		next;
	    }
	    $bytes = hex($bytes);

	    if ($bytes != 0) {

		sleep 1;
	    }
	    last;
	} # while MUTEX
	close(MUTEX);
    } while ($bytes != 0);

} # wait_mutex

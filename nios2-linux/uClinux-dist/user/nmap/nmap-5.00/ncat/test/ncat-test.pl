#!/usr/bin/perl -w

# This file contains tests of the external behavior of Ncat.

require HTTP::Response;
require HTTP::Request;

use MIME::Base64;
use File::Temp qw/ tempfile /;
use URI::Escape;
use Data::Dumper;

use IPC::Open2;
use strict;

my $NCAT = "../ncat";
my $HOST = "localhost";
my $IPV6_ADDR = "::1";
my $PORT = 40000;
my $PROXY_PORT = 40001;

my $BUFSIZ = 1024;

my $num_tests = 0;
my $num_failures = 0;

# Run $NCAT with the given arguments.
sub ncat {
	my $pid;
	local *OUT;
	local *IN;
	# print join(" ", ($NCAT, @_)) . "\n";
	$pid = open2(*OUT, *IN, $NCAT, @_);
	if (!defined $pid) {
		die "open2 failed";
	}
	return ($pid, *OUT, *IN);
}

sub ncat_server {
	return ncat($HOST, $PORT, "-l", @_);
}

sub ncat_client {
	return ncat($HOST, $PORT, @_);
}

# Kill all child processes.
sub kill_children {
	local $SIG{TERM} = "IGNORE";
	kill "TERM", -$$;
	while (waitpid(-1, 0) > 0) {
	}
}

# Read until a timeout occurs. Return undef on EOF or "" on timeout.
sub timeout_read {
	my $fh = shift;
	my $timeout = 1.0;
	if (scalar(@_) > 0) {
		$timeout = shift;
	}
	my $result = "";
	my $rd = "";
	my $frag;
	vec($rd, fileno($fh), 1) = 1;
	while (select($rd, undef, undef, $timeout) != 0) {
		return ($result or undef) if sysread($fh, $frag, $BUFSIZ) == 0;
		$result .= $frag;
		$timeout = 0;
	}
	return $result;
}

$Data::Dumper::Terse = 1;
$Data::Dumper::Useqq = 1;
$Data::Dumper::Indent = 0;
sub d {
	return Dumper(@_);
}

# Run the code reference received as an argument. Count it as a pass if the
# evaluation is successful, a failure otherwise.
sub test {
	my $desc = shift;
	my $code = shift;
	$num_tests++;
	if (eval { &$code() }) {
		print "PASS $desc\n";
	} else {
		$num_failures++;
		print "FAIL $desc\n";
		print "     $@";
	}
}

my ($s_pid, $s_out, $s_in, $c_pid, $c_out, $c_in, $p_pid, $p_out, $p_in);

# Handle a common test situation. Start up a server and client with the given
# arguments and call test on a code block. Within the code block the server's
# PID, output filehandle, and input filehandle are accessible through
#   $s_pid, $s_out, and $s_in
# and likewise for the client:
#   $c_pid, $c_out, and $c_in.
sub server_client_test {
	my $desc = shift;
	my $server_args = shift;
	my $client_args = shift;
	my $code = shift;
	($s_pid, $s_out, $s_in) = ncat_server(@$server_args);
	($c_pid, $c_out, $c_in) = ncat_client(@$client_args);
	test($desc, $code);
	kill_children;
}

sub server_client_test_multi {
	my $protos = shift;
	my $desc = shift;
	my $server_args = shift;
	my $client_args = shift;
	my $code = shift;

	for my $proto (@$protos) {
		if ($proto eq "tcp") {
			server_client_test($desc . " (TCP)",
				$server_args, $client_args, $code);
		} elsif ($proto eq "udp") {
			server_client_test($desc . " (UDP)",
				[@$server_args, ("--udp")], [@$client_args, ("--udp")], $code);
		} elsif ($proto eq "ssl") {
			server_client_test($desc . " (SSL)",
				[@$server_args, ("--ssl", "--ssl-key", "test-cert.pem", "--ssl-cert", "test-cert.pem")], [@$client_args, ("--ssl")], $code);
		} else {
			die "Unknown protocol $proto";
		}
	}
}

# Like server_client_test, but run the test once each for TCP, UDP, and SSL.
sub server_client_test_tcp_udp_ssl {
	server_client_test_multi(["tcp", "udp", "ssl"], @_);
}

sub server_client_test_tcp_ssl {
	server_client_test_multi(["tcp", "ssl"], @_);
}

# Set up a proxy running on $PROXY_PORT and connect a client to it. Start a
# server listening on $PORT for the convenience of having something for the
# proxy to connect to. The proxy is controlled through the variables
#   $p_pid, $p_out, and $p_in.
sub proxy_test {
	my $desc = shift;
	my $proxy_args = shift;
	my $server_args = shift;
	my $client_args = shift;
	my $code = shift;
	($p_pid, $p_out, $p_in) = ncat(($HOST, $PROXY_PORT, "-l", "--proxy-type", "http"), @$proxy_args);
	($s_pid, $s_out, $s_in) = ncat(($HOST, $PORT, "-l"), @$server_args);
	($c_pid, $c_out, $c_in) = ncat(($HOST, $PROXY_PORT), @$client_args);
	test($desc, $code);
	kill_children;
}

sub max_conns_test {
	my $desc = shift;
	my $server_args = shift;
	my $client_args = shift;
	my $count = shift;
	my @client_pids;
	my @client_outs;

	($s_pid, $s_out, $s_in) = ncat_server(@$server_args, ("--max-conns", $count));
	test $desc, sub {
		my ($i, $resp);

		# Fill the connection limit exactly.
		for ($i = 0; $i < $count; $i++) {
			my @tmp;
			($c_pid, $c_out, $c_in) = ncat_client(@$client_args);
			push @client_pids, $c_pid;
			push @client_outs, $c_out;
			syswrite($c_in, "abc\n");
			$resp = timeout_read($s_out, 2.0);
			if (!$resp) {
				syswrite($s_in, "abc\n");
				$resp = timeout_read($c_out);
			}
			$resp = "" if not defined($resp);
			$resp eq "abc\n" or die "--max-conns $count server did not accept client #" . ($i + 1);
		}
		# Try a few more times. Should be rejected.
		for (; $i < $count + 2; $i++) {
			($c_pid, $c_out, $c_in) = ncat_client(@$client_args);
			push @client_pids, $c_pid;
			push @client_outs, $c_out;
			syswrite($c_in, "abc\n");
			$resp = timeout_read($s_out, 2.0);
			if (!$resp) {
				syswrite($s_in, "abc\n");
				$resp = timeout_read($c_out);
			}
			!$resp or die "--max-conns $count server accepted client #" . ($i + 1);
		}
		# Kill one of the connected clients, which should open up a
		# space.
		{
			kill "TERM", $client_pids[0];
			while (waitpid($client_pids[0], 0) > 0) {
			}
			shift @client_pids;
			shift @client_outs;
			sleep 2;
		}
		if ($count > 0) {
			($c_pid, $c_out, $c_in) = ncat_client(@$client_args);
			push @client_pids, $c_pid;
			push @client_outs, $c_out;
			syswrite($c_in, "abc\n");
			$resp = timeout_read($s_out, 2.0);
			if (!$resp) {
				syswrite($s_in, "abc\n");
				$resp = timeout_read($c_out);
			}
			$resp = "" if not defined($resp);
			$resp eq "abc\n" or die "--max-conns $count server did not accept client #$count after freeing one space";
		}
		return 1;
	};
	kill_children;
}

sub max_conns_test_multi {
	my $protos = shift;
	my $desc = shift;
	my $server_args = shift;
	my $client_args = shift;
	my $count = shift;

	for my $proto (@$protos) {
		if ($proto eq "tcp") {
			max_conns_test("$desc (TCP)",
				$server_args, $client_args, $count);
		} elsif ($proto eq "udp") {
			max_conns_test("$desc (UDP)",
				[@$server_args, ("--udp")], [@$client_args, ("--udp")], $count);
		} elsif ($proto eq "ssl") {
			max_conns_test("$desc (SSL)",
				[@$server_args, ("--ssl", "--ssl-key", "test-cert.pem", "--ssl-cert", "test-cert.pem")], [@$client_args, ("--ssl")], $count);
		} else {
			die "Unknown protocol $proto";
		}
	}
}

sub max_conns_test_tcp_ssl {
	max_conns_test_multi(["tcp", "ssl"], @_);
}

sub max_conns_test_tcp_udp_ssl {
	max_conns_test_multi(["tcp", "udp", "ssl"], @_);
}

# Ignore broken pipe signals that result when trying to read from a terminated
# client.
$SIG{PIPE} = "IGNORE";
# Don't have to wait on children.
$SIG{CHLD} = "IGNORE";

# Individual tests begin here.

# Test server with no hostname or port.
($s_pid, $s_out, $s_in) = ncat("-l");
test "Server default listen address and port",
sub {
	my $resp;

	my ($c_pid, $c_out, $c_in) = ncat($HOST);
	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};
kill_children;

# Test server with no hostname.
($s_pid, $s_out, $s_in) = ncat("-l", $HOST);
test "Server default port",
sub {
	my $resp;

	my ($c_pid, $c_out, $c_in) = ncat($HOST);
	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};
kill_children;

# Test server with no port.
($s_pid, $s_out, $s_in) = ncat("-l", $PORT);
test "Server default listen address",
sub {
	my $resp;

	my ($c_pid, $c_out, $c_in) = ncat($HOST, $PORT);
	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};
kill_children;

# Test that the server closes its output stream after a client disconnects.
# This is for uses like
#   ncat -l | tar xzvf -
#   tar czf - <files> | ncat localhost --send-only
# where tar on the listening side could be any program that potentially buffers
# its input. The listener must close its standard output so the program knows
# to stop reading and process what remains in its buffer.
server_client_test_tcp_ssl "Server sends EOF after client disconnect",
[], ["--send-only"], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	close($c_in);
	$resp = timeout_read($s_out) or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
	$resp = timeout_read($s_out);
	!defined($resp) or die "Server didn't send EOF";
};
kill_children;

# Test connection persistence with --keep-open.

($s_pid, $s_out, $s_in) = ncat_server("--keep-open");
test "--keep-open",
sub {
	my $resp;

	my ($c1_pid, $c1_out, $c1_in) = ncat_client();
	syswrite($c1_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";

	my ($c2_pid, $c2_out, $c2_in) = ncat_client();
	syswrite($c2_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};
kill_children;

server_client_test_tcp_udp_ssl "-C translation on input",
["-C"], ["-C"], sub {
	my $resp;
	my $expected = "\r\na\r\nb\r\n---\r\nc\r\nd\r\n---e\r\n\r\nf\r\n";

	syswrite($c_in, "\na\nb\n");
	syswrite($c_in, "---");
	syswrite($c_in, "\r\nc\r\nd\r\n");
	syswrite($c_in, "---");
	syswrite($c_in, "e\n\nf\n");
	$resp = timeout_read($s_out) or die "Read timeout";
	$resp eq $expected or die "Server received " . d($resp) . ", not " . d($expected);

	syswrite($s_in, "\na\nb\n");
	syswrite($s_in, "---");
	syswrite($s_in, "\r\nc\r\nd\r\n");
	syswrite($s_in, "---");
	syswrite($s_in, "e\n\nf\n");
	$resp = timeout_read($c_out) or die "Read timeout";
	$resp eq $expected or die "Client received " . d($resp) . ", not " . d($expected);
};
kill_children;

server_client_test_tcp_udp_ssl "-C server no translation on output",
["-C"], [], sub {
	my $resp;
	my $expected = "\na\nb\n---\r\nc\r\nd\r\n";

	syswrite($c_in, "\na\nb\n");
	syswrite($c_in, "---");
	syswrite($c_in, "\r\nc\r\nd\r\n");
	$resp = timeout_read($s_out) or die "Read timeout";
	$resp eq $expected or die "Server received " . d($resp) . ", not " . d($expected);
};
kill_children;

server_client_test_tcp_ssl "-C client no translation on output",
[], ["-C"], sub {
	my $resp;
	my $expected = "\na\nb\n---\r\nc\r\nd\r\n";

	syswrite($s_in, "\na\nb\n");
	syswrite($s_in, "---");
	syswrite($s_in, "\r\nc\r\nd\r\n");
	$resp = timeout_read($c_out) or die "Read timeout";
	$resp eq $expected or die "Client received " . d($resp) . ", not " . d($expected);
};
kill_children;

# --send-only tests.

server_client_test_tcp_udp_ssl "--send-only client",
[], ["--send-only"], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";

	syswrite($s_in, "abc\n");
	$resp = timeout_read($c_out);
	!$resp or die "Client received \"$resp\" in --send-only mode";
};

server_client_test_tcp_udp_ssl "--send-only server",
["--send-only"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server received \"$resp\" in --send-only mode";

	syswrite($s_in, "abc\n");
	$resp = timeout_read($c_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Client got \"$resp\", not \"abc\\n\"";
};

($s_pid, $s_out, $s_in) = ncat_server("--broker", "--send-only");
test "--send-only broker",
sub {
	my $resp;

	my ($c1_pid, $c1_out, $c1_in) = ncat_client();
	my ($c2_pid, $c2_out, $c2_in) = ncat_client();

	syswrite($s_in, "abc\n");
	$resp = timeout_read($c1_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Client got \"$resp\", not \"abc\\n\"";
	$resp = timeout_read($c2_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Client got \"$resp\", not \"abc\\n\"";

	syswrite($c1_in, "abc\n");
	$resp = timeout_read($c2_out);
	!$resp or die "--send-only broker relayed \"$resp\"";
};
kill_children;

# --recv-only tests.

# Note this test is TCP-only. The --recv-only UDP client never sends anything to
# the server, so the server never knows to start sending its data.
server_client_test_tcp_ssl "--recv-only client",
[], ["--recv-only"], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server received \"$resp\" from --recv-only client";

	syswrite($s_in, "abc\n");
	$resp = timeout_read($c_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Client got \"$resp\", not \"abc\\n\"";
};

server_client_test_tcp_udp_ssl "--recv-only server",
["--recv-only"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";

	syswrite($s_in, "abc\n");
	$resp = timeout_read($c_out);
	!$resp or die "Client received \"$resp\" from --recv-only server";
};

($s_pid, $s_out, $s_in) = ncat_server("--broker", "--recv-only");
test "--recv-only broker",
sub {
	my $resp;

	my ($c1_pid, $c1_out, $c1_in) = ncat_client();
	my ($c2_pid, $c2_out, $c2_in) = ncat_client();

	syswrite($s_in, "abc\n");
	$resp = timeout_read($c1_out);
	!$resp or die "Client received \"$resp\" from --recv-only broker";
	$resp = timeout_read($c2_out);
	!$resp or die "Client received \"$resp\" from --recv-only broker";

	syswrite($c1_in, "abc\n");
	$resp = timeout_read($c2_out);
	!$resp or die "Client received \"$resp\" from --recv-only broker";
};
kill_children;

# HTTP proxy tests.

sub http_request {
	my ($method, $uri) = @_;
	return "$method $uri HTTP/1.0\r\n\r\n";
};

server_client_test "HTTP proxy bad request",
["--proxy-type", "http"], [], sub {
	syswrite($c_in, "bad\r\n\r\n");
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 400 or die "Expected response code 400, got $code";
};

server_client_test "HTTP CONNECT no port number",
["--proxy-type", "http"], [], sub {
	# Supposed to have a port number.
	my $req = http_request("CONNECT", "$HOST");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 400 or die "Expected response code 400, got $code";
};

server_client_test "HTTP CONNECT no port number",
["--proxy-type", "http"], [], sub {
	# Supposed to have a port number.
	my $req = http_request("CONNECT", "$HOST:");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 400 or die "Expected response code 400, got $code";
};

server_client_test "HTTP CONNECT good request",
["--proxy-type", "http"], [], sub {
	my $req = http_request("CONNECT", "$HOST:$PORT");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 200 or die "Expected response code 200, got $code";
};

server_client_test "HTTP CONNECT IPv6 address, no port number",
["--proxy-type", "http", "-6"], ["-6"], sub {
	# Supposed to have a port number.
	my $req = http_request("CONNECT", "[$IPV6_ADDR]");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 400 or die "Expected response code 400, got $code";
};

server_client_test "HTTP CONNECT IPv6 address, no port number",
["--proxy-type", "http", "-6"], ["-6"], sub {
	# Supposed to have a port number.
	my $req = http_request("CONNECT", "[$IPV6_ADDR]:");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 400 or die "Expected response code 400, got $code";
};

server_client_test "HTTP CONNECT IPv6 address, good request",
["--proxy-type", "http", "-6"], ["-6"], sub {
	my $req = http_request("CONNECT", "[$IPV6_ADDR]:$PORT");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 200 or die "Expected response code 200, got $code";
};

# Check that the proxy relays in both directions.
proxy_test "HTTP CONNECT proxy relays",
[], [], [], sub {
	my $req = http_request("CONNECT", "$HOST:$PORT");
	syswrite($c_in, $req);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 200 or die "Expected response code 200, got $code";
	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out) or die "Read timeout";
	$resp eq "abc\n" or die "Proxy relayed \"$resp\", not \"abc\\n\"";
	syswrite($s_in, "def\n");
	$resp = timeout_read($c_out) or die "Read timeout";
	$resp eq "def\n" or die "Proxy relayed \"$resp\", not \"abc\\n\"";
};

# Proxy client shouldn't see the status line returned by the proxy server.
server_client_test "HTTP CONNECT client hides proxy server response",
["--proxy-type", "http"], ["--proxy", "$HOST:$PORT", "--proxy-type", "http"], sub {
	my $resp = timeout_read($c_out);
	!$resp or die "Proxy client sent " . d($resp) . " to the user stream";
};

server_client_test "HTTP CONNECT client, different Status-Line",
[], ["--proxy", "$HOST:$PORT", "--proxy-type", "http"], sub {
	my $resp;
	syswrite($s_in, "HTTP/1.1 200 Go ahead\r\n\r\nabc\n");
	$resp = timeout_read($c_out);
	if (!defined($resp)) {
		die "Client didn't recognize connection";
	} elsif ($resp ne "abc\n") {
		die "Proxy client sent " . d($resp) . " to the user stream";
	}
	return 1;
};

server_client_test "HTTP CONNECT client, server sends header",
[], ["--proxy", "$HOST:$PORT", "--proxy-type", "http"], sub {
	my $resp;
	syswrite($s_in, "HTTP/1.0 200 OK\r\nServer: ncat-test 1.2.3\r\n\r\nabc\n");
	$resp = timeout_read($c_out);
	if (!defined($resp)) {
		die "Client didn't recognize connection";
	} elsif ($resp ne "abc\n") {
		die "Proxy client sent " . d($resp) . " to the user stream";
	}
	return 1;
};

# Check that the proxy doesn't consume anything following the request when
# request and body are combined in one send. Section 3.3 of the CONNECT spec
# explicitly allows the client to send data before the connection is
# established.
proxy_test "HTTP CONNECT server doesn't consume anything after request",
[], [], [], sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\nUser-Agent: ncat-test\r\n\r\nabc\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 200 or die "Expected response code 200, got $code";

	$resp = timeout_read($s_out) or die "Read timeout";
	$resp eq "abc\n" or die "Proxy relayed \"$resp\", not \"abc\\n\"";
};

server_client_test "HTTP CONNECT overlong Request-Line",
["--proxy-type", "http"], [], sub {
	syswrite($c_in, "CONNECT " . ("A" x 24000) . ":$PORT HTTP/1.0\r\n\r\n");
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 413 or $code == 414 or die "Expected response code 413 or 414, got $code";
};

server_client_test "HTTP CONNECT overlong header",
["--proxy-type", "http"], [], sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	for (my $i = 0; $i < 10000; $i++) {
		syswrite($c_in, "Header: Value\r\n");
	}
	syswrite($c_in, "\r\n");
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 413 or die "Expected response code 413, got $code";
};

server_client_test "HTTP GET hostname only",
["--proxy-type", "http"], [], sub {
	my $req = http_request("GET", "$HOST");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 400 or die "Expected response code 400, got $code";
};

server_client_test "HTTP GET path only",
["--proxy-type", "http"], [], sub {
	my $req = http_request("GET", "/");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 400 or die "Expected response code 400, got $code";
};

proxy_test "HTTP GET absolute URI",
[], [], [], sub {
	my $req = http_request("GET", "http://$HOST:$PORT/");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out) or die "Read timeout";
	$resp =~ /^GET \/ HTTP\/1\./ or die "Proxy sent \"$resp\"";
};

proxy_test "HTTP GET absolute URI, no path",
[], [], [], sub {
	my $req = http_request("GET", "http://$HOST:$PORT");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out) or die "Read timeout";
	$resp =~ /^GET \/ HTTP\/1\./ or die "Proxy sent \"$resp\"";
};

proxy_test "HTTP GET percent escape",
[], [], [], sub {
	my $req = http_request("GET", "http://$HOST:$PORT/%41");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out) or die "Read timeout";
	uri_unescape($resp) =~ /^GET \/A HTTP\/1\./ or die "Proxy sent \"$resp\"";
};

proxy_test "HTTP GET remove Connection header fields",
[], [], [], sub {
	my $req = "GET http://$HOST:$PORT/ HTTP/1.0\r\nKeep-Alive: 300\r\nOne: 1\r\nConnection: keep-alive, two, close\r\nTwo: 2\r\nThree: 3\r\n\r\n";
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out) or die "Read timeout";
	$resp = HTTP::Request->parse($resp);
	!defined($resp->header("Keep-Alive")) or die "Proxy did not remove Keep-Alive header field";
	!defined($resp->header("Two")) or die "Proxy did not remove Keep-Alive header field";
	$resp->header("One") eq "1" or die "Proxy modified One header field";
	$resp->header("Three") eq "3" or die "Proxy modified Three header field";
};

proxy_test "HTTP GET combine multiple headers with the same name",
[], [], [], sub {
	my $req = "GET http://$HOST:$PORT/ HTTP/1.0\r\nConnection: keep-alive\r\nKeep-Alive: 300\r\nConnection: two\r\nOne: 1\r\nConnection: close\r\nTwo: 2\r\nThree: 3\r\n\r\n";
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out) or die "Read timeout";
	$resp = HTTP::Request->parse($resp);
	!defined($resp->header("Keep-Alive")) or die "Proxy did not remove Keep-Alive header field";
	!defined($resp->header("Two")) or die "Proxy did not remove Keep-Alive header field";
	$resp->header("One") eq "1" or die "Proxy modified One header field";
	$resp->header("Three") eq "3" or die "Proxy modified Three header field";
};

# RFC 2616 section 5.1.2: "In order to avoid request loops, a proxy MUST be able
# to recognize all of its server names, including any aliases, local variations,
# and the numeric IP address."
server_client_test "HTTP GET request loop",
["--proxy-type", "http"], [], sub {
	my $req = http_request("GET", "http://$HOST:$PORT/");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 403 or die "Expected response code 403, got $code";
};

server_client_test "HTTP GET IPv6 request loop",
["-6", "--proxy-type", "http"], ["-6"], sub {
	my $req = http_request("GET", "http://[$IPV6_ADDR]:$PORT/");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 403 or die "Expected response code 403, got $code";
};

proxy_test "HTTP HEAD absolute URI",
[], [], [], sub {
	my $req = http_request("HEAD", "http://$HOST:$PORT/");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out) or die "Read timeout";
	$resp = HTTP::Request->parse($resp);
	$resp->method eq "HEAD" or die "Proxy sent \"" . $resp->method . "\"";
};

proxy_test "HTTP POST",
[], [], [], sub {
	my $req = "POST http://$HOST:$PORT/ HTTP/1.0\r\nContent-Length: 4\r\n\r\nabc\n";
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out) or die "Read timeout";
	$resp = HTTP::Request->parse($resp);
	$resp->method eq "POST" or die "Proxy sent \"" . $resp->method . "\"";
	$resp->content eq "abc\n" or die "Proxy sent \"" . $resp->content . "\"";
};

proxy_test "HTTP POST short Content-Length",
[], [], [], sub {
	my $req = "POST http://$HOST:$PORT/ HTTP/1.0\r\nContent-Length: 2\r\n\r\nabc\n";
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out) or die "Read timeout";
	$resp = HTTP::Request->parse($resp);
	$resp->method eq "POST" or die "Proxy sent \"" . $resp->method . "\"";
	$resp->content eq "ab" or die "Proxy sent \"" . $resp->content . "\"";
};

proxy_test "HTTP POST long Content-Length",
[], [], [], sub {
	my $req = "POST http://$HOST:$PORT/ HTTP/1.0\r\nContent-Length: 10\r\n\r\nabc\n";
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out) or die "Read timeout";
	$resp = HTTP::Request->parse($resp);
	$resp->method eq "POST" or die "Proxy sent \"" . $resp->method . "\"";
	$resp->content eq "abc\n" or die "Proxy sent \"" . $resp->content . "\"";
};

proxy_test "HTTP POST chunked transfer encoding",
[], [], [], sub {
	my $req = "POST http://$HOST:$PORT/ HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nabc\n0\r\n";
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($s_out);
	# We expect the proxy to relay the request or else die with an error
	# saying it can't do it.
	if ($resp) {
		$resp = HTTP::Request->parse($resp);
		$resp->method eq "POST" or die "Proxy sent \"" . $resp->method . "\"";
		$resp->content eq "abc\n" or die "Proxy sent \"" . $resp->content . "\"";
	} else {
		$resp = timeout_read($c_out) or die "Read timeout";
		$resp = HTTP::Response->parse($resp);
		$resp->code == 400 or $resp->code == 411 or die "Proxy returned code " . $resp->code;
	}
};

server_client_test "HTTP proxy unknown method",
["--proxy-type", "http"], [], sub {
	# Supposed to have a port number.
	my $req = http_request("NOTHING", "http://$HOST:$PORT/");
	syswrite($c_in, $req);
	close($c_in);
	my $resp = timeout_read($c_out) or die "Read timeout";
	my $code = HTTP::Response->parse($resp)->code;
	$code == 405 or die "Expected response code 405, got $code";
};

# Check that proxy auth is base64 encoded properly. 's' and '~' are 0x77 and
# 0x7E respectively, printing characters with many bits set.
for my $auth ("", "a", "a:", ":a", "user:sss", "user:ssss", "user:sssss", "user:~~~", "user:~~~~", "user:~~~~~") {
server_client_test "HTTP proxy auth base64 encoding: \"$auth\"",
[], ["--proxy", "$HOST:$PORT", "--proxy-type", "http", "--proxy-auth", $auth], sub {
	my $resp = timeout_read($s_out) or die "Read timeout";
	my $auth_header = HTTP::Response->parse($resp)->header("Proxy-Authorization") or die "Proxy client didn't send Proxy-Authorization header field";
	my ($b64_auth) = ($auth_header =~ /^Basic (.*)/) or die "No auth data in \"$auth_header\"";
	my $dec_auth = decode_base64($b64_auth);
	$auth eq $dec_auth or die "Proxy client sent \"$b64_auth\" for \"$auth\", decodes to \"$dec_auth\"";
};
}

# These proxy auth tests sometimes fail because the proxy client dies before we
# can read the response from it, creating a broken pipe. I need to find a way
# to keep that information.

server_client_test_tcp_ssl "HTTP proxy server auth challenge",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code == 407 or die "Expected response code 407, got $code";
	my $auth = $resp->header("Proxy-Authenticate");
	$auth or die "Proxy server didn't send Proxy-Authenticate header field";
};

server_client_test_tcp_ssl "HTTP proxy server correct auth",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	syswrite($c_in, "Proxy-Authorization: Basic " . encode_base64("user:pass") . "\r\n");
	syswrite($c_in, "\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code == 200 or die "Expected response code 200, got $code";
};

server_client_test_tcp_ssl "HTTP proxy server wrong user",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	syswrite($c_in, "Proxy-Authorization: Basic " . encode_base64("nobody:pass") . "\r\n");
	syswrite($c_in, "\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code == 407 or die "Expected response code 407, got $code";
};

server_client_test_tcp_ssl "HTTP proxy server wrong pass",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	syswrite($c_in, "Proxy-Authorization: Basic " . encode_base64("user:word") . "\r\n");
	syswrite($c_in, "\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code == 407 or die "Expected response code 407, got $code";
};

server_client_test_tcp_ssl "HTTP proxy server correct auth, different case",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	syswrite($c_in, "pROXY-aUTHORIZATION: Basic " . encode_base64("user:pass") . "\r\n");
	syswrite($c_in, "\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code == 200 or die "Expected response code 200, got $code";
};

# Test that header field values can be split across lines with LWS.
server_client_test_tcp_ssl "HTTP proxy server LWS",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	syswrite($c_in, "Proxy-Authorization:\t  Basic  \r\n\t  \n dXNlcjpwYXNz\r\n");
	syswrite($c_in, "\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code == 200 or die "Expected response code 200, got $code";
};

server_client_test_tcp_ssl "HTTP proxy server LWS",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	syswrite($c_in, "Proxy-Authorization: Basic\r\n dXNlcjpwYXNz\r\n");
	syswrite($c_in, "\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code == 200 or die "Expected response code 200, got $code";
};

server_client_test_tcp_ssl "HTTP proxy server no auth",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	syswrite($c_in, "Proxy-Authorization: \r\n");
	syswrite($c_in, "\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code != 200 or die "Got unexpected 200 response";
};

server_client_test_tcp_ssl "HTTP proxy server broken auth",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	syswrite($c_in, "Proxy-Authorization: French fries\r\n");
	syswrite($c_in, "\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code != 200 or die "Got unexpected 200 response";
};

server_client_test_tcp_ssl "HTTP proxy server extra auth",
["--proxy-type", "http", "--proxy-auth", "user:pass"],
[],
sub {
	syswrite($c_in, "CONNECT $HOST:$PORT HTTP/1.0\r\n");
	syswrite($c_in, "Proxy-Authorization: Basic " . encode_base64("user:pass") . " extra\r\n");
	syswrite($c_in, "\r\n");
	my $resp = timeout_read($c_out) or die "Read timeout";
	$resp = HTTP::Response->parse($resp);
	my $code = $resp->code;
	$code != 200 or die "Got unexpected 200 response";
};

# Allow and deny list tests.

server_client_test_tcp_udp_ssl "Allow localhost (IPv4 address)",
["--allow", "127.0.0.1"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};

server_client_test_tcp_udp_ssl "Allow localhost (host name)",
["--allow", "localhost"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};

# Anyone not allowed is denied.
server_client_test_tcp_udp_ssl "Allow non-localhost",
["--allow", "1.2.3.4"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server did not reject host not in allow list";
};

# --allow options should accumulate.
server_client_test_tcp_udp_ssl "--allow options accumulate",
["--allow", "127.0.0.1", "--allow", "1.2.3.4"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};

server_client_test_tcp_udp_ssl "Deny localhost (IPv4 address)",
["--deny", "127.0.0.1"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server did not reject host in deny list";
};

server_client_test_tcp_udp_ssl "Deny localhost (host name)",
["--deny", "localhost"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server did not reject host in deny list";
};

# Anyone not denied is allowed.
server_client_test_tcp_udp_ssl "Deny non-localhost",
["--deny", "1.2.3.4"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};

# --deny options should accumulate.
server_client_test_tcp_udp_ssl "--deny options accumulate",
["--deny", "127.0.0.1", "--deny", "1.2.3.4"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server did not reject host in deny list";
};

# If a host is both allowed and denied, denial takes precedence.
server_client_test_tcp_udp_ssl "Allow and deny",
["--allow", "127.0.0.1", "--deny", "127.0.0.1"], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server did not reject host in deny list";
};

# Test that --allowfile and --denyfile handle blank lines and more than one
# specification per line.
for my $contents (
"1.2.3.4

localhost",
"1.2.3.4 localhost"
) {
my ($fh, $filename) = tempfile("ncat-test-XXXXX", SUFFIX => ".txt");
print $fh $contents;
server_client_test_tcp_udp_ssl "--allowfile",
["--allowfile", $filename], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};
server_client_test_tcp_udp_ssl "--denyfile",
["--denyfile", $filename], [], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server did not reject host in --denyfile list";
};
unlink $filename;
}

# Test --ssl sending.
server_client_test "SSL server relays",
["--ssl", "--ssl-key", "test-cert.pem", "--ssl-cert", "test-cert.pem"], ["--ssl"], sub {
	my $resp;

	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";

	syswrite($s_in, "abc\n");
	$resp = timeout_read($c_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Client got \"$resp\", not \"abc\\n\"";
};

# Test that an SSL server gracefully handles non-SSL connections.
($s_pid, $s_out, $s_in) = ncat_server("--ssl", "--ssl-key", "test-cert.pem", "--ssl-cert", "test-cert.pem");
test "SSL server handles non-SSL connections",
sub {
	my $resp;

	my ($c1_pid, $c1_out, $c1_in) = ncat_client();
	syswrite($c1_in, "abc\n");
	kill "TERM", $c1_pid;
	waitpid $c1_pid, 0;

	my ($c2_pid, $c2_out, $c2_in) = ncat_client("--ssl");
	syswrite($c2_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
	kill "TERM", $c2_pid;
	waitpid $c2_pid, 0;
};
kill_children;

($s_pid, $s_out, $s_in) = ncat_server("--ssl", "--ssl-key", "test-cert.pem", "--ssl-cert", "test-cert.pem");
test "SSL verification, correct domain name",
sub {
	my $resp;

	($c_pid, $c_out, $c_in) = ncat($HOST, $PORT, "--ssl-verify", "--ssl-trustfile", "test-cert.pem");
	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	$resp or die "Read timeout";
	$resp eq "abc\n" or die "Server got \"$resp\", not \"abc\\n\"";
};
kill_children;

($s_pid, $s_out, $s_in) = ncat_server("--ssl", "--ssl-key", "test-cert.pem", "--ssl-cert", "test-cert.pem");
test "SSL verification, wrong domain name",
sub {
	my $resp;

	# Use the IPv6 address as an alternate name that doesn't match the one
	# on the certificate.
	($c_pid, $c_out, $c_in) = ncat($IPV6_ADDR, $PORT, "-6", "--ssl-verify", "--ssl-trustfile", "test-cert.pem");
	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server got \"$resp\" when verification should have failed";
};
kill_children;

($s_pid, $s_out, $s_in) = ncat_server("--ssl");
test "SSL verification, no server cert",
sub {
	my $resp;

	($c_pid, $c_out, $c_in) = ncat($HOST, $PORT, "--ssl-verify", "--ssl-trustfile", "test-cert.pem");
	syswrite($c_in, "abc\n");
	$resp = timeout_read($s_out);
	!$resp or die "Server got \"$resp\" when verification should have failed";
};
kill_children;

# Test --max-conns.
for my $count (0, 1, 10) {
	max_conns_test_tcp_ssl("--max-conns $count --keep-open", ["--keep-open"], [], $count);
}

for my $count (0, 1, 10) {
	max_conns_test_tcp_ssl("--max-conns $count --broker", ["--broker"], [], $count);
}

for my $count (0, 1, 10) {
	max_conns_test_tcp_udp_ssl("--max-conns $count with exec", ["--exec", "/bin/cat"], [], $count);
}

print "$num_failures failures in $num_tests tests.\n";

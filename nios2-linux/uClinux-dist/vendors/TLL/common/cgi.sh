#!/bin/sh

# start with the headers
cat << EOF
Content-type: text/html

EOF

# then the html
cat << EOF
<html>
<head>
 <title>Boa CGI Example (sh)</title>
</head>
<body>
EOF

echo "The current time is: `date`"

cat << EOF
</body>
</html>
EOF

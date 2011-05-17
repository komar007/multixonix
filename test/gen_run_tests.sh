#!/bin/bash

testsuites=`ls tests/*.h`

echo '#define private public'
echo '#include "tester.h"'
for ts in $testsuites; do
	echo '#include "'$ts'"'
done
echo -e 'int main() {'
echo -e '\tTester tester;'
n=0
for ts in $testsuites; do
	clsname=`grep 'class' $ts | tail -n 1 | sed -e 's/class \([a-zA-Z0-9_]\+\) : public TestSuite {/\1/'`
	echo -e '\t'$clsname t$n';'
	echo -e '\ttester.add_suite(t'$n');'
	n=$[$n+1]
done
echo -e '\ttester.run();'
echo -e '\treturn 0;'
echo '}'

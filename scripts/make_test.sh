path=$1

if [[ ${path:0:13} = 'include/actl/' ]]
then
test=tests${path:12:${#path}-4}
test=${test:0:${#test}-4}.cpp
mkdir -p $(dirname $test)
touch $test
head -4 $path > $test
echo "
#include <${path:8}>
#include \"test.hpp\"

TEST_CASE(\"\") {}" >> $test
else
echo error: include path expected
fi

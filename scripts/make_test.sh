path=$1

if [[ ${path:0:13} = 'include/actl/' ]]
then
test=tests${path:12:${#path}-4}
test=${test:0:${#test}-4}.cpp
mkdir -p $(dirname $test)
touch $test
head -6 $path > $test
echo "
#include <${path:8}>
#include <actl/test.hpp>

using namespace ac;
" >> $test
echo 'TEST("") {}' >> $test
else
echo error: include path expected
fi

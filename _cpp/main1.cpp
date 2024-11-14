#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

#include <iostream>

#include <iostream>
#include <string>
#include <cassert>
#include <fstream>

void genBrackets(int n, int nopen, int nclosed, char* buf, std::ostream& cout)
{
	int index = nopen+2*nclosed;
	if (nopen+nclosed==n)
    {
        for(int i=0; i<nopen; ++i)
        	buf[index+i]=')';
        cout << buf << std::endl;
        return;
    }

    buf[index]='(';
    genBrackets(n, nopen+1, nclosed, buf, cout);

    if (nopen>0)
    {
    	buf[index]=')';
    	genBrackets(n, nopen-1, nclosed+1, buf, cout);
    }
}

int main()
{
	int n = 0;
 	std::cin >> n;
	char buf[100] = {'('};
	assert(2*n+1<100);
    buf[2*n]=0;
    std::fstream myfile;
    myfile.open("output.txt", std::ios::out);

    if (n>0)
  	genBrackets(n, 1, 0, buf, std::cout);

	return 0;
}

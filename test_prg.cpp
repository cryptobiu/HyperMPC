#include "AES_PRG.h"
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
   PRG & prg = PRG::instance();

   cout << "PRG got instance " << endl;

   cout << "PRG first random : " <<  prg.getRandom() << endl;

   cout << "Additional 3 randoms from the same 128bit : " << endl;

   cout <<  prg.getRandom() << endl;
   cout <<  prg.getRandom() << endl;
   cout <<  prg.getRandom() << endl;

   cout << "Print 10 requests for 128 bit" <<endl;

   for (int i =0; i < 10; i++) {
		for (int j=0; j <4; j++) {
			cout << prg.getRandom() << ":";
		}
		cout <<endl;
   }

   cout << "Perform 10 requests for 100 random uint32. Printing xor of each 100 to see we do not repeat" << endl;

   for (int i =0; i < 10; i++) {
      uint32_t tmp=0;
      for (int j = 0; j < 100; j++) {
         tmp ^= prg.getRandom();
      }
      cout << i << ": " << tmp << endl;
   }

   cout << "DONE" << endl;
   return 0;
}

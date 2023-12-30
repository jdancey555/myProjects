#ifndef CARDS_H
#define CARDS_H
#include <string>
#include <vector> 
#include <memory>
using namespace std;
class Cards {
	private:
	 	char suit;
		char rank;
		string card_name;
	public:
		Cards();
		Cards(char, char);
		~Cards();
		char getsuit();
		char getrank();
		string getcardname() const;


friend ostream& operator<<(ostream &, const vector <shared_ptr<Cards>> &);

};
#endif

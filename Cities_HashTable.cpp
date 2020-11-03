#include <iostream>
#include <string>
#include <fstream>
using namespace std;
int const size=30000;
string states [52] = {"AL","AK","AZ","AR","CA","CO","CT","DC","DE","FL","GA","HI","ID","IL",
"IN","IA","KS","KY","LA","ME","MD","MA","MI","MN","MS","MO","MT","NE","NV","NH","NJ","NM","NY","NC","ND","OH","OK","OR","PA","PR","RI","SC","SD","TN","TX","UT","VT","VA","WA", "WV","WI","WY"};
struct place{
   int nCode,pop,rCode;
   string state,name;
   double area,lat,lon,dist;
   place(int nc, string st, string na, int p, double a, double la, double lo, int rc, double d){
	   nCode=nc; state=st;
	   name=na; pop=p;
	   area=a; lat=la;
	   lon=lo; rCode=rc;
	   dist=d;
	} 
};
struct node{
	place *data;
	node *next;
	node(place *p,node * n=NULL){
		data=p;
		next=n;
	}
};
class HashTable{
	protected:
		node * data[size];
	public:
		HashTable(){
			for(int i=0;i<size;i++){
				data[i]=NULL;
			}
		}
		int HashN(string s);
		void addPlace(place *p);
		void addData(string file);
		void printPlace(place *p);
		bool findStatePlace(string st, string na);
		void findAllPlaces(string na);
};
int HashTable::HashN(string s){
	int val=1759619;
	for(int i=0;i<s.length();i++){
		val=val*787+s[i];
	}
	if(val<0){
		val=-val;
	}
	return val%size;
}
void HashTable::addPlace(place *p){
	string st_na=(p->state)+(p->name);
	int pos=HashN(st_na);
	data[pos]=new node(p,data[pos]);
}
void HashTable::addData(string file){
        ifstream fin(file);
	int nc, p,rc;
	string block,st,na,temp;
	double a,la,lo,d;
	while(!fin.eof()){
		fin>>block;
		while((fin.peek()<=48 || fin.peek()>=57)&&!fin.eof()){
			if(fin.get()==' '&&fin.peek()==' '){
				break;
			}
			fin>>temp;
			block=block+" "+temp;
		}
		fin>>p>>a>>la>>lo>>rc>>d;
		nc=stoi(block.substr(0,8));
		st.assign(block,8,2);
		na.assign(block,10);
		if(!fin.eof()){
			addPlace(new place(nc,st,na,p,a,la,lo,rc,d));
		}
	}
	fin.close();
}
void HashTable::printPlace(place *p){
	cout<<p->nCode<<" "<<p->state<<" "<<p->name<<" "<<p->pop<<" "<<p->area<<" "
	<<p->lat<<" "<<p->lon<<" "<<p->rCode<<" "<<p->dist<<endl;
}
bool HashTable::findStatePlace(string st, string na){
	string st_na=st+na;
	int pos=HashN(st_na);
	node * curr=data[pos];
	if(data[pos]==NULL){
		return false;
	}else{
		while(curr!=NULL){
			if(curr->data->state==st&&curr->data->name==na){
				printPlace(curr->data);
				return true;
			}
			curr=curr->next;
		}
		return false;
	}
}
void HashTable::findAllPlaces(string na){
	bool flag=false;
	for(int i=0;i<52;i++){
		if(findStatePlace(states[i],na)){
			flag=true;	
		}
	}
	if(!flag){
		cout<<"Place Not in Data"<<endl;
	}
}
int main(){
	string cmd="Start",placeN, state;
	HashTable H;
	H.addData("USA_Cities.txt");
	cout<<"Available Commands are Search by State and Name (S), Places with Same Name (N), and Quit (Q). CAPS Sensitive!"<<endl;
	while(cmd!="Q"){
		cin>>cmd;
		if(cmd=="S"){
			cout<<"Please input place name:"<<endl;
			cin.ignore();
			getline(cin,placeN);
			cout<<"Please input state abbreviation:"<<endl;
			cin>>state;
			bool found=H.findStatePlace(state,placeN);
			if(!found){
				cout<<"Place Not in Data"<<endl;
			}
		}else if(cmd=="N"){
			cout<<"Please input place name:"<<endl;
			cin.ignore();
			getline(cin,placeN);
			H.findAllPlaces(placeN);
		}
		else if(cmd=="Q"){
			cout<<"You have exited the program."<<endl;
		}else{
			cout<<"Incorrect command. Available commands are S, N, and Q"<<endl;
		}
	}
}

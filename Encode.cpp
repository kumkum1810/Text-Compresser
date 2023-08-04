#include<bits/stdc++.h>
using namespace std;
#define char_size 256

struct node{
	unsigned char ch;
	long long int freq;
	node *left,*right;
	node(unsigned char c,long long int f,node *l=NULL,node *r=NULL){
		ch=c;
		freq=f;
		left=l;
		right=r;
	}
};

class compare{
	public:
	bool operator()(node* a,node* b){
		return (a->freq > b->freq);
	}
};

node* huffman(long long int count[]){
	priority_queue<node*,vector<node*> , compare> minheap;
	for(int i=0;i<char_size;++i){
		if(count[i]!=0)minheap.push(new node(i,count[i]));
	}
	while( minheap.size() != 1 ){
		node *n1,*n2;
		n1=minheap.top();
		minheap.pop();
		n2=minheap.top();
		minheap.pop();
		node *new_node = new node(-1,n1->freq+n2->freq,n1,n2);
		minheap.push(new_node);
	}
	return minheap.top();
}

void store_tree(ofstream &input,node* root){
    if(!root->left&&!root->right)
    {
        input<<'1';
        input<<root->ch;
    }
    else
    {
        input<<'0';
        store_tree(input,root->left);
        store_tree(input,root->right);
    }
}

void store_codes(node* Root,char single_code[],int index,vector<long long int> &Huffman_codemap){
    if(Root->left)
    {
        single_code[index]='0';
        store_codes(Root->left,single_code,index+1,Huffman_codemap);

    }
    if(Root->right)
    {
        single_code[index]='1';
        store_codes(Root->right,single_code,index+1,Huffman_codemap);
    }
    if(!Root->left&&!Root->left)
    {
        for(int i=index;i>=0;i--)
        {
          if(i!=index)
          {
            Huffman_codemap[Root->ch]*=10;
            Huffman_codemap[Root->ch]+=single_code[i]-'0';
          }
          else
            Huffman_codemap[Root->ch]=1;
        }
    }
}

void Write_compressed(ifstream &input,ofstream &output,vector<long long int > &Huffman_codemap){
    char ch;
    unsigned char bits_8;
    long long int counter=0;
    while(input.get(ch))
    {
        long long int temp=Huffman_codemap[static_cast<unsigned char>(ch)];
        while(temp!=1)
        {
          bits_8<<=1;
          if((temp%10)!=0)
                bits_8|=1;
          temp/=10;
          counter++;
          if(counter==8)
            {
                output<<bits_8;
                counter=bits_8=0;
            }
        }
    }
    while(counter!=8)
    {
        bits_8<<=1;
        counter++;
    }
    output<<bits_8;
    output.close();
}

int main(){
	vector<long long int> huffman_map;
	huffman_map.resize(char_size);
	long long int count[char_size]={0};
	
	string filename;
	cout<<"Enter Filename: ";
	cin>>filename;
	ifstream input_file(filename.c_str(),ios::binary);
	if(!input_file.good()){
		perror("Something wrong occured...\n");
		exit(-1);
	}
	cout<<"\nCompressing the file...";
	clock_t start_time = clock();
	char ch;
	while(input_file.get(ch)){
		count[static_cast<unsigned char>(ch)]++;
	}
	input_file.clear();
	input_file.seekg(0);

	node* tree = huffman(count);
	ofstream output_file((filename+".huf").c_str(),ios::binary); 
	if(!output_file.good()){
        perror("Error:\t");
        exit(-1);
    }

	output_file << tree->freq;
	output_file<<',';
    store_tree(output_file,tree);
	output_file<<' ';

	char single_code[16];
	store_codes(tree,single_code,0,huffman_map);
	Write_compressed(input_file,output_file,huffman_map);
    
	input_file.close();
	output_file.close();

	clock_t stop_time=clock();

	if(remove(filename.c_str())!=0) perror("Error deleting the compressed file:\t");

	cout<<"\n\n*********************************************File Compressed Successfully! :-)*********************************************\n\n";
    cout<<"Time taken to Compress:\t"<<double(stop_time-start_time)/CLOCKS_PER_SEC<<" seconds\n\n";
}








#include<bits/stdc++.h>
using namespace std;

struct node{
	unsigned char ch;
	node *left,*right;
	node(unsigned char c,node *l=NULL,node *r=NULL){
		ch=c;
		left=l;
		right=r;
	}
};

node* Make_Huffman_tree(ifstream &input)
{
    char ch;
    input.get(ch);
    if(ch=='1')
    {
        input.get(ch);
        return (new node(ch));
    }
    else
    {
        node* left=Make_Huffman_tree(input);
        node* right=Make_Huffman_tree(input);
        return(new node(-1,left,right));
    }
}
void decode(ifstream &input,string filename,node* Root,long long int Total_Freq)
{
    ofstream output((filename.erase(filename.size()-4)).c_str(),ios::binary);
    if(!output.good())
    {
        perror("Error:\t");
        exit(-1);
    }
    bool eof_flag=false;
    char bits_8;
    node* pointer=Root;
    while(input.get(bits_8))
    {
        int counter=7;
        while(counter>=0)
        {
            if(!pointer->left&&!pointer->right)
            {
                output<<pointer->ch;
                Total_Freq--;
                if(!Total_Freq)
                {
                    eof_flag=true;
                    break;
                }
                pointer=Root;
                continue;
            }
            if((bits_8&(1<<counter)))
            {
                pointer=pointer->right;
                counter--;
            }
            else
            {
                pointer=pointer->left;
                counter--;
            }
        }
        if(eof_flag)
            break;
    }
    output.close();
}
int main()
{
    string filename;
    cout<<"Enter the Filename: ";
    cin>>filename;
    ifstream input_file(filename.c_str(),ios::binary);
    if(!input_file.good()){
        perror("Error:\t");
        exit(-1);
    }
    if(filename.find(".huf")==string::npos)
    {
        cout<<"Error: File is already decompressed\n\n";
        exit(-1);
    }
    cout<<"\nDecompressing the file....";
    clock_t start_time=clock();
    long long int Total_freq=0;
    char ch;
    while(input_file.get(ch)){
        if(ch==',')
            break;
        Total_freq*=10;
        Total_freq+=ch-'0';
    }
    node* Huffman_tree=Make_Huffman_tree(input_file);
    input_file.get(ch); 
    decode(input_file,filename,Huffman_tree,Total_freq);
    input_file.close();
    clock_t stop_time=clock();
    if(remove(filename.c_str())!=0) 
        perror("Error deleting the compressed file:\t");
    cout<<"\n\n*******************************************File Decompressed Successfully! :-)*******************************************\n\n";
    cout<<"Time taken to Compress:\t"<<double(stop_time-start_time)/CLOCKS_PER_SEC<<" seconds\n\n";
}

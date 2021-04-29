/* This code should be compiled in LINUX g++ environemnt */

/* Name- Kartikay Goel
Roll Number - 180101033 */

// 2 Pass Assembler

#include<bits/stdc++.h>
using namespace std;    

map<char, int> hexToDec; 
map<int, char> decToHex;  
list<pair<string,string>> instrTable;
list<pair<string,string>> symtable;

string decimal_to_hex(int n) 
{    
    string res ="";  
    while(n) 
    {    
    	res += decToHex[n%16]; 
        n/=16;       
    } 
    reverse(res.begin(),res.end());
    return res;
} 

int powerFunc(int x, int y)
{
	unsigned long long int result = 1;
	int i;

	for (i = 0; i < y; i++)
	{
		result *= x;
	}

	return result;
}

int hex_to_decimal(string hexaNumber)
{
	int i;
	int power = hexaNumber.length() - 1;
	int checkLength = hexaNumber.length();
	unsigned long long int decimalResult = 0;

	for (i = 0; i < checkLength; i++)
	{
		if (int(hexaNumber[i]) >= '0' && int(hexaNumber[i]) <= '9') 
		{																			// check if FirstHexaNumber 0 to 9
			decimalResult += ((int(hexaNumber[i])) - 48) * powerFunc(16, power);
		}
		else if (int(hexaNumber[i]) >= 'A' && int(hexaNumber[i]) <= 'F')			// check if FirstHexaNumber is A to F
		{
			decimalResult += ((int(hexaNumber[i])) - 55)*powerFunc(16, power);
		}
		else if (int(hexaNumber[i]) >= 'a' && int(hexaNumber[i]) <= 'f')			// check if FirstHexaNumber is a to f
		{
			decimalResult += ((int(hexaNumber[i])) - 87)*powerFunc(16, power);
		}
		power--;
	}

	return decimalResult;
}


string addTwoHexStr(string a, string b) 
{ 
	int x=hex_to_decimal(a);
	int y = hex_to_decimal(b);
	int result = x+y;
	string res = decimal_to_hex(result);
	return res;
} 

string subTwoHexStr(string a, string b) 
{ 
	int x=hex_to_decimal(a);
	int y = hex_to_decimal(b);
	int result = x-y;
	string res = decimal_to_hex(result);
	return res;
} 


bool optab_search(string key){
    list<pair<string,string>> :: iterator it; 
    for(it = instrTable.begin(); it != instrTable.end(); ++it) 
    {
        if(it->first==key){
            return true;
        }
    }
    return false;
}

void optab_insert(string mnemonic,string opcode){
    instrTable.push_back({mnemonic,opcode});
}

string optab_retrieve(string key){
    list<pair<string,string>> :: iterator it; 
    for(it = instrTable.begin(); it != instrTable.end(); ++it) 
    {
        if(it->first==key){
            return it->second;
        }
    }
    return "-1";
}	


bool symtab_search(string label){
    list<pair<string,string>> :: iterator it; 
    for(it = symtable.begin(); it != symtable.end(); ++it) 
    {
        if(it->first==label){
            return true;
        }
    }
    return false;
}

void symtab_insert(string label,string addr){
    symtable.push_back({label,addr});
}

string symtab_retrieve(string label){
    list<pair<string,string>> :: iterator it; 
    for(it = symtable.begin(); it != symtable.end(); ++it) 
    {
        if(it->first==label){
            return it->second;
        }
    }
    return "-1";
}


string start_line;
string LOCCTR;
string starting_address;
string program_length;
string input_file;
ifstream fin;
ofstream fout,fobj;
char Buffer[256];
char Index;


void initialise_hexToDec()
{
    hexToDec.insert({{'0', 0},{'1', 1},{'2', 2},{'3', 3},{'4', 4},{'5', 5},{'6', 6},{'7', 7},{'8', 8},{'9', 9},
        {'A', 10},{'B', 11},{'C', 12 },{'D', 13},{'E', 14},{'F', 15}});
}

void initialise_decToHex()
{

    decToHex.insert({{0, '0'},{1, '1'},{2, '2'},{3, '3'},{4, '4'},{5, '5'},{6, '6'},{7, '7'},{8, '8'},{9, '9'},
        {10,'A'},{11,'B'},{12,'C'},{13,'D'},{14,'E'},{15,'F'}});
}


void initialise_instrTable(){

    optab_insert("ADD","18");
    optab_insert("AND","40");
	optab_insert("COMP","28");
	optab_insert("DIV","24");
	optab_insert("J","3C");
	optab_insert("JEQ","30");
	optab_insert("JGT","34");
	optab_insert("JLT","38");
	optab_insert("JSUB","48");
	optab_insert("LDA","00");
	optab_insert("LDCH","50");
	optab_insert("LDL","08");
	optab_insert("LDX","04");
	optab_insert("MUL","20");
    optab_insert("OR","44");
	optab_insert("RD","D8");
	optab_insert("RSUB","4C");
	optab_insert("STA","0C");
	optab_insert("STCH","54");
	optab_insert("STSW","E8");
	optab_insert("STL","14");
	optab_insert("STX","10");
	optab_insert("SUB","1C");
	optab_insert("TD","E0");
	optab_insert("TIX","2C");
	optab_insert("WD","DC");

}

void pass1()
{
    fout.open("intermediate_file.txt");
    string label,opcode,operand,templine,loc;
    FILE* fptr = fopen("input.asm","r");


    // read first input line
    templine = fgets(Buffer,256,fptr);
    templine.pop_back(); 
    
    // fetching label, opcode, operand from first input line
    vector <string> tokens; 
    stringstream check1(templine); 
      
    string intermediate; 
    while(getline(check1, intermediate, ' ')) 
    { 
        tokens.push_back(intermediate); 
    } 

    label = tokens[0];
    if(label != ".")
    {
    	opcode = tokens[1];
    	operand = tokens[2];
    }

    if(opcode=="START")
    {
        // save operand as starting address
        starting_address = operand;

        // initialize LOCTOR to starting_address 
        LOCCTR = starting_address;

        // write line to intermediate file
        fout<<LOCCTR<<"\t"<<label<<"\t"<<opcode<<"\t"<<operand<<"\n";
        
    }        
    else
    {
        // initialize LOCTOR to starting_address 
        starting_address="0000";
        LOCCTR="0000";
    }

    while(1)
    {

        loc=LOCCTR;

        // read next input line
        templine = fgets(Buffer,256,fptr);
        templine.pop_back();

        // fetchinh label, opcode, operand from first input line

        vector <string> tokens; 
	    stringstream check1(templine); 
	      
	    string intermediate; 
	      
	    while(getline(check1, intermediate, ' ')) 
	    { 
	        tokens.push_back(intermediate); 
	    } 


	    label = tokens[0];
	    if(label!=".")
	    {
	    	opcode = tokens[1];
	    	operand = tokens[2];
	    }

        if(opcode=="END")
        break;

        // if this is not comment line
        if(templine[0]!='.')
        {
            // if there is a symbol in  LABEL field
            if(label!="*")
            {
                // search SYMTAB for LABEL

                // if found
                if(symtab_search(label))
                {
                    cout<<"ERROR: DUPLICATE SYMBOL"<<endl;
                }
                    
                else{
                    // insert (LABEL,LOCTOR) into SYMTAB
                    symtab_insert(label,LOCCTR); 
                }
                    
            }

            // search OPTAB for OPCODE
            if(optab_search(opcode))  
                LOCCTR = addTwoHexStr(LOCCTR,"3");
            else if(opcode=="WORD") 
                LOCCTR = addTwoHexStr(LOCCTR,"3");
            else if(opcode=="RESW")
                LOCCTR = addTwoHexStr(LOCCTR,to_string(3*stoi(operand)));
            else if(opcode=="RESB"){
                char hex_string[12];
                sprintf(hex_string, "%X", stoi(operand));
                LOCCTR=addTwoHexStr(LOCCTR,hex_string);
            }
            else if(opcode=="BYTE")
            {
                int len=0;
                if(operand[0]!='X')
                    len = (operand.length()-3);
                else
                	len = (operand.length()-3)/2;
                    
                LOCCTR = addTwoHexStr(LOCCTR,to_string(len));
            }
            else
            {
                cout<<"ERROR: INVALID OPCODE\n";
            }

            fout<<loc<<"\t"<<label<<"\t"<<opcode<<"\t"<<operand<<"\n";

        }

    }

    fout<<"*\t"<<label<<"\t"<<opcode<<"\t"<<operand<<"\n";

    fclose(fptr);
	fout.close();
    program_length = decimal_to_hex(hex_to_decimal(LOCCTR)-hex_to_decimal(starting_address));
}

void pass2()
{
	
	string first_address,prog_name,templine,obj_code,txt_record;
    bool RES;
    int txt_record_length=0,incr=0;
    stringstream text_ss(txt_record),obj_ss(obj_code);

    FILE* fptr = fopen("intermediate_file.txt","r");
	fout.open("line_by_line_object_codes1.txt");
    fobj.open("output1.o");


    templine = fgets(Buffer,256,fptr);
    templine.pop_back(); 

    vector <string> tokens; 
    stringstream check1(templine); 
      
    string intermediate; 
    while(getline(check1, intermediate, '\t')) 
    { 
        tokens.push_back(intermediate); 
    } 

    string address;
	string label;
	string opcode;
	string operand;

    address = tokens[0];
    label = tokens[1];
    opcode = tokens[2];
    operand = tokens[3];

    prog_name = label;

    if(opcode=="START")
	{
		fout<<address<<"\t";
		fout<<label<<"\t";
		fout<<opcode<<"\t";
		fout<<operand<<"\t";
		fout<<"\t\n";
        templine = fgets(Buffer,256,fptr);
    	templine.pop_back(); 

	    vector <string> tokens; 
	    stringstream check1(templine); 
	      
	    string intermediate; 
	    while(getline(check1, intermediate, '\t')) 
	    { 
	        tokens.push_back(intermediate); 
	    } 

	    address = tokens[0];
	    label = tokens[1];
	    opcode = tokens[2];
	    operand = tokens[3];

	}

    //write header record to object program
	fobj<<"H^"<<prog_name<<"\t";

	fobj<<"^"<<setw(6)<<setfill('0')<<hex<<starting_address<<"^";

	fobj<<setw(6)<<setfill('0')<<hex<<program_length<<"\n";
    
    first_address = address;
    int cnt=0;

/*
Following algorithm is implemented as per the Systems Programming book Pg 56.
*/


    while(1)
	{
        if(opcode=="END"){
            break;
        }
        // if this is not comment line
		if(label!=".")
		{	if(optab_search(opcode))    // if found
			{
                // if there is a symbol in the operand field  
				if(operand!="*") 
				{
					string actual_operand = "";
					bool relative=false;
					for(int i=0;i<operand.size();i++)
					{
						if(i+2 >= operand.size())
						{
							continue;
						}
						actual_operand+=operand[i];
						if(operand[i+1]==',' && operand[i+2]=='X')
						{
							relative=true;
							break;
						}
					}
					if(relative)
					{
                        if(symtab_search(actual_operand))  // search symtab for actual operand
						{
                            string temp = optab_retrieve(opcode);

                            obj_ss << temp<<setw(4)<<setfill('0')<<addTwoHexStr(symtab_retrieve(actual_operand),"8000");
						}
						else
						{
                            string temp = optab_retrieve(opcode);
                            obj_ss << temp<<"0000";
                            cout<<"ERROR: |"<<operand<<"| UNDEFINED\n";
                        }
					}
					else if(symtab_search(operand))
                    {

                            string temp = optab_retrieve(opcode);
                            string temp2 = symtab_retrieve(operand);       
                            obj_ss << temp<<setw(4)<<setfill('0')<<hex<<temp2;
                    }
					else
					{
                            obj_ss << optab_retrieve(opcode)<<"0000";
                            cout<<"ERROR: |"<<operand<<"| UNDEFINED\n";
					}
				}
				else{
                            string temp = optab_retrieve(opcode);
                            obj_ss << optab_retrieve(opcode)<<"0000";
                }
				incr=3;
			}
			else if(!(opcode=="BYTE"&&opcode=="WORD"))
			{
				if(operand[0]=='C')
				{
                            for(int i=0;i<operand.size();i++)
                            {
                            	if(i<2)
                            		continue;
                            	if(i+1 >= operand.size())
                            		continue;
                                obj_ss<<hex<<(int)operand[i];
                            }
                            incr=operand.length()-3;
				}
				else if(operand[0]=='X')
				{
                            for(int i=0;i<operand.size();i++)
                            {
                            	if(i<2)
                            		continue;
                            	if(i+1 >= operand.size())
                            		continue;
                                obj_ss<<(char)toupper(operand[i]);
                            }
                            incr=(operand.length()-3)/2;
				}
				else
				{
                            obj_ss<<setw(6)<<setfill('0')<<hex<<stoi(operand);
                            incr=3;
				}
			}
			txt_record_length+=incr;
			if(txt_record_length>30)
			{
                            string text_ss_str = text_ss.str();

                            fobj<<"T^"<<setfill('0')<<setw(6)<<first_address<<"^"<<setfill('0')<<setw(2)<<decimal_to_hex(txt_record_length-incr)<<text_ss_str<<"\n";
                            txt_record_length=incr;
                            first_address = address;
                            text_ss.str("");
			}
			text_ss<<"^"<<obj_ss.str();
        }
		fout<<address<<"\t";
		fout<<label<<"\t";
		fout<<opcode<<"\t";
		fout<<operand<<"\t";
		fout<<obj_ss.str()<<"\n";
        obj_ss.str(std::string());

        templine = fgets(Buffer,256,fptr);
	    templine.pop_back(); 

	    vector <string> tokens; 
	    stringstream check1(templine); 
	      
	    string intermediate; 
	    while(getline(check1, intermediate, '\t')) 
	    { 
	        tokens.push_back(intermediate); 
	    } 

	    address = tokens[0];
	    label = tokens[1];
	    opcode = tokens[2];
	    operand = tokens[3];


		RES=false;
		if(opcode=="RESW"||opcode=="RESB")
		{
            string text_ss_str = text_ss.str();
			fobj<<"T^"<<setw(6)<<setfill('0')<<first_address<<"^"<<setw(2)<<setfill('0')<<hex<<txt_record_length<<text_ss_str<<"\n";
			txt_record_length=0;
			text_ss.str("");
		}
		while(opcode=="RESW"||opcode=="RESB")
		{
			fout<<address<<"\t";
            fout<<label<<"\t";
            fout<<opcode<<"\t";
            fout<<operand<<"\n";
            templine = fgets(Buffer,256,fptr);
		    templine.pop_back(); 

		    vector <string> tokens; 
		    stringstream check1(templine); 
		      
		    string intermediate; 
		    while(getline(check1, intermediate, '\t')) 
		    { 
		        tokens.push_back(intermediate); 
		    } 

		    address = tokens[0];
		    label = tokens[1];
		    opcode = tokens[2];
		    operand = tokens[3];

			RES=true;
		}
		if(RES)
			first_address=address;
	}
    //---------------------------------- end of while
	if(!RES){
            string text_ss_str = text_ss.str();

    		fobj<<"T^"<<setfill('0')<<setw(6)<<first_address<<"^"<<setfill('0')<<setw(2)<<hex<<txt_record_length<<text_ss.str()<<"\n";
    }
	    fobj<<"E^"<<setfill('0')<<setw(6)<<hex<<starting_address;
		fout<<address<<"\t";
        fout<<label<<"\t";
        fout<<opcode<<"\t";
        fout<<operand<<"\t";

		fclose(fptr);
		fout.close();
		fobj.close();
}

void toUpperIntermediate()
{
	FILE* fptr;  
	fout.open("line_by_line_object_codes.txt");
    char file[50] = { "line_by_line_object_codes1.txt" }; 
    char ch; 
  
    fptr = fopen(file, "r"); 
    ch = fgetc(fptr); 
  
    while (ch != EOF) { 
        ch = toupper(ch); 
        fout<<ch;
        ch = fgetc(fptr); 
    } 
    fout.close();
    fclose(fptr); 
}

void toUpperOutput()
{
	FILE* fptr;  
	fout.open("output.o");

    char file[50] = { "output1.o" }; 
    char ch; 
  
    fptr = fopen(file, "r"); 
    ch = fgetc(fptr); 
  
    while (ch != EOF) { 
        ch = toupper(ch); 
        fout<<ch;
        ch = fgetc(fptr); 
    } 
    fout.close();
    fclose(fptr); 
}

int main()
{
    initialise_hexToDec();
    initialise_decToHex();
    initialise_instrTable();
    pass1();
    pass2();
    toUpperIntermediate();
    toUpperOutput();
    remove("output1.o");
    remove("line_by_line_object_codes1.txt");
	return 0;
}


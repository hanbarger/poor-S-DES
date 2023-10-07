#include<iostream>
#include<bits/stdc++.h>
using namespace std;
static int IP[]={2,6,3,1,4,8,5,7}; //初始置换 
static int IP_1[]={4,1,3,5,7,2,8,6};  //最终置换 

//密钥生成 
static int P10[]={3,5,2,7,4,10,1,9,8,6}; //密钥扩展 
static int P8[]={6,3,7,4,8,5,10,9}; //密钥压缩 
//轮函数F  
static int P4[]={2,4,3,1};  //SPBpx 直接置换 
static int EP[]={4,1,2,3,2,3,4,1};  //EPBox 扩展置换 
static int S0[4][4]={{1,0,3,2},{3,2,1,0},{0,2,1,3},{3,1,0,2}};
static int S1[4][4]={{0,1,2,3},{2,3,1,0},{3,0,1,2},{2,1,0,3}};

//合并数组函数 
void merge(char *num,char *x,char *y,int n){ 
	n/=2;
	for(int i=0;i<n;i++){
		num[i]=x[i];
		num[i+n]=y[i];  //长度为n/2的数组x和y合并成为数组num 
	}
}

//置换函数 
void change(char *z1,char *z2,int *P,int n){ //n为新数组长度 
		for(int i=0;i<n;i++)
			z2[i]=z1[(P[i]-1)];  //将z1数组的第(P[i]-1) 个数置于z2数组的第i个 
}

//二进制整数型转字符型函数 
char intz(int n){
	if(n==1)
	return '1';
	else 
	return '0';
}

//子密钥生成 
void key(char *mkey,char *k1,char *k2)
{
	char temp1[5];char temp2[5];//S1-L,S2-L 存放左/右半部分 
	char mkey1[10];//临时数组
	change(mkey,mkey1,P10,10);//初始密钥进行P10置换 
	for(int j=0;j<5;j++)//左右拆分
	{
		temp1[j]=mkey1[j];
		temp2[j]=mkey1[j+5];
	}
	char x=temp1[0];char y=temp2[0];//左移一位
	for(int k=0;k<4;k++)
	{
		temp1[k]=temp1[k+1];
		temp2[k]=temp2[k+1];
	}
	temp1[4]=x;temp2[4]=y; //最开头的一位到最后 
	merge(mkey1,temp1,temp2,10);//合并
	change(mkey1,k1,P8,8);//P8置换 得子密钥k1 
//	cout<<"子密钥k1为："<<endl;
//	for(int i=0;i<8;i++)
//	cout<<k1[i];
//	cout<<endl; 
	/*
	char a=temp1[0],b=temp1[1];
	char c=temp2[0],d=temp2[1];
	for(int f=0;f<3;f++)//左移两位
	{ 
		temp1[f]=temp1[f+2];
		temp2[f]=temp2[f+2];
	}
	temp1[3]=a,temp1[4]=b;
	temp2[3]=c,temp2[4]=d;
	*/
	x=temp1[0];y=temp2[0];//左移一位
	for(int k=0;k<4;k++)
	{
		temp1[k]=temp1[k+1];
		temp2[k]=temp2[k+1];
	}
	temp1[4]=x;temp2[4]=y; //最开头的一位到最后 

    merge(mkey1,temp1,temp2,10);//合并 
    change(mkey1,k2,P8,8);//P8置换 得子密钥k2 
  //  cout<<"子密钥k2为："<<endl;
  //  for(int i=0;i<8;i++)
  //	cout<<k2[i];
//	cout<<endl; 
}

//加密主过程 
void DES(char *temp,char *k1,char *Rm,char *Lm1){ //temp为明文 
	char Lm[4];  //左半部分 
	int i=0;
	char Rm1[8];  //右半部分Rm进行EP扩展后得Rm1 
	for(i=0;i<4;i++){//拆分
		Lm[i]=temp[i];
		Rm[i]=temp[i+4];
	} 
	 
	for(i=0;i<8;i++){//EP扩展置换和Rm1与子密钥K1按位异或 结果为Rm1 
		Rm1[i]=Rm[(EP[i]-1)];
		
		if(Rm1[i]==k1[i])
		Rm1[i]='0';
		else
		Rm1[i]='1';
	} 	
	    
	/*	cout<<endl<<"EP扩展与密钥按位异或后的Rm1为："<<endl;
		for(int i=0;i<8;i++)
			cout<<Rm1[i];  */
	int x1,y1,x2,y2;
	x1=(Rm1[1]-'0')*2+(Rm1[2]-'0');//S0盒替换 
	y1=(Rm1[0]-'0')*2+(Rm1[3]-'0');
	x2=(Rm1[5]-'0')*2+(Rm1[6]-'0');//S1盒替换 
	y2=(Rm1[4]-'0')*2+(Rm1[7]-'0');
	int s0=S0[y1][x1];
	int s1=S1[y2][x2];
	Rm1[0]=intz((s0/2)%2); //Rm1进行SBoxes替换 
	Rm1[1]=intz(s0%2);
	Rm1[2]=intz((s1/2)%2); 
	Rm1[3]=intz(s1%2);
	/*	cout<<"经过S盒后的Rm1为："<<endl;
		for(int i=0;i<4;i++)
			cout<<Rm1[i]; */
	char Rm2[4];
	for(int i=0;i<4;i++){//Rm1进行P4置换得Rm2再和Lm做异或 结果为Lm1 
		Rm2[i]=Rm1[(P4[i]-1)];
	/*	cout<<"P4置换后的RM1为："<<Rm2[i]<<endl; */
		if(Lm[i]==Rm2[i])
		Lm1[i]='0';
		else
		Lm1[i]='1';
	}
	/*	cout<<"经过P4置换与Lm异或的Lm1为："<<endl;
		for(int i=0;i<4;i++)
			cout<<Lm1[i]; */
}

// 将ASCII字符串转换为二进制字符串
void asciiToBinaryString(const char *ascii, char *binary) {
    for (int i = 0; i < 8; i++) {
        char c = ascii[i];
        for (int j = 0; j < 8; j++) {
            binary[i * 8 + j] = ((c >> (7 - j)) & 1) ? '1' : '0';
        }
    }
}

// 将二进制字符串转换为ASCII字符串
void binaryStringToAscii(const char *binary, char *ascii) {
    for (int i = 0; i < 8; i++) {
        char c = 0;
        for (int j = 0; j < 8; j++) {
            c = (c << 1) | (binary[i * 8 + j] - '0');
        }
        ascii[i] = c;
    }
}



int main(){
	int choose;
	cout<<"Please enter 1 FOR encryption   2 FOR decryption    3 FOR finding a key   4 FOR ASCII"<<endl;
	cin>>choose;
    //加密 
	if(choose==1){
	char mkey[10],m[8],temp[8];//主密钥，明文，临时数组 
	char k1[8],k2[8];//子密钥K1，K2 
    char Rm[4],Lm1[4];//分段
    while(cin>>mkey>>m){
		key(mkey,k1,k2);//生成子密钥 K1 K2 
		change(m,temp,IP,8);//明文IP置换 
		DES(temp,k1,Rm,Lm1);//第一次循环,产生Rm和Lm1 
	//	cout<<"第一次循环得到的Rm为："<<Rm[0]<<Rm[1]<<Rm[2]<<Rm[3]; 
	//	cout<<"第一次循环得到的Lm1为："<<Lm1[0]<<Lm1[1]<<Lm1[2]<<Lm1[3]; 
		char m1[8],Rm1[4],Lm11[4]; 
		merge(m1,Rm,Lm1,8);//将Rm和Lm1左右互换合成m1  
		DES(m1,k2,Rm1,Lm11);//第二次循环,产生Rm1和Lm11 
	//	cout<<"第二次循环得到的Rm1为："<<Rm1[0]<<Rm1[1]<<Rm1[2]<<Rm1[3]; 
	//	cout<<"第二次循环得到的Lm11为："<<Lm11[0]<<Lm11[1]<<Lm11[2]<<Lm11[3]; 
		char mw[8],mw1[8];//最终结果密文为mw1 
		merge(mw,Lm11,Rm1,8);//将Rm1和Lm11合成mw 
	/*	cout<<"IP负1置换前的mw为："<<endl;
			for(int i=0;i<8;i++)
			cout<<mw[i];
			cout<<endl; */
		change(mw,mw1,IP_1,8);//mw 进行IP-1置换，得到密文mw1 
		for(int i=0;i<8;i++)
			cout<<mw1[i];
		cout<<endl;
	    }
    }
    //解密 
    if(choose==2){
    char mkey[10],m[8],temp[8];//主密钥，密文，临时数组 
	char k1[8],k2[8];//子密钥K1，K2 
    char Rm[4],Lm1[4];//分段
    while(cin>>mkey>>m){ //这里的m即为密文 
		key(mkey,k1,k2);//生成子密钥 K1 K2 
		change(m,temp,IP,8);//密文IP置换 
		DES(temp,k2,Rm,Lm1);//第一次循环,产生Rm和Lm1 先用k2 
		char m1[8],Rm1[4],Lm11[4]; 
		merge(m1,Rm,Lm1,8);//将Rm和Lm1左右互换合成m1  
		DES(m1,k1,Rm1,Lm11);//第二次循环,产生Rm1和Lm11 后用k1 
		char mw[8],mw1[8];//最终结果明文为mw1 
		merge(mw,Lm11,Rm1,8);//将Rm1和Lm11合成mw 
		change(mw,mw1,IP_1,8);//mw 进行IP-1置换，得到明文mw1 
		for(int i=0;i<8;i++)
			cout<<mw1[i];
		cout<<endl;
	    }
    }
    
        if(choose==3){
        bool get = false; //确认是否找到了密钥 
        char m[8],mw[8],temp[8];//明文，密文，临时数组 
        char mkey[10] = {'0','0','0','0','0','0','0','0','0','0'};//初始化密钥全为0 
	    char k1[8],k2[8];//子密钥K1，K2 
        char Rm[4],Lm1[4];//分段 
		cout<<"Please enter the plaintext:"<<endl;
		cin>>m;
		cout<<"Please enter the ciphertext:"<<endl;
		cin>>mw; 
        int sum=0;
        while(sum!=11)
		{
	    	for(int i=0;i<10;i++)
            {
            	if(mkey[i]=='0'){
            	mkey[i]='1';
            	break;
	            }
        	    if(mkey[i]=='1'){
        		mkey[i]='0';
		    	}
	        
            } //二进制数组全排列 
            sum=0;
            for(int i=0;i<10;i++)
	    	{
                if(mkey[i]=='1')sum++;
	    	}
	
	    	//开始尝试：对明文加密看结果是否和密文相符 
	     	key(mkey,k1,k2);//生成子密钥 K1 K2 
	    	change(m,temp,IP,8);//明文IP置换 
	    	DES(temp,k1,Rm,Lm1);//第一次循环,产生Rm和Lm1 
	    	char m1[8],Rm1[4],Lm11[4]; 
	    	merge(m1,Rm,Lm1,8);//将Rm和Lm1左右互换合成m1  
	    	DES(m1,k2,Rm1,Lm11);//第二次循环,产生Rm1和Lm11 
	    	char mw1[8],mw11[8];//最终结果密文为mw11 
	    	merge(mw1,Lm11,Rm1,8);//将Rm1和Lm11合成mw1
	    	change(mw1,mw11,IP_1,8);//mw1 进行IP-1置换，得到密文mw11 
	    	int count=0;
	    	for(int i=0;i<8;i++) //比对加密出的密文和输入的密文 
			{
				if(mw[i]==mw11[i])count+=1;
			}
			if(count==8)
			{
				cout<<"破译成功，密钥为："<<endl;
				get = true;
				for(int i=0;i<10;i++)
			    cout<<mkey[i];
			    cout<<endl;
			}
			if(sum==10)sum+=1; //全排列结束条件 数组全为1的试完 
	    		
        }
        if(get == false) cout<<"无对应密钥";
    
        }
        
    // 字符串加密 
    if (choose == 4) {
    	cout<<"Enter 1 for encryption, 2 for decryption"<<endl;
    	int choose1;
    	cin>>choose1;
        char mkey[10], temp[8],cut[8]; // 主密钥，临时数组,切割数组 
        char k1[8], k2[8]; // 子密钥 K1，K2
        char Rm[4], Lm1[4]; // 分段
        cin>>mkey;
        string mm; //利用字符串特性输入统计输入的字符个数  明/密文 
        cin>>mm;
        int length =mm.length(); //输入的字符个数 
       // cout<<length<<endl;
        char m[length]; //字符串转字符数组 
        strcpy(m, mm.c_str());
        
       
	       //将ASCII明文转换为二进制字符串
            char binaryM[length*8];
            asciiToBinaryString(m, binaryM);
         //   cout<<"字符明文/密文转换得到的二进制数组为：";
         //  	for(int i=0;i<length*8;i++)
		//	cout<<binaryM[i];
		//    cout<<endl;
            
            int k=0;
            int flag=0;
            //按照8位一组进行加密，分别输出
			key(mkey,k1,k2);//生成子密钥 K1 K2 
			while(flag<length*8)
			{
		//	if(flag!=0)k+=1;
			for(int i=0;i<8;i++)
			{
				cut[i]=binaryM[k];
			    k++;	
			} //切割数组存放8位 
        //    cout<<"二进制数组进行IP置换得到的cut为：";
        //   	for(int i=0;i<8;i++)
		//	cout<<cut[i];
		//    cout<<endl;
            change(cut, temp, IP, 8);   //明文 IP置换
            if(choose1==1)
	    	DES(temp,k1,Rm,Lm1);//第一次循环,产生Rm和Lm1 
	    	else
	    	DES(temp,k2,Rm,Lm1);//第一次循环,产生Rm和Lm1
	    	
			char m1[8],Rm1[4],Lm11[4]; 
	     	merge(m1,Rm,Lm1,8);//将Rm和Lm1左右互换合成m1  
	     	if(choose1==1)
	    	DES(m1,k2,Rm1,Lm11);//第二次循环,产生Rm1和Lm11 
	    	else
	    	DES(m1,k1,Rm1,Lm11);//第二次循环,产生Rm1和Lm11 
	    	char mw[8],mw1[8];//最终结果明文为mw1 
	    	merge(mw,Lm11,Rm1,8);//将Rm1和Lm11合成mw 
	    	change(mw,mw1,IP_1,8);//mw 进行IP-1置换，得到明文mw1 
	    	int sum1=0;
	    //	cout<<"加/解密后得到的二进制密文为：";
	    	//将8位二进制密文转为对应的字符 
	    	int mw1_int;
	    	for(int i=0;i<8;i++)
	    	{
		//	cout<<mw1[i];
			if(mw1[i]=='0')mw1_int=0;
			else mw1_int=1;
			sum1+=mw1_int*(pow(2,7-i));
	     	}  
		  //  cout<<endl;
          //  cout<<"密文int/明文int"<<sum1<<endl;
          //  cout<<"密文char/明文char"<<char(sum1)<<endl;
            cout<<char(sum1);
			/*if(sum1<=127)cout<<"密文char/明文char"<<char(sum1)<<endl;
			else cout<<"密文char/明文char"<<char(sum1%127)<<endl; */
            
            /*
            //将加密后的二进制结果转换为ASCII密文
            char encryptedAscii[8];
            binaryStringToAscii(mw1, encryptedAscii);

            // 输出加密后的ASCII密文
            cout << "二进制密文转换为字符密文： " << encryptedAscii << endl; 
            */
            flag+=8;
			}

	
        } 
    
    return 0;
}



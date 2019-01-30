#include "yuukouhai.h"
#include <iostream>

using namespace std;

//手牌セット
void Yuukouhai::set_tehai(int t[])
{
	for(int i=0;i<38;i++){
		tehai[i]=t[i];
	}
	fuurosuu=0;
}

//エラーチェック
bool Yuukouhai::error_check()
{
	int tmp=0;
	for(int i=0;i<38;i++){
		tmp+=tehai[i];
	}
	if(tmp%3!=1||tmp>=14||tmp<1) return true;
	return false;
}


void Yuukouhai::NotKoritu ()
{
	int work[38];//作業用
	int i=0,p=0;
	//配列初期化
	memset(work,0,sizeof(work));
	memset(not_koritu,0,sizeof(not_koritu));
	//計算
	for(;i<=37;i++){ 
		for(;!tehai[i];i++) if(i>=38) continue;
		//数牌
		if(i<30){
			if(i%10==1) work[i]=work[i+1]=work[i+2]=1;
			else if(i%10==2) work[i]=work[i+1]=work[i+2]=work[i-1]=1;
			else if(i%10>=3 &&i%10<=7) work[i]=work[i-1]=work[i+1]=work[i-2]=work[i+2] =1;
			else if(i%10==8) work[i]=work[i+1]=work[i-2]=work[i-1]=1;
			else if(i%10==9) work[i]=work[i-1]=work[i-2]=1;             
		}
		//字牌
		else if(i>30) work[i]=1;
	}
	//結果を格納
	for(i=0;i<=37;i++){ 
		for(;!work[i];i++) if(i>=38) continue;
				not_koritu[p++]=i;
	}
}

/*
* チートイツの有効牌
*/
/*
bool Yuukouhai::TiitoiYuukou (vector<int>  TiitoiYuukou [])
{
	if(error_check()) return false;

	int i=0;//配列の添え字ポインタ（孤立牌）
	int syanten=TiitoituSyanten();  //現在の牌のシャンテン数 
	TiitoiYuukou->clear();//初期化 
	while(i<=37){
		//否孤立牌を追加
		for(;!tehai[i];i++);if(i>=38) continue;
		tehai[i]++;
		//現在値と比較
		if(TiitoituSyanten() <syanten)
		TiitoiYuukou->push_back(i);
		//追加した否孤立牌をマイナス
		tehai[i]--;
		i++;
	}	
	return true;
}
*/
/* * チートイツの有効牌 *int TiitoiYuukou [38]に「牌番号」を順に入れています */
void Yuukouhai::TiitoiYuukou (int TiitoiYuukou [])
{
	int i=0;//配列の添え字ポインタ（孤立牌）
	int p_yuukou=0;//配列の添え字ポインタ（有効牌）
	int syanten=TiitoituSyanten();  //現在の牌のシャンテン数 
	//memset(TiitoiYuukou,0,sizeof(TiitoiYuukou)*38);//初期化 
	while(i<=37){
		//否孤立牌を追加
		for(;!tehai[i];i++) if(i>=38) continue;
		tehai[i]++;
		//現在値と比較
		if(TiitoituSyanten() <syanten)
		TiitoiYuukou [p_yuukou++]=i;
		//追加した否孤立牌をマイナス
		tehai[i]--;
		i++;
	}	
}


/*
* 国士無双の有効牌
*/ 
bool Yuukouhai::KokusiYuukou (vector<int>  KokusiYuukou [])
{
	if(error_check()) return false;

	int i=0;
	int syanten=KokusiSyanten();//現在の牌のシャンテン数
	KokusiYuukou->clear();//初期化 
	int yaotyuu[13]={1,9,11,19,21,29,31,32,33,34,35,36,37};
	for(;i<13;i++){
		//否孤立牌を追加
		tehai[yaotyuu[i]]++;
		//現在値と比較
		if(KokusiSyanten() <syanten)
			KokusiYuukou->push_back(yaotyuu[i]);
		//追加した否孤立牌をマイナス
		tehai[yaotyuu[i]]--;
	}
	return true;
}

//国士シャンテン
int Yuukouhai::KokusiSyanten()
{
	int kokusi_toitu=0,syanten_kokusi=13,i;             
	//老頭牌
	for(i=1;i<30;i++){        
		if(i%10==1||i%10==9||i%20==1||i%20==9){	   
			if(tehai[i])
				syanten_kokusi--;
			if(tehai[i] >=2 && kokusi_toitu==0)
				kokusi_toitu=1;	
		}
	}             
	//字牌
	for(i=31;i<38;i++){      
		if(tehai[i]){	        
			syanten_kokusi--;
			if(tehai[i] >=2 && kokusi_toitu==0)
				kokusi_toitu=1;			            
		}
	}             
	//頭
	syanten_kokusi-= kokusi_toitu;             
	return syanten_kokusi;
}

//チートイシャンテン
int Yuukouhai::TiitoituSyanten()
{
	int i=1,toitu=0,syurui=0,syanten_tiitoi;
	//トイツ数を数える
	for(;i<=37;i++){ 
		for(;!tehai[i];i++)
		if(i>=38) continue;
		syurui++;
		if(tehai[i] >=2)
		toitu++;
	}
	syanten_tiitoi=6-toitu;
	//４枚持ちを考慮
	if(syurui<7)
		syanten_tiitoi+= 7-syurui;
	return syanten_tiitoi;
}


//通常手シャンテン
int Yuukouhai::NormalSyanten()
{
	//初期化
	mentu=0;
	toitu=0;
	kouho=0;
	temp=0;
	syanten_normal=8;
	for(int i=0;i<38;i++)
	{
		//頭抜き出し
		if(2 <= tehai[i])
		{            
			toitu++;
			tehai[i] -= 2;
			mentu_cut(1);
			tehai[i] += 2;
			toitu--;
		}
	}
	mentu_cut(1);   //頭無しと仮定して計算

	return syanten_normal-fuurosuu*2;	//最終的な結果
}

//メンツ抜き出し
void Yuukouhai::mentu_cut(int i)
{
	for(;!tehai[i];i++);
	if(i>=38){taatu_cut(1);return;}//メンツを抜き終わったのでターツ抜きへ
	//コーツ
	if(tehai[i]>=3)
	{
		mentu++;
		tehai[i]-=3;
		mentu_cut(i);
		tehai[i]+=3;
		mentu--;
	}
	//シュンツ
	if(tehai[i+1]&&tehai[i+2]&&i<30)
	{
		mentu++;
		tehai[i]--,tehai[i+1]--,tehai[i+2]--;
		mentu_cut(i);
		tehai[i]++,tehai[i+1]++,tehai[i+2]++;
		mentu--;
	}
	//メンツ無しと仮定
	mentu_cut(i+1);
}
//ターツ抜き出し
void Yuukouhai::taatu_cut(int i)
{													   
	for(;!tehai[i];i++);
	if(i>=38) //抜き出し終了
	{
		temp=8-mentu*2-kouho-toitu;
		if(temp<syanten_normal) { syanten_normal=temp; }
		return;
	}
	if(mentu+kouho+fuurosuu<4)
	{            
		//トイツ
		if(tehai[i]==2)
		{
			kouho++;
			tehai[i]-=2;
			taatu_cut(i);
			tehai[i]+=2;
			kouho--;
		}
	             
		//ペンチャンorリャンメン
		if(tehai[i+1]&&i<30)
		{
			kouho++;
			tehai[i]--,tehai[i+1]--;
			taatu_cut(i);
			tehai[i]++,tehai[i+1]++;
			kouho--;
		}
	             
		//カンチャン
		if(tehai[i+2]&&i<30&&i%10<=8)
		{
			kouho++;
			tehai[i]--,tehai[i+2]--;
			taatu_cut(i);
			tehai[i]++,tehai[i+2]++;
			kouho--;
		}
	}
	taatu_cut(i+1);
}

/*
* 通常手の有効牌
*/ 

int Yuukouhai::NormalYuukou ()
{
	// if(error_check()){
    //     printf("error\n");
    //     return false;
    // }
    int p_koritu=0;		//配列の添え字ポインタ（孤立牌）
	int p_yuukou=0;		//配列の添え字ポインタ（有効牌）
	int syanten=NormalSyanten();   	//現在の牌のシャンテン数 
    int yuukou=0;
	NotKoritu ();
	while(not_koritu[p_koritu] != 0){
		//否孤立牌を追加
		tehai[not_koritu[p_koritu]]++;
		//現在値と比較
		if(NormalSyanten()<syanten)
            yuukou++;
		//追加した否孤立牌をマイナス
		tehai[not_koritu[p_koritu]]--;
		p_koritu++;
	}
    // printf("true\n");
	return yuukou;
}

/*
int Yuukouhai::NormalYuukou2 ()
{
	int p_koritu=0;		//配列の添え字ポインタ（孤立牌）
	int p_yuukou=0;		//配列の添え字ポインタ（有効牌）
	int syanten=NormalSyanten();   	//現在の牌のシャンテン数
    int yuukou=0;   //有効牌の枚数
	NotKoritu ();
	while(not_koritu[p_koritu] != 0){
		//否孤立牌を追加
		tehai[not_koritu[p_koritu]]++;
		//現在値と比較
		if(NormalSyanten()<syanten){
            yuukou++;
        }
		//追加した否孤立牌をマイナス
		tehai[not_koritu[p_koritu]]--;
		p_koritu++;
	}
	return yuukou;
}
*/
/*
int Yuukouhai::NormalYuukou2(){
	int p_koritu=0;		//配列の添え字ポインタ（孤立牌）
	int p_yuukou=0;		//配列の添え字ポインタ（有効牌）
	int syanten=NormalSyanten();   	//現在の牌のシャンテン数
    int yuukou=0;   //有効牌の枚数
	NotKoritu ();
	while(not_koritu[p_koritu] != 0){
		//否孤立牌を追加
		tehai[not_koritu[p_koritu]]++;
		//現在値と比較
		if(NormalSyanten()<syanten){
            yuukou++;
        }
		//追加した否孤立牌をマイナス
		tehai[not_koritu[p_koritu]]--;
		p_koritu++;
	}
	return yuukou;
}
*/
/*
int Yuukouhai::NormalYuukou2 ()
{
	int i=0;//配列の添え字ポインタ（孤立牌）
	int p_yuukou=0;//配列の添え字ポインタ（有効牌）
	int syanten=NormalSyanten();  //現在の牌のシャンテン数 
    int yuukou=0;
	//memset(TiitoiYuukou,0,sizeof(TiitoiYuukou)*38);//初期化 
	while(i<=37){
		//牌を切る
		for(;!tehai[i];i++) if(i>=38) continue;
		tehai[i]--;
		//現在値と比較
		if(TiitoituSyanten() <syanten)
		yuukou++;
        //切った牌を戻す
		tehai[i]++;
		i++;
	}	
}
*/


int main() {
	int i;
    int yuukou_syurui_num[38];
    int yuukou_syurui[38];
    int yuukou_maisu[38];
    int kouho[38];
    for(i=0;i<38;i++){
        yuukou_syurui_num[i]=0;
        yuukou_syurui[i]=0;
        yuukou_maisu[i]=0;
    }
    Yuukouhai yuukouhai;
    // 手牌、中身は枚数
    int t[38]={0,
    1,1,0,0,1,1,0,0,0,0,
    0,0,1,0,2,0,0,1,2,0,
    0,0,0,0,2,1,1,0,0,0,
    0,0,0,0,0,0,0};
    // for(i=0;i<38;i++){
    //     cin >> t[i];
    // }

    int j=0;
    for(i=0;i<38;i++){
        if(t[i]){
            t[i]--;
            yuukouhai.set_tehai(t);//手配をセット
            yuukou_syurui[j] = yuukouhai.NormalYuukou();
            t[i]++;
            j++;
        }
    }
    j=0;
    for(i=0;i<38;i++){
        if(t[i]){
            yuukou_maisu[j]=yuukou_syurui[j]*4-t[i];
            j++;
        }
    }        
    int tmp=0;
    int tmp2=0;    
    for (i=0; i<38; ++i) {
        for (j=i+1; j<38; ++j) {
            if (yuukou_maisu[i] < yuukou_maisu[j]) {
                tmp =  yuukou_maisu[i];
                tmp2 = yuukou_syurui[i];
                yuukou_maisu[i] = yuukou_maisu[j];
                yuukou_syurui[i] = yuukou_syurui[j];
                yuukou_maisu[j] = tmp;
                yuukou_syurui[j] = tmp2;
            }
        }
    }
    for(i=0;i<14;i++){
        cout << "yuukou_maisu\t" << yuukou_maisu[i] << endl;
        cout << "hai_number\t" << yuukou_syurui[i] << endl;
    }

    int syanten_number;
    syanten_number = yuukouhai.NormalSyanten();
	cout << "syanten" << syanten_number << endl;
        
	return 0;
}
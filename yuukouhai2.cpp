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
void Yuukouhai::NormalYuukou2 (vector<int>  NormalYuukou[])
{
	int p_koritu=0;		//配列の添え字ポインタ（孤立牌）
	int p_yuukou=0;		//配列の添え字ポインタ（有効牌）
	int syanten=NormalSyanten();   	//現在の牌のシャンテン数 
	NormalYuukou->clear();//初期化 
	NotKoritu ();
	while(not_koritu[p_koritu] != 0){
		//否孤立牌を追加
		tehai[not_koritu[p_koritu]]++;
		//現在値と比較
		if(NormalSyanten() <syanten)
			NormalYuukou->push_back(not_koritu[p_koritu]);
		//追加した否孤立牌をマイナス
		tehai[not_koritu[p_koritu]]--;
		p_koritu++;
	}
}

int main() {
    Yuukouhai yuukouhai;
    int i,j=0,k=0;
    int hai_number[38];
    int yuukou_syurui[38];
    int yuukou_maisuu[38];
    for(i=0;i<38;i++){
        yuukou_syurui[i]=0;
        yuukou_maisuu[i]=0;
        hai_number[i]=0;
    }
    int t[38]={0,
    1,0,1,0,2,2,0,0,0,0,
    0,0,1,1,1,0,0,0,0,0,
    0,0,1,2,2,0,0,0,0,0,
    0,0,0,0,0,0,0};
    for(i=0;i<38;i++){
        cin >> t[i];
    }

    // yuukouhai.set_tehai(t);//手配をセット
    vector<int>  result(38,0);
    int result2[38][38];
    for(i=0;i<38;i++){
        for(j=0;j<38;j++){
            result2[i][j]=0;
        }
    }
    vector<int>  init(38,0);
    for(i=0;i<38;i++){
        if(t[i]){
            yuukouhai.set_tehai(t);//手配をセット
        	int syanten=yuukouhai.NormalSyanten();   	//現在の牌のシャンテン数 
            // cout << "syanten " << syanten << endl;
            t[i]--;
            yuukouhai.set_tehai(t);//手配をセット
        	// cout << "syanten2 " << yuukouhai.NormalSyanten() << endl;   	//現在の牌のシャンテン数 
    		if(yuukouhai.NormalSyanten()<=syanten){
                // cout << "i=" << i << endl;
                hai_number[k]=i;
                // cout << "t[i]=" << t[i] << endl;
                yuukouhai.NormalYuukou2(&result);
                for(j=0;j<38;j++){
                    if(result[j]){
                        // cout << result[j] <<endl;
                        result2[k][j]=result[j];
                        yuukou_syurui[k]++;
                    //     cout << result2[k][j] << endl;
                    // cout << k << ", " << j << endl;
                    }
                }
                result=init;//初期化
                k++;
            }
            t[i]++;
        }
    }
    k=0;
    for(i=0;i<38;i++){
        yuukou_maisuu[k]=yuukou_syurui[i]*4;
        k++;
    }
    int l=0;
    // cout << "aaa"<<result2[1][3] << endl;
    for(k=0;k<38;k++){
        for(i=0;i<38;i++){
            if(t[i]){
                for(j=0;j<38;j++){
                    if(result2[k][j]==i){
                        // cout << k << ", " << j << endl;
                        // cout << "test" << endl;
                        yuukou_maisuu[k]-=t[i];
                    }
                }
            }
        }
    }

    int tmp=0;
    int tmp2=0;    
    for (i=0; i<38; ++i) {
        for (j=i+1; j<38; ++j) {
            if (yuukou_maisuu[i] < yuukou_maisuu[j]) {
                tmp =  yuukou_maisuu[i];
                tmp2 = yuukou_syurui[i];
                yuukou_maisuu[i] = yuukou_maisuu[j];
                yuukou_syurui[i] = yuukou_syurui[j];
                yuukou_maisuu[j] = tmp;
                yuukou_syurui[j] = tmp2;
            }
        }
    }
    //出力 シャンテン数 捨て牌候補の種類数 牌番号
    int syanten_number;
    int kouhosuu;
    syanten_number = yuukouhai.NormalSyanten();
	// cout << "syanten" << syanten_number << endl;
	cout << syanten_number << " ";
    #define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))
    kouhosuu=ARRAY_LENGTH(hai_number);
    if(kouhosuu>5){
        cout << 5 << " ";
        kouhosuu = 5;
    } else {
        cout << kouhosuu << " ";
    }
    for(i=0;i<kouhosuu;i++){
        // cout << "yuukou_syurui\t" << yuukou_syurui[i] << endl;
        // cout << "hai_number" << hai_number[i] << endl;
        cout << hai_number[i] << " ";
    }
    cout << endl;



    // for(i=0;i<15;i++){
    // cout << yuukou_maisuu[i] << endl;
    // }


    

	return 0;
}
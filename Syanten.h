#pragma once
#ifndef __SYANTEN
#define __SYANTEN


//シャンテン数を返すクラス
class Syanten{

	void mentu_cut(int);//メンツ抜き関数
	void taatu_cut(int);//ターツ抜き関数

	int tehai[39];
	int mentu;		//メンツ数
	int toitu;		//トイツ数
	int kouho;		//ターツ数
	int fuurosuu;		//フーロ数
	int temp;		//シャンテン数（計算用）
	int syanten_normal;	//シャンテン数（結果用）

public:
	Syanten(){fuurosuu=0;};//コンストラクタ
	int NormalSyanten();//通常手シャンテン
	int KokusiSyanten();//国士シャンテン
	int TiitoituSyanten();//チートイシャンテン

	void set_tehai(int t[]);//手牌セット（int[38]以上、赤統合済み）
	void set_fuurosuu(int a){fuurosuu=a;}//フーロ数
	void clear();//中身クリア
};

#endif
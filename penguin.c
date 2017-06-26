/***********************************/
/*                                 */
/*         ペンギン飛ばし          */
/*                                 */
/* (c)2004 ヅラChu＠てとら★ぽっと */
/*                                 */
/***********************************/

#include <piece.h>
#include "gamelib.h"

BOOL demo;	// デモモード

#define _GY_	0.05
#define _POW_	0.2
#define _BOUND_	0.5
#define _FRIC_	0.5

PIECE_BMP bg[4], board, pl, pg, scr;
extern unsigned char BG0[], BG1[], BG2[], BG3[], BOARD[], PL[], PG[], SCR[];
const short bgx[4] = { 88, 56, 56,  0 };
const short bgy[4] = {  1, 60, 44, 40 };
const short boardx[5] = { -104, -204, -304, -404, -504 };
int scroll;
float pgx, pgy, pgvx, pgvy;
short scry, score;
BOOL swing;
#define _PLX_	92
#define _PLY_	60
#define _PLW_	24
#define _PGX_	98
#define _PGY_	 2
#define _PGW_	16

PCEWAVEINFO snd1, snd2, snd3, snd4, snd5, snd6;
extern unsigned char SND1[], SND2[], SND3[], SND4[], SND5[], SND6[];

unsigned char gameMode;
#define _GM_INIT_	1
#define _GM_START_	2
#define _GM_FALL_	3
#define _GM_FLY_	4
#define _GM_SCORE_	5

//=============================================================================
//  背景描画
//=============================================================================
void Draw_BackGround()
{
	short i;
	
	pceLCDPaint( 1, 0,  0, 128, 55 );
	pceLCDPaint( 0, 0, 55, 128, 33 );
	for( i = 0; i < 4; i++ ) {
		if( bgx[i]+scroll < 128 ) {
			Draw_Object( &bg[i], bgx[i]+scroll, bgy[i], 0, 0, bg[i].header.w, bg[i].header.h, DRW_NOMAL );
		}
	}
	for( i = 0; i < 5; i++ ) {
		if( boardx[i]+scroll > -22 && boardx[i]+scroll < 128 ) {
			Draw_Object( &board, boardx[i]+scroll, 66, i*24, 0, 24, 16, DRW_NOMAL );
		}
	}
	LCDUpdate = TRUE;
}

//=============================================================================
//  アプリケーションの初期化
//=============================================================================
void pceAppInit( void )
{
	pceCPUSetSpeed( CPU_SPEED_NORMAL );
	pceAppSetProcPeriod( 25 );
	App_Init();		// 初期化処理
	
	demo = FALSE;
	
	Get_PieceBmp( &bg[0], BG0 );
	Get_PieceBmp( &bg[1], BG1 );
	Get_PieceBmp( &bg[2], BG2 );
	Get_PieceBmp( &bg[3], BG3 );
	Get_PieceBmp( &board, BOARD );
	Get_PieceBmp( &pl, PL );
	Get_PieceBmp( &pg, PG );
	Get_PieceBmp( &scr, SCR );

	Get_PieceWave( &snd1, SND1 );
	Get_PieceWave( &snd2, SND2 );
	Get_PieceWave( &snd3, SND3 );
	Get_PieceWave( &snd4, SND4 );
	Get_PieceWave( &snd5, SND5 );
	Get_PieceWave( &snd6, SND6 );

	pceFontSetType( 2 );
	pceFontSetTxColor( 3 );
	pceFontSetBkColor( FC_SPRITE );

	gameMode = _GM_INIT_;
}


//=============================================================================
//  メインプロセス
//=============================================================================
void pceAppProc( int cnt )
{
	short i;
	
	if( pcePadGet() & TRG_D ) { pceAppReqExit( 0 ); }
	if( pcePadGet() & ( TRG_UP | TRG_DN | TRG_LF | TRG_RI ) ) {
		demo = !demo;
		if( demo ) { pcePowerSetReport( PWR_RPTOFF ); }
		else       { pcePowerSetReport( PWR_RPTON  ); }
		LCDUpdate = TRUE;
	}
	if( demo ) { pceAppActiveResponse( AAR_ACTIVE ); }	// デモモード

	switch( gameMode ) {
		case _GM_INIT_:	// 画面・変数初期化
			scroll = 0;
			swing = FALSE;
			Draw_BackGround();
			Draw_Object( &pl, _PLX_, _PLY_, 0, 0, _PLW_, pl.header.h, DRW_NOMAL );
			Draw_Object( &pg, _PGX_, _PGY_, 0, 0, _PGW_, pg.header.h, DRW_NOMAL );
			pceWaveDataOut( 1, &snd1 );	// 鳴き声
			pgx = _PGX_;
			pgy = _PGY_;
			pgvx = pgvy = 0;
			score = 0;
			gameMode = _GM_START_;
			break;
		case _GM_START_:
			if( pcePadGet() & TRG_A ) {
				pceWaveDataOut( 2, &snd2 );	// バット引き音＆飛び立ち音
				gameMode = _GM_FALL_;
			}
			break;
		case _GM_FALL_:
			pgvy += _GY_;
			pgy += pgvy;
			if( pgy >= 76 ) {
				swing = TRUE;
				pgy = 76;
				scry = -24;
				gameMode = _GM_SCORE_;
				pceWaveDataOut( 2, &snd3 );	// 突き刺さり音
			} else {
				if( pcePadGet() & TRG_A && !swing ) {
					swing = TRUE;
					if( pgy >= 50 && pgy <= 70 ) {
						pgvx =  _POW_ * (pgy-35);
						pgvy = -_POW_ * (70-pgy);
						gameMode = _GM_FLY_;
						pceWaveDataOut( 1, &snd4 );	// ヒット音
					} else {
						pceWaveDataOut( 1, &snd5 );	// 空振り音
					}
				}
			}
			Draw_BackGround();
			if( swing ) {
				Draw_Object( &pl, _PLX_, _PLY_, 48, 0, _PLW_, pl.header.h, DRW_NOMAL );
			} else {
				Draw_Object( &pl, _PLX_, _PLY_, 24, 0, _PLW_, pl.header.h, DRW_NOMAL );
			}
			Draw_Object( &pg, pgx, pgy, 16, 0, _PGW_, pg.header.h, DRW_NOMAL );
			break;
		case _GM_FLY_:
			pgx -= pgvx;
			if( pgx <= 48 ) {
				scroll = 48 - pgx;
			}
			score = (_PGX_-pgx)*5;
			pgvy += _GY_;
			pgy += pgvy;
			if( pgy >= 76 ) {
				pgy = 76;
				if( pgvx / 2 < pgvy && swing ) {
					scry = -24;
					gameMode = _GM_SCORE_;
					pceWaveDataOut( 2, &snd3 );	// 突き刺さり音
				} else {
					swing = FALSE;
					if( pgvy > _GY_*10 ) {
						pceWaveDataOut( 2, &snd6 );	// 摩擦音
					}
					pgvy = -pgvy*_BOUND_;
					pgvx -= _FRIC_;
					if( pgvy > -_GY_*10 ) {
						pgvy = 0;
						if( pgvx <= 0 ) {
							scry = -24;
							gameMode = _GM_SCORE_;
						}
					}
				}
			}
			Draw_BackGround();
			if( _PLX_+scroll < 128 ) {
				Draw_Object( &pl, _PLX_+scroll, _PLY_, 48, 0, _PLW_, pl.header.h, DRW_NOMAL );
			}
			if( pgvx / 2 < -pgvy ) {
				i = 48;
			} else if( pgvx / 2 < pgvy ) {
				i = 112;
			} else if( pgvx / 5 < -pgvy ) {
				i = 64;
			} else if( pgvx / 5 < pgvy ) {
				i = 96;
			} else {
				i = 80;
			}
			Draw_Object( &pg, pgx+scroll, pgy, i, 0, _PGW_, pg.header.h, DRW_NOMAL );
			break;
		case _GM_SCORE_:
			if( scry < 60 ) {
				scry += 2;
				Draw_BackGround();
				Draw_Object( &pl, _PLX_+scroll, _PLY_, 0, 0, _PLW_, pl.header.h, DRW_NOMAL );
				Draw_Object( &scr, pgx-19+scroll, scry, 0, 0, scr.header.w, scr.header.h, DRW_NOMAL );
				pceFontSetPos( pgx-15+scroll, scry+5 );
				pceFontPrintf( "%3d.%1d", score/10, score%10 );
				if( !swing ) {
					Draw_Object( &pg, pgx+scroll, pgy, 80, 0, _PGW_, pg.header.h, DRW_NOMAL );
				} else {
					Draw_Object( &pg, pgx+scroll, pgy, 32, 0, _PGW_, pg.header.h, DRW_NOMAL );
				}
			} else {
				if( pcePadGet() & TRG_A ) { gameMode = _GM_INIT_; }
			}
			break;
	}

	if( LCDUpdate ) {	// 画面描画
		LCDUpdate = FALSE;
		pceLCDTrans();
	}
}


//=============================================================================
//  アプリケーションの終了処理
//=============================================================================
void pceAppExit( void )
{
	App_Exit();		// 終了処理
}


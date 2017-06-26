# Piece 用 makefile

# 拡張子の定義

.SUFFIXES:
.SUFFIXES:  .o .s .c

# 生成コマンド・オプションのマクロ

CC = c33-pcc
CFLAGS = -c -gp=0x0 -near -O2 -Wall
AS = c33-pcc
ASFLAGS = -c -gp=0x0 -near
LD = c33-pcc
LDFLAGS = -ls -lm

# 生成規則

.c.o:
	$(CC) $(CFLAGS) $<

.s.o:
	$(AS) $(ASFLAGS) $<

# 構成ファイル・生成ファイルのマクロ

PRGNAME = penguin
FILENAME = penguin
CAPTION = ペンギン飛ばし
ICON = icon.pid
OBJS =	penguin.o\
		gamelib.o\
		bg0.o\
		bg1.o\
		bg2.o\
		bg3.o\
		pl.o\
		pg.o\
		scr.o\
		board.o\
		snd1.o\
		snd2.o\
		snd3.o\
		snd4.o\
		snd5.o\
		snd6.o\

$(PRGNAME).srf : $(OBJS)
	$(LD) $(LDFLAGS) -e$(PRGNAME).srf $(OBJS)

# 依存関係

# フラッシュ書き込みイメージ生成
pex : $(PRGNAME).srf
	ppack -e $(PRGNAME).srf -o$(FILENAME).pex -n$(CAPTION) -i$(ICON)

# クリーンアップ
clean:
	del $(PRGNAME).srf
	del $(FILENAME).pex
	del $(PRGNAME).sym
	del $(PRGNAME).map
	del *.o


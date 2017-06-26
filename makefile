# Piece �p makefile

# �g���q�̒�`

.SUFFIXES:
.SUFFIXES:  .o .s .c

# �����R�}���h�E�I�v�V�����̃}�N��

CC = c33-pcc
CFLAGS = -c -gp=0x0 -near -O2 -Wall
AS = c33-pcc
ASFLAGS = -c -gp=0x0 -near
LD = c33-pcc
LDFLAGS = -ls -lm

# �����K��

.c.o:
	$(CC) $(CFLAGS) $<

.s.o:
	$(AS) $(ASFLAGS) $<

# �\���t�@�C���E�����t�@�C���̃}�N��

PRGNAME = penguin
FILENAME = penguin
CAPTION = �y���M����΂�
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

# �ˑ��֌W

# �t���b�V���������݃C���[�W����
pex : $(PRGNAME).srf
	ppack -e $(PRGNAME).srf -o$(FILENAME).pex -n$(CAPTION) -i$(ICON)

# �N���[���A�b�v
clean:
	del $(PRGNAME).srf
	del $(FILENAME).pex
	del $(PRGNAME).sym
	del $(PRGNAME).map
	del *.o


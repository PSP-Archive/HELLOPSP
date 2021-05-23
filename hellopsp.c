// Hello World for PSP
// Created 2005.04.30  by nem

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "syscall.h"
#include "pg.h"

#include "bitmap.c"


volatile int bSleep=0;

// -----------------------------------------------------------------------------

// �z�[���{�^���I�����ɃR�[���o�b�N
int exit_callback(void)
{
	bSleep=1;
//	set_cpu_clock(0);
//	save_config();
//	if (rom_get_loaded() && rom_has_battery())
//		save_sram(get_sram(), rom_get_info()->ram_size);

	sceKernelExitGame();
	return 0;
}

// �d���X�C�b�`���쎞��s����ɃR�[���o�b�N�B
// ���̊֐����܂����s���ł��T�X�y���h�E�X�^���o�C�ɓ���\��������B
void power_callback(int unknown, int pwrflags)
{
	//if(pwrflags & (POWER_CB_SUSPEND|POWER_CB_STANDBY)){
	if(pwrflags & POWER_CB_POWER){
		if (!bSleep){
			bSleep=1;

			// �t�@�C���A�N�Z�X���ɃT�X�y���h�E�X�^���o�C�����
			// 0byte�̃Z�[�u�t�@�C�����ł��Ă��܂����Ƃ�����̂ŁA
			// �������ݒ��̓T�X�y���h�E�X�^���o�C�𖳌����B
			sceKernelPowerLock(0);
//			set_cpu_clock(0);
//			save_config();
//			if (rom_get_loaded() && rom_has_battery())
//				save_sram(get_sram(), rom_get_info()->ram_size);
			sceKernelPowerUnlock(0);
		}
	}
	if(pwrflags & POWER_CB_BATLOW){
		//renderer_set_msg("PSP Battery is Low!");
		if (!bSleep){
			bSleep=1;

			sceKernelPowerLock(0);
//			set_cpu_clock(0);
//			save_config();
//			if (rom_get_loaded() && rom_has_battery())
//				save_sram(get_sram(), rom_get_info()->ram_size);
			sceKernelPowerUnlock(0);

			// �����T�X�y���h�B
			// �o�b�e������10%��؂�p���[�����v���_�ł��n�߂�ƁA
			// ���삪�ɒ[�ɒx���Ȃ�t���[�Y������Z�[�u�ł��Ȃ��Ȃ����肷��B
			// �s�̃Q�[���ł�0%�܂Ŏg���Ă�悤�Ȃ̂���B
			scePowerRequestSuspend();
		}
	}
	if(pwrflags & POWER_CB_RESCOMP){
		bSleep=0;
	}
	// �R�[���o�b�N�֐��̍ēo�^
	// �i��x�Ă΂ꂽ��ēo�^���Ă����Ȃ��Ǝ��ɃR�[���o�b�N����Ȃ��j
	int cbid = sceKernelCreateCallback("Power Callback", power_callback, NULL);
	scePowerRegisterCallback(0, cbid);
}

// �|�[�����O�p�X���b�h
int CallbackThread(int args, void *argp)
{
	int cbid;

	// �R�[���o�b�N�֐��̓o�^
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	cbid = sceKernelCreateCallback("Power Callback", power_callback, NULL);
	scePowerRegisterCallback(0, cbid);

	// �|�[�����O
	sceKernelSleepThreadCB();

	return 0;
}

int SetupCallbacks(void)
{
	int thid = 0;

	// �|�[�����O�p�X���b�h�̐���
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
		sceKernelStartThread(thid, 0, 0);

	return thid;
}

int xmain(void)
{
	unsigned long fc;
	unsigned long r,g,b,rgb;

	pgInit();
	SetupCallbacks();
	pgScreenFrame(2,0);

	pgFillvram(0);
	pgBitBlt(160,40,80,80,2,image_seclogo);
	pgScreenFlipV();
	pgWaitVn(100);

	pgFillvram(0);
	pgPrint4(4,7,0xffff,"Presents");
	pgBitBlt(160,40,80,80,2,image_seclogo);
	pgScreenFlipV();
	pgWaitVn(200);

	pgFillvram(0);
	pgPrint4(2,1,0xffff,"Produced by");
	pgBitBlt(60,100,80,80,2,image_nem);
	pgBitBlt(260,100,80,80,2,image_sampu);
	pgScreenFlipV();
	pgWaitVn(250);

	pgFillvram(0);
	pgPrint4(0,1,0xffff,"Special Thanks");
	pgScreenFlipV();
	pgWaitVn(50);

	pgFillvram(0);
	pgPrint4(0,1,0xffff,"Special Thanks");
	pgPrint4(4,2,0xffff,"to:     ");
	pgScreenFlipV();
	pgWaitVn(50);

	pgFillvram(0);
	pgPrint4(0,1,0xffff,"Special Thanks");
	pgPrint4(4,2,0xffff,"to:     ");
	pgBitBlt(160,110,80,80,2,image_gelabo);
	pgScreenFlipV();
	pgWaitVn(160);

	pgFillvram(0);
	pgPrint4(0,1,0xffff,"Special Thanks");
	pgPrint4(4,2,0xffff,"to:     ");
	pgPrint4(2,5,0xffff,"ps2dev.org");
	pgPrint4(3,6,0xffff,"and     ");
	pgPrint4(4,7,0xffff,"fellows!");
	pgScreenFlipV();
	pgWaitVn(160);

	pgFillvram(0);
	pgScreenFlipV();
	pgWaitVn(100);

	pgFillvram(0x1880);
	pgPrint(18,32,0xffff,"by nem / sampu-syokunin");
	pgBitBlt(200,160,80,80,1,image_seclogo);
	pgScreenFlipV();

	pgFillvram(0x1880);
	pgPrint(18,32,0xffff,"by nem / sampu-syokunin");
	pgBitBlt(200,160,80,80,1,image_seclogo);
	pgScreenFlipV();

	fc=0;
	while (1) {
		fc++;
		if (fc>=1536) fc=0;

		if (fc<256) {
			r=255;		g=0;		b=fc;
		} else if (fc<512) {
			r=511-fc;	g=0;		b=255;
		} else if (fc<768) {
			r=0;		g=fc-512;	b=255;
		} else if (fc<1024) {
			r=0;		g=255;		b=1023-fc;
		} else if (fc<1280) {
			r=fc-1024;	g=255;		b=0;
		} else {
			r=255;		g=1535-fc;	b=0;
		}
		r>>=3;
		g>>=3;
		b>>=3;
		rgb=(b<<10)+(g<<5)+(r<<0)+0x8000;
		/* 5432 1098 7654 3210 */
		/* 1-bb bb-g ggg- rrrr */

		pgPrint2(4,3,rgb,"Hello World,");
		pgPrint4(6,3,rgb,"P S P !  ");

		pgScreenFlipV();
	}

	return 0;
}

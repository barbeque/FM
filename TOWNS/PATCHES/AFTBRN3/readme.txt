Afterburner 3 YSSCSICD.SYS Patch

Usage:

Compile patch.c, and apply patch as:

patch.exe AB3.EXP AB3PATCH.EXP

Or, if you run PATCH.EXP you can apply patch on FM TOWNS (real hardware or emulator).  AB3.EXP is the executable in the CD-ROM of Afterburner 3 for FM TOWNS.

The patched executable can run from Towns OS V1.1.  It most likely crash if you start from V2.1.

To run the patched executable, do the following:

run386 -callbufs 24 AB3PATCH.EXP

If you omit -callbufs 24, it will not have adequate buffer size for calling real-mode BIOS, and most likely crash in the middle.





�g����:

patch.c���R���p�C����A

patch.exe AB3.EXP AB3PATCH.EXP

�Ƃ��ăp�b�`�𓖂Ă�B���邢�́APATCH.EXP���g����FM TOWNS��(���@�ł��G�~�����[�^�ł�)�Ńp�b�`�𓖂Ă邱�Ƃ��ł���BAB3.EXP��FM TOWNS�pAfterburner 3��CD-ROM��̎��s�t�@�C���B

���̎��s�t�@�C����Towns OS V1.1����N�����\�BV2.1���Ɠr���ŃN���b�V������\���������B

���s�́A

run386 -callbufs 24 AB3PATCH.EXP

�̂悤�ɂ���B-callbufs�I�v�V�������ȗ������CD-ROM��ǂݍ��ނƂ��̃��A�����[�h�Ăяo���p�o�b�t�@������Ȃ��ăN���b�V������B


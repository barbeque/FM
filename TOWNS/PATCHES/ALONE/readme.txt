Alone In The Dark   Patch for Playing BGM from SCSI CD-ROM

Alone In The Dark for FM OTWNS can start from an external SCSI CD-ROM drive using YSSCSICD.SYS if the internal drive is broken.  However, it doesn't play CDDA BGM.

After applying this patch to INDARK.EXP, you can start Alone In The Dark from an external SCSI CD-ROM drive, and it plays CDDA BGM.


Usage:
  patch.exe input.exp output.exp

Please use any C compiler to compile patch.c to build patch.exe.

input.exp is INDARK.EXP in the Alone In The Dark for FM TOWNS CD-ROM.  Output.exp can be something like INDARKP.EXP.


Starting the Game:

Boot into Towns OS V2.1.  The easiest way is to use SCSI CD-ROM Boot floppy disk.  Then, run it from a batch file like:

Q:
CD ALONE
RUN386 A:INDARKP.EXP

If your INDEARKP.EXP is not in A drive, modify it accoringly.





�A���[���E�C���E�U�E�_�[�N FM TOWNS�� SCSI CD-ROM����BGM�Đ��p�b�`

�A���[���E�C���E�U�E�_�[�N FM TOWNS�ł́A����CD�h���C�u�����Ɍ̏Ⴕ�Ă���ꍇYSSCSICD.SYS���g�����ƂŊO�t��SCSI CD-ROM�h���C�u����N���ł���B�������ACDDA�ɂ��BGM�͍Đ�����Ȃ��B

���̃p�b�`��K�p���邱�ƂŁAYSSCSICD.SYS�𗘗p���ĊO�t��CD-ROM�h���C�u����A���[���E�C���E�U�E�_�[�N FM TOWNS�ł��N�������ꍇ�ł�CDDA�ɂ��BGM���Đ������悤�ɂȂ�B


�g�p���@:
  patch.exe input.exp output.exp

patch.c��K����C�R���p�C���ŃR���p�C������patch.exe���r���h����B

input.exp�̓Q�[����CD-ROM�h���C�u��INDARK.EXP�Boutput.exp�͗Ⴆ��INDARKP.EXP�̂悤�ɂ���B


�Q�[���̊J�n:

�܂�Towns OS V2.1���N������B�ȒP�Ȃ̂̓A���[���E�C���E�U�E�_�[�N��CD-ROM��V2.1�N���t���b�s�[�f�B�X�N���g�����@�B���̌�A���̂悤�ȃo�b�`�t�@�C���ŃQ�[�����N���B

Q:
CD ALONE
RUN386 A:INDARKP.EXP

�p�b�`��K�p����INDARKP.EXP��A�h���C�u�ȊO�ɂ���ꍇ�� A: ��K���ɒu��������B


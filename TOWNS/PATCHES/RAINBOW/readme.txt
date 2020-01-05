Rainbow Island YSSCSICD.SYS Patch

This patch will redirect CD-ROM I/O writes to CD-ROM BIOS calls, and Rainbow Island for FM TOWNS will play CDDA BGM from an external SCSI CD-ROM drive.  Without this patch, Rainbos Island can still start and play from an external SCSI CD-ROM drive, but it did not play CDDA BGM.

CDDAPlay will use an expanded function of YSSCSICD.SYS (AX=72C0H) therefore you need to install YSSCSICD.SYS in the boot disk to play CDDA BGM.

Thanks nabe-abk (https://github.com/nabe-abk) for the file format of .EXP!  The executables were encrypted, and without appending patch-applier after the decryptor, I could not make it work.  Without his documentation, I could not append my code after the decryptor.



Usage:

(1) Compile patch to build patch.exe
(2) Copy RI.EXP to the same directory as the patches from Rainbow Island for FM TOWNS CD-ROM..
(3) Run the following command:
  .\patch.exe RI.EXP RIPATCH.EXP
(4) If things go well, you get RIPATCH.EXP
(5) As far as I experimented, it can just be started from Towns OS V1.1 or V2.1 as:
Q:
RUN386 A:RIPATCH.EXP




Rainbow Island YSSCSICD.SYS�p�p�b�`

���̃p�b�`��K�p���邱�ƂŁARainbow Island���O�t��CD-ROM�h���C�u������s�����Ƃ�CDDA�ɂ��BGM���Đ��ł���悤�ɂȂ�B�p�b�`��K�p���Ȃ��Ă��v���C�����͉\�B

CDDA�ɂ��BGM��YSSCSICD.SYS�̊g���@�\(AX=72C0H)���g�p����̂ŁAYSSCSICD.SYS��g�ݍ��܂Ȃ���CDDA�ɂ��BGM�͉��t����Ȃ��B

�g����:

(1) patch.c���R���p�C������patch.exe���r���h�B
(2) Rainbow Island��CD-ROM����RI.EXP���p�b�`�̎��s�t�@�C���Ɠ����f�B���N�g���ɃR�s�[�B
(3) patch.exe�����s:
  .\patch.exe RI.EXP RIPATCH.EXP
(4) ���܂��s���Ă���RIPATCH.EXP���ł��Ă���͂��B
(5) ������������ł́ATowns OS V1.1����ł�V2.1����ł����s�ł���͗l�B�Ⴆ�Ύ��̂悤�ȃo�b�`�t�@�C�����g���B
Q:
RUN386 A:\RIPATCH.EXP



Free386��҂�nabe-abk����(https://github.com/nabe-abk)��EXP�t�@�C���t�H�[�}�b�g�����J���Ă����������������ł��̃p�b�`�������ł��܂����I���s�t�@�C���͈Í�������Ă����̂ŁA�����R�[�h�̖����Ƀp�b�`�K�p�R�[�h��ǉ�����K�v���������̂ł����AEXP�t�@�C���̃t�H�[�}�b�g���킩��Ȃ������̂łł����ɂ��܂����I���肪�Ƃ��������܂��I


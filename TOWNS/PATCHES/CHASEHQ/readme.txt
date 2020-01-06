TAITO Chase HQ for FM TOWNS  YSSCSICD.SYS Patch

This patch enables TAITO Chase HQ for FM TOWNS to start from external SCSI CD-ROM drive by redirecting CD-ROM I/O manipulations to CD-ROM BIOS.

CDDAPlay will use an expanded function of YSSCSICD.SYS (AX=72C0H) therefore you need to install YSSCSICD.SYS in the boot disk to play CDDA BGM.

Thanks nabe-abk (https://github.com/nabe-abk) for the file format of .EXP!  The executables were encrypted, and without appending patch-applier after the decryptor, I could not make it work.  Without his documentation, I could not append my code after the decryptor.

By the way, you don't need a boot floppy if you have a working hard-drive TownsOS installation.  I was able to start directly (without rebooting) from Towns OS V2.1 L31 GUI.



Usage:

(1) Compile patch.c and patch2.c to build patch.exe and patch2.exe.
(2) Copy LASINT.EXP and LASINT2.EXP to the same directory as the patches from TAITO Chase HQ for FM TOWNS CD-ROM..
(3) Run patch.exe, and then patch2.exe.  No parameters are needed.
(4) If things go well, you get LASINTP.EXP and LASINT2P.EXP.
(5) Copy LASINTP.EXP and LASINT2P.EXP to a Towns OS SCSI CD Boot floppy disk.
(6) Use the following AUTOEXEC.BAT



Q:
RUN386 SYS
IF ERRORLEVEL 1 GOTO NORMAL
:LABEL
RUN386 A:LASINTP
RUN386 ENDING
GOTO LABEL

:NORMAL
RUN386 A:LASINT2P
GOTO NORMAL





TAITO Chase HQ for FM TOWNS YSSCSICD.SYS�Ή��p�b�`

���̃p�b�`��K�p���邱�ƂŁAFM TOWNS��TAITO Chase HQ���O�t��CD-ROM�h���C�u������s�ł���悤�ɂȂ�B

CDDA�ɂ��BGM��YSSCSICD.SYS�̊g���@�\(AX=72C0H)���g�p����̂ŁAYSSCSICD.SYS��g�ݍ��܂Ȃ���CDDA�ɂ��BGM�͉��t����Ȃ��B

�n�[�h�f�B�X�N����Towns OS���N���\�ł���΋N���t���b�s�[�f�B�X�N���g��Ȃ��Ă�GUI���璼�ڋN���ł���͗l�BTowns OS V2.1 L31 GUI���畁�ʂɋN���ł����B

�g����:

(1) patch.c, patch2.c���R���p�C������patch.exe��patch2.exe���r���h�B
(2) LASINT.EXP��LASINT2.EXP��TAITO Chase HQ��CD-ROM����p�b�`�̎��s�t�@�C���Ɠ����f�B���N�g���ɃR�s�[�B
(3) patch.exe��patch2.exe�����Ɏ��s�B���Ԃ͋t�ł��������ǁB
(4) ���܂��s���Ă���LASINTP.EXP��LASINT2P.EXP���ł��Ă���͂��B
(5) �쐬����LASINTP.EXP��LASINT2P.EXP��Towns OS SCSI CD�N���t���b�s�[�f�B�X�N�ɃR�s�[�B
(7) ��ɏ������悤��AUTOEXEC.BAT���g���ċN���B



Free386��҂�nabe-abk����(https://github.com/nabe-abk)��EXP�t�@�C���t�H�[�}�b�g�����J���Ă����������������ł��̃p�b�`�������ł��܂����I���s�t�@�C���͈Í�������Ă����̂ŁA�����R�[�h�̖����Ƀp�b�`�K�p�R�[�h��ǉ�����K�v���������̂ł����AEXP�t�@�C���̃t�H�[�}�b�g���킩��Ȃ������̂łł����ɂ��܂����I���肪�Ƃ��������܂��I


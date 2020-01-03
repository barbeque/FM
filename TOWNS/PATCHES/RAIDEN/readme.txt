Raiden for FM TOWNS, Patch for starting from SCSI CD-ROM Drive

Instruction:

Compile and apply patch to R.EXE in the Raiden CD-ROM.

Modify Towns OS V1.1 boot floppy disk so that it changes the drive to Q and then starts the patched R.EXE in the floppy disk.

Or, you can run PATCH.EXP like:

run386 PATCH.EXP Q:\R.EXE A:\RPATCH.EXE

To make a patched executable in FM TOWNS (can be done in AUTOEXEC.BAT)



Reverse Engineering:

Oh my god!  R.EXE has its own CD-ROM BIOS in the executable!  Why did they waste it!?  Raiden's own CD-ROM BIOS takes exactly the same command set as FM TOWNS original CD-ROM BIOS.  What I had to do was to replace Raiden's own BIOS call to INT 93H (FM TOWNS's original CD-ROM BIOS).  It doesn't seem to working as a copy protection.  Did VING's programmers just want to explore more about FM TOWNS CD-ROM I/O then?

The patched R.EXe needs to run from Towns OS V1.1.  Unpatched version runs from Towns OS V2.1 if all the drivers are ripped, but the patched version destroys something while reading a file.  I located where the program is crashing, but it was just a plain fread and didn't seem to be doing anything special.  At this time, I cannot make it run from Towns OS V2.1.  Anyway Raiden crashes if drivers of Towns OS V2.1 is installed.  Therefore it requires restart.  Making it runnable from Towns OS V2.1 is not very meaningful.





���d�`�� for FM TOWNS SCSI CD-ROM�N���p�p�b�`

�g����:

patch.c���R���p�C��&���s���ė��d�`����R.EXE�Ƀp�b�`�𓖂Ă�BTowns OS V1.1�pSCSI CD-ROM�N���t���b�s�[�f�B�X�N���C�����āATowns MENU�̑���Ƀp�b�`�𓖂Ă����s�t�@�C�������s����悤�ɏ���������B

���邢�́APATCH.EXP�����̂悤�Ɏ��s���邱�Ƃ�FM TOWNS��Ńp�b�`�𓖂Ă邱�Ƃ��\�B

run386 PATCH.EXP Q:\R.EXE A:\RPATCH.EXE

AUTOEXEC.BAT�̒��Ŏ��s���邱�ƂŃt���b�s�[�f�B�X�N����N�������^�C�~���O�Ńp�b�`�𓖂Ă邱�Ƃ��\�B



���:

�Ȃ��! ���d�`�����s�t�@�C��R.EXE�͒��ɓƎ���CD-ROM BIOS�������Ă����B�Ȃ�ł���Ȃ��̂��킴�킴�������̂�!?R.EXE�Ǝ�CD-ROM BIOS��FM TOWNS�W��CD-ROM BIOS�Ɠ����R�}���h���󂯕t����̂ŁA�P�ɌĂяo����INT 93H�Ƀ��_�C���N�g���邾���ŗǂ������B�R�s�[�v���e�N�V�����Ƃ��Ă̈Ӗ�������悤�ɂ��v���Ȃ����AVING�̃v���O���}�[�͒P��FM TOWNS��CD-ROM I/O���������Ă݂��������������낤��?

�Ȃ��A�p�b�`�𓖂Ă�R.EXE��Towns OS V1.1����N������K�v������B�p�b�`�𓖂ĂĂ��Ȃ��o�[�W�������ƃh���C�o�������C���X�g�[�����Ă��Ȃ�Towns OS V2.1����̎��s���\���������ǁA�p�b�`�𓖂Ă��o�[�W�����̓t�@�C���̓ǂݍ��݂ŉ�����j�󂵂Ă��܂��͗l(�N���b�V�����Ă���ӏ������肵���񂾂��ǁA���ʂ�fread�݂���������)�B�����_�ł�Towns OS V2.1����̋N���͂ł��Ȃ��B�Ƃ������A�ǂ���Towns OS�̃h���C�o�������Ă���ƃN���b�V�����邩�炪��΂���V2.1����N���ł���悤�ɂ���Ӗ��͂��܂薳���B


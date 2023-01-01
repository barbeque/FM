#include <fssimplewindow.h>

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <string>
#include <chrono>

#include "fm77avkeyboardemu.h"
#include "filedialog.h"
#ifndef YS_RASPBERRYPI
	#include "comport.h"
#else
	#include "RaspPITransmit.h"
#endif
#include "cheaputil.h"
#include "gui.h"
#include "clipboard.h"



class FM77AVKeyboardEmulatorMain
{
private:
	FM77AVKeyboardEmulator fm77avKeyboardEmu;
	CheapGUI gui;
	std::vector <std::string> availablePort;
	std::string port;
	int bitPerSec;
	bool autoPortScan;

	std::string prevPort;
	int prevMode;
	int prevIRToyState;
	int prevIRToyError;
	bool prevAutoTyping;
	std::string prevRKana;

	class CommandParameterInfo
	{
	public:
		bool portSpecified;
		std::string portName;
		CommandParameterInfo()
		{
			Initialize();
		}
		void Initialize(void)
		{
			portSpecified=false;
			portName="";
		}
	};
	CommandParameterInfo cpi;

	CheapGUI::Text *titleTxt=nullptr;
	CheapGUI::Text *statusText=nullptr;
	CheapGUI::Text *errorText=nullptr;

	std::vector <CheapGUI::CheckBox *> portBtn;

	CheapGUI::CheckBox *translateModeBtn=nullptr;
	CheapGUI::CheckBox *directModeBtn=nullptr;
	CheapGUI::CheckBox *rKanaModeBtn=nullptr;
	std::vector <CheapGUI::CheckBox *> modeBtn;

	CheapGUI::PushButton *autoTypingBtn=nullptr,*autoTypingBasicBtn=nullptr,*autoTypeClipboardBtn=nullptr;

	CheapGUI::PushButton *exitBtn=nullptr;
	CheapGUI::PushButton *secretBtn=nullptr;
	CheapGUI::PushButton *capsBtn=nullptr;
	CheapGUI::PushButton *kanaBtn=nullptr;
	CheapGUI::CheckBox *autoStopBtn=nullptr;
	CheapGUI::CheckBox *autoFireBtn=nullptr;

	CheapGUI::Text *rKanaTxt=nullptr;
	CheapGUI::CheckBox *pauseBtn=nullptr;

	CheapGUI::Text *irToyVersionTxt=nullptr;
	CheapGUI::Text *protocolVersionTxt=nullptr;

	CheapGUI::Text *zxcTxt=nullptr;
	CheapGUI::CheckBox *ZXCforLMRSpaceBtn=nullptr;
	CheapGUI::Text *arrowTxt=nullptr;
	CheapGUI::CheckBox *arrowForArrowBtn=nullptr,*arrowFor4DirBtn=nullptr,*arrowFor8DirBtn=nullptr;
	std::vector <CheapGUI::Widget *> assignmentWidget;

	CheapGUI::PushButton *saveCOMPortLogBtn=nullptr;

public:
	bool quit;
	bool ignoreNextLButtonUp;

public:
	bool RecognizeCommandParameter(int ac,char *av[]);
	void Configure(void);

	void Initialize(void);
	void SetUpCheapGUI(void);
	void ProcessUserInput(void);
	void RunOneStep(void);
	void UpdateGUI(void);
	void Draw(void) const;
	void ResetGUI(void);

	void Quit(void);

	bool NeedRedraw(void) const;

private:
	void UpdateIRToyState(CheapGUI::Text *textPtr,int irToyState,int prevIRToyState);
	void UpdateIRToyErrorCode(CheapGUI::Text *textPtr,int irToyError,int prevIRToyError,bool autoTyping,bool prevAutoTyping);
};

void FM77AVKeyboardEmulatorMain::Initialize(void)
{
	quit=false;
	ignoreNextLButtonUp=false;
	autoPortScan=true;

#ifndef YS_RASPBERRYPI
	availablePort=YsCOMPort::FindAvailablePortName();

	port=(0<availablePort.size() ? availablePort.back() : "0");
	if(0<availablePort.size())
	{
		fm77avKeyboardEmu.Connect(port);
	}
	else
	{
		fm77avKeyboardEmu.SetIRToyNotFoundError();
	}
#else
	InitTransmitter();
#endif

	prevPort="";
	prevMode=FM77AVKeyboardEmulator::MODE_NULL;
	prevIRToyState=IRToy_Controller::STATE_NULL;
	prevIRToyError=IRToy_Controller::ERROR_NULL;
	prevAutoTyping=false;
	prevRKana.clear();
}

bool FM77AVKeyboardEmulatorMain::RecognizeCommandParameter(int ac,char *av[])
{
	cpi.Initialize();
	for(int i=1; i<ac; ++i)
	{
		std::string s(av[i]);
		if(s=="-port")
		{
			cpi.portSpecified=true;
			cpi.portName=av[i+1];
			++i;
		}
		else if(s=="-h" || s=="-help")
		{
			printf("Options:\n");
			printf("  -port (portName)\n");
		}
		else
		{
			fprintf(stderr,"Error in the parameter!\n");
			return false;
		}
	}
	return true;
}

void FM77AVKeyboardEmulatorMain::Configure(void)
{
	if(true==cpi.portSpecified)
	{
		autoPortScan=false;
		fm77avKeyboardEmu.Connect(cpi.portName);
		port=cpi.portName;
	}
}

void FM77AVKeyboardEmulatorMain::SetUpCheapGUI(void)
{
	int guiY=8;

	titleTxt=gui.AddText(8,guiY,400,32,"FM77AV Keyboard Emulator");
	guiY+=32;

	gui.AddText(8,guiY,400,32,"CaptainYS (http://www.ysflight.com)");
	guiY+=32;

	statusText=gui.AddText(8,guiY,400,32,"STATUS");
	guiY+=32;

	errorText=gui.AddText(8,guiY,400,32,"CONDITION:OK");
	errorText->SetColor(0,128,0,255);
	guiY+=34;

	if(1<availablePort.size())
	{
		gui.AddText(8,guiY,64,32,"PORT:");
		int guiX=96;
		int portIdx=1;
		for(auto p : availablePort)
		{
		#ifndef _WIN32
			char msg[256];
			sprintf(msg,"%d",portIdx++);
			portBtn.push_back(gui.AddCheckBox(guiX,guiY,32,32,msg));
		#else
			portBtn.push_back(gui.AddCheckBox(guiX,guiY,32,32,p.c_str()));
		#endif
			if(p==this->port)
			{
				portBtn.back()->SetCheck(true);
			}
			guiX+=40;
		}
		gui.SetRadioButtonGroup(portBtn);
		guiY+=34;
	}

	translateModeBtn=gui.AddCheckBox(8,guiY,240,32,"TRANSLATION");
	translateModeBtn->SetCheck(true);
	directModeBtn=gui.AddCheckBox(258,guiY,160,32,"DIRECT");
	rKanaModeBtn=gui.AddCheckBox(428,guiY,140,32,"RKANA");
	guiY+=34;

	modeBtn.push_back(translateModeBtn);
	modeBtn.push_back(directModeBtn);
	modeBtn.push_back(rKanaModeBtn);
	gui.SetRadioButtonGroup(modeBtn);

	exitBtn=gui.AddPushButton(8,guiY,300,32,"EXIT(Shift+ESC)");
	secretBtn=gui.AddPushButton(328,guiY,300,32,"77AV SECRET MSG");
	guiY+=34;

	capsBtn=gui.AddPushButton(8,guiY,128,32,"CAPS");
	kanaBtn=gui.AddPushButton(144,guiY,128,32,"KANA");
	rKanaTxt=gui.AddText(580,guiY,72,32,"");
	guiY+=34;

	pauseBtn=gui.AddCheckBox(8,guiY,112,32,"PAUSE");
	pauseBtn->SetCheck(fm77avKeyboardEmu.GetPause());
	autoStopBtn=gui.AddCheckBox(128,guiY,150,32,"AUTO-STOP");
	autoFireBtn=gui.AddCheckBox(286,guiY,150,32,"AUTO-FIRE");
	guiY+=34;

	autoTypingBtn=gui.AddPushButton(8,guiY,150,32,"AUTO-TYPE");
	autoTypingBasicBtn=gui.AddPushButton(176,guiY,260,32,"AUTO-TYPE(BASIC)");
#ifdef _WIN32
	autoTypeClipboardBtn=gui.AddPushButton(440,guiY,112,32,"PASTE");
#endif
	guiY+=34;

	zxcTxt=gui.AddText(8,guiY,112,32,"ZXC:");
	ZXCforLMRSpaceBtn=gui.AddCheckBox(112,guiY,200,32,"L-M-R SPACE");
	guiY+=34;
	arrowTxt=gui.AddText(8,guiY,112,32,"ARROW:");
	arrowForArrowBtn=gui.AddCheckBox(128,guiY,112,32,"ARROW");
	arrowForArrowBtn->SetCheck(true);
	arrowFor4DirBtn=gui.AddCheckBox(256,guiY,112,32,"2468");
	arrowFor8DirBtn=gui.AddCheckBox(384,guiY,160,32,"12346789");
	guiY+=34;

	assignmentWidget.push_back(zxcTxt);
	assignmentWidget.push_back(ZXCforLMRSpaceBtn);
	assignmentWidget.push_back(arrowTxt);
	assignmentWidget.push_back(arrowForArrowBtn);
	assignmentWidget.push_back(arrowFor4DirBtn);
	assignmentWidget.push_back(arrowFor8DirBtn);
	std::vector <CheapGUI::CheckBox *> arrowBtnGrp;
	arrowBtnGrp.push_back(arrowForArrowBtn);
	arrowBtnGrp.push_back(arrowFor4DirBtn);
	arrowBtnGrp.push_back(arrowFor8DirBtn);
	gui.SetRadioButtonGroup(arrowBtnGrp);

	irToyVersionTxt=gui.AddText(8,guiY,80,32,"");
	protocolVersionTxt=gui.AddText(88,guiY,80,32,"");

	guiY+=34;
	guiY+=34;
	guiY+=34;

	saveCOMPortLogBtn=gui.AddPushButton(8,guiY,440,32,"Save COM Log for Debugging");

	gui.needRedraw=true;
}

void FM77AVKeyboardEmulatorMain::ProcessUserInput(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key && (0!=FsGetKeyState(FSKEY_SHIFT) || 0!=FsGetKeyState(FSKEY_CTRL)))
	{
		Quit();
	}

	if(FSKEY_ESC==key && true==fm77avKeyboardEmu.IsAutoTyping())
	{
		fm77avKeyboardEmu.StopAutoTyping();
	}

	if(FsGetKeyState(FSKEY_T) && FsGetKeyState(FSKEY_R) && FsGetKeyState(FSKEY_A))
	{
		if(fm77avKeyboardEmu.GetMode()!=FM77AVKeyboardEmulator::MODE_TRANSLATION)
		{
			fm77avKeyboardEmu.SetMode(FM77AVKeyboardEmulator::MODE_TRANSLATION);
		}
	}
	if(FsGetKeyState(FSKEY_D) && FsGetKeyState(FSKEY_I) && FsGetKeyState(FSKEY_R))
	{
		if(fm77avKeyboardEmu.GetMode()!=FM77AVKeyboardEmulator::MODE_DIRECT)
		{
			fm77avKeyboardEmu.SetMode(FM77AVKeyboardEmulator::MODE_DIRECT);
		}
	}
	if(FsGetKeyState(FSKEY_R) && FsGetKeyState(FSKEY_K) && FsGetKeyState(FSKEY_A))
	{
		if(fm77avKeyboardEmu.GetMode()!=FM77AVKeyboardEmulator::MODE_RKANA)
		{
			fm77avKeyboardEmu.SetMode(FM77AVKeyboardEmulator::MODE_RKANA);
		}
	}
	if(FsGetKeyState(FSKEY_SHIFT) &&
	   FsGetKeyState(FSKEY_DEL) &&
	   true!=fm77avKeyboardEmu.IsAutoTyping())
	{
		const char *prog=nullptr;
		if(FSKEY_0==key)
		{
			prog="\x08\x08\x08\x08\x08\x08\x08\x08"
				"10 OPEN \"I\",#1,\"COM0:(F8N1)\"\r\n"
				"20 LINE INPUT #1,A$\r\n"
				"30 CLOSE\r\n"
				"40 EXEC VARPTR(A$)\r\n"
				"RUN\r\n";
		}
		else if(FSKEY_1==key)
		{
			prog="\x08\x08\x08\x08\x08\x08\x08\x08"
				"10 OPEN \"I\",#1,\"COM1:(F8N1)\"\r\n"
				"20 LINE INPUT #1,A$\r\n"
				"30 CLOSE\r\n"
				"40 EXEC VARPTR(A$)\r\n"
				"RUN\r\n";
		}
		else if(FSKEY_HOME==key)
		{
			fm77avKeyboardEmu.SendStroke(AVKEY_KANA,false,false,false);
		}
		if(nullptr!=prog)
		{
			key=FSKEY_NULL;
			while(0!=FsInkeyChar());
			std::vector <char> toSend;
			for(int i=0; 0!=prog[i]; ++i)
			{
				toSend.push_back(prog[i]);
			}
			fm77avKeyboardEmu.StartAutoTyping(toSend,500);
		}
	}
	if(FsGetKeyState(FSKEY_H) &&
	   FsGetKeyState(FSKEY_E) &&
	   FsGetKeyState(FSKEY_L) &&
	   FsGetKeyState(FSKEY_P))
	{
		std::vector <char> toSend;
		std::string base=
			"WIDTH 80,25\r\n"
			"60000 REM SHIFT+DEL+0     Auto-Type RS232C Client Loader (COM0).\r\n"
			"60001 REM ShIFT+DEL+1     Auto-Type RS232C Client Loader (COM1).\r\n"
			"60002 REM SHIFT+DEL+HOME  Emit KANA key.\r\n"
			"60003 REM D+I+R           Direct Mode.\r\n"
			"60004 REM T+R+A           Translation Mode.\r\n";
			"60005 REM R+K+A           Romaji Mode (Need to set KANA mode).\r\n";
		switch(fm77avKeyboardEmu.mode)
		{
		case FM77AVKeyboardEmulator::MODE_TRANSLATION:
			base+="60006 REM Now Translation Mode.\r\n";
			break;
		case FM77AVKeyboardEmulator::MODE_DIRECT:
			base+="60007 REM Now Direct Mode.\r\n";
			break;
		case FM77AVKeyboardEmulator::MODE_RKANA:
			base+="60008 REM Now Romaji Kana Mode.\r\n";
			break;
		}
		toSend.push_back(8);
		toSend.push_back(8);
		toSend.push_back(8);
		toSend.push_back(8);
		toSend.push_back(8);
		toSend.push_back(8);
		toSend.push_back(8);
		toSend.push_back(8);
		for(auto c : base)
		{
			toSend.push_back(c);
		}
		fm77avKeyboardEmu.StartAutoTyping(toSend,500);
	}


	int lb,mb,rb,mx,my;
	auto mouseEvt=FsGetMouseEvent(lb,mb,rb,mx,my);
	if(FSMOUSEEVENT_LBUTTONUP==mouseEvt)
	{
		if(true==ignoreNextLButtonUp)
		{
			ignoreNextLButtonUp=false;
		}
		else
		{
			gui.NotifyLButtonUp(mx,my);
		}
	}
	if(FSMOUSEEVENT_LBUTTONDOWN==mouseEvt)
	{
		ignoreNextLButtonUp=false;
	}

	auto lastClicked=gui.GetLastClicked();
	if(nullptr!=lastClicked)
	{
		if(exitBtn==lastClicked)
		{
			Quit();
		}
		if(secretBtn==lastClicked)
		{
			fm77avKeyboardEmu.SendFM77AVSecretMessage();
		}
		if(capsBtn==lastClicked)
		{
			fm77avKeyboardEmu.SendStroke(AVKEY_CAPS,false,false,false);
		}
		if(kanaBtn==lastClicked)
		{
			fm77avKeyboardEmu.SendStroke(AVKEY_KANA,false,false,false);
		}
		if(pauseBtn==lastClicked)
		{
			fm77avKeyboardEmu.SetPause(pauseBtn->GetCheck());
			if(true==fm77avKeyboardEmu.GetPause())
			{
				titleTxt->SetText("FM77AV Keyboard Emulator (PAUSED)");
			}
			else
			{
				titleTxt->SetText("FM77AV Keyboard Emulator");
			}
		}
		if(autoTypingBtn==lastClicked && fm77avKeyboardEmu.GetIRToyState()==IRToy_Controller::STATE_GOWILD)
		{
			FileDialogOption opt;
			auto fName=SelectFile(opt);
			fm77avKeyboardEmu.StartAutoTyping(fName.c_str(),0);
			ignoreNextLButtonUp=true;
		}
		if(autoTypingBasicBtn==lastClicked && fm77avKeyboardEmu.GetIRToyState()==IRToy_Controller::STATE_GOWILD)
		{
			FileDialogOption opt;
			auto fName=SelectFile(opt);
			fm77avKeyboardEmu.StartAutoTyping(fName.c_str(),500);
			ignoreNextLButtonUp=true;
		}
		if(autoTypeClipboardBtn==lastClicked && fm77avKeyboardEmu.GetIRToyState()==IRToy_Controller::STATE_GOWILD)
		{
			auto clipboard=ReadFromClipboard();
			fm77avKeyboardEmu.StartAutoTyping(clipboard,500);
			ignoreNextLButtonUp=true;
		}
		if(autoStopBtn==lastClicked)
		{
			fm77avKeyboardEmu.SetAutoStop(autoStopBtn->GetCheck());
		}
		if(autoFireBtn==lastClicked)
		{
			fm77avKeyboardEmu.SetAutoFire(autoFireBtn->GetCheck());
		}
		if(translateModeBtn==lastClicked ||
		   directModeBtn==lastClicked ||
		   rKanaModeBtn==lastClicked)
		{
			if(translateModeBtn==lastClicked)
			{
				fm77avKeyboardEmu.SetMode(FM77AVKeyboardEmulator::MODE_TRANSLATION);
			}
			else if(directModeBtn==lastClicked)
			{
				fm77avKeyboardEmu.SetMode(FM77AVKeyboardEmulator::MODE_DIRECT);
			}
			else if(rKanaModeBtn==lastClicked)
			{
				fm77avKeyboardEmu.SetMode(FM77AVKeyboardEmulator::MODE_RKANA);
			}
		}
		if(portBtn.size()==availablePort.size())
		{
			for(int i=0; i<portBtn.size(); ++i)
			{
				if(portBtn[i]==lastClicked)
				{
					autoPortScan=false;
					fm77avKeyboardEmu.Connect(availablePort[i]);
					port=availablePort[i];
					break;
				}
			}
		}
		if(ZXCforLMRSpaceBtn==lastClicked)
		{
			if(true==ZXCforLMRSpaceBtn->GetCheck())
			{
				fm77avKeyboardEmu.keyMap.ZXCForLMRSpzce();
			}
			else
			{
				fm77avKeyboardEmu.keyMap.ZXCForZXC();
			}
		}
		if(arrowForArrowBtn==lastClicked)
		{
			fm77avKeyboardEmu.keyMap.ArrowForArrow();
			fm77avKeyboardEmu.SetArrowFor8Dir(false);
		}
		if(arrowFor4DirBtn==lastClicked)
		{
			fm77avKeyboardEmu.keyMap.ArrowFor2468();
			fm77avKeyboardEmu.SetArrowFor8Dir(false);
		}
		if(arrowFor8DirBtn==lastClicked)
		{
			fm77avKeyboardEmu.keyMap.ArrowForNone();
			fm77avKeyboardEmu.SetArrowFor8Dir(true);
		}
		if(saveCOMPortLogBtn==lastClicked)
		{
			auto log=fm77avKeyboardEmu.GetIRToyCOMTransactionLog();
			FILE *fp=fopen("comlog.bin","wb");
			if(nullptr!=fp)
			{
				auto wSize=fwrite(log.data(),1,log.size(),fp);
				fclose(fp);
				if(wSize==log.size())
				{
					statusText->SetText("Saved comlog.bin");
				}
				else
				{
					statusText->SetText("File write error.");
				}
			}
			else
			{
				statusText->SetText("Cannot open log file.");
			}
		}
	}

	fm77avKeyboardEmu.ProcessChar(FsInkeyChar());
	bool shift=FsGetKeyState(FSKEY_SHIFT)!=0;
	bool ctrl=FsGetKeyState(FSKEY_CTRL)!=0;
	fm77avKeyboardEmu.ProcessKeyStroke(key,shift,ctrl,false);
	fm77avKeyboardEmu.ProcessKeyState();
}

void FM77AVKeyboardEmulatorMain::RunOneStep(void)
{
	fm77avKeyboardEmu.RunOneStep();
	if(true==autoPortScan && true==fm77avKeyboardEmu.ConnectionFailed())
	{
		std::string nextPort;
		for(int i=1; i<availablePort.size(); ++i)
		{
			if(availablePort[i]==port)
			{
				nextPort=availablePort[i-1];
				break;
			}
		}
		if(""!=nextPort)
		{
			fm77avKeyboardEmu.Connect(nextPort);
			port=nextPort;
		}
		else
		{
			autoPortScan=false;
		}
	}
	else if(IRToy_Controller::STATE_GOWILD==fm77avKeyboardEmu.GetIRToyState())
	{
		autoPortScan=false;
	}

	if(IRToy_Controller::STATE_CLOSED==fm77avKeyboardEmu.GetIRToyState())
	{
		quit=true;
	}
}

void FM77AVKeyboardEmulatorMain::UpdateGUI(void)
{
	auto newRKana=fm77avKeyboardEmu.GetRKanaBuf();
	if(prevRKana!=newRKana)
	{
		rKanaTxt->SetText(newRKana.c_str());
	}
	prevRKana=newRKana;

	auto irToyState=fm77avKeyboardEmu.GetIRToyState();
	UpdateIRToyState(statusText,irToyState,prevIRToyState);
	if(irToyState!=prevIRToyState)
	{
		irToyVersionTxt->SetText(fm77avKeyboardEmu.GetIRToyVersion().c_str());
		protocolVersionTxt->SetText(fm77avKeyboardEmu.GetIRToyProtocolVersion().c_str());
	}
	prevIRToyState=irToyState;

	auto irToyError=fm77avKeyboardEmu.GetIRToyErrorCode();
	auto autoTyping=fm77avKeyboardEmu.IsAutoTyping();
	UpdateIRToyErrorCode(errorText,irToyError,prevIRToyError,autoTyping,prevAutoTyping);
	prevIRToyError=irToyError;
	prevAutoTyping=autoTyping;

	auto emuMode=fm77avKeyboardEmu.GetMode();
	if(prevMode!=emuMode)
	{
		for(auto btn : modeBtn)
		{
			btn->SetCheck(false);
		}
		switch(emuMode)
		{
		case FM77AVKeyboardEmulator::MODE_TRANSLATION:
			translateModeBtn->SetCheck(true);
			break;
		case FM77AVKeyboardEmulator::MODE_DIRECT:
			directModeBtn->SetCheck(true);
			break;
		case FM77AVKeyboardEmulator::MODE_RKANA:
			rKanaModeBtn->SetCheck(true);
			break;
		}
	}
	prevMode=emuMode;

	if(port!=prevPort && availablePort.size()==portBtn.size())
	{
		for(int i=0; i<availablePort.size(); ++i)
		{
			if(port==availablePort[i])
			{
				portBtn[i]->SetCheck(true);
			}
			else
			{
				portBtn[i]->SetCheck(false);
			}
		}
	}
	prevPort=port;

	if(FM77AVKeyboardEmulator::MODE_DIRECT==emuMode && true==ZXCforLMRSpaceBtn->hide)
	{
		for(auto btnPtr : assignmentWidget)
		{
			btnPtr->Show();
		}
	}
	else if(FM77AVKeyboardEmulator::MODE_DIRECT!=emuMode && true!=ZXCforLMRSpaceBtn->hide)
	{
		for(auto btnPtr : assignmentWidget)
		{
			btnPtr->Hide();
		}
	}
}

void FM77AVKeyboardEmulatorMain::Draw(void) const
{
	gui.Draw();
}

void FM77AVKeyboardEmulatorMain::Quit(void)
{
	if(IRToy_Controller::STATE_GOWILD!=fm77avKeyboardEmu.GetIRToyState() && 
	   IRToy_Controller::STATE_CLOSING!=fm77avKeyboardEmu.GetIRToyState())
	{
		quit=true;
	}
	else
	{
		fm77avKeyboardEmu.StartCloseIRToySequence();
	}
}

bool FM77AVKeyboardEmulatorMain::NeedRedraw(void) const
{
	return gui.needRedraw;
}

void FM77AVKeyboardEmulatorMain::UpdateIRToyState(CheapGUI::Text *statusText,int irToyState,int prevIRToyState)
{
	if(irToyState!=prevIRToyState)
	{
		switch(irToyState)
		{
		case IRToy_Controller::STATE_DISCONNECT:
			statusText->SetText("Disconnected");
			statusText->SetColor(200,200,0,255);
			break;
		case IRToy_Controller::STATE_HANDSHAKE:
			statusText->SetText("Handshake");
			statusText->SetColor(200,200,0,255);
			break;
		case IRToy_Controller::STATE_INITIALIZING:
			statusText->SetText("Initializing");
			statusText->SetColor(200,200,0,255);
			break;
		case IRToy_Controller::STATE_SELFTEST:
			statusText->SetText("Running Self Test");
			statusText->SetColor(200,200,0,255);
			break;
		case IRToy_Controller::STATE_SETSAMPLERMODE:
			statusText->SetText("Entering the Sampler Mode");
			statusText->SetColor(200,200,0,255);
			break;
		case IRToy_Controller::STATE_CONFIGURE:
			statusText->SetText("Final Configuration");
			statusText->SetColor(200,200,0,255);
			break;
		case IRToy_Controller::STATE_GOWILD:
		case IRToy_Controller::STATE_TRANSMITTING:
			if(prevIRToyState!=IRToy_Controller::STATE_GOWILD &&
			   prevIRToyState!=IRToy_Controller::STATE_TRANSMITTING)
			{
				statusText->SetText("Ready");
				statusText->SetColor(0,128,0,255);
			}
			break;

		case IRToy_Controller::STATE_CLOSING:
			statusText->SetText("Closing Connection");
			statusText->SetColor(200,200,0,255);
			break;
		case IRToy_Controller::STATE_CLOSED:
			statusText->SetText("Closed");
			statusText->SetColor(200,200,0,255);
			break;

		case IRToy_Controller::STATE_ERROR:
			statusText->SetText("Device I/O Error");
			statusText->SetColor(255,0,0,255);
			break;
		}
	}
}

void FM77AVKeyboardEmulatorMain::UpdateIRToyErrorCode(CheapGUI::Text *textPtr,int irToyError,int prevIRToyError,bool autoTyping,bool prevAutoTyping)
{
	if(irToyError!=prevIRToyError || autoTyping!=prevAutoTyping)
	{
		if(true==autoTyping)
		{
			textPtr->SetText("AUTO TYPING...");
			textPtr->SetColor(0,255,0,255);
		}
		else
		{
			switch(irToyError)
			{
			case IRToy_Controller::ERROR_NOERROR:
				textPtr->SetText("");
				textPtr->SetColor(0,128,0,255);
				break;
			case IRToy_Controller::ERROR_INITIALIZATION_TIMEOUT:
				textPtr->SetText("CONDITION:INITIALIZATION TIME OUT");
				textPtr->SetColor(255,0,0,255);
				break;
			case IRToy_Controller::ERROR_PACKET_SEND_FAILURE:
				textPtr->SetText("CONDITION:PACKET_SEND_FAILURE");
				textPtr->SetColor(255,0,0,255);
				break;
			case IRToy_Controller::ERROR_IRTOY_NOT_FOUND:
				textPtr->SetText("CONDITION:IR-DEVICE NOT FOUND");
				textPtr->SetColor(255,0,0,255);
				break;
			}
		}
	}
}



////////////////////////////////////////////////////////////

bool OnCloseWindow(void *incoming)
{
	auto appPtr=(FM77AVKeyboardEmulatorMain *)incoming;
	appPtr->Quit();
	return false;
}

int main(int ac,char *av[])
{
	FsOpenWindow(0,0,640,480,1);
	FsChangeToProgramDir();

	CheapGUI::CreateBitmapFontRenderer();



	FM77AVKeyboardEmulatorMain app;
	app.Initialize();
	app.RecognizeCommandParameter(ac,av);
	app.SetUpCheapGUI();
	app.Configure();

	FsRegisterCloseWindowCallBack(OnCloseWindow,&app);
	while(true!=app.quit)
	{
		FsPollDevice();
		app.ProcessUserInput();
		app.RunOneStep();
		app.UpdateGUI();

		if(true==app.NeedRedraw() || 0!=FsCheckWindowExposure())
		{
			int wid,hei;
			FsGetWindowSize(wid,hei);

			CheapGUI::SetBitmapFontRendererViewportSize(wid,hei);

			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glViewport(0,0,wid,hei);
			app.Draw();
			FsSwapBuffers();
		}

		FsSleep(1); // Give some CPU time for COM driver.
	}

	CheapGUI::DeleteBitmapFontRenderer();

	return 0;
}


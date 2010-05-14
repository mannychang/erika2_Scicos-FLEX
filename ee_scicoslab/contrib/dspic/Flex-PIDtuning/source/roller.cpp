// MakeTempFilenameDLL.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "roller.h"
//using namespace std;

#define u(i,j) ((double *)block->inptr[i])[j]
#define y(i,j) ((double *)block->outptr[i])[j]
#define ipar(i) (block->ipar[i])
#define rpar(i) (block->rpar[i])

// Define time in ms wait mutex
#define MUTEX_TIME_MAX			10 //100
#define EVIDENCE_ERROR_RESOURCE 1
#define EVIDENCE_OK_ROLLER		0
#define EVIDENCE_ERROR_OTHER	2
#define MAX_CHANNELS			7
#define RNUM 18

// --------------------- Thread ---------------------------
// Thread Read Packet in the Serial Port. 
int ReadPkgRx(void); 
// Handler Thread ReadPkgRx.
HANDLE hReadPkgRx; 
// Id Thread ReadPkgRx.
DWORD dwReadPkgRxId;
// --------------------------------------------------------

// ---------------- mutex data variable -------------------
// Mutex for State.
HANDLE MutexState;
HANDLE thStart;
static unsigned int temp_rxError = 0;
static unsigned int temp_rxState = 0;
// --------------------------------------------------------

SettingsWindow::SettingsWindow(int x , int y , int w , int h , const char* t) {
  fig = new Fl_Window(x, y, w, h, t);

actual_portname = new Fl_Input(110, 20, 100, 25, "COM port:");
actual_portname->align(FL_ALIGN_LEFT);
actual_portname->value("COM16");

actual_baudrate = new Fl_Input(110, 50, 100, 25, "COM Baudrate:");
actual_baudrate->align(FL_ALIGN_LEFT);
actual_baudrate->value("38400");

button_open = new Fl_Button(50, 80, 60, 60, "Open");
button_open->callback((Fl_Callback*)Button_CB, (void*)("open"));

button_close = new Fl_Button(120, 80, 60, 60, "Close");
button_close->callback((Fl_Callback*)Button_CB, (void*)("close"));

fig->end();
fig->set_non_modal();
}

SettingsWindow::~SettingsWindow() {
  delete fig;
}

const char* SettingsWindow::return_value(const char* s) {
  const char* sret = "unknown";
if(strcmp(s,"com")==0)
{
     sret = actual_portname->value();
}
else if(strcmp(s,"baudrate")==0)
{
     sret = actual_baudrate->value();
}
return sret;
}

PropertiesWindow::PropertiesWindow(int x ,int y ,int w , int h , const char* t ) {
  fig = new Fl_Window(x, y, w, h, t);
actual_mode = new Fl_Output(110, 20, 100, 25, "Mode:");
actual_mode->align(FL_ALIGN_LEFT);
actual_mode->value("normal");
actual_mode->color(fl_rgb_color(225, 225, 225));
actual_state = new Fl_Output(110, 50, 100, 25, "State:");
actual_state->align(FL_ALIGN_LEFT);
actual_state->value("off");
actual_state->color(fl_rgb_color(225, 225, 225));
actual_portname = new Fl_Output(110, 80, 100, 25, "COM port:");
actual_portname->align(FL_ALIGN_LEFT);
actual_portname->value(st.return_value("com"));
actual_portname->color(fl_rgb_color(225, 225, 225));
actual_baudrate = new Fl_Output(110, 110, 100, 25, "COM Baudrate:");
actual_baudrate->align(FL_ALIGN_LEFT);
actual_baudrate->value(st.return_value("baudrate"));
actual_baudrate->color(fl_rgb_color(225, 225, 225));
actual_data = new Fl_Output(110, 140, 100, 25, "Data:");
actual_data->align(FL_ALIGN_LEFT);
actual_data->value("to do");
actual_data->color(fl_rgb_color(225, 225, 225));
actual_error = new Fl_Output(110, 170, 100, 25, "Error:");
actual_error->align(FL_ALIGN_LEFT);
actual_error->value("none");
actual_error->color(fl_rgb_color(225, 225, 225));
fig->end();
fig->set_non_modal();
}

PropertiesWindow::~PropertiesWindow() {
  delete fig;
}

void PropertiesWindow::update(void) {
  //actual_mode->value(mode);
//if(START_FLAG==1)
//    actual_state->value("ON");
//else if(START_FLAG==0)
//    actual_state->value("OFF");
actual_portname->value(st.return_value("com"));
actual_baudrate->value(st.return_value("baudrate"));
actual_data->value("to do");
}

void PropertiesWindow::mode(const char* s) {
  actual_mode->value(s);
}

void PropertiesWindow::state(const char* s) {
  actual_state->value(s);
}

void PropertiesWindow::error(const char*s) {
  actual_error->value(s);
}

ParametersWindow::ParametersWindow(int w, int h, const char* t) {
  fig = new Fl_Window(w, h, t);
fig->end();
fig->set_non_modal();
}

ParametersWindow::~ParametersWindow() {
  delete fig;
}

AboutWindow::AboutWindow(int w, int h, const char* t) {
    fig = new Fl_Window(w, h, t);
	text_about = new Fl_Multiline_Output(10,10,w-30,h-30);
	text_about->value("\t\tPID TUNING GUI\n\nversion: 1.0.0\nInfo at: www.evidence.eu.com");
	text_about->align(FL_ALIGN_BOTTOM);
    fig->end();
    fig->set_non_modal();
}

AboutWindow::~AboutWindow() {
  delete fig;
}




//static FILE* usc = fopen("cmd_log.txt","w"); 
volatile unsigned char START_FLAG; 
volatile unsigned char CMD_FLAG; 
volatile unsigned char MODE_FLAG; 
volatile unsigned char UPDATE_FLAG; 
volatile unsigned char RESET_FLAG; 
volatile unsigned char SEND_FLAG; 
volatile unsigned char lError; 
volatile unsigned char rxEndThread = 0; 
volatile unsigned char OKThread = 0; 
volatile int timer_counter; 
volatile int packet_id; 
volatile int packet_counter; 
volatile double PATH_NUMBER = 0.0;

Fl_Roller* KPxp; 
Fl_Roller* TIxp; 
Fl_Roller* TDxp; 
Fl_Roller* KPyp; 
Fl_Roller* TIyp; 
Fl_Roller* TDyp; 

volatile double TIMEOUT_PERIOD = 1e-1;
static Fl_Multiline_Output *text; 
static Fl_Multiline_Output *text_kpx; 
static Fl_Multiline_Output *text_Tix; 
static Fl_Multiline_Output *text_Tdx; 
static Fl_Multiline_Output *text_kpy; 
static Fl_Multiline_Output *text_Tiy; 
static Fl_Multiline_Output *text_Tdy; 
SettingsWindow st; 
PropertiesWindow pr; 
ParametersWindow par; 
AboutWindow about; 
volatile double Kpx_max		= 10.0; 
volatile double Kpx_step	= 1e-3;
volatile double Kpx_value	= 1.85;
volatile double Tix_max		= 10.0; 
volatile double Tix_step	= 1e-3;
volatile double Tix_value	= 1.85;
volatile double Tdx_max		= 10.0; 
volatile double Tdx_step	= 1e-3;
volatile double Tdx_value	= 1.85;
volatile double Kpy_max		= 10.0; 
volatile double Kpy_step	= 1e-3;
volatile double Kpy_value	= 1.85;
volatile double Tiy_max		= 10.0; 
volatile double Tiy_step	= 1e-3;
volatile double Tiy_value	= 1.85;
volatile double Tdy_max		= 10.0; 
volatile double Tdy_step	= 1e-3;
volatile double Tdy_value	= 1.85;

Fl_Menu_Item menu_[] = {
 {"File", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 //{"Properties", 0,  (Fl_Callback*)Properties_CB, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Quit", 0,  (Fl_Callback*)WinQuit_CB, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 //{"Tools", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 //{"Settings...", 0,  (Fl_Callback*)Settings_CB, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 //{"Parameters...", 0,  (Fl_Callback*)Parameters_CB, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 //{0,0,0,0,0,0,0,0,0},
 {"Info", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"About...", 0,  (Fl_Callback*)About_CB, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Menu_Item menuc_[] = {
 {"None", 0,  (Fl_Callback*)Choice_CB, (void*)("option_1"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Circle", 0,(Fl_Callback*)Choice_CB, (void*)("option_2"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Square", 0,(Fl_Callback*)Choice_CB, (void*)("option_3"), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Button *Button_Update=(Fl_Button *)0;

//int main(int argc, char **argv) {
//  Fl_Double_Window* w;
//  { Fl_Double_Window* o = new Fl_Double_Window(915, 259, "Control Panel");
//    w = o;
//    o->box(FL_UP_BOX);
//    o->color((Fl_Color)33);
//    o->callback((Fl_Callback*)WinQuit_CB);
//    { Fl_Menu_Bar* o = new Fl_Menu_Bar(0, 0, 1137, 20);
//      o->menu(menu_);
//    } // Fl_Menu_Bar* o
//    { Fl_Roller* o = new Fl_Roller(10, 26, 105, 174, "Kp");
//      o->labelcolor(FL_BACKGROUND2_COLOR);
//      o->maximum(10);
//      o->step(0.001);
//      o->value(1.85);
//      KPp = o;
//    } // Fl_Roller* o
//    { Fl_Roller* o = new Fl_Roller(130, 26, 105, 174, "Ti");
//      o->labelcolor(FL_BACKGROUND2_COLOR);
//      o->maximum(10);
//      o->step(0.001);
//      o->value(0.71);
//      Tip = o;
//    } // Fl_Roller* o
//    { Fl_Roller* o = new Fl_Roller(250, 26, 105, 174, "Td");
//      o->labelcolor(FL_BACKGROUND2_COLOR);
//      o->maximum(10);
//      o->step(0.001);
//      o->value(1.85);
//      Tdp = o;
//    } // Fl_Roller* o
//    { Button_Update = new Fl_Button(368, 73, 60, 60, "Update");
//      Button_Update->box(FL_EMBOSSED_BOX);
//      Button_Update->down_box(FL_ENGRAVED_BOX);
//      Button_Update->color((Fl_Color)1);
//      Button_Update->callback((Fl_Callback*)Button_CB, (void*)("update"));
//    } // Fl_Button* Button_Update
//    o->end();
//  } // Fl_Double_Window* o
//  /************ Graphics ***********/
//w->begin();
//
//text = new Fl_Multiline_Output(500,26,360,180,"Multiline Output");
//text->value("Set the configuration parameters\nand press the start button....");
//text->align(FL_ALIGN_BOTTOM);
//
//text_kp = new Fl_Multiline_Output(10,222,105,20,"");
//text_kp->value("off");
//text_kp->align(FL_ALIGN_BOTTOM);
//
//text_Ti = new Fl_Multiline_Output(130,222,105,20,"");
//text_Ti->value("off");
//text_Ti->align(FL_ALIGN_BOTTOM);
//
//text_Td = new Fl_Multiline_Output(250,222,105,20,"");
//text_Td->value("off");
//text_Td->align(FL_ALIGN_BOTTOM);
//
//w->end();
//
///************ Variables ***********/
//timer_counter = 0;
//packet_counter = 0;
//START_FLAG = 0;
//MODE_FLAG = 0;
//CMD_FLAG = 0;
//UPDATE_FLAG = 0;
//RESET_FLAG = 0;
//SEND_FLAG = 0;
//
//TIMEOUT_PERIOD = 1;
//
//KP = 0.0;
//Ti = 0.0;
//Td = 0.0;
//  w->show(argc, argv);
//  return Fl::run();
//}

void Properties_CB(Fl_Widget*, void*) {
  pr.update();
pr.fig->show();
}

void Settings_CB(Fl_Widget*, void*) {
  if(!START_FLAG)
    st.fig->show();
}

void Parameters_CB(Fl_Widget*, void*) {
  if(!START_FLAG)
    par.fig->show();
}

void About_CB(Fl_Widget*, void*) {
  about.fig->show();
}

void Button_CB(Fl_Widget*, void* userdata) {
  //printf("Button: '%s' \n", (const char*)userdata);
if(strcmp((const char*)userdata,"start")==0)
{
    if(START_FLAG)
    {
        return;
    }
}
}

void WinQuit_CB(Fl_Widget*, void*) {
	//  fclose(usc);
	//printf("Bye Bye!\n");
	//exit(0);
	rxEndThread = 1;
}








// ---------------- local function utility ----------------

void InitMutex(void)
{
	// Init Mutex for State.
	MutexState = CreateMutex(0, false, 0);
	thStart = CreateMutex(0, false, 0);
	if (WaitForSingleObject(thStart, 10000) != WAIT_OBJECT_0) {
		lError =  EVIDENCE_ERROR_RESOURCE;
		return;
	}
}

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
// ----------------------------- Api Dll ------------------------------------------

void EvidenceGetData(struct Pid_Parameters* ppx, struct Pid_Parameters* ppy)
{
	static char str[100];
	//if( WaitForSingleObject(MutexState, MUTEX_TIME_MAX ) == WAIT_OBJECT_0 )
	//{
		// to do...
		//printf("pp: %d\n",pp);
		//printf("KPp get: %d/n", KPp);
		if(rxEndThread==0)
		{
			ppx->Kp = KPxp->value();
			//printf("Kpp: %f\n",((Fl_Roller*)KPp)->value());
			ppx->Ti = TIxp->value();
			//printf("Tip: %f\n",((Fl_Roller*)Tip)->value());
			ppx->Td = TDxp->value();
			//printf("Tdp: %f\n",((Fl_Roller*)Tdp)->value());
			ppy->Kp = KPyp->value();
			//printf("Kpp: %f\n",((Fl_Roller*)KPp)->value());
			ppy->Ti = TIyp->value();
			//printf("Tip: %f\n",((Fl_Roller*)Tip)->value());
			ppy->Td = TDyp->value();
			//printf("Tdp: %f\n",((Fl_Roller*)Tdp)->value());
			str[0] = '\0';
			sprintf_s(str,"%f",ppx->Kp);
			text_kpx->value(str);
			str[0] = '\0';
			sprintf_s(str,"%f",ppx->Ti);
			text_Tix->value(str);
			str[0] = '\0';
			sprintf_s(str,"%f",ppx->Td);
			text_Tdx->value(str);
			text_kpx->redraw();
			text_Tix->redraw();
			text_Tdx->redraw();
			str[0] = '\0';
			sprintf_s(str,"%f",ppy->Kp);
			text_kpy->value(str);
			str[0] = '\0';
			sprintf_s(str,"%f",ppy->Ti);
			text_Tiy->value(str);
			str[0] = '\0';
			sprintf_s(str,"%f",ppy->Td);
			text_Tdy->value(str);
			text_kpy->redraw();
			text_Tiy->redraw();
			text_Tdy->redraw();
		}
	//ReleaseMutex( MutexState );
	//}

	return;
}
// --------------------------------------------------------------------------------
void EvidenceCloseRoller(void)
{
	rxEndThread = 1;
	// to do...
	//exit(1);
	return;
}
// --------------------------------------------------------------------------------
//ROLLER_LIB_API int EvidenceOpenRoller(const char* comname, long int com_baudrate)
int EvidenceOpenRoller(void)
{
	InitMutex();
	if(1)
	{
		rxEndThread = 0;
		lError = EVIDENCE_OK_ROLLER;
		hReadPkgRx = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ReadPkgRx, 0, 0, &dwReadPkgRxId );
		//printf("hReadPkgRx: %d\n",hReadPkgRx);
		if(hReadPkgRx == false)
		{
			lError = EVIDENCE_ERROR_RESOURCE;
			rxEndThread = 1;
		}
		if(WaitForSingleObject(thStart, 10000) != WAIT_OBJECT_0) 
		{
			lError = EVIDENCE_ERROR_RESOURCE;
			rxEndThread = 1;
		}
	}
	else
	{
		rxEndThread = 1;
		//lError = EVIDENCE_ERROR_OTHER;
	}
	//printf("rxEndThread: %d\n",rxEndThread);

	return !rxEndThread;
}
// --------------------------------------------------------------------------------
unsigned char EvidenceReturnErrorRoller(void)
{
	// to do...
	return lError;
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
unsigned char EvidenceSetRollerParameters(double* vet)
{
	Kpx_max		= vet[0]; 
	Kpx_step	= vet[1];
	Kpx_value	= vet[2];

	Tix_max		= vet[3]; 
	Tix_step	= vet[4];
	Tix_value	= vet[5];

	Tdx_max		= vet[6]; 
	Tdx_step	= vet[7];
	Tdx_value	= vet[8];

	Kpy_max		= vet[9]; 
	Kpy_step	= vet[10];
	Kpy_value	= vet[11];

	Tiy_max		= vet[12]; 
	Tiy_step	= vet[13];
	Tiy_value	= vet[14];

	Tdy_max		= vet[15]; 
	Tdy_step	= vet[16];
	Tdy_value	= vet[17];
	return lError;
}
// --------------------------------------------------------------------------------

ROLLER_LIB_API void EvidenceAmazingRollers(scicos_block *block,int flag)
{
	int i;
	static struct Pid_Parameters tmpx; //struct Pid_Parameters tmpx;
    static struct Pid_Parameters tmpy; //struct Pid_Parameters tmpy;
    double v[18];        // = {10.0,1e-3,1.85,10.0,1e-3,1.85,10.0,1e-3,1.85,10.0,1e-3,1.85,10.0,1e-3,1.85,10.0,1e-3,1.85};

    switch (flag)
	{
        //**Init
         case 4:
			tmpx.Kp = 0.0;
			tmpx.Ti = 0.0;
			tmpx.Td = 0.0;
			tmpy.Kp = 0.0;
			tmpy.Ti = 0.0;
			tmpy.Td = 0.0;
			PATH_NUMBER = 0.0;
			InitRollerParameters(block,v);
			EvidenceSetRollerParameters(v);
			EvidenceOpenRoller();
			for (i=0;i<MAX_CHANNELS;i++)
				y(0,i)=0.0;
            break;

        //** output update
        case 1:
            EvidenceGetData(&tmpx,&tmpy);
            y(0,0)= tmpx.Kp;
			y(0,1)= tmpx.Ti;
			y(0,2)= tmpx.Td;
			y(0,3)= tmpy.Kp;
			y(0,4)= tmpy.Ti;
			y(0,5)= tmpy.Td;
			y(0,6)= PATH_NUMBER;
            break;

       //** ending
        case 5:
            EvidenceCloseRoller();
            break;
    }
	return;
}

ROLLER_LIB_API void amazing_ground(scicos_block *block,int flag)
{
    switch (flag)
	{
        //**Init
         case 4:
            break;

        //** output update
        case 1:
            break;

       //** ending
        case 5:
            break;
    }
	return;
}

void InitRollerParameters(scicos_block *block, double* p)
{
    int i = 0;
    for(i=0;i<RNUM;i++)
        p[i] = rpar(i);
    return;                      
}

// -------------------- Thread Read Packet in Serial Port --------------------------
//void Timer_CB(void*);
int ReadPkgRx(void) 
{ 
	ReleaseMutex(thStart);
	Fl_Double_Window* w;
	{	Fl_Double_Window* o = new Fl_Double_Window(915, 525, "Control Panel");
		w = o;
		o->box(FL_UP_BOX);
		o->color((Fl_Color)33);
		o->callback((Fl_Callback*)WinQuit_CB);
		{	Fl_Menu_Bar* o = new Fl_Menu_Bar(0, 0, 1137, 20);
		o->menu(menu_);
		}	// Fl_Menu_Bar* o
		{	Fl_Roller* o = new Fl_Roller(10, 26, 105, 174, "Kpx");
		o->labelcolor(FL_BACKGROUND2_COLOR);
		o->maximum(Kpx_max); //o->maximum(10); //o->maximum(Kpx_max);
		o->step(Kpx_step);   //o->step(0.001); //o->step(Kpx_step);
		o->value(Kpx_value); //o->value(1.85); //o->value(Kpx_value); 
		KPxp = o;
		//printf("o: %d/n", o);
		//printf("KPp: %d/n", KPp);
		}	// Fl_Roller* o
		{	Fl_Roller* o = new Fl_Roller(130, 26, 105, 174, "Tix");
		o->labelcolor(FL_BACKGROUND2_COLOR);
		o->maximum(Tix_max); //o->maximum(10); //o->maximum(Tix_max);
		o->step(Tix_step);   //o->step(0.001); //o->step(Tix_step);
		o->value(Tix_value); //o->value(0.71); //o->value(Tix_value); 
		TIxp = o;
		}	// Fl_Roller* o
		{	Fl_Roller* o = new Fl_Roller(250, 26, 105, 174, "Tdx");
		o->labelcolor(FL_BACKGROUND2_COLOR);
		o->maximum(Tdx_max); //o->maximum(10); //o->maximum(Tdx_max);
		o->step(Tdx_step);   //o->step(0.001); //o->step(Tdx_step);
		o->value(Tdx_value); //o->value(1.85); //o->value(Tdx_value); 
		TDxp = o;
		}	// Fl_Roller* o
		{	Fl_Roller* o = new Fl_Roller(10, 300, 105, 174, "Kpy");
		o->labelcolor(FL_BACKGROUND2_COLOR);
		o->maximum(Kpy_max); //o->maximum(10); //o->maximum(Kpy_max);
		o->step(Kpy_step);   //o->step(0.001); //o->step(Kpy_step);
		o->value(Kpy_value); //o->value(1.85); //o->value(Kpy_value); 
		KPyp = o;
		//printf("o: %d/n", o);
		//printf("KPp: %d/n", KPp);
		}	// Fl_Roller* o
		{	Fl_Roller* o = new Fl_Roller(130, 300, 105, 174, "Tiy");
		o->labelcolor(FL_BACKGROUND2_COLOR);
		o->maximum(Tiy_max); //o->maximum(10); //o->maximum(Tiy_max);
		o->step(Tiy_step);   //o->step(0.001); //o->step(Tiy_step);
		o->value(Tiy_value); //o->value(0.71); //o->value(Tiy_value); 
		TIyp = o;
		}	// Fl_Roller* o
		{	Fl_Roller* o = new Fl_Roller(250, 300, 105, 174, "Tdy");
		o->labelcolor(FL_BACKGROUND2_COLOR);
		o->maximum(Tdy_max); //o->maximum(10); //o->maximum(Tdy_max);
		o->step(Tdy_step);   //o->step(0.001); //o->step(Tdy_step);
		o->value(Tdy_value); //o->value(1.85); //o->value(Tdy_value); 
		TDyp = o;
		}	// Fl_Roller* o
		//{	Button_Update = new Fl_Button(368, 73, 60, 60, "Update");
		//Button_Update->box(FL_EMBOSSED_BOX);
		//Button_Update->down_box(FL_ENGRAVED_BOX);
		//Button_Update->color((Fl_Color)1);
		//Button_Update->callback((Fl_Callback*)Button_CB, (void*)("update"));
		//}	// Fl_Button* Button_Update
		{ Fl_Choice* o = new Fl_Choice(400, 26, 100, 25, "Path");
		o->down_box(FL_BORDER_BOX);
		o->labelcolor(FL_SELECTION_COLOR);
		o->menu(menuc_);
		} // Fl_Choice* o
		o->end();
	} // Fl_Double_Window* o
	/************ Graphics ***********/
	w->begin();
	char aux1[1000];
	char aux2[100];
	text = new Fl_Multiline_Output(550,26,330,480,"Multiline Output");
	aux1[0] = '\0';
	strcat_s(aux1,"\t\t\tPID TUNING GUI\n\n");
	aux2[0] = '\0'; 
	sprintf_s(aux2,"Rollers configuration:\n\n");
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\t\t\tRoller Kpx\n");
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tKpx max:\t\t%f\n",Kpx_max);
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tKpx step:\t\t%f\n",Kpx_step);
	strcat_s(aux1,aux2);
	//aux2[0] = '\0'; 
	//sprintf_s(aux2,"\tKpx initial value:\t%f\n",Kpx_value);
	//strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\t\t\tRoller Tix\n");
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tTix max:\t\t%f\n",Tix_max);
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tTix step:\t\t%f\n",Tix_step);
	strcat_s(aux1,aux2);
	//aux2[0] = '\0'; 
	//sprintf_s(aux2,"\tTix initial value:\t%f\n",Tix_value);
	//strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\t\t\tRoller Tdx\n");
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tTdx max:\t\t%f\n",Tdx_max);
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tTdx step:\t\t%f\n",Tdx_step);
	strcat_s(aux1,aux2);
	//aux2[0] = '\0'; 
	//sprintf_s(aux2,"\tTdx initial value:\t%f\n",Tdx_value);
	//strcat_s(aux1,aux2);

	aux2[0] = '\0'; 
	sprintf_s(aux2,"\t\t\tRoller Kpy\n");
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tKpy max:\t\t%f\n",Kpy_max);
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tKpy step:\t\t%f\n",Kpy_step);
	strcat_s(aux1,aux2);
	//aux2[0] = '\0'; 
	//sprintf_s(aux2,"\tKpy initial value:\t%f\n",Kpy_value);
	//strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\t\t\tRoller Tiy\n");
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tTiy max:\t\t%f\n",Tiy_max);
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tTiy step:\t\t%f\n",Tiy_step);
	strcat_s(aux1,aux2);
	//aux2[0] = '\0'; 
	//sprintf_s(aux2,"\tTiy initial value:\t%f\n",Tiy_value);
	//strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\t\t\tRoller Tdy\n");
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tTdy max:\t%f\n",Tdy_max);
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\tTdy step:\t%f\n",Tdy_step);
	strcat_s(aux1,aux2);
	//aux2[0] = '\0'; 
	//sprintf_s(aux2,"\tTdy initial value:\t%f\n",Tdy_value);
	//strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\n\nInfo at:\n",Tdy_value);
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\twww.evidence.eu.com\n",Tdy_value);
	strcat_s(aux1,aux2);
	aux2[0] = '\0'; 
	sprintf_s(aux2,"\twww.scilab.org\n",Tdy_value);
	strcat_s(aux1,aux2);

	text->value(aux1);
	text->align(FL_ALIGN_BOTTOM);

	text_kpx = new Fl_Multiline_Output(10,222,105,20,"");
	text_kpx->value("off");
	text_kpx->align(FL_ALIGN_BOTTOM);
	text_Tix = new Fl_Multiline_Output(130,222,105,20,"");
	text_Tix->value("off");
	text_Tix->align(FL_ALIGN_BOTTOM);
	text_Tdx = new Fl_Multiline_Output(250,222,105,20,"");
	text_Tdx->value("off");
	text_Tdx->align(FL_ALIGN_BOTTOM);
	text_kpy = new Fl_Multiline_Output(10,496,105,20,"");
	text_kpy->value("off");
	text_kpy->align(FL_ALIGN_BOTTOM);
	text_Tiy = new Fl_Multiline_Output(130,496,105,20,"");
	text_Tiy->value("off");
	text_Tiy->align(FL_ALIGN_BOTTOM);
	text_Tdy = new Fl_Multiline_Output(250,496,105,20,"");
	text_Tdy->value("off");
	text_Tdy->align(FL_ALIGN_BOTTOM);
	w->end();

	/************ Variables ***********/
	timer_counter = 0;
	packet_counter = 0;
	START_FLAG = 0;
	MODE_FLAG = 0;
	CMD_FLAG = 0;
	UPDATE_FLAG = 0;
	RESET_FLAG = 0;
	SEND_FLAG = 0;
	TIMEOUT_PERIOD = 1;
	//char *st = "";//"/cygdrive/c/cygwin_work/rollers/fluid_work/ctrl_window";
	//char **argv = &st;
	w->show();//(1, argv);
	//Fl::add_timeout(TIMEOUT_PERIOD, Timer_CB);
	rxEndThread = 0;
	//return Fl::run();
	while(Fl::check())
	{
		if(rxEndThread)
		{
		    CloseHandle(MutexState);
			CloseHandle(hReadPkgRx);
			w->hide();
			//fprintf(usc,"ciao!");
			//fclose(usc);
			break;
		}
	}
	return 1;
	//   while( !rxEndThread ) 
	//{
	//	// There are a Packet?.
	//	while( pkgProcess( &BufferRx ) == true )
	//	{		
	//		/* Mutex. */
	//		thread_counter++;
	//		//printf("thread_counter: %d\n",thread_counter);
	//		if (WaitForSingleObject(MutexState, 0) == WAIT_OBJECT_0) 
	//		{
	//			rxEError	= pkgGetRxError();
	//			if( rxEError == (unsigned char)(0xD1) )
	//			{
	//				...
	//			}
	//			ReleaseMutex(MutexState);
	//		}
	//	} 
	//} 
} 

void Choice_CB(Fl_Menu_*, void* userdata)
{
	if(strcmp((const char*)userdata,"option_1")==0)	// None
	{
		PATH_NUMBER = 1.111e+001;
	}
	else if(strcmp((const char*)userdata,"option_2")==0)	// Circle
	{
		PATH_NUMBER = 2.222e+004;
	}
	else if(strcmp((const char*)userdata,"option_3")==0)	// Square
	{
		PATH_NUMBER = 3.333e+003;
	}
	return;
}

// ---------------------------------------------------------------------------------

//void Timer_CB(void*) 
//{
//	static char str[100];
//	if( WaitForSingleObject(MutexState, MUTEX_TIME_MAX ) == WAIT_OBJECT_0 )
//	{
//		// to do...
//		printf("pp: %d\n",pp);
//
//		pp->Kp = ((Fl_Roller*)KPp)->value();
//		printf("Kpp: %f\n",((Fl_Roller*)KPp)->value());
//		pp->Ti = ((Fl_Roller*)Tip)->value();
//		printf("Tip: %f\n",((Fl_Roller*)Tip)->value());
//		pp->Td = ((Fl_Roller*)Tdp)->value();
//		printf("Tdp: %f\n",((Fl_Roller*)Tdp)->value());
//
//		str[0] = '\0';
//		sprintf_s(str,"%f",pp->Kp);
//		text_kp->value(str);
//		str[0] = '\0';
//		sprintf_s(str,"%f",pp->Ti);
//		text_Ti->value(str);
//		str[0] = '\0';
//		sprintf_s(str,"%f",pp->Td);
//		text_Td->value(str);
//
//		text_kp->redraw();
//		text_Ti->redraw();
//		text_Td->redraw();
//		
//		ReleaseMutex( MutexState );
//	}
//	Fl::add_timeout(TIMEOUT_PERIOD, Timer_CB);
//}
#line 1 "usbdsc.c"
#line 1 "usbdsc.c"

#line 33 "usbdsc.c"
 


#line 41 "usbdsc.c"
 
 
 
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
 


#line 39 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

typedef unsigned char   byte;           
typedef unsigned int    word;           
typedef unsigned long   dword;          

typedef union _BYTE
{
    byte _byte;
    struct
    {
        unsigned b0:1;
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
    };
} BYTE;

typedef union _WORD
{
    word _word;
    struct
    {
        byte byte0;
        byte byte1;
    };
    struct
    {
        BYTE Byte0;
        BYTE Byte1;
    };
    struct
    {
        BYTE LowB;
        BYTE HighB;
    };
    struct
    {
        byte v[2];
    };
} WORD;
#line 84 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 85 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

typedef union _DWORD
{
    dword _dword;
    struct
    {
        byte byte0;
        byte byte1;
        byte byte2;
        byte byte3;
    };
    struct
    {
        word word0;
        word word1;
    };
    struct
    {
        BYTE Byte0;
        BYTE Byte1;
        BYTE Byte2;
        BYTE Byte3;
    };
    struct
    {
        WORD Word0;
        WORD Word1;
    };
    struct
    {
        byte v[4];
    };
} DWORD;
#line 119 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 120 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 121 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 122 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

typedef void(*pFunc)(void);

typedef union _POINTER
{
    struct
    {
        byte bLow;
        byte bHigh;
        
    };
    word _word;                         
    
    

    byte* bRam;                         
                                        
    word* wRam;                         
                                        

    rom byte* bRom;                     
    rom word* wRom;
    
    
    
    
} POINTER;

typedef enum _BOOL { FALSE = 0, TRUE } BOOL;

#line 153 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 154 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 156 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 44 "usbdsc.c"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
 

#line 38 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"


#line 45 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
 

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 32 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
 


#line 36 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

 
#line 39 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 40 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

 
#line 43 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 44 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"


#line 47 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
 



 
#line 53 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"




#line 60 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
 
#line 62 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 63 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 64 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 65 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 66 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

 

#line 70 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
 

 
#line 74 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 75 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 76 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 78 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 79 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 81 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 82 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

 

#line 88 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 89 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"


#line 94 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 95 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"


#line 100 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 101 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"


#line 104 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 106 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 47 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
 


#line 39 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
 

#line 42 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"

 
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
 

#line 156 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 44 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"


 

 
#line 50 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 51 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 52 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 53 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 54 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"


#line 62 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
 
#line 64 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 65 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 66 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 67 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 68 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 69 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 70 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 71 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 72 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 73 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 74 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 75 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 76 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 77 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 78 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 79 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 80 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 81 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 82 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 83 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 84 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 85 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 86 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 87 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 88 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 89 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 90 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 91 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 92 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 93 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"

 
#line 96 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 97 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 98 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"

 
#line 101 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 102 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 103 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 104 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"

 
#line 107 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 108 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 109 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 110 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"

 
#line 113 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 114 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 115 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"


 


#line 121 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
 
typedef struct _USB_DEV_DSC
{
    byte bLength;       byte bDscType;      word bcdUSB;
    byte bDevCls;       byte bDevSubCls;    byte bDevProtocol;
    byte bMaxPktSize0;  word idVendor;      word idProduct;
    word bcdDevice;     byte iMFR;          byte iProduct;
    byte iSerialNum;    byte bNumCfg;
} USB_DEV_DSC;


#line 133 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
 
typedef struct _USB_CFG_DSC
{
    byte bLength;       byte bDscType;      word wTotalLength;
    byte bNumIntf;      byte bCfgValue;     byte iCfg;
    byte bmAttributes;  byte bMaxPower;
} USB_CFG_DSC;


#line 143 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
 
typedef struct _USB_INTF_DSC
{
    byte bLength;       byte bDscType;      byte bIntfNum;
    byte bAltSetting;   byte bNumEPs;       byte bIntfCls;
    byte bIntfSubCls;   byte bIntfProtocol; byte iIntf;
} USB_INTF_DSC;


#line 153 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
 
typedef struct _USB_EP_DSC
{
    byte bLength;       byte bDscType;      byte bEPAdr;
    byte bmAttributes;  word wMaxPktSize;   byte bInterval;
} USB_EP_DSC;

#line 161 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_std_dsc.h"
#line 48 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"

#line 33 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"
 


#line 38 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"
 


#line 42 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"

 
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
 

#line 156 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 44 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 32 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
 


#line 47 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 60 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 70 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 88 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 94 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 100 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 106 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 45 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"


#line 48 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
 

#line 38 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

 
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
 

#line 156 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 40 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"


 

 
#line 46 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 47 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 48 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 49 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 50 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 51 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

 
#line 54 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 55 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 56 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

 
#line 59 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 60 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"


 
#line 64 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

 
#line 67 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

 
#line 70 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 71 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 72 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"


#line 89 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
 
#line 91 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"


#line 108 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
 
#line 110 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"


#line 127 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
 
#line 129 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

 
typedef struct _USB_HID_DSC_HEADER
{
    byte bDscType;
    word wDscLength;
} USB_HID_DSC_HEADER;

typedef struct _USB_HID_DSC
{
    byte bLength;       byte bDscType;      word bcdHID;
    byte bCountryCode;  byte bNumDsc;
    USB_HID_DSC_HEADER hid_dsc_header[1 ];
    
#line 144 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
 
} USB_HID_DSC;

 
extern byte hid_rpt_rx_len;

 
void HIDInitEP(void);
void USBCheckHIDRequest(void);
void HIDTxReport(char *buffer, byte len);
byte HIDRxReport(char *buffer, byte len);

#line 157 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 48 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"

#line 50 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"

#line 52 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"
#line 54 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
 

#line 45 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 59 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 61 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 63 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 65 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 67 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 69 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 72 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 55 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"


 

#line 64 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"
#line 65 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"


#line 71 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"
#line 72 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"

 
extern rom USB_DEV_DSC device_dsc;
extern rom struct{ USB_CFG_DSC cd01; USB_INTF_DSC i00a00; USB_HID_DSC hid_i00a00; USB_EP_DSC ep01i_i00a00; USB_EP_DSC ep01o_i00a00;} cfg01 ;
extern rom struct{ USB_CFG_DSC cd01; USB_INTF_DSC i00a00; USB_EP_DSC ep01i_i00a00; USB_EP_DSC ep01o_i00a00;} cfg02 ;
extern rom const unsigned char *rom USB_CD_Ptr[];
extern rom const unsigned char *rom USB_SD_Ptr[];

extern rom struct{byte report[29];} hid_rpt01;
extern rom pFunc ClassReqHandler[1];

#line 84 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbdsc.h"
#line 49 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"


#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_ep0_buff.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_ep0_buff.h"
 


#line 39 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_ep0_buff.h"
 

#line 42 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_ep0_buff.h"

 
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
 

#line 156 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 44 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_ep0_buff.h"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 32 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
 


#line 47 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 60 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 70 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 88 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 94 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"

#line 100 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 106 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/autofiles\usbcfg.h"
#line 45 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_ep0_buff.h"
       


#line 58 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_ep0_buff.h"
 
typedef union _CTRL_TRF_SETUP
{
     
    struct
    {
        byte _byte[8 ];
    };
    
     
    struct
    {
        byte bmRequestType;
        byte bRequest;    
        word wValue;
        word wIndex;
        word wLength;
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        WORD W_Value;
        WORD W_Index;
        WORD W_Length;
    };
    struct
    {
        unsigned Recipient:5;           
        unsigned RequestType:2;         
        unsigned DataDir:1;             
        unsigned :8;
        byte bFeature;                  
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        byte bDscIndex;                 
        byte bDscType;                  
        word wLangID;                   
        unsigned :8;
        unsigned :8;
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        BYTE bDevADR;                   
        byte bDevADRH;                  
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        byte bCfgValue;                 
        byte bCfgRSD;                   
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        byte bAltID;                    
        byte bAltID_H;                  
        byte bIntfID;                   
        byte bIntfID_H;                 
        unsigned :8;
        unsigned :8;
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        byte bEPID;                     
        byte bEPID_H;                   
        unsigned :8;
        unsigned :8;
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned EPNum:4;               
        unsigned :3;
        unsigned EPDir:1;               
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
     
    
} CTRL_TRF_SETUP;


#line 178 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_ep0_buff.h"
 
typedef union _CTRL_TRF_DATA
{
     
    struct
    {
        byte _byte[8 ];
    };
    
     
    struct
    {
        byte _byte0;
        byte _byte1;
        byte _byte2;
        byte _byte3;
        byte _byte4;
        byte _byte5;
        byte _byte6;
        byte _byte7;
    };
    struct
    {
        word _word0;
        word _word1;
        word _word2;
        word _word3;
    };

} CTRL_TRF_DATA;

#line 210 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdefs\usbdefs_ep0_buff.h"
#line 51 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
 


#line 39 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"

 
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
 

#line 156 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 41 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"


 

 
#line 47 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 48 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 49 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 50 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 51 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 52 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 53 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 54 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 55 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"

 
#line 58 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 59 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 60 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 61 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 62 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 63 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 64 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"

 
#line 67 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 68 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"

 
typedef union _USB_DEVICE_STATUS
{
    byte _byte;
    struct
    {
        unsigned RemoteWakeup:1;
        unsigned ctrl_trf_mem:1;
    };
} USB_DEVICE_STATUS;

typedef union _BD_STAT
{
    byte _byte;
    struct{
        unsigned BC8:1;
        unsigned BC9:1;
        unsigned BSTALL:1;              
        unsigned DTSEN:1;               
        unsigned INCDIS:1;              
        unsigned KEN:1;                 
        unsigned DTS:1;                 
        unsigned UOWN:1;                
    };
    struct{
        unsigned BC8:1;
        unsigned BC9:1;
        unsigned PID0:1;
        unsigned PID1:1;
        unsigned PID2:1;
        unsigned PID3:1;
        unsigned :1;
        unsigned UOWN:1;
    };
    struct{
        unsigned :2;
        unsigned PID:4;                 
        unsigned :2;
    };
} BD_STAT;                              

typedef union _BDT
{
    struct
    {
        BD_STAT Stat;
        byte Cnt;
        byte ADRL;                      
        byte ADRH;                      
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        byte* ADR;                      
    };
} BDT;                                  

 
extern byte usb_device_state;
extern USB_DEVICE_STATUS usb_stat;
extern byte usb_active_cfg;
extern byte usb_alt_intf[1 ];

extern volatile far BDT ep0Bo;          
extern volatile far BDT ep0Bi;          
extern volatile far BDT ep1Bo;          
extern volatile far BDT ep1Bi;          
extern volatile far BDT ep2Bo;          
extern volatile far BDT ep2Bi;          
extern volatile far BDT ep3Bo;          
extern volatile far BDT ep3Bi;          
extern volatile far BDT ep4Bo;          
extern volatile far BDT ep4Bi;          
extern volatile far BDT ep5Bo;          
extern volatile far BDT ep5Bi;          
extern volatile far BDT ep6Bo;          
extern volatile far BDT ep6Bi;          
extern volatile far BDT ep7Bo;          
extern volatile far BDT ep7Bi;          
extern volatile far BDT ep8Bo;          
extern volatile far BDT ep8Bi;          
extern volatile far BDT ep9Bo;          
extern volatile far BDT ep9Bi;          
extern volatile far BDT ep10Bo;         
extern volatile far BDT ep10Bi;         
extern volatile far BDT ep11Bo;         
extern volatile far BDT ep11Bi;         
extern volatile far BDT ep12Bo;         
extern volatile far BDT ep12Bi;         
extern volatile far BDT ep13Bo;         
extern volatile far BDT ep13Bi;         
extern volatile far BDT ep14Bo;         
extern volatile far BDT ep14Bi;         
extern volatile far BDT ep15Bo;         
extern volatile far BDT ep15Bi;         

extern volatile far CTRL_TRF_SETUP SetupPkt;
extern volatile far CTRL_TRF_DATA CtrlTrfData;

#line 170 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
extern volatile far unsigned char hid_report_out[64 ];
extern volatile far unsigned char hid_report_in[64 ];
#line 173 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"

#line 175 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 179 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"



#line 183 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbmmap.h"
#line 52 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"


#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"

#line 36 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
 


#line 40 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"

 
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
 

#line 156 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 42 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
 

#line 45 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 59 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 61 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 63 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 65 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 67 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 69 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 72 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 43 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"


 

 
#line 49 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 50 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 51 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 52 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 53 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 54 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 55 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 56 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 57 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 58 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"

 
#line 61 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 62 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 63 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 64 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 65 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
                                    


#line 75 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
 
#line 77 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 78 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"

#line 80 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 81 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"

#line 83 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 84 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 85 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 86 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 87 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 88 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 89 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 90 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 91 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 92 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 93 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 94 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 95 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 96 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 97 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 98 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 99 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 100 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 101 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 102 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 103 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 104 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 105 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 106 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 107 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 108 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 109 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 110 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 111 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 112 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 113 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 114 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"


#line 135 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
 

#line 140 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 141 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"


#line 158 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
 
#line 160 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"

#line 164 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"

#line 165 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
 


#line 189 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
 

#line 195 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 196 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"

 

 

 
void USBCheckBusStatus(void);
void USBDriverService(void);
void USBRemoteWakeup(void);
void USBSoftDetach(void); 

void ClearArray(byte* startAdr,byte count);
#line 209 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbdrv\usbdrv.h"
#line 54 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
 

#line 38 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"

 
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
 

#line 156 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 40 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"


 

 
#line 46 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 47 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 48 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"

 
#line 51 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 52 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 53 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"

 
#line 56 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 57 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"

#line 59 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 60 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 61 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"

#line 63 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 64 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 65 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 66 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"

 
extern byte ctrl_trf_session_owner;

extern POINTER pSrc;
extern POINTER pDst;
extern WORD wCount;

 
void USBCtrlEPService(void);
void USBCtrlTrfTxService(void);
void USBCtrlTrfRxService(void);
void USBCtrlEPServiceComplete(void);
void USBPrepareForNextSetupTrf(void);


#line 83 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usbctrltrf\usbctrltrf.h"
#line 55 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
 

#line 38 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"

 
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
 

#line 156 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\typedefs.h"
#line 40 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"


 


#line 47 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
 
#line 49 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 50 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 51 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 52 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 53 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 54 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 55 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 56 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 57 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 58 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 59 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"

 
#line 62 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 63 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"


#line 79 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
 

#line 87 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 88 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"

 

 
void USBCheckStdRequest(void);

#line 95 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb9\usb9.h"
#line 56 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"


#line 59 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 1 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

#line 35 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
 

#line 89 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

#line 108 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

#line 127 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"

#line 144 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 157 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\class\hid\hid.h"
#line 59 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 61 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"

#line 63 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 65 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
          
#line 67 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 69 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"


#line 72 "C:/develop/svn_feanor/flex/flex2pc_usb/pic18/system\usb\usb.h"
#line 45 "usbdsc.c"


 
#pragma romdata

 
rom USB_DEV_DSC device_dsc=
{    
    sizeof(USB_DEV_DSC),    
    0x01 ,                
    0x0200,                 
    0x00,                   
    0x00,                   
    0x00,                   
    8 ,          
    0x04D8,                 
    0x0033,                 
    0x0001,                 
    0x01,                   
    0x02,                   
    0x03,                   
    0x02                    
};

 
rom struct{ USB_CFG_DSC cd01; USB_INTF_DSC i00a00; USB_HID_DSC hid_i00a00; USB_EP_DSC ep01i_i00a00; USB_EP_DSC ep01o_i00a00;} cfg01 ={
     
    sizeof(USB_CFG_DSC),    
    0x02 ,                
    sizeof(cfg01),          
    1,                      
    1,                      
    2,                      
    0x01<<7 ,               
    50,                     

     
    sizeof(USB_INTF_DSC),   
    0x04 ,               
    0,                      
    0,                      
    2,                      
    0x03 ,               
    0,                      
    0,                      
    0,                      

     
    sizeof(USB_HID_DSC),    
    0x21 ,                
    0x0001,                 
    0x00,                   
    1,                      
    0x22 ,                
    sizeof(hid_rpt01),      

     
    sizeof(USB_EP_DSC),
    0x05 ,
    0x81 ,
    0x02 ,
    64 ,
    0x01,

     
    sizeof(USB_EP_DSC),
    0x05 ,
    0x01 ,
    0x02 ,
    64 ,
    0x01
};

 
rom struct{ USB_CFG_DSC cd01; USB_INTF_DSC i00a00; USB_EP_DSC ep01i_i00a00; USB_EP_DSC ep01o_i00a00;} cfg02 ={
     
    sizeof(USB_CFG_DSC),    
    0x02 ,                
    sizeof(cfg02),          
    1,                      
    2,                      
    4,                      
    0x01<<7 ,               
    50,                     

     
    sizeof(USB_INTF_DSC),   
    0x04 ,               
    0,                      
    0,                      
    2,                      
    0xFF,                   
    0,                      
    0,                      
    0,                      

     
    sizeof(USB_EP_DSC),
    0x05 ,
    0x81 ,
    0x02 ,
    64 ,
    0x01,

     
    sizeof(USB_EP_DSC),
    0x05 ,
    0x01 ,
    0x02 ,
    64 ,
    0x01
};

rom struct{byte bLength;byte bDscType;word string[1];}sd000={
sizeof(sd000),0x03 ,0x0409};

rom struct{byte bLength;byte bDscType;word string[25];}sd001={
sizeof(sd001),0x03 ,
'M','i','c','r','o','c','h','i','p',' ','T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'};

rom struct{byte bLength;byte bDscType;word string[35];}sd002={
sizeof(sd002),0x03 ,
'P','I','C','k','i','t',' ','2',' ','M','i','c','r','o','c','o','n','t','r','o','l','l','e','r',' ','P','r','o','g','r','a','m','m','e','r'};

rom struct{byte bLength;byte bDscType;word string[10];}sd003={
sizeof(sd003),0x03 ,
'P','I','C','1','8','F','2','5','5','0'};

rom struct{byte bLength;byte bDscType;word string[24];}sd004={
sizeof(sd004),0x03 ,
'P','I','C','k','i','t',' ','2',' ','C','o','n','f','i','g','u','r','a','t','i','o','n',' ','2'};

rom struct{byte report[29];}hid_rpt01={
    0x06, 0x00, 0xFF,       
    0x09, 0x01,             
    0xA1, 0x01,             
    0x19, 0x01,             

    0x29, 0x40,             
    0x15, 0x00,             
    0x26, 0xFF, 0x00,       
    0x75, 0x08,             

    0x95, 0x40,             
    0x81, 0x02,             
    0x19, 0x01,             

    0x29, 0x40,             
    0x91, 0x02,             
    0xC0};                  

rom const unsigned char *rom USB_CD_Ptr[]={&cfg01,&cfg02};
rom const unsigned char *rom USB_SD_Ptr[]={&sd000,&sd001,&sd002,&sd003,&sd004};

rom pFunc ClassReqHandler[1]=
{
    &USBCheckHIDRequest
};

#pragma code

 

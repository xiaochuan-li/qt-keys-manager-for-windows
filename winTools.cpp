#include "winTools.h"

void copyText(void){
    keybd_event(VK_CONTROL,0,0,0);
    keybd_event(86,0,0,0);
    keybd_event(86,0,KEYEVENTF_KEYUP,0);
    keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
}
void clickAt(void){
    keybd_event(VK_LBUTTON,0,0,0);
    keybd_event(VK_LBUTTON,0,KEYEVENTF_KEYUP,0);
}

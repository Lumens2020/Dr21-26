#include "adc.h"
#include "keyad.h"
#include "key.h"
#include "improve.h"
extern ADCPara        ADCVar; 

#define EVENT_ADCB(ev)   if(handle->cb[ev])handle->cb[ev]((ADButton*)handle)

static struct ADButton* head_handle1=NULL;

//����״̬��ȡ�ӿ�
uint16_t read_button1_AD(void) 
{
//	return  ADCVar.uiKeyAdcValue;
	return  ADCVar.uiADC_ResultBuff[Key_CHN];
}

/*****************************************************************************
�������� : ADbutton_attach
�������� : �����¼�ע��
������� : handle:Ŀ���ַ
           event:�¼����
           cb:�¼������ص�
���ز��� : ��
*****************************************************************************/
void ADbutton_attach(struct ADButton* handle, PressEvent event, BtnCallback cb)
{
  handle->cb[event] = cb;
}

/*****************************************************************************
�������� : ADbutton_handler
�������� : AD��������״̬�ж�
������� : handle:Ŀ���ַ
���ز��� : ��
*****************************************************************************/
void ADbutton_handler(struct ADButton* handle)
{    
  uint16_t read_gpio_level = handle->hal_button_Level();   
  
  //ticks counter working..    
  if(((handle->state)>0)&&(handle->ticks<65530)) handle->ticks++;   
  /*------------button debounce handle---------------*/   

  if(read_gpio_level!=handle->button_level)   //not equal to prev one    
  {        
  //continue read 3 times same new level change      
  if(++(handle->debounce_cnt)>=AD_DEBOUNCE_TICKS)        
  	 {            
  	   handle->button_level = read_gpio_level;            
       handle->debounce_cnt = 0;        
	 }   
  }    
  else     //leved not change ,counter reset.   
  {        
   handle->debounce_cnt=0;   
  }   
  /*-----------------State machine-------------------*/    
 switch (handle->state)   
 	{       
 	 case 0:           
	 	if((handle->button_level>=handle->active_level_lower)&&(handle->button_level<=handle->active_level_upper))  	//start press down           
	 	    {             
	 	    handle->event=(u8)PRESS_DOWN;             
			EVENT_ADCB(PRESS_DOWN);              
			handle->ticks = 0;               
			handle->repeat = 1;              
			handle->state = 1;            
			}            
		else      
			{       
			handle->event=(u8)NONE_PRESS;    
			}           
		break;       
	 case 1:           
		if((handle->button_level<handle->active_level_lower)||(handle->button_level>handle->active_level_upper))	//released press up 	 
	 	    {               
	 	    handle->event=(u8)PRESS_UP;               
			EVENT_ADCB(PRESS_UP);               
			handle->ticks = 0;               
			handle->state = 2;            
			} 
       else if(handle->ticks>AD_LONG1_TICKS)      
			{              
			handle->event = (u8)LONG_PRESS1_START;         
			EVENT_ADCB(LONG_PRESS1_START);          
			if(handle->cb[LONG_PRESS1_START])handle->state = 5;          
			}  
	   else if(handle->ticks>AD_HOLD_TICKS)      
			{ 
			handle->event = (u8)LONG_PRESS_HOLD;         
			if(handle->cb[LONG_PRESS_HOLD])handle->state = 5;          
			}  
		 break;       
	 case 2:           
	 	if((handle->button_level>=handle->active_level_lower)&&(handle->button_level<=handle->active_level_upper))   //press down again       
	 	   {            
//	 	    handle->event=(uint8_t)PRESS_DOWN;           
//			EVENT_CB(PRESS_DOWN);              
			if(handle->repeat<15)handle->repeat++;        
	 	    handle->event=(u8)PRESS_REPEAT;           
			EVENT_ADCB(PRESS_REPEAT); // repeat hit                
			handle->ticks = 0;              
			handle->state = 3;           
		   }          
		else if(handle->ticks>AD_SHORT_TICKS)     //released timeout           
		   {                
		    if(handle->repeat == 1)             
				{                 
				handle->event = (u8)SINGLE_CLICK;              
				EVENT_ADCB(SINGLE_CLICK);             
				}               
			else if(handle->repeat == 2)           
				{                    
				handle->event = (u8)DOUBLE_CLICK;             
				EVENT_ADCB(DOUBLE_CLICK); // repeat hit               
				}                
			handle->state = 0;         
			}           
		break;      
	case 3:         
		if((handle->button_level<handle->active_level_lower)||(handle->button_level>handle->active_level_upper))	//released press up 	 
		   {              
		    handle->event = (u8)PRESS_UP;            
			EVENT_ADCB(PRESS_UP);               
			if(handle->ticks < AD_SHORT_TICKS)         
				{                  
				handle->ticks = 0;                   
				handle->state = 2; //repeat press             
				}             
			else              
				{                    
				handle->state = 0;        
				}         
			}            
		break;        
	case 5:            
		if((handle->button_level>=handle->active_level_lower)&&(handle->button_level<=handle->active_level_upper))   
			{                
			//continue hold trigger          
			handle->event=(u8)LONG_PRESS_HOLD;     
			EVENT_ADCB(LONG_PRESS_HOLD);        
			}        
		else//releasd         
	    	{                
		   // handle->event = (u8)PRESS_UP;      
		    //EVENT_CB(PRESS_UP);                
			handle->state = 0; //reset            
			}           
		break;    
		}
}

/*****************************************************************************
�������� : ADbutton_init
�������� : ��������ʼ��
������� : handle:Ŀ���ַ
           *pin_level:����״̬����ָ��
           active_level_lower:AD������Ч����ֵ״̬
           active_level_upper:AD������Ч����ֵ״̬
���ز��� : ��
*****************************************************************************/
void ADbutton_init(struct ADButton* handle,uint16_t(*pin_level)(),uint16_t active_level_lower,uint16_t active_level_upper)
{
 memset(handle,0,sizeof(struct ADButton));
 handle->event=(u8)NONE_PRESS;
 handle->hal_button_Level=pin_level;
 handle->button_level=handle->hal_button_Level();
 handle->active_level_lower=active_level_lower;
 handle->active_level_upper=active_level_upper;
}
/*****************************************************************************
�������� : get_button_event
�������� : ��ע�ᰴ���¼���Ż�ȡ
������� : handle:Ŀ���ַ
���ز��� : ��ע�ᰴ���¼����
*****************************************************************************/
PressEvent get_adbutton_event(struct ADButton* handle)
{   
  return (PressEvent)(handle->event);
}
/*****************************************************************************
�������� : button_start
�������� : ��������
������� : handle:Ŀ���ַ
���ز��� : 0:�����������ַOK  -1:�����������ַ�غ�
*****************************************************************************/
int ADbutton_start(struct ADButton* handle)
{
 struct ADButton* ADtarget=head_handle1;
 while(ADtarget)
 	{
 	if(ADtarget==handle)return -1;
	ADtarget=ADtarget->next;
 	}
 handle->next=head_handle1;
 head_handle1=handle;
 return 0;
}

/*****************************************************************************
�������� : button_stop
�������� : ֹͣ����
������� : handle:Ŀ���ַ
���ز��� : ��
*****************************************************************************/
void ADbutton_stop(struct ADButton* handle)
{   
  struct ADButton** curr;    
  for(curr = &head_handle1; *curr; )  
  	{        
  	struct ADButton* entry = *curr;      
	if (entry == handle)       
		{            
		 *curr = entry->next;    
		}       
	else       
		curr = &entry->next;   
	}
}

void ADbutton1_callback(void* adbutton)
{// ����ఴ��
 uint32_t btn_event_val;
 btn_event_val=get_adbutton_event((struct ADButton*)adbutton);
 switch(btn_event_val)
 	{
	 case PRESS_UP:
	 	//�̰������
	 	 error_clear();
		ADKEY_CLEAN_PRESS_UP();
	    break;
	 case LONG_PRESS1_START:
		 //������������
	 	 error_clear();
		 ADKEY_CLEAN_LONG_PRESS();
	    break;
     default:
	    break;
 	}
}
void ADbutton2_callback(void* adbutton)
{// ��������
 uint32_t btn_event_val;
 btn_event_val=get_adbutton_event((struct ADButton*)adbutton);
 switch(btn_event_val)
 	{
	 case PRESS_UP:
	 error_clear();
		ADKEY_MODE_PRESS_UP();
	    break;
     default:
	    break;
 	}
}

void ADbutton3_callback(void* adbutton)
{// AD�����ɿ����� �����������
 uint32_t btn_event_val;
 btn_event_val=get_adbutton_event((struct ADButton*)adbutton);
 switch(btn_event_val)
 	{
	 case LONG_PRESS_HOLD:
		ADKEY_RELESE();
		break;
     default:
	    break;
 	}
}

/*****************************************************************************
�������� : button_ticks
�������� : ����״̬������
������� : ��
���ز��� : ��
*****************************************************************************/
void ADbutton_ticks(void)
{
	struct ADButton* target1;
    for(target1=head_handle1;target1;target1=target1->next)
 	{
 	 ADbutton_handler(target1);
 	}
}


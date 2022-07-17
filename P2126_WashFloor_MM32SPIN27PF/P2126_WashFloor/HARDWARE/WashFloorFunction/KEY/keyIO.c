#include "keyio.h"
#include "key.h"
#include "gpio.h"
#include "improve.h"

#define EVENT_CB(ev)     if(handle->cb[ev])handle->cb[ev]((Button*)handle)

static struct Button* head_handle=NULL;

//����״̬��ȡ�ӿ�
uint8_t read_button1_GPIO(void) 
{
	return pbKey_Spk;
}


/*****************************************************************************
�������� : button_attach
�������� : �����¼�ע��
������� : handle:Ŀ���ַ
           event:�¼����
           cb:�¼������ص�
���ز��� : ��
*****************************************************************************/
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb)
{
  handle->cb[event] = cb;
}


/*****************************************************************************
�������� : button_handler
�������� : ��������״̬�ж�
������� : handle:Ŀ���ַ
���ز��� : ��
*****************************************************************************/
void button_handler(struct Button* handle)
{    
  uint8_t read_gpio_level = handle->hal_button_Level();   
  
  //ticks counter working..    
  if((handle->state)>0)
  	{
  	  if(handle->ticks<65530)handle->ticks++;  
	    handle->relese_ticks = 0;
  	}
  else
  	{
			if(++handle->relese_ticks>HOLD_RELESE_TICKS)		//10s,�ɿ�10S
			{
				handle->event=(u8)RELESE_PRESS_10S; 		      //�ͷŰ�����ʵ	
				EVENT_CB(RELESE_PRESS_10S); 	
				handle->relese_ticks = 0;
			}   
  	}
  /*------------button debounce handle---------------*/   
  if(read_gpio_level!=handle->button_level)   //not equal to prev one    
  {        
  //continue read 3 times same new level change      
  if(++(handle->debounce_cnt)>=DEBOUNCE_TICKS)        
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
	 	if(handle->button_level==handle->active_level)  	//start press down           
	 	    {             
	 	    handle->event=(u8)PRESS_DOWN;             
			EVENT_CB(PRESS_DOWN);              
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
	 	if(handle->button_level!=handle->active_level)   //released press up      
	 	    {               
	 	    handle->event=(u8)PRESS_UP;               
			EVENT_CB(PRESS_UP);               
			handle->ticks = 0;               
			handle->state = 2;            
			} 
		else if(handle->ticks>LONG2_TICKS)      
			{              
			handle->event = (u8)LONG_PRESS2_START;         
			EVENT_CB(LONG_PRESS2_START);          
			handle->state = 5;          
			}   
		 break;       
	 case 2:           
	 	if(handle->button_level==handle->active_level)   //press down again       
	 	   {            
			if(handle->repeat<15)handle->repeat++;        
	 	    handle->event=(u8)PRESS_REPEAT;           
			EVENT_CB(PRESS_REPEAT); // repeat hit                
			handle->ticks = 0;              
			handle->state = 3;           
		   }          
		else if(handle->ticks>SHORT_TICKS)     //released timeout           
		   {                
		    if(handle->repeat == 1)             
				{                 
				handle->event = (u8)SINGLE_CLICK;              
				EVENT_CB(SINGLE_CLICK);             
				}               
			else if(handle->repeat == 2)           
				{                    
				handle->event = (u8)DOUBLE_CLICK;             
				EVENT_CB(DOUBLE_CLICK); // repeat hit               
				}                
			handle->state = 0;         
			}           
		break;      
	case 3:         
		if(handle->button_level!= handle->active_level)   //released press up       
		   {              
		    handle->event = (u8)PRESS_UP;            
			EVENT_CB(PRESS_UP);               
			if(handle->ticks < SHORT_TICKS)         
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
		if(handle->button_level==handle->active_level)   
			{                
			//continue hold trigger          
			handle->event=(u8)LONG_PRESS_HOLD;     
			EVENT_CB(LONG_PRESS_HOLD);        
			}        
		else//releasd         
	    	{                
			handle->state = 0; //reset            
			}           
		break;    
		}
}
/*****************************************************************************
�������� : button_init
�������� : ��������ʼ��
������� : handle:Ŀ���ַ
           *pin_level:����״̬����ָ��
           active_level:������Ч״̬
���ز��� : ��
*****************************************************************************/
void button_init(struct Button* handle,uint8_t(*pin_level)(),uint8_t active_level)
{
 memset(handle,0,sizeof(struct Button));           //�ṹ���б�����ʼ��
 handle->event=(u8)NONE_PRESS;
 handle->hal_button_Level=pin_level;               //�ص�read_button1_GPIO
 handle->button_level=handle->hal_button_Level();
 handle->active_level=active_level;
}


/*****************************************************************************
�������� : get_button_event
�������� : ��ע�ᰴ���¼���Ż�ȡ
������� : handle:Ŀ���ַ
���ز��� : ��ע�ᰴ���¼����
*****************************************************************************/
PressEvent get_button_event(struct Button* handle)
{   
  return (PressEvent)(handle->event);
}

/*****************************************************************************
�������� : button_start
�������� : ��������
������� : handle:Ŀ���ַ
���ز��� : 0:�����������ַOK  -1:�����������ַ�غ�
*****************************************************************************/
int button_start(struct Button* handle)
{
 struct Button* target=head_handle;  //head_handle��ʼ��ΪNULL
 while(target)
 	{
 	if(target==handle)return -1;
	target=target->next;
 	}
 handle->next=head_handle;
 head_handle=handle;                //ͷָ�벻Ϊ��
 return 0;
}

/*****************************************************************************
�������� : button_stop
�������� : ֹͣ����
������� : handle:Ŀ���ַ
���ز��� : ��
*****************************************************************************/
void button_stop(struct Button* handle)
{   
  struct Button** curr;    
  for(curr = &head_handle; *curr; )  
  	{        
  	struct Button* entry = *curr;      
	if (entry == handle)       
		{            
		 *curr = entry->next;    
		}       
	else       
		curr = &entry->next;   
	}
}

void button1_callback(void* button)
{//��������
	uint32_t btn_event_val;
	btn_event_val=get_button_event((struct Button*)button);
	switch(btn_event_val)
	   {
		case PRESS_UP:// �̰�����
			error_clear();
			KEY_VOICE_PRESS_UP();
		   break;
		case LONG_PRESS2_START:// ��������
			error_clear();
			KEY_VOICE_LONG_PRESS();
		   break;
		case RELESE_PRESS_10S:// �ɿ�����10S����
			KEY_VOICE_RELESE_10S();
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
void button_ticks(void)
{
	struct Button* target;
    for(target=head_handle;target;target=target->next)
 	{
 	 button_handler(target);
 	}
	
}


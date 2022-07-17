#include "keyio.h"
#include "key.h"
#include "gpio.h"
#include "improve.h"

#define EVENT_CB(ev)     if(handle->cb[ev])handle->cb[ev]((Button*)handle)

static struct Button* head_handle=NULL;

//按键状态读取接口
uint8_t read_button1_GPIO(void) 
{
	return pbKey_Spk;
}


/*****************************************************************************
函数名称 : button_attach
功能描述 : 按键事件注册
输入参数 : handle:目标地址
           event:事件编号
           cb:事件函数回调
返回参数 : 无
*****************************************************************************/
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb)
{
  handle->cb[event] = cb;
}


/*****************************************************************************
函数名称 : button_handler
功能描述 : 按键消抖状态判断
输入参数 : handle:目标地址
返回参数 : 无
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
			if(++handle->relese_ticks>HOLD_RELESE_TICKS)		//10s,松开10S
			{
				handle->event=(u8)RELESE_PRESS_10S; 		      //释放按键事实	
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
函数名称 : button_init
功能描述 : 单按键初始化
输入参数 : handle:目标地址
           *pin_level:按键状态函数指针
           active_level:按键有效状态
返回参数 : 无
*****************************************************************************/
void button_init(struct Button* handle,uint8_t(*pin_level)(),uint8_t active_level)
{
 memset(handle,0,sizeof(struct Button));           //结构体中变量初始化
 handle->event=(u8)NONE_PRESS;
 handle->hal_button_Level=pin_level;               //回调read_button1_GPIO
 handle->button_level=handle->hal_button_Level();
 handle->active_level=active_level;
}


/*****************************************************************************
函数名称 : get_button_event
功能描述 : 已注册按键事件编号获取
输入参数 : handle:目标地址
返回参数 : 已注册按键事件编号
*****************************************************************************/
PressEvent get_button_event(struct Button* handle)
{   
  return (PressEvent)(handle->event);
}

/*****************************************************************************
函数名称 : button_start
功能描述 : 启动按键
输入参数 : handle:目标地址
返回参数 : 0:按键链表结点地址OK  -1:按键链表结点地址重合
*****************************************************************************/
int button_start(struct Button* handle)
{
 struct Button* target=head_handle;  //head_handle初始化为NULL
 while(target)
 	{
 	if(target==handle)return -1;
	target=target->next;
 	}
 handle->next=head_handle;
 head_handle=handle;                //头指针不为空
 return 0;
}

/*****************************************************************************
函数名称 : button_stop
功能描述 : 停止按键
输入参数 : handle:目标地址
返回参数 : 无
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
{//语音按键
	uint32_t btn_event_val;
	btn_event_val=get_button_event((struct Button*)button);
	switch(btn_event_val)
	   {
		case PRESS_UP:// 短按处理
			error_clear();
			KEY_VOICE_PRESS_UP();
		   break;
		case LONG_PRESS2_START:// 长按处理
			error_clear();
			KEY_VOICE_LONG_PRESS();
		   break;
		case RELESE_PRESS_10S:// 松开按键10S处理
			KEY_VOICE_RELESE_10S();
			break;
		default:
		   break;
	   }
}


/*****************************************************************************
函数名称 : button_ticks
功能描述 : 按键状态机处理
输入参数 : 无
返回参数 : 无
*****************************************************************************/
void button_ticks(void)
{
	struct Button* target;
    for(target=head_handle;target;target=target->next)
 	{
 	 button_handler(target);
 	}
	
}


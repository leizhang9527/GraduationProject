#include "key.h"
#include "delay.h"

/*************************
* ���������˿ڳ�ʼ������
*************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 			//����key1�˿�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;		//�ڲ�����
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			//����key2�˿�����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*************************
* ��������ɨ�躯��
* modeѡ���Ƿ�֧������
* ���ض�Ӧ�����������
*************************/
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�ɿ���־
	if(mode)
	{
		delay_ms(100);
		key_up=1;  //֧������
	}		
	if(key_up&&(KEY1==0||KEY2==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
	}
	else if(KEY1==1&&KEY2==1)
		key_up=1; 	    
 	return 0;// �ް�������
}

/*************************
* ���������ж�ģʽ��ʼ������
*************************/
void KEY_INT_Init(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;					//����һ��EXTI�ṹ�����
	NVIC_InitTypeDef  NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//ʹ��IO���ù��ܣ�ʹ���жϹ�����Ҫ������

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);//���ö˿�C��13����Ϊ�ж�Դ	  ��Ҫ���� ���ϱ��INT2
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0); //���ö˿�E��0����Ϊ�ж�Դ	  ��Ҫ���� ���ϱ��INT1
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line13;	//
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽΪ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�½��س���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//ʹ���ж���
	EXTI_Init(&EXTI_InitStructure);								//���ݲ�����ʼ���жϼĴ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//�趨�ж�ԴΪPC13
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�ж�ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							   	//���ݲ�����ʼ���жϼĴ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//�趨�ж�ԴΪPE0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�ж�ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							   	//���ݲ�����ʼ���жϼĴ���
}

/*************************
* ���󰴼�IO��ʼ������
*************************/
void KeyPlus_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	//��������Ϊ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	//��������Ϊ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;								//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure); 	
}

/*************************
* ���󰴼�ɨ�躯��
*************************/
u8 KeyPlus_Scan(void)
{	
	PFout(1)=0;PFout(2)=1;PFout(3)=1;PFout(4)=1;//����һ��
	if(!PFin(5)||!PFin(6)||!PFin(7)||!PFin(8))
	{
		delay_ms(10);
		if(!PFin(5))	return 16;
		if(!PFin(6))	return 15;
		if(!PFin(7))	return 14;
		if(!PFin(8))	return 13;
	}
	PFout(1)=1;PFout(2)=0;PFout(3)=1;PFout(4)=1;//���ڶ���
	if(!PFin(5)||!PFin(6)||!PFin(7)||!PFin(8))
	{
		delay_ms(10);
		if(!PFin(5))	return 12;
		if(!PFin(6))	return 11;
		if(!PFin(7))	return 10;
		if(!PFin(8))	return 9;
	}
	PFout(1)=1;PFout(2)=1;PFout(3)=0;PFout(4)=1;//��������
	if(!PFin(5)||!PFin(6)||!PFin(7)||!PFin(8))
	{
		delay_ms(10);
		if(!PFin(5))	return 8;
		if(!PFin(6))	return 7;
		if(!PFin(7))	return 6;
		if(!PFin(8))	return 5;
	}
	PFout(1)=1;PFout(2)=1;PFout(3)=1;PFout(4)=0;//��������
	if(!PFin(5)||!PFin(6)||!PFin(7)||!PFin(8))
	{
		delay_ms(10);
		if(!PFin(5))	return 4;
		if(!PFin(6))	return 3;
		if(!PFin(7))	return 2;
		if(!PFin(8))	return 1;
	}
	return 0;
}

/*
Key2  �ж�15_10��ں���			
*/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13)!= RESET)  	//�ж��Ƿ����жϣ��������жϱ�־��1
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line13);		//����жϹ���λ����Ҫ����			
}
/*
Key1 �ж�0��ں���			
*/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)	 //�ж��Ƿ����жϣ��������жϱ�־��1
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line0);		     //����жϹ���λ����Ҫ����			
}
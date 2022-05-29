/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"


/*
 *�����ں�Ƶ�ʵ��趨�����Բ鿴board.h�ļ�
 *��board_init��,�Ѿ���P54��������Ϊ��λ
 *�����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
 */
 
//�����̽�֧�ִ�����ı���������

//������������
#define SPEEDL_PLUSE   CTIM0_P34
#define SPEEDR_PLUSE   CTIM3_P04
//���巽������
#define SPEEDL_DIR     P35
#define SPEEDR_DIR     P53

#define DIR_1 P63
#define DIR_2 P61
#define PWM_1 PWMA_CH2P_P62
#define PWM_2 PWMA_CH1P_P60

int16 templ_pluse = 0;
int16 tempr_pluse = 0;
uint8  dir = 0;
int16 dutyl = 2000;
int16 dutyr = 2000;
int16 spd_set = 1700;
int16 errorr = 0;
int16 errorl =  0;
int16 errorr_last = 0;
int16 errorl_last =  0;
int16 errorr_i = 0;
int16 errorl_i =  0;
int16 errorr_d = 0;
int16 errorl_d =  0;
uint8 data_to_send[20];
uint16 adc_data[12];

void main()
{
    WTST = 0;               //���ó������ȴ���������ֵΪ0�ɽ�CPUִ�г�����ٶ�����Ϊ���
    
	DisableGlobalIRQ();		//�ر����ж�
	
    //sys_clk��ѡֵ:35000000��30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600��
    //����ϵͳƵ�ʣ���Ƶ����Ҫ��STC-ISP����е� <�����û��������е�IRCƵ��>ѡ���Ƶ��һ�¡�
    //���Ƶ�����ò��ԣ����ᵼ�´��ڵ����ݲ�����,PWM�Ĺ����������ȵȡ�
    sys_clk = 35000000;     //����ϵͳƵ��Ϊ35MHz
    
	board_init();		//��ʼ��
	
	pwm_init(PWM_1, 17000, 0); //��ʼ��PWM1  ʹ��P60����  ��ʼ��Ƶ��Ϊ17Khz
	pwm_init(PWM_2, 17000, 0); //��ʼ��PWM2  ʹ��P62����  ��ʼ��Ƶ��Ϊ17Khz
    
	ctimer_count_init(SPEEDL_PLUSE);	//��ʼ����ʱ��0��Ϊ�ⲿ����
	ctimer_count_init(SPEEDR_PLUSE);	//��ʼ����ʱ��3��Ϊ�ⲿ����
//	lcd_init();							//1.8��TFT��ʼ��
	ips114_init();						//��ʼ��1.14��IPS��Ļ
//	oled_init_spi();					//OLED��ʼ��
	adc_init(ADC_P01, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P02, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P03, ADC_SYSclk_DIV_2);
	adc_init(ADC_P04, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P05, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P06, ADC_SYSclk_DIV_2);		
	
	EnableGlobalIRQ();					//�������ж�
    while(1)
	{
        //��ȡ�ɼ����ı�����������
        templ_pluse = ctimer_count_read(SPEEDL_PLUSE);
		tempr_pluse = ctimer_count_read(SPEEDR_PLUSE);

        //����������
        ctimer_count_clean(SPEEDL_PLUSE);
		ctimer_count_clean(SPEEDR_PLUSE);

		
		//��TFT����ʾ����Ҫ��ʼ��1.8��TFT��Ļ������ʹ�á�
//		lcd_showstr(0, 0, "pluse_l =");			lcd_showint16(8*10, 0, templ_pluse);
//		lcd_showstr(0, 1, "pluse_r =");			lcd_showint16(8*10, 1, tempr_pluse);

		//��1.14IPS��Ļ����ʾ����Ҫ��ʼ��1.14��ips��Ļ������ʹ�á�
		// ips114_showstr(0, 0, "pluse_l =");		ips114_showint16(8*10, 0, templ_pluse);
		// ips114_showstr(0, 1, "pluse_r =");		ips114_showint16(8*10, 1, tempr_pluse);
		ips114_showint16(0,1, adc_data[0]);
		ips114_showint16(8*10,1, adc_data[1]);
		ips114_showint16(0,2, adc_data[2]);
		ips114_showint16(8*10,2, adc_data[3]);
		ips114_showint16(0,3, adc_data[4]);
		ips114_showint16(8*10,3, adc_data[5]);
		
		//��OLED��Ļ����ʾ����Ҫ��ʼ��OLED��Ļ������ʹ�á�
//		oled_p6x8str_spi(0, 0, "pluse_l =");	oled_int16_spi(8*10, 0, templ_pluse);
//		oled_p6x8str_spi(0, 1, "pluse_r =");	oled_int16_spi(8*10, 1, tempr_pluse);��
		//��ʱ100ms
		adc_data[0] = adc_once(ADC_P10, ADC_10BIT)/82;	//�ɼ�һ��ADC������10λ
		adc_data[1] = adc_once(ADC_P11, ADC_10BIT)/82;	//�ɼ�һ��ADC������10λ
		adc_data[2] = adc_once(ADC_P54, ADC_10BIT)/82;	//�ɼ�һ��ADC������10λ
		adc_data[3] = adc_once(ADC_P13, ADC_10BIT)/82;	//�ɼ�һ��ADC������10λ
		adc_data[4] = adc_once(ADC_P14, ADC_10BIT)/82;	//�ɼ�һ��ADC������10λ
		adc_data[5] = adc_once(ADC_P15, ADC_10BIT)/82;	//�ɼ�һ��ADC������10λ
		delay_ms(100);
		
		// errorr_last = errorr;
        // errorl_last =  errorl;
		// errorl = spd_set - templ_pluse;
		// errorr = spd_set - tempr_pluse;
		// errorl_i = errorl_i + errorl;
		// errorr_i = errorr_i + errorr;
		// errorr_d = errorr_last - errorr;
		// errorl_d = errorl_last - errorl;
		// dutyl = dutyl + 0.02*errorl + 0.001*errorl_i + 0.01*errorl_d;
		// dutyr = dutyr + 0.02*errorr + 0.001*errorr_i + 0.01*errorr_d;
		// DIR_1 = 0;
		// pwm_duty(PWM_2, dutyl);
		// DIR_2 = 0;
		// pwm_duty(PWM_1, dutyr);
		// ips114_showint16(0, 2, dutyl);
		// ips114_showint16(0, 3, dutyr);
	    // printf("%d,%d\n",templ_pluse,tempr_pluse);
	}
}
                      

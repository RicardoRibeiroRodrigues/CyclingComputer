/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include <asf.h>
#include <string.h>
#include "ili9341.h"
#include "lvgl.h"
#include "touch/touch.h"
#include "components/header_footer.h"
#include "logo.h"
#include "roda_logo.h"
#include "quadrado.h"
#include "play_pause_button.h"
#include "stop_button.h"
#include "components/screen_1_parts.h"
#include "components/screen_2_parts.h"
#include "hardware.h"

LV_FONT_DECLARE(MontAltEL20);

typedef struct  {
	uint32_t year;
	uint32_t month;
	uint32_t day;
	uint32_t week;
	uint32_t hour;
	uint32_t minute;
	uint32_t second;
} calendar;

typedef struct{
	uint32_t hour;
	uint32_t min;
	uint32_t sec;	
} viagem_time;

enum screen {
	SCR1,
	SCR2
};

#define PI 3.14159265359

/************************************************************************/
/* Globals                                                              */
/************************************************************************/
volatile enum screen curr_screen = SCR1;


/************************************************************************/
/* Prototypes                                                           */
/************************************************************************/
void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type);


/************************************************************************/
/* LCD / LVGL                                                           */
/************************************************************************/

#define LV_HOR_RES_MAX          (240)
#define LV_VER_RES_MAX          (320)

/*A static or global variable to store the buffers*/
static lv_disp_draw_buf_t disp_buf;

/*Static or global buffer(s). The second buffer is optional*/
static lv_color_t buf_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];
static lv_disp_drv_t disp_drv;          /*A variable to hold the drivers. Must be static or global.*/
static lv_indev_drv_t indev_drv;

// declarar a tela como global e estática
static lv_obj_t * scr1;  // screen 1
static lv_obj_t * scr2;  // screen 2

/************************************************************************/
/* RTOS                                                                 */
/************************************************************************/

#define TASK_LCD_STACK_SIZE                (1024*6/sizeof(portSTACK_TYPE))
#define TASK_LCD_STACK_PRIORITY            (tskIDLE_PRIORITY)
#define TASK_RTC_STACK_SIZE                (1024*6/sizeof(portSTACK_TYPE))
#define TASK_RTC_STACK_PRIORITY            (tskIDLE_PRIORITY)
#define TASK_INFOS_STACK_SIZE                (1024*6/sizeof(portSTACK_TYPE))
#define TASK_INFOS_STACK_PRIORITY            (tskIDLE_PRIORITY)

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,  signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern void xPortSysTickHandler(void);

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName) {
	printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	for (;;) {	}
}

extern void vApplicationIdleHook(void) { }

extern void vApplicationTickHook(void) { }

extern void vApplicationMallocFailedHook(void) {
	configASSERT( ( volatile void * ) NULL );
}

// Semaforos e filas
SemaphoreHandle_t xSemaphoreHorario, xSemaphoreTempoViagem;


QueueHandle_t xPulseQueue, xQueueViagem;

// Mutex
SemaphoreHandle_t xMutex;

/************************************************************************/
/* Handlers/Callbacks                                                   */
/************************************************************************/

void RTC_Handler(void) {
	uint32_t ul_status = rtc_get_status(RTC);
	
	/* seccond tick */
	if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC) {
		// o código para irq de segundo vem aqui
		xSemaphoreGiveFromISR(xSemaphoreHorario, 0);
		xSemaphoreGiveFromISR(xSemaphoreTempoViagem, 0);
	}
	
	/* Time or date alarm */
	if ((ul_status & RTC_SR_ALARM) == RTC_SR_ALARM) {
		// o código para irq de alame vem aqui
		
	}
	
	rtc_clear_status(RTC, RTC_SCCR_SECCLR);
	rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
	rtc_clear_status(RTC, RTC_SCCR_ACKCLR);
	rtc_clear_status(RTC, RTC_SCCR_TIMCLR);
	rtc_clear_status(RTC, RTC_SCCR_CALCLR);
	rtc_clear_status(RTC, RTC_SCCR_TDERRCLR);
}

/************************************************************************/
/* lvgl                                                                 */
/************************************************************************/

static void event_handler(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);

	if(code == LV_EVENT_CLICKED) {
		LV_LOG_USER("Clicked");
		
	}
	else if(code == LV_EVENT_VALUE_CHANGED) {
		LV_LOG_USER("Toggled");
	}
}

void settings_handler(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
	
	if(code == LV_EVENT_CLICKED) {
		LV_LOG_USER("Clicked");
		if (curr_screen == SCR1) {
			lv_scr_load(scr2);
			curr_screen = SCR2;
		} else {
			lv_scr_load(scr1);
			curr_screen = SCR1;
		}
		
	}
	else if(code == LV_EVENT_VALUE_CHANGED) {
		LV_LOG_USER("Toggled");
	}
}


/************************************************************************/
/* TASKS                                                                */
/************************************************************************/

static void task_lcd(void *pvParameters) {
	int px, py;

	// Criando duas telas
	scr1 = lv_obj_create(NULL);
	scr2 = lv_obj_create(NULL);
	lv_obj_clear_flag(scr1, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_clear_flag(scr2, LV_OBJ_FLAG_SCROLLABLE);
	// Monta tela 1
	lv_obj_set_style_bg_color(scr1, lv_color_white(), LV_PART_MAIN );
	create_header(scr1, &logo, &MontAltEL20);
	create_footer(scr1);
	create_speed_section(scr1, &MontAltEL20, &quadrado);
	viagem_imgs imgs = {&roda_logo, &play_pause_button, &stop_button};
    create_viagem_section(scr1, &MontAltEL20, imgs);
	// Monta tela 2
	lv_obj_set_style_bg_color(scr2, lv_color_white(), LV_PART_MAIN );
	create_header(scr2, &logo, &MontAltEL20);
	create_footer(scr2);
	create_peso_section(scr2, &MontAltEL20, &quadrado);
	create_aro_section(scr2, &MontAltEL20, &roda_logo);
	horario_imgs img = {&roda_logo, &play_pause_button, &stop_button};
	create_horario_section(scr2, &MontAltEL20, img);
	create_footer(scr2);
	// Carrega na tela.
	lv_scr_load(scr1);

	for (;;)  {
		xSemaphoreTake( xMutex, portMAX_DELAY );
		lv_tick_inc(50);
		lv_task_handler();
		xSemaphoreGive( xMutex );	
		vTaskDelay(50);
	}
}

static void task_rtc(void *pvParameters) {
	/** Configura RTC -> Usa o RTC para atualizar o horario todo segundo.**/
	calendar rtc_initial = {2022, 11, 19, 12, 14, 01, 1};
	RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_SECEN);
	uint32_t current_hour, current_min, current_sec;
	init_pins();
	RTT_init(RTT_FREQ, 0, 0);
	
	while(1) {
		if (xSemaphoreTake(xSemaphoreHorario, 0) == pdTRUE) {
			rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
			xSemaphoreTake( xMutex, portMAX_DELAY );
			lv_label_set_text_fmt(labelClockHeader, "%02d:%02d:%02d", current_hour, current_min, current_sec);
			xSemaphoreGive( xMutex );
		}
	}
}


static void task_infos(void *pvParameters) {
	// Cria queue para receber status da viagem
	xQueueViagem = xQueueCreate(32, sizeof(char));
	if (xQueueViagem == NULL) {
		printf("Erro ao criar os semáforos\n");
	}
	char received_queue;
	char viagem_rodando = 0;
	// Tempo da viagem
	viagem_time v_time = {0, 0, 0};
	// dt em pulsos recebido pelo sensor
	uint32_t dt_pulsos;
	// Valores para imprimir na tela
	float vm_km_per_h = 0.0;
	float raio_m = 0.254;
	float t_in_hour = 0.0;
	float dist_km = 0.0;
	float last_v_km_per_hour = 0;
	float aceleration_km_per_hour_sq = 0.0;
	
	while (1) {
		if (xSemaphoreTake(xSemaphoreTempoViagem, 0) == pdTRUE) {
			if (viagem_rodando) {
				v_time.sec++;
				if (v_time.sec >= 60) {
					v_time.sec = 0;
					v_time.min++;
				}
				if (v_time.min >= 60) {
					v_time.min = 0;
					v_time.hour++;
				}
				xSemaphoreTake( xMutex, portMAX_DELAY );
				lv_label_set_text_fmt(labelViagemClock, "%02d:%02d", v_time.hour, v_time.min);
				xSemaphoreGive( xMutex );
			}
		}
		if (xQueueReceive(xPulseQueue, &dt_pulsos, 0)) {
			printf("dT: %d\n", dt_pulsos);
			
			if (viagem_rodando) {
				dist_km += ((float)(2.0 * PI * raio_m) / 1000.0);
				t_in_hour = v_time.hour + ((float)v_time.min/60.0) + ((float)v_time.sec/3600.0);
				vm_km_per_h = dist_km / (t_in_hour);
				printf("dist: %f, vm: %f, \n", dist_km, vm_km_per_h);
			}
			// Calculo da velocidade instantanea 
			float dt_secs = ((float) dt_pulsos / RTT_FREQ);
			printf("dt_secs = %f\n", dt_secs);
			float f_hz = ((float) 1.0 / dt_secs);
			float w_rads_per_sec = ((float)2 * PI * f_hz);
			float v_km_per_hour =  ((float)w_rads_per_sec * raio_m) * 3.6;
			// Calculo da aceleracao
			aceleration_km_per_hour_sq = (v_km_per_hour - last_v_km_per_hour) / (dt_secs / 3600.0);
			last_v_km_per_hour = v_km_per_hour;
			printf("V = %f\n", v_km_per_hour);
			printf("a = %f\n", aceleration_km_per_hour_sq);
			xSemaphoreTake( xMutex, portMAX_DELAY );
			lv_label_set_text_fmt(labelSpeedValue, "%.01f", v_km_per_hour);
			write_acceleration(aceleration_km_per_hour_sq);
			if (viagem_rodando) {
				lv_label_set_text_fmt(labelDistValue, "%.01f", dist_km);
				lv_label_set_text_fmt(labelVelMValue, "%.01f", vm_km_per_h);
			}
			xSemaphoreGive( xMutex );
		}
		if (xQueueReceive(xQueueViagem, &received_queue, 0)) {
			// p -> Play
			// P -> Pause
			// S -> Stop
			if (received_queue == 'p') {
				printf("Entrou no play\n");
				viagem_rodando = 1;
			} else if (received_queue == 'P') {
				printf("Entrou no pause\n");
				viagem_rodando = 0;
			} else {
				printf("Entrou no stop\n");
				xSemaphoreTake( xMutex, portMAX_DELAY );
				viagem_rodando = 0;
				v_time.hour = 0;
				v_time.min = 0;
				v_time.sec = 0;
				dist_km = 0;
				vm_km_per_h = 0;
				lv_label_set_text_fmt(labelDistValue, "%.01f", dist_km);
				lv_label_set_text_fmt(labelVelMValue, "%.01f", vm_km_per_h);
				lv_label_set_text_fmt(labelViagemClock, "%02d:%02d", v_time.hour, v_time.min);
				xSemaphoreGive( xMutex );
			}
		}
	}
}



#include "arm_math.h"

#define TASK_SIMULATOR_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_SIMULATOR_STACK_PRIORITY (tskIDLE_PRIORITY)

#define RAIO 0.508/2
#define VEL_MAX_KMH  5.0f
#define VEL_MIN_KMH  0.5f
//#define RAMP

/**
* raio 20" => 50,8 cm (diametro) => 0.508/2 = 0.254m (raio)
* w = 2 pi f (m/s)
* v [km/h] = (w*r) / 3.6 = (2 pi f r) / 3.6
* f = v / (2 pi r 3.6)
* Exemplo : 5 km / h = 1.38 m/s
*           f = 0.87Hz
*           t = 1/f => 1/0.87 = 1,149s
*/
float kmh_to_hz(float vel, float raio) {
	float f = vel / (2*PI*raio*3.6);
	return(f);
}

static void task_simulador(void *pvParameters) {

	pmc_enable_periph_clk(ID_PIOC);
	pio_set_output(PIOC, PIO_PC31, 1, 0, 0);

	float vel = VEL_MAX_KMH;
	float f;
	int ramp_up = 1;

	while(1){
		pio_clear(PIOC, PIO_PC31);
		delay_ms(1);
		pio_set(PIOC, PIO_PC31);
		#ifdef RAMP
		if (ramp_up) {
			printf("[SIMU] ACELERANDO: %d \n", (int) (10*vel));
			vel += 0.5;
			} else {
			printf("[SIMU] DESACELERANDO: %d \n",  (int) (10*vel));
			vel -= 0.5;
		}
		#endif

		if (vel >= VEL_MAX_KMH)
		ramp_up = 0;
		else if (vel <= VEL_MIN_KMH)
		ramp_up = 1;
		#ifndef RAMP
		vel = 5;
		//printf("[SIMU] CONSTANTE: %d \n", (int) (10*vel));
		#endif
		f = kmh_to_hz(vel, RAIO);
		int t = 965*(1.0/f); //UTILIZADO 965 como multiplicador ao invés de 1000
		//para compensar o atraso gerado pelo Escalonador do freeRTOS
		delay_ms(t);
	}
}

/************************************************************************/
/* configs                                                              */
/************************************************************************/

static void configure_lcd(void) {
	/**LCD pin configure on SPI*/
	pio_configure_pin(LCD_SPI_MISO_PIO, LCD_SPI_MISO_FLAGS);  //
	pio_configure_pin(LCD_SPI_MOSI_PIO, LCD_SPI_MOSI_FLAGS);
	pio_configure_pin(LCD_SPI_SPCK_PIO, LCD_SPI_SPCK_FLAGS);
	pio_configure_pin(LCD_SPI_NPCS_PIO, LCD_SPI_NPCS_FLAGS);
	pio_configure_pin(LCD_SPI_RESET_PIO, LCD_SPI_RESET_FLAGS);
	pio_configure_pin(LCD_SPI_CDS_PIO, LCD_SPI_CDS_FLAGS);
	
	ili9341_init();
	ili9341_backlight_on();
}

static void configure_console(void) {
	const usart_serial_options_t uart_serial_options = {
		.baudrate = USART_SERIAL_EXAMPLE_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT,
	};

	/* Configure console UART. */
	stdio_serial_init(CONSOLE_UART, &uart_serial_options);

	/* Specify that stdout should not be buffered. */
	setbuf(stdout, NULL);
}

/************************************************************************/
/* port lvgl                                                            */
/************************************************************************/

void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
	ili9341_set_top_left_limit(area->x1, area->y1);   ili9341_set_bottom_right_limit(area->x2, area->y2);
	ili9341_copy_pixels_to_screen(color_p,  (area->x2 + 1 - area->x1) * (area->y2 + 1 - area->y1));
	
	/* IMPORTANT!!!
	* Inform the graphics library that you are ready with the flushing*/
	lv_disp_flush_ready(disp_drv);
}

void my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data) {
	int px, py, pressed;
	
	if (readPoint(&px, &py))
		data->state = LV_INDEV_STATE_PRESSED;
	else
		data->state = LV_INDEV_STATE_RELEASED; 
	
	data->point.x = py;
	data->point.y = 320 - px;
}

void configure_lvgl(void) {
	lv_init();
	lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);
	
	lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
	disp_drv.draw_buf = &disp_buf;          /*Set an initialized buffer*/
	disp_drv.flush_cb = my_flush_cb;        /*Set a flush callback to draw to the display*/
	disp_drv.hor_res = LV_HOR_RES_MAX;      /*Set the horizontal resolution in pixels*/
	disp_drv.ver_res = LV_VER_RES_MAX;      /*Set the vertical resolution in pixels*/

	lv_disp_t * disp;
	disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
	
	/* Init input on LVGL */
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = my_input_read;
	lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);
}

void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type) {
	/* Configura o PMC */
	pmc_enable_periph_clk(ID_RTC);

	/* Default RTC configuration, 24-hour mode */
	rtc_set_hour_mode(rtc, 0);

	/* Configura data e hora manualmente */
	rtc_set_date(rtc, t.year, t.month, t.day, t.week);
	rtc_set_time(rtc, t.hour, t.minute, t.second);

	/* Configure RTC interrupts */
	NVIC_DisableIRQ(id_rtc);
	NVIC_ClearPendingIRQ(id_rtc);
	NVIC_SetPriority(id_rtc, 4);
	NVIC_EnableIRQ(id_rtc);

	/* Ativa interrupcao via alarme */
	rtc_enable_interrupt(rtc,  irq_type);
}

/************************************************************************/
/* main                                                                 */
/************************************************************************/
int main(void) {
	/* board and sys init */
	board_init();
	sysclk_init();
	configure_console();

	/* LCd, touch and lvgl init*/
	configure_lcd();
	ili9341_set_orientation(ILI9341_FLIP_Y | ILI9341_SWITCH_XY);
	configure_touch();
	configure_lvgl();
	
	xSemaphoreHorario = xSemaphoreCreateBinary();
	if (xSemaphoreHorario == NULL) {
		printf("Failed to create semaphore \n");
	}
	xSemaphoreTempoViagem = xSemaphoreCreateBinary();
	if (xSemaphoreTempoViagem == NULL) {
		printf("Failed to create semaphore \n");
	}
	
	xMutex = xSemaphoreCreateMutex();
	if (xMutex == NULL){
		printf("Failed to create mutex\n");
	}
	xPulseQueue = xQueueCreate(32, sizeof(uint32_t));
	if (xPulseQueue == NULL){
		printf("Failed to create Queue\n");
	}

	/* Create task to control oled */
	if (xTaskCreate(task_lcd, "LCD", TASK_LCD_STACK_SIZE, NULL, TASK_LCD_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create lcd task\r\n");
	}
	if (xTaskCreate(task_rtc, "RTC", TASK_RTC_STACK_SIZE, NULL, TASK_RTC_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create lcd task\r\n");
	}
	if (xTaskCreate(task_simulador, "SIMUL", TASK_SIMULATOR_STACK_SIZE, NULL, TASK_SIMULATOR_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create lcd task\r\n");
	}
	if (xTaskCreate(task_infos, "INFOS", TASK_INFOS_STACK_SIZE, NULL, TASK_INFOS_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create infos task\r\n");
	}
	/* Start the scheduler. */
	vTaskStartScheduler();

	while(1){ }
}

/*
 * hardware.c
 *
 * Created: 11/28/2022 9:59:22 PM
 *  Author: riywa
 */ 
#include "hardware.h"

void RTT_Handler(void) {
	uint32_t ul_status;
	ul_status = rtt_get_status(RTT);

	/* IRQ due to Alarm */
	if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
	}
}

void sensor_callback() {
	uint32_t ul_status = rtt_read_timer_value(RTT);
	RTT_init(RTT_FREQ, 0, 0);
	xQueueSendFromISR(xPulseQueue, &ul_status, 0);
}

void init_pins(void) {
	pmc_enable_periph_clk(SENSOR_PIO_ID);
	pio_set_input(SENSOR_PIO, SENSOR_IDX_MASK, PIO_DEFAULT);
	// pio_set_debounce_filter(SENSOR_PIO, SENSOR_IDX_MASK, 60);

	// Configura handler para o botao 1 para interrupcao
	pio_handler_set(
		SENSOR_PIO,
		SENSOR_PIO_ID,
		SENSOR_IDX_MASK,
		PIO_IT_FALL_EDGE,
		sensor_callback
	);

	// Ativa interrupção e limpa primeira IRQ do botao 1 gerada na ativacao
	pio_enable_interrupt(SENSOR_PIO, SENSOR_IDX_MASK);
	pio_get_interrupt_status(SENSOR_PIO);

	// Configura NVIC para receber interrupcoes do PIO do botao 1
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(SENSOR_PIO_ID);
	NVIC_SetPriority(SENSOR_PIO_ID, 5);

}

void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource) {

	uint16_t pllPreScale = (int) (((float) 32768) / freqPrescale);
	
	rtt_sel_source(RTT, false);
	rtt_init(RTT, pllPreScale);
	
	if (rttIRQSource & RTT_MR_ALMIEN) {
		uint32_t ul_previous_time;
		ul_previous_time = rtt_read_timer_value(RTT);
		while (ul_previous_time == rtt_read_timer_value(RTT));
		rtt_write_alarm_time(RTT, IrqNPulses+ul_previous_time);
	}

	/* config NVIC */
	NVIC_DisableIRQ(RTT_IRQn);
	NVIC_ClearPendingIRQ(RTT_IRQn);
	NVIC_SetPriority(RTT_IRQn, 4);
	NVIC_EnableIRQ(RTT_IRQn);

	/* Enable RTT interrupt */
	if (rttIRQSource & (RTT_MR_RTTINCIEN | RTT_MR_ALMIEN))
	rtt_enable_interrupt(RTT, rttIRQSource);
	else
	rtt_disable_interrupt(RTT, RTT_MR_RTTINCIEN | RTT_MR_ALMIEN);
	
}
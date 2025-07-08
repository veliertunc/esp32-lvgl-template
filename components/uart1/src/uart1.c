#include "uart1.h"

void uart1_init(void) {
  uart_config_t uart_config = {
      .baud_rate = UART_BAUD_RATE,
      .data_bits = UART_DATA_BITS,
      .parity = UART_PARITY,
      .stop_bits = UART_STOP_BITS,
      .flow_ctrl = UART_FLOW_CTRL,
  };

  ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, UART_TX_PIN, UART_RX_PIN,
                               UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
  ESP_ERROR_CHECK(
      uart_driver_install(UART_PORT_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0));

  ESP_LOGI(TAG,
           "UART1 initialized: TX=%d RX=%d Baud=%d DataBits=%d Parity=%d "
           "StopBits=%d FlowCtrl=%d",
           UART_TX_PIN, UART_RX_PIN, UART_BAUD_RATE, UART_DATA_BITS,
           UART_PARITY, UART_STOP_BITS, UART_FLOW_CTRL);
}

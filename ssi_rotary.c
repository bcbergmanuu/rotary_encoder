#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>


uint16_t grayToBinary(uint16_t gray)
{
	uint16_t binary=0;
	for(;gray;gray=gray>>1)
	{
		binary^=gray; //binary=binary^gray;
	}
	return binary;
}

int main() {
    stdio_init_all();

    // Init SPI at 500 kHz, mode 1    

    spi_init(spi_default, 100 * 1000);
	

    spi_set_format(spi_default,
                   15,      // bits per transfer
                   SPI_CPOL_0,
                   SPI_CPHA_1,
                   SPI_MSB_FIRST);    

    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);    
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);    
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);    
    //
    //spi_set_format(spi_default, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    

    uint16_t rx[1] = {0};
    //uint16_t tx[2] = {0};

    while (true) {
        // Perform 16-bit transfer
        static int doPrint = 0;
        spi_read16_blocking(spi_default, 0, rx, 1);

        //uint16_t raw = (rx[0] << 8) | rx[1];
        //uint16_t position = raw >> ((CLOCK_PULSE_BUTES * 8) - ENCODER_BITS);
        sleep_ms(10);

        if(doPrint % 10 == 0) {            
            //uint16_t result = ((uint16_t)rx[1] << 8) | rx[0];            
            uint16_t received_mask = (rx[0] & 8191);
            printf("received %u\t", rx[0]);
            printf("masked %u\t", received_mask);
            uint16_t binary = grayToBinary(received_mask);
            printf("tobinary %u\n", binary);
        }
        doPrint ++;        
    }
}


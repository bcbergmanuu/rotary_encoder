#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>


uint16_t grayToBinary(uint16_t gray)
{
	uint16_t binary=0;
	for(;gray;gray=gray>>1)
	{
		binary^=gray; 
	}
	return binary;
}

int main() {
    stdio_init_all();

    spi_init(spi_default, 100 * 1000);
	

    spi_set_format(spi_default,
                   15,      
                   SPI_CPOL_0,
                   SPI_CPHA_1,
                   SPI_MSB_FIRST);    

    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);    
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);    
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);    
    

    uint16_t rx[1] = {0};
    uint16_t last = 0;

    while (true) {                
        spi_read16_blocking(spi_default, 0, rx, 1);
        sleep_ms(1);
        
        if (last != rx[0]) {
            uint16_t received_mask = (rx[0] & 16383);
            //printf("received %u\t", rx[0]);
            //printf("masked %u\t", received_mask);
            uint16_t binary = grayToBinary(received_mask);
            printf("%u\n", binary);
            last = rx[0];
        }        
    }
}


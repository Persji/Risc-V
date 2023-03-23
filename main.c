
#define RAW_SPACE(addr)     (*(volatile unsigned long *)(addr))

#define SYKOM_CTRL_ADDR     (0x00100460)

#define SYKOM_ID_ADDR       ((SYKOM_CTRL_ADDR)+4)
#define SYKOM_UART_ADDR     ((SYKOM_CTRL_ADDR)+8)

#define SYKOM_EXIT_VAL      (0x00003333)
#define SYKOM_UART_VAL      (0x00008888)

static unsigned int tab[16] = {220, 162, 507, 755, 594, 544, 85, 341, 41, 95, 512, 634, 61, 921, 336, 229};
static const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void my_simulation_exit(unsigned char ret_code) {
    RAW_SPACE(SYKOM_CTRL_ADDR) = SYKOM_EXIT_VAL | (((unsigned long)ret_code)<<16);
}

void my_putchar(unsigned char c) {
    RAW_SPACE(SYKOM_UART_ADDR)=SYKOM_UART_VAL | ((0x000000FF & (unsigned long)c)<<16);
}

unsigned long my_get_cpu_id(void) {
    return RAW_SPACE(SYKOM_ID_ADDR);
}

void putchars(unsigned char *s) {
	  for(; *s; ++s){
      my_putchar(*s);
    }
}

char val2hex(unsigned int i) {
    return hex[i];            
}

void print_me(unsigned long v) {
    unsigned long mask = 0xF0000000;
    unsigned char hex[9];
    int i =0;
    for(; i < 8; ++i) {
      unsigned int val = (unsigned int)((v & mask) >> 28);
      hex[i] = val2hex(val);
      v <<= 4;
    }
    hex[i] = 0;
    putchars(&hex[0]);
}

void uart() {
    unsigned int counter = 0;
    for(int i =0; i < 16; ++i)
        if(!(tab[i]%8))
          ++counter;
    print_me(counter);
}

void tabselect(unsigned long v) {
    unsigned long mask = 0xF;
    unsigned int val = (unsigned int)(v & mask);
    print_me((unsigned long)(tab[val]));
}


int main(void) {
    putchars("RISCV-APP: test ("__FILE__", "__DATE__", "__TIME__")\n");
    putchars("Processor ID = 0x");
    print_me(my_get_cpu_id());
    putchars("\n");
    putchars("UART = 0x");
    uart();
    putchars("\n");
    putchars("SelectedValue = 0x");
    tabselect(my_get_cpu_id());
    putchars("\n");
    my_simulation_exit(0);

    return 0;
}
